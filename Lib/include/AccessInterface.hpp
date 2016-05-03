//===========================================================================
//                           AccessInterface.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterface.hpp
//!
//! Declares class AccessInterface
//!
//===========================================================================

#ifndef ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_
  #define ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

#include "ParentNode.hpp"
#include "BinaryVector.hpp"
#include <functional>
#include <vector>

namespace mast
{
class AccessInterfaceProtocol;

//! Represents an access point to scan chain (the physical interface to the SUT)
//!
class DLL_EXPORT AccessInterface : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterface() = default;
  AccessInterface()  = delete;
  AccessInterface(std::experimental::string_view name, std::shared_ptr<AccessInterfaceProtocol> protocol)
    : ParentNode (name)
    , m_protocol (protocol)
  {}

  //! Primitives interface defining the Access Interface prototocol
  //! @note The argument is a void* so it can be recast to the actual parameters
  //!
  using Primitive = std::function<int(void*)>; //!< Defines pre- and post- condition functors

  //! Prototype of the commands used to access a derivation with as parameters:
  //!  - The protocol table of available functions
  //!  - The optional data the AI might requite
  //!  - The total number of the derivations
  //!  - The vector to push into the SUT (obtained from the current System Model)
  //!  - The vector received from the SUT, which must be used to update the System Model
  //!    after the command finishes
  //!
  using Command  = std::function<void(Primitive*, void*, int, BinaryVector*, BinaryVector*)>;

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  uint32_t                                 m_numberOfDerivations = 0; //!< Number of nodes (derivations) accessible through the access interface
  std::shared_ptr<AccessInterfaceProtocol> m_protocol;                //!< Protocol to use to manage physical interface
//+  std::vector<Primitive>         m_primitives;                   //!< Primitives composing the protocol
//+  std::vector<Command>           m_actions;                      //!< Provide Actions to access the derivations based on the set of primitives
};
//
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
