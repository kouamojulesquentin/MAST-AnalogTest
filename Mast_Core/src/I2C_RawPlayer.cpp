//===========================================================================
//                           I2C_RawPlayer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_RawPlayer.cpp
//!
//! Implements class I2C_RawPlayer
//!
//===========================================================================

#include "I2C_RawPlayer.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <sstream>

using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::vector;
using std::initializer_list;

using namespace mast;
using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;


//! Constructor from vector
//!
//! @param addresses        Array of I2C addresses for managed endpoints (value at offset 0 is reserved for reset)
//!
I2C_RawPlayer::I2C_RawPlayer (vector<uint32_t> addresses)
  : m_addresses     (std::move(addresses))
{
  CHECK_PARAMETER_GTE(m_addresses.size(), 2u, "I2C Addresses must have at least two entries");
}
//
//  End of: I2C_RawPlayer::I2C_RawPlayer
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
I2C_RawPlayer::I2C_RawPlayer (const std::string& parameters)
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
//  End of: I2C_RawPlayer::I2C_RawPlayer
//---------------------------------------------------------------------------




//! Creates an I2C command associated to endpoint identifier and BinaryVector to send to SUT
//!
//! @param addresses        Array of I2C addresses for managed endpoints (value at offset 0 is reserved)
//!
string I2C_RawPlayer::CreateI2CCommand (uint32_t endpointId, const BinaryVector& toSutData)
{
  ostringstream os;
  string_view commandType;

  if (endpointId == 0)
  {
    os << "I2C_RESET()\n";
  }
  else
  {
    auto address = GetAddress(endpointId);

    os  << "I2C_READ(0x"  << std::hex << address << ")\n";
    os  << "I2C_WRITE(0x" << std::hex << address << ", " << toSutData.DataAsMixString() << ")\n";
  }

  auto i2cCommand = os.str();

  return i2cCommand;
}
//
//  End of: I2C_RawPlayer::CreateI2CCommand
//---------------------------------------------------------------------------


//! Returns address for specified endpoint
//!
//! @param endpointId   EndPoint identifier [1..N]
//!
uint32_t I2C_RawPlayer::GetAddress (uint32_t endpointId) const
{
  if (endpointId >= m_addresses.size())
  {
    THROW_INVALID_ARGUMENT("EndPointId must be '0' for Reset or '1' to "s + std::to_string(m_addresses.size() - 1));
  }

  auto address = m_addresses[endpointId];
  return address;
}
//
//  End of: I2C_RawPlayer::GetAddress
//---------------------------------------------------------------------------


//! sends request for I2C_READ and I2C_WRITE callbacks and waits for response

BinaryVector I2C_RawPlayer::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{
  BinaryVector result;
  string i2c_FormattedData;
  BinaryVector callback_toSutData;
  
  //Prepapre formatted SVF data
  ostringstream os_read,os_write;

  void *address_data=static_cast<void *>(m_addresses.data());
  
  auto address = GetAddress(endpointId);

  os_read   << "(0x"  << std::hex << address << ");\n";
  os_write  << "(0x" << std::hex << address << ", " << toSutData.DataAsMixString() << ");\n";

  if (endpointId == 0) //No data in the request dor Reset operation
      {
     CallbackRequest reset_request(I2C_RESET,callback_toSutData,i2c_FormattedData,address_data);
     PushRequest(reset_request);
     result = PopfromSut();
      }
  else
   {
  callback_toSutData = toSutData;
  CallbackRequest  read_request(I2C_READ,callback_toSutData,os_read.str(),address_data);
  PushRequest(read_request);
  result = PopfromSut(); //Need to remove from queue, but return data in not useful

  CallbackRequest write_request(I2C_WRITE,callback_toSutData,os_write.str(),address_data);
  PushRequest(write_request);
  result = PopfromSut();
  }
  
  return result;
}

//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void I2C_RawPlayer::DoReset(bool doSynchronousReset)
{
  if (doSynchronousReset){}; //Null operation, used to silence warning

  CallbackRequest request(I2C_RESET);
  PushRequest(request);
  PopfromSut();
}


//===========================================================================
// End of I2C_RawPlayer.cpp
//===========================================================================
