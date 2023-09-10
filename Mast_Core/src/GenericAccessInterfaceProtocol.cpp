//===========================================================================
//                           GenericAccessInterfaceProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GenericAccessInterfaceProtocol.cpp
//!
//! Implements class GenericAccessInterfaceProtocol
//!
//===========================================================================

#include "GenericAccessInterfaceProtocol.hpp"
#include "Utility.hpp"

#include <string>

using std::vector;
using std::initializer_list;

using namespace std::string_literals;
using namespace mast;



//! Constructor from initializer_list
//!
GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol (initializer_list<Action> callbacks, initializer_list<Primitive> primitives)
  : m_callbacks    (callbacks)
  , m_primitives (primitives)
{
  if (m_callbacks.size() < 2)
  {
    THROW_INVALID_ARGUMENT("Actions table must have at least two entries (including one reserved for reset)");
  }

  if (m_primitives.size() < 2)
  {
    THROW_INVALID_ARGUMENT("Primitives table must have at least two entries (including one reserved for reset)");
  }
}
//
//  End of: GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol
//---------------------------------------------------------------------------




//! Constructor from vectors
//!
GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol (vector<Action> callbacks, vector<Primitive> primitives)
  : m_callbacks    (std::move(callbacks))
  , m_primitives (std::move(primitives))
{
  if (m_callbacks.size() < 2)
  {
    THROW_INVALID_ARGUMENT("Actions table must have at least two entries (including one reserved for reset)");
  }

  if (m_primitives.size() < 2)
  {
    THROW_INVALID_ARGUMENT("Primitives table must have at least two entries (including one reserved for reset)");
  }
}
//
//  End of: GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol
//---------------------------------------------------------------------------


//! Calls callback associated with AccessInterface Channel
//!
//! @param channelId   Identifies the Channel to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bits stream retrieved from SUT
BinaryVector GenericAccessInterfaceProtocol::DoCallback (RVFRequest Request, uint32_t channelId)
{
  if (channelId >= m_callbacks.size())
  {
    THROW_INVALID_ARGUMENT("Only have callbacks for "s + std::to_string(m_callbacks.size() - 1) + " channels");
  }

  auto& callback       = m_callbacks[channelId];
  auto fromSutVector = callback(m_primitives, Request.interfaceData(), Request.ToSutVector());

  return fromSutVector;
}
//
//  End of: GenericAccessInterfaceProtocol::DoCallback
//---------------------------------------------------------------------------



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void GenericAccessInterfaceProtocol::DoReset(bool doSynchronousReset)
{
  auto& primitive = m_primitives[0];
  primitive(static_cast<void*>(&doSynchronousReset));
}


//===========================================================================
// End of GenericAccessInterfaceProtocol.cpp
//===========================================================================
