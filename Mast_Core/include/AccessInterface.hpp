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

  //! Returns current protocol for streaming register value to/from SUT
  //!
  std::shared_ptr<AccessInterfaceProtocol>  Protocol() const { return m_protocol; }

  //! Sets protocol for streaming register value to/from SUT
  //!
  void  SetProtocol (std::shared_ptr<AccessInterfaceProtocol> protocol) { m_protocol = protocol; }

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "AccessInterface"; } //!< Returns readable type name

  // ---------------- Private  Fields
  //
  private:
  uint32_t                                 m_numberOfDerivations = 0; //!< Number of nodes (derivations) accessible through the access interface
  std::shared_ptr<AccessInterfaceProtocol> m_protocol;                //!< Protocol to use to manage physical interface
};
//
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
