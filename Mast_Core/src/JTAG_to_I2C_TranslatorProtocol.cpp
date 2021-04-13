//===========================================================================
//                           Emulation_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Emulation_TranslationProtocol.cpp
//!
//! Implements class Emulation_TranslationProtocol
//!
//===========================================================================

#include "JTAG_to_I2C_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>

#include "g3log/g3log.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;

using std::vector;
using std::initializer_list;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed endpoints (value at offset 0 is reserved for reset)
//!
JTAG_to_I2C_TranslatorProtocol::JTAG_to_I2C_TranslatorProtocol (vector<uint32_t> addresses)
  : m_addresses     (std::move(addresses))
{
  CHECK_PARAMETER_GTE(m_addresses.size(), 2u, "I2C Addresses must have at least two entries");
}
//
//  End of: JTAG_to_I2C_TranslatorProtocol::JTAG_to_I2C_TranslatorProtocol
//---------------------------------------------------------------------------


//! Initializes with addresses defined by a string
//!
//! @note Expected (comma separated) parameters are:
//!   - addresses        Array of I2C addresses for managed endpoints (value at offset 0 is reserved for reset)
//!
//! @note Example of parameter:
//!    0x30, 0x31, 0x32
//!    0x30u, 0x31u
//!    48, 49, 0x32
//!
JTAG_to_I2C_TranslatorProtocol::JTAG_to_I2C_TranslatorProtocol (const std::string& parameters)
{
  auto parts = Utility::Split(parameters, ",");

  CHECK_PARAMETER_GTE(parts.size(), 2u, "Parameters must defined at least two I2C Addresses, got: \""sv + parameters + "\""sv);

  auto afterAddressIndex = 0u; 

  auto makeParameterMessage = [&parts](auto partIndex)
  {
    auto part = parts[partIndex];
    Utility::TrimBoth(part);

    string message;
    message.append("Parameter: ").append(std::to_string(partIndex));
    message.append(" \"").append(part.cbegin(), part.cend()).append("\"");
    return message;
  };

  // ---------------- Get Adresses
  //
  try
  {
    for (auto part : parts)
    {
      auto number = std::stoull(string(part), nullptr, 0);
      CHECK_PARAMETER_LTE(number, UINT32_MAX, makeParameterMessage(afterAddressIndex).append(", is out of range for a 32 bit number"));

      m_addresses.push_back(static_cast<uint32_t>(number));
      ++afterAddressIndex;
    }
  }
  catch(std::out_of_range& exc)     // Conversion to number is not possible ==> it must be the prefix
  {
    THROW_INVALID_ARGUMENT(makeParameterMessage(afterAddressIndex).append(", is out of range for a 32 bit number"));
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Got std::exception: " << exc.what();
    throw;
  }
  catch (...)
  {
    LOG(ERROR_LVL) << "Caught unexpected, unknown exception while parsing parameters for I2C_RawPlayer";
    throw;   // Unexpected exception are rethrown
  }

  CHECK_PARAMETER_GTE(m_addresses.size(), 2u, "I2C Addresses must have at least two entries");

  if (parts.size() > afterAddressIndex)
  {
    THROW_INVALID_ARGUMENT(makeParameterMessage(afterAddressIndex).append(", is not a number (for address) or not the last one (for a prefix)"));
  }
}
//
//  End of: JTAG_to_I2C_TranslatorProtocol::JTAG_to_I2C_TranslatorProtocol
//---------------------------------------------------------------------------


//! Returns address for specified endpoint
//!
//! @param endpointId   EndPoint identifier [1..N]
//!
uint32_t JTAG_to_I2C_TranslatorProtocol::GetAddress (uint32_t endpointId) const
{
  if (endpointId >= m_addresses.size())
  {
    THROW_INVALID_ARGUMENT("EndPointId must be '0' for Reset or '1' to "s + std::to_string(m_addresses.size() - 1));
  }

  auto address = m_addresses[endpointId];
  return address;
}
//
//  End of: JTAG_to_I2C_TranslatorProtocol::GetAddress
//---------------------------------------------------------------------------

BinaryVector JTAG_to_I2C_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  BinaryVector result;
  string i2c_FormattedData;
  BinaryVector callback_toSutData;
  auto toSutData = current_request.ToSutVector();
  int32_t endpointId=-1;
  
  //Prepapre formatted SVF data
  ostringstream os_read,os_write;

  void *address_data=static_cast<void *>(m_addresses.data());
  
  LOG(DEBUG) << "TransformationCallback: recevied a Request for a "<< current_request.CallbackId()<<" operation";
  if (current_request.CallbackId()==TRST)
    endpointId = 0;
  if (current_request.CallbackId()==SIR)
    endpointId = 1;
  if (current_request.CallbackId()==SDR)
    endpointId = 2;
  if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return result;
   }
  CHECK_PARAMETER_GTE(endpointId,0,"Error: unsupported Callback "+current_request.CallbackId());
  
  auto address = GetAddress(endpointId);

  os_read   << "(0x"  << std::hex << address << ");\n";
  os_write  << "(0x" << std::hex << address << ", " << toSutData.DataAsMixString() << ");\n";

  if (endpointId == 0) //No data in the request dor Reset operation
      {
     RVFRequest reset_request(I2C_RESET,callback_toSutData,i2c_FormattedData,address_data);
     PushRequest(reset_request);
     result = PopfromSut();
      }
  else
   {
  LOG(DEBUG) << "TransformationCallback: Issue an I2C_READ operation";
  callback_toSutData = toSutData;
  RVFRequest read_request(I2C_READ,callback_toSutData,os_read.str(),address_data);
  PushRequest(read_request);
  result = PopfromSut(); //Need to remove from queue, but return data in not useful
  LOG(DEBUG) << "TransformationCallback: I2C_READ finished: drop return data";

  LOG(DEBUG) << "TransformationCallback: Issue an I2C_WRITE operation";
  RVFRequest write_request(I2C_WRITE,callback_toSutData,os_write.str(),address_data);
  PushRequest(write_request);
  result = PopfromSut();
  LOG(DEBUG) << "TransformationCallback: Issue an I2C_WRITE operation";
  }
  
  return result;
}
//
//  End of: JTAG_to_I2C_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of JTAG_to_I2C_TranslatorProtocol.cpp
//===========================================================================
