//===========================================================================
//                           SIT_Printer.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SIT_Printer.hpp
//!
//! Declares SIT_Printer class
//!
//===========================================================================

#ifndef SIT_PRINTER_H__73C2B85A_6AA6_4A9C_37B3_34CE8DB75436__INCLUDED_
  #define SIT_PRINTER_H__73C2B85A_6AA6_4A9C_37B3_34CE8DB75436__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <string>
#include <sstream>
#include <memory>
#include <experimental/string_view>

namespace mast
{
//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree using SIT file syntax/grammar
//!
class SIT_Printer final: public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~SIT_Printer() = default;
  SIT_Printer()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitAccessInterfaceTranslator (AccessInterfaceTranslator&accessInterfaceTranslator) override;
  
  std::string SIT_Text() const { return m_os.str(); } //!< Returns currently visited nodes representation using SIT files syntax


  //! Returns model representation starting from a "top" node using SIT files syntax
  //!
  static std::string MakeSIT(SystemModelNode& topNode);

  //! Returns model representation starting from a "top" node using SIT files syntax
  //!
  static std::string MakeSIT(std::shared_ptr<SystemModelNode> topNode);

  // ---------------- Private  Methods
  //
  private:
  void PrintChildren (const ParentNode& parentNode);

  std::ostringstream& StreamDepth()
  {
    m_os << std::string(m_depth * 2u, ' ');
    return m_os;
  }

  std::ostringstream& StreamNodeName (std::ostringstream& os, const SystemModelNode& node);

  void StreamNodeHeader (std::experimental::string_view type, const SystemModelNode& node,       std::experimental::string_view notes = "");
  void StreamParentNode (std::experimental::string_view type, const ParentNode&      parentNode, std::experimental::string_view notes = "");

  // ---------------- Private  Fields
  //
  private:
  std::ostringstream m_os;           //!< Stream to build up a representation of visited system model nodes
  uint32_t           m_depth = 0u;   //!< Current nodes tree depth
  bool               m_first = true; //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of SIT_Printer class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SIT_PRINTER_H__73C2B85A_6AA6_4A9C_37B3_34CE8DB75436__INCLUDED_
//===========================================================================
// End of SIT_Printer.hpp
//===========================================================================
