//===========================================================================
//                           ToSutVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ToSutVisitor.hpp
//!
//! Declares ToSutVisitor class
//!
//===========================================================================


#ifndef TOSUTVISITOR_H__F4EB9DAF_F1CB_4A62_2CB6_DA151ADF18A8__INCLUDED_
  #define TOSUTVISITOR_H__F4EB9DAF_F1CB_4A62_2CB6_DA151ADF18A8__INCLUDED_

#include "SystemModelNode.hpp"
#include "SystemModelVisitor.hpp"
#include "BinaryVector.hpp"
#include "Mast_Core_export.hpp"

#include <vector>

namespace mast
{
//! Collects content of active Registers along with their identifier
//!
class MAST_CORE_EXPORT ToSutVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~ToSutVisitor() = default;
  ToSutVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitAccessInterfaceTranslator (AccessInterfaceTranslator&accessInterfaceTranslator) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  using NodeIdentifier = SystemModelNode::NodeIdentifier;

  bool                               IgnorePendingState()         const { return m_ignorePendingState; } //!< Returns true all active paths are visited (pending or not)
  const BinaryVector&                ToSutVector()                const { return m_toSutVector;        } //!< Provides bit stream to shift into SUT
  const std::vector<NodeIdentifier>& ActiveRegistersIdentifiers() const { return m_activeRegisters;    } //!< Provides identifiers of active registers (those used for built up of "to SUT" vector)

  void  IgnorePendingState (bool ignorePendingState) { m_ignorePendingState = ignorePendingState; } //!< Sets whether or not should ignore pending state while traversing the tree

  void Reset()
  {
    m_toSutVector.Clear();
    m_activeRegisters.clear();
  }

  // ---------------- Private  Fields
  //
  private:
  BinaryVector                m_toSutVector;                //!< Collects chain of active registers values
  std::vector<NodeIdentifier> m_activeRegisters;            //!< Collects active registers when traversing for creating "to sut" vector
  bool                        m_ignorePendingState = false; //!< When true all active paths are visited (pending or not)
};
//
//  End of ToSutVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined TOSUTVISITOR_H__F4EB9DAF_F1CB_4A62_2CB6_DA151ADF18A8__INCLUDED_

//===========================================================================
// End of ToSutVisitor.hpp
//===========================================================================
