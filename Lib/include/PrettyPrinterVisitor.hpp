//===========================================================================
//                           PrettyPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PrettyPrinterVisitor.hpp
//!
//! Declares PrettyPrinterVisitor class
//!
//===========================================================================


#ifndef PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_
  #define PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <string>
#include <sstream>
#include <experimental/string_view>

namespace mast
{

//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree
//!
class DLL_EXPORT PrettyPrinterVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~PrettyPrinterVisitor() = default;
  PrettyPrinterVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitTap             (Tap&             tap)             override;

  std::string GetPrettyPrint() const { return m_os.str(); }     //!< Returns currently visited nodes representation

  // ---------------- Private  Methods
  //
  private:

  void PrintChildren (const ParentNode& parentNode);

  void StreamDepth()
  {
    m_os << std::string(m_depth * 2, '-');
  }

  void StreamNodeHeader(std::experimental::string_view type, const SystemModelNode& node);

  // ---------------- Private  Fields
  //
  private:
  uint32_t           m_depth = 0u; //!< Current nodes tree depth
  std::ostringstream m_os;         //!< Stream to build up a representation of visited system model nodes
};
//
//  End of PrettyPrinterVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_

//===========================================================================
// End of PrettyPrinterVisitor.hpp
//===========================================================================
