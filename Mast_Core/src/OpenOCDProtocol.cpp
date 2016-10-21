//===========================================================================
//                           OpenOCDProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file OpenOCDProtocol.cpp
//!
//! Implements class OpenOCDProtocol
//!
//===========================================================================

#include "Utility.hpp"
#include "g3log/g3log.hpp"
#include "OpenOCDProtocol.hpp"

#include <vector>
#include <string>

#ifdef USE_OPEN_OCD

extern "C"
{
    #include <config.h>
    #include <jtag/driver.h>
    #include <jtag/jtag.h>
    #include <transport/transport.h>
    #include <helper/ioutil.h>
    #include <helper/util.h>
    #include <helper/configuration.h>
    #include <flash/nor/core.h>
    #include <flash/nand/core.h>
    #include <pld/pld.h>
    #include <flash/mflash.h>

    #include <server/server.h>
    #include <server/gdb_server.h>
}


static const char openocd_startup_tcl[] = {
#include "startup_tcl.inc"
0 /* Terminate with zero */
};

extern "C" struct command_context *setup_command_handler(Jim_Interp *interp);
#endif



using namespace std::string_literals;
using namespace mast;
using std::vector;
using std::experimental::string_view;

//! Initializes OpenOCD "engine"
//!
#ifndef USE_OPEN_OCD
OpenOCDProtocol::OpenOCDProtocol (string_view /* configFilePath */, string_view /* designName */, int /* iIrLength */)
{
}
#else
OpenOCDProtocol::OpenOCDProtocol (string_view configFilePath, string_view designName, int iIrLength)
{
  m_openOCD_initialized = false;
  // setup_command_handler registers all handlers used by all different blocs from OpenOCD,
  // such as callbacks for JTAG, Flash, drivers, and so on.
  m_cmd_ctx = setup_command_handler(nullptr);

  command_context_mode(this->m_cmd_ctx, COMMAND_CONFIG);
  command_set_output_handler(this->m_cmd_ctx, configuration_output_handler, nullptr);

  // configuration files are parsed through JimTcl.
  // command_run_line() can process interface and TAP description scripts, indicate the required debug_level...
  // This is why "script " prior string is required.
  auto adapter_config_string = "script "s + configFilePath;

  auto ret = command_run_line(m_cmd_ctx, const_cast<char*>(adapter_config_string.c_str()));
  CHECK_TRUE(ret != ERROR_COMMAND_CLOSE_CONNECTION, "[OpenOCD] Command requesting the parsing of the configuration file is unsupported.");
  CHECK_TRUE(ret == ERROR_OK,                       "[OpenOCD] Error while parsing the adapter configuration file.");

  ret = util_init(this->m_cmd_ctx);
  CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] util_init has failed.");

  ret = ioutil_init(this->m_cmd_ctx);
  CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] ioutil_init has failed.");

  ret = adapter_init(this->m_cmd_ctx);
	CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] adapter_init has failed.");

  auto tap = static_cast<jtag_tap*>(calloc(1, sizeof(jtag_tap)));

  CHECK_VALUE_NOT_NULL(tap, "[OpenOCD] Making the TAP returns NULL.");

  tap->chip = strdup(designName.data());

  // We want OpenOCD working in "single TAP mode"
  tap->tapname       = strdup("tap");
  auto s_dotted_name = designName + ".tap"s;

  // As a chip may have multiple TAPs, OpenOCD describes each TAP by a dotted name: chip.tapname
  tap->dotted_name = strdup(s_dotted_name.data());

  // For boundary scans, OpenOCD only needs the size of the IR register.
  tap->ir_length = iIrLength;
  tap->enabled   = true;

  jtag_tap_init(tap);

  m_supportTrst = jtag_get_reset_config() & RESET_HAS_TRST;		// We check if the current JTAG adapter provides a TRST pin.


  if(m_supportTrst)         // Some adapters and JTAG TAP do not provide a TRST pin.
    jtag_add_reset(1, 0);
  else
    jtag_add_reset(0, 0);

  ret = jtag_execute_queue();

  CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] jtag_execute_queue has failed.");
  
  m_openOCD_initialized = true;
}
#endif
//
//  End of: OpenOCDProtocol::OpenOCDProtocol
//---------------------------------------------------------------------------

//! Destructor for OpenOCD "engine"
//!
OpenOCDProtocol::~OpenOCDProtocol()
{

  #ifdef USE_OPEN_OCD

  if (!m_openOCD_initialized) //There was an error during construction
    return;
    
  // Here we put the tap in RESET state
  if(m_supportTrst)
    jtag_add_reset(1, 0);           // Hardware reset is supported, TRST is enabled for one TCK cycle.
  else
    jtag_add_statemove(TAP_RESET);  // At least one of the toolchain components does not provide a TRST pin.
                                    // Using FSM instead.

  int ret = jtag_execute_queue();

  if (ret == ERROR_OK)
    return;


  auto tap = jtag_all_taps();

  // We need to destruct all TAPs.
  while(tap)
  {
    auto tap_n = tap->next_tap;
    jtag_tap_free(tap);
    tap = tap_n;
  }

  unregister_all_commands(m_cmd_ctx, nullptr);
  command_done(m_cmd_ctx);
  adapter_quit();
  #endif
}
//
//  End of: OpenOCDProtocol::~OpenOCDProtocol
//---------------------------------------------------------------------------


//! Does any action required to transfer scan data to and from SUT
//!
//! @param derivationId   Identifies the derivation to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bitstream retrieved from SUT
//!
#ifndef USE_OPEN_OCD
BinaryVector OpenOCDProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  switch (derivationId)
  {
    case 0u:
      LOG(INFO) << "OpenOCD_RST_LB";
      break;
    case 1u:
      LOG(INFO) << "OpenOCD_IR_LB(" << toSutData.DataAsMixString() << ")";
      break;
    case 2u:
      LOG(INFO) << "OpenOCD_DR_LB(" << toSutData.DataAsMixString() << ")";
      break;
    default:
      THROW_INVALID_ARGUMENT("DerivationId must be '0' (for Reset), '1' (for IR) or '2' (for DR)");
      break;
  }
  return toSutData; // Just do a loopback
}
#else
BinaryVector OpenOCDProtocol::DoAction (uint32_t derivationId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto bitsCount         = toSutData.BitsCount();
  auto fromSutDataBuffer = vector<uint8_t>(toSutData.BytesCount());

  /** Nota on states:
  Please check §17.2 on http://openocd.org/doc/html/JTAG-Commands.html :
  - 'The values to be latched in upcoming DRUPDATE or IRUPDATE states may not be as expected.', hence the use of dr/ir scan requires stable end-states. This is why, to get a register update, you must select TAP_IDLE rather than TAP_PAUSE.
  */

  switch (derivationId)
  {
    case 0u:
      DoReset(false);
      break;
    case 1u:
      {
        vector<uint8_t> v_openocd_ir = toSutData.DataRightAligned();			// OpenOCD requires data to be structured with the LSB first, MSB last. Here we have each byte with LSb on the right, MSb on the left.
        reverse(v_openocd_ir.begin(), v_openocd_ir.end());								// Now we reverse the order, as the LSB is the last one of the vector.
        LOG(INFO) << "OpenOCD_IR(" << toSutData.DataAsMixString() << ")";
        jtag_add_plain_ir_scan(bitsCount, v_openocd_ir.data(), fromSutDataBuffer.data(), TAP_IDLE);		// Adding irscan to the scheduler.
      }
      break;
    case 2u:
      {
        vector<uint8_t> v_openocd_dr = toSutData.DataRightAligned();		// See case 1u.
        reverse(v_openocd_dr.begin(), v_openocd_dr.end());
        LOG(INFO) << "OpenOCD_DR(" << toSutData.DataAsMixString() << ")";
        jtag_add_plain_dr_scan(bitsCount, v_openocd_dr.data(), fromSutDataBuffer.data(), TAP_IDLE);
      }
      break;
    default:
      THROW_INVALID_ARGUMENT("DerivationId must be '0' (for Reset), '1' (for SIR) or '2' (for SDR)");
      break;
  }

  auto ir = jtag_execute_queue();				// Executing tasks queued in the scheduler.

  CHECK_TRUE(ir == ERROR_OK, "[OpenOCD] jtag_execute_queue has failed.");

  vector<uint8_t> v_openocd_out = fromSutDataBuffer;				// Inverse of what is done in 1u and 2u derivationId. We set incoming data in the MAST-supported format.
  reverse(v_openocd_out.begin(), v_openocd_out.end());

  auto   fromSutData = BinaryVector::CreateFromRightAlignedBuffer(v_openocd_out, bitsCount);

  return fromSutData;
}
#endif
//
//  End of: OpenOCDProtocol::DoAction
//---------------------------------------------------------------------------


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void OpenOCDProtocol::DoReset(bool doSynchronousReset)
{
  LOG(INFO) << "OpenOCD_RST sync = " << std::boolalpha << doSynchronousReset;

  #ifdef USE_OPEN_OCD
  if(m_supportTrst && !doSynchronousReset)
  {
    jtag_add_reset(1, 0);           // TRST is enabled for one TCK cycle.
  }
  else
  {
    jtag_add_statemove(TAP_RESET);  // One of the toolchain components does not provide a TRST pin ==> Use FSM instead.
  }
  #endif
}

bool OpenOCDProtocol::is_initialized()
 {
  return m_openOCD_initialized;
 }

//===========================================================================
// End of OpenOCDProtocol.cpp
//===========================================================================
