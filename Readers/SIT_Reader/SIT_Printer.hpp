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
class BinaryVector;
class PathSelector;


//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree
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

  std::string SIT_Text()      const { return m_os.str();      } //!< Returns currently visited nodes representation using SIT files syntax
  bool        IsVerbose()     const { return m_verbose;       } //!< Returns whether verbose mode is active
  bool        UseAutoFormat() const { return m_useAutoFormat; } //!< Returns true if pretty printer is configured to print Register values as hexadecimal string

  void SetVerbose    (bool verbose)       { m_verbose       = verbose;       } //!< Changes verbosity mode
  void UseAutoFormat (bool useAutoFormat) { m_useAutoFormat = useAutoFormat; } //!< Sets whether pretty printer is configured to print Register values as hexadecimal string (except not complete nibbles that are displayed using binary format)


  //! Returns model representation starting from a "top" node using SIT files syntax
  //!
  static std::string MakeSIT(SystemModelNode& topNode);

  //! Returns model representation starting from a "top" node using SIT files syntax
  //!
  static std::string MakeSIT(std::shared_ptr<SystemModelNode> topNode);


  // ---------------- Private  Methods
  //
  private:
  using pos_type = std::ostringstream::pos_type;

  void PrintChildren (const ParentNode& parentNode);

  void AlignOnNewLine  (pos_type targetPos);
  void AlignRelativeTo (pos_type refPos, pos_type targetPos);

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
  uint32_t                      m_depth                  = 0u;    //!< Current nodes tree depth
  std::shared_ptr<PathSelector> m_selector;                       //!< This is used to tell when a child of a linker is selected/active
  uint32_t                      m_childId                = 0u;    //!< When m_selector is valid, it tells what endpoint id is currently beeing "SIT_ Printed"
  std::ostringstream            m_os;                             //!< Stream to build up a representation of visited system model nodes
  bool                          m_processingSelector     = false; //!< When true, we are visiting a path selector (while visiting a linker)
  pos_type                      m_startPos               = 0;     //!< Position, in stream, of first character of current line
  bool                          m_useAutoFormat          = false; //!< When true, register values are displayed as hexadecimal string if large enough and not complete nibble as binary
  bool                          m_verbose                = false; //!< When true, more information are printed
  bool                          m_showProtocol           = false; //!< When true, protocol kind is displayed for AccessInterface
  bool                          m_showSelectionState     = false; //!< When true, selected/active linker node are reported as so
  bool                          m_showSelectionValue     = false; //!< When true, for all linker children nodes, their selection state is reported
  bool                          m_showSelectorProperties = false; //!< When true, selector options are reported
  bool                          m_ShowNodeIsIgnored      = false; //!< When true, it is reported if node name is ignored in path composition
  bool                          m_first                  = true;  //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of SIT_Printer class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SIT_PRINTER_H__73C2B85A_6AA6_4A9C_37B3_34CE8DB75436__INCLUDED_
//===========================================================================
// End of SIT_Printer.hpp
//===========================================================================
