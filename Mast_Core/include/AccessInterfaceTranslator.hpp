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

#ifndef ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_
  #define ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

#include "ParentNode.hpp"
#include "BinaryVector.hpp"
#include "CallbackTranslator.hpp"
#include <functional>
#include <vector>
#include <queue>          // std::queue

namespace mast
{

//! Represents an access point to scan chain (the physical interface to the SUT)
//!
class MAST_CORE_EXPORT AccessInterfaceTranslator : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceTranslator() = default;
  AccessInterfaceTranslator()  = default;
  AccessInterfaceTranslator(std::experimental::string_view name)
    : ParentNode (name)
  {}

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "AccessInterfaceTranslator"; } //!< Returns readable type name

  // ---------------- Private  Fields
  //
  private:
  uint32_t                                 m_numberOfEndPoints = 0; //!< Number of nodes (endpoints) accessible through the access interface
  
  std::queue<CallbackTranslator> CallbackQueue;  
  std::queue<BinaryVector> fromSutQueue;  
};
//
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
