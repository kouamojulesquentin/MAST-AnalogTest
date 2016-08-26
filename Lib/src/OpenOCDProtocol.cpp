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

#include <vector>

using namespace mast;
using std::vector;
using std::experimental::string_view;

//! Initializes OpenOCD "engine"
//!
//! @param configFilePath path of the configuration file for the adapter (USB dongle, FTDI...)
//! @param designName name of the SystemModel.
//! @param iIrLength size of the IR register within the system.
OpenOCDProtocol::OpenOCDProtocol (string_view configFilePath, string_view designName, int iIrLength)
{

    // setup_command_handler registers all handlers used by all different blocs from OpenOCD,
    // such as callbacks for JTAG, Flash, drivers, and so on. 
    this->cmd_ctx = setup_command_handler(NULL);

    command_context_mode(this->cmd_ctx, COMMAND_CONFIG);

    command_set_output_handler(this->cmd_ctx, configuration_output_handler, NULL);

    // configuration files are parsed through JimTcl.
    // command_run_line() can process interface and TAP description scripts, indicate the required debug_level...
    // This is why "script " prior string is required.
    string_view adapter_config_string = "script " + configFilePath;

    int ret = command_run_line(cmd_ctx, (char*)adapter_config_string.data());

	CheckParameterCondition<int>("OpenOCDProtocol.cpp", "OpenOCDProtocol", 35, ret, (ret != ERROR_COMMAND_CLOSE_CONNECTION), "[OpenOCD] Command requesting the parsing of the configuration file is unsupported.");

	CheckParameterCondition<int>("OpenOCDProtocol.cpp", "OpenOCDProtocol", 37, ret, (ret == ERROR_OK), "[OpenOCD] Error while parsing the adapter configuration file.");

    ret = util_init(this->cmd_ctx);
	CheckParameterCondition<int>("OpenOCDProtocol.cpp", "OpenOCDProtocol", 40, ret, (ret == ERROR_OK), "[OpenOCD] util_init has failed.");

    ret = ioutil_init(this->cmd_ctx);
	CheckParameterCondition<int>("OpenOCDProtocol.cpp", "OpenOCDProtocol", 43, ret, (ret == ERROR_OK), "[OpenOCD] ioutil_init has failed.");

    // Initialising the adapter.
    adapter_init(this->cmd_ctx);

    // Now we build the corresponding System Description
    struct jtag_tap *m_tap = NULL;

    m_tap = (struct jtag_tap*)calloc(1, sizeof(*m_tap));

    CheckValueIsNotNullptr<struct jtag_tap*>("OpenOCDProtocol.cpp", "OpenOCDProtocol", 51, m_tap, "[OpenOCD] Making the TAP returns NULL.");

    m_tap->chip = strdup(designName.data());

    // We want OpenOCD working in "boudary scan mode"
    m_tap->tapname = strdup("bs");

    string_view s_dotted_name = designName + ".bs";

    // As a chip may have multiple TAPs, OpenOCD describes each TAP by a dotted name: chip.tapname
    m_tap->dotted_name = strdup(s_dotted_name.data());

    // For boundary scans, OpenOCD only needs the size of the IR register.
    m_tap->ir_length = iIrLength;

    m_tap->enabled = true;

    jtag_tap_init(m_tap);

    jtag_add_reset(0, 0);

}

//
//  End of: OpenOCDProtocol::OpenOCDProtocol
//---------------------------------------------------------------------------

//! Destructor for OpenOCD "engine"
//!
OpenOCDProtocol::~OpenOCDProtocol()
{
    struct jtag_tap *m_tap = jtag_all_taps();

    // We need to destruct all TAPs.
    while(m_tap != NULL) {
        struct jtag_tap *m_tap_n = m_tap->next_tap;
        jtag_tap_free(m_tap);
        m_tap = m_tap_n;
    }
    
    unregister_all_commands(cmd_ctx, NULL);

	command_done(cmd_ctx);

	adapter_quit();
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
    case 1u:
      jtag_add_plain_ir_scan(bitsCount, toSutData.Data(), fromSutDataBuffer.data(), TAP_IDLE);
      break;
    case 2u:
      jtag_add_plain_dr_scan(bitsCount, toSutData.Data(), fromSutDataBuffer.data(), TAP_IDLE);
      break;
    default:
      THROW_INVALID_ARGUMENT("DerivationId must be '1' (for SIR), or '2' (for SDR)");
      break;
  }

  int ir = jtag_execute_queue();

  CheckParameterCondition<int>("OpenOCDProtocol.cpp", "DoAction", 131, ir, (ir==ERROR_OK), "[OpenOCD] jtag_execute_queue has failed.");

  auto fromSutData = BinaryVector(fromSutDataBuffer, bitsCount);
  return fromSutData;
}
//
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------

//===========================================================================
// End of OpenOCDProtocol.cpp
//===========================================================================
