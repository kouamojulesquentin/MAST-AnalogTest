//===========================================================================
//                           PropagatePendingVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PropagatePendingVisitor.hpp
//!
//! Declares PropagatePendingVisitor class
//!
//===========================================================================


#ifndef PROPAGATEPENDINGVISITOR_H__27D56DAD_131D_48B7_7497_844F54AB975C__INCLUDED_
  #define PROPAGATEPENDINGVISITOR_H__27D56DAD_131D_48B7_7497_844F54AB975C__INCLUDED_

#include "SystemModelVisitor.hpp"
#include "Mast_Core_export.hpp"
#include <cstdint>
#include <utility> 

namespace mast
{
//! Revisits all system model to propagate up pending state
//!
//! @note This is necessary in cases pending state of a mux register was set
//!       before the decision of its effective value (because configuration visited
//!       it before)
//!
//! @note Visiting system model nodes can only set nodes pending flags (it never resets it)
class MAST_CORE_EXPORT PropagatePendingVisitor final : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~PropagatePendingVisitor() = default;
  PropagatePendingVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitAccessInterfaceTranslator (AccessInterfaceTranslator&accessInterfaceTranslator) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitStreamer        (Streamer&        streamer)        override;

  // ---------------- Protected Methods
  //
  protected:
  virtual std::pair<uint32_t,uint32_t>  ChildrenPendings (const ParentNode& parentNode);
};
//
//  End of PropagatePendingVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PROPAGATEPENDINGVISITOR_H__27D56DAD_131D_48B7_7497_844F54AB975C__INCLUDED_

//===========================================================================
// End of PropagatePendingVisitor.hpp
//===========================================================================
