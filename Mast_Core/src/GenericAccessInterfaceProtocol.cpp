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
GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol (initializer_list<Action> actions, initializer_list<Primitive> primitives)
  : m_actions    (actions)
  , m_primitives (primitives)
{
  if (m_actions.size() < 2)
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
GenericAccessInterfaceProtocol::GenericAccessInterfaceProtocol (vector<Action> actions, vector<Primitive> primitives)
  : m_actions    (std::move(actions))
  , m_primitives (std::move(primitives))
{
  if (m_actions.size() < 2)
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


//! Calls action associated with AccessInterface derivation
//!
//! @param derivationId   Identifies the derivation to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bits stream retrieved from SUT
BinaryVector GenericAccessInterfaceProtocol::DoAction (uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData)
{
  if (derivationId >= m_actions.size())
  {
    THROW_INVALID_ARGUMENT("Only have actions for "s + std::to_string(m_actions.size() - 1) + " derivations");
  }

  auto& action       = m_actions[derivationId];
  auto fromSutVector = action(m_primitives, interfaceData, toSutData);

  return fromSutVector;
}
//
//  End of: GenericAccessInterfaceProtocol::DoAction
//---------------------------------------------------------------------------



//===========================================================================
// End of GenericAccessInterfaceProtocol.cpp
//===========================================================================
