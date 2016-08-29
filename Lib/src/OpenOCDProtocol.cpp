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

#include "OpenOCDProtocol.hpp"
#include "Utility.hpp"

#include <vector>
#include <string>

#ifndef _WIN32

static const char openocd_startup_tcl[] = {
#include "startup_tcl.inc"
0 /* Terminate with zero */
};

extern "C" struct command_context *setup_command_handler(Jim_Interp *interp);
#endif  // not define _WIN32



using namespace std::string_literals;
using namespace mast;
using std::vector;
using std::experimental::string_view;

//! Initializes OpenOCD "engine"
//!
#ifdef _WIN32
OpenOCDProtocol::OpenOCDProtocol (string_view /* configFilePath */, string_view /* designName */, int /* iIrLength */)
{
}
#else
OpenOCDProtocol::OpenOCDProtocol (string_view configFilePath, string_view designName, int iIrLength)
{
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

  adapter_init(this->m_cmd_ctx);

  auto tap = static_cast<jtag_tap*>(calloc(1, sizeof(jtag_tap)));

  CHECK_VALUE_NOT_NULL(tap, "[OpenOCD] Making the TAP returns NULL.");

  tap->chip = strdup(designName.data());

  // We want OpenOCD working in "boudary scan mode"
  tap->tapname       = strdup("bs");
  auto s_dotted_name = designName + ".bs"s;

  // As a chip may have multiple TAPs, OpenOCD describes each TAP by a dotted name: chip.tapname
  tap->dotted_name = strdup(s_dotted_name.data());

  // For boundary scans, OpenOCD only needs the size of the IR register.
  tap->ir_length = iIrLength;
  tap->enabled   = true;

  jtag_tap_init(tap);

  // OpenOCD has an 'internal' variable (i.e. static) indicating if hardware TRST is supported by
  // the adapter and the target.
  this->m_supported_resets = jtag_get_reset_config();

  
  // Some adapters and JTAG TAP do not provide a TRST pin. If the m_supported_resets attribute has the
  // RESET_HAS_TRST flag positive, we do it by hardware, otherwise we pass by the state machine.
  if(this->m_supported_resets & RESET_HAS_TRST)
    jtag_add_reset(1, 0);
  else
    jtag_add_reset(0, 0);

  ret = jtag_execute_queue();

  CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] jtag_execute_queue has failed.");
}
#endif  // not define _WIN32
//
//  End of: OpenOCDProtocol::OpenOCDProtocol
//---------------------------------------------------------------------------

//! Destructor for OpenOCD "engine"
//!
OpenOCDProtocol::~OpenOCDProtocol()
{
  #ifndef _WIN32
  
  // Here we put the tap in RESET state
  if(this->m_supported_resets & RESET_HAS_TRST)
    jtag_add_reset(1, 0);           // Hardware reset is supported, TRST is enabled for one TCK cycle.
  else
    jtag_add_statemove(TAP_RESET);  // At least one of the toolchain components does not provide a TRST pin.
                                    // Using FSM instead.

  int ret = jtag_execute_queue();

  CHECK_TRUE(ret == ERROR_OK, "[OpenOCD] jtag_execute_queue has failed.");


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
  #endif  // not define _WIN32
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
#if defined(_WIN32)
BinaryVector OpenOCDProtocol::DoAction (uint32_t /* derivationId */, void* /* interfaceData */, const BinaryVector& toSutData)
{
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
      // Some adapters and JTAG TAP do not provide a TRST pin. If the m_supported_resets attribute has the
      // RESET_HAS_TRST flag positive, we do it by hardware, otherwise we pass by the state machine.
      if(this->m_supported_resets & RESET_HAS_TRST)
        jtag_add_reset(1, 0);           // TRST is enabled for one TCK cycle.
      else
        jtag_add_statemove(TAP_RESET);  // One of the toolchain components does not provide a TRST pin.
                                        // Using FSM instead.
      break;
    case 1u:
      jtag_add_plain_ir_scan(bitsCount, toSutData.Data(), fromSutDataBuffer.data(), TAP_IDLE);
      break;
    case 2u:
      jtag_add_plain_dr_scan(bitsCount, toSutData.Data(), fromSutDataBuffer.data(), TAP_IDLE);
      break;
    default:
      THROW_INVALID_ARGUMENT("DerivationId must be '0' (for Reset), '1' (for SIR) or '2' (for SDR)");
      break;
  }

  auto ir = jtag_execute_queue();

  CHECK_TRUE(ir == ERROR_OK, "[OpenOCD] jtag_execute_queue has failed.");

  auto   fromSutData = BinaryVector(fromSutDataBuffer, bitsCount);
  return fromSutData;
}
#endif
//
//  End of: OpenOCDProtocol::DoAction
//---------------------------------------------------------------------------

//===========================================================================
// End of OpenOCDProtocol.cpp
//===========================================================================
