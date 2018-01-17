//===========================================================================
//                           PrettyPrinter.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PrettyPrinter.hpp
//!
//! Declares PrettyPrinter class
//!
//===========================================================================

#ifndef PRETTYPRINTER_H__4F4E33D0_FA7_4FAA_86BE_FFC320DECD62__INCLUDED_
  #define PRETTYPRINTER_H__4F4E33D0_FA7_4FAA_86BE_FFC320DECD62__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <string>
#include <sstream>
#include <memory>
#include <experimental/string_view>

namespace mast
{
class BinaryVector;
class PathSelector;

//! Options of Pretty printer
//!
enum class PrettyPrinterOptions
{
  None                   = 0,
  Verbose                = 1 << 0, //!< To report whether a node is pending, has conditioner, its priority, is ignored for path specification, protocol name for access interface, all values for registers
  DisplayValueAuto       = 1 << 1, //!< To report Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
  ShowNodesIdentifier    = 1 << 2, //!< To report nodes identifier
  ShowProtocol           = 1 << 3, //!< To report Protocol name of an access interface
  ShowSelectionState     = 1 << 4, //!< To report whether a node is selected or active
  ShowSelectionValue     = 1 << 5, //!< To report which selector value select the nodes
  ShowSelectorProperties = 1 << 6, //!< To report selector properties (kind, can_select_none, inverted_bits, reverted_order...)
  ShowSelectorTables     = 1 << 7, //!< To report path selector select/deselect tables (when table based) ==> Need ShowSelectorProperties
  ShowNodeIsIgnored      = 1 << 8, //!< To report parent node that are ignored when defining a node path

  Default            = ShowNodesIdentifier,
  Std                = Verbose | DisplayValueAuto | ShowNodesIdentifier,
  Parser_debug       = ShowSelectorProperties | ShowProtocol | ShowNodeIsIgnored | ShowProtocol | ShowNodesIdentifier,
  Parser_debug_no_id = ShowSelectorProperties | ShowProtocol | ShowNodeIsIgnored | ShowProtocol,
  All                =   Verbose
                       | DisplayValueAuto
                       | ShowNodesIdentifier
                       | ShowProtocol
                       | ShowSelectionState
                       | ShowSelectionValue
                       | ShowSelectorProperties
                       | ShowSelectorTables,
};

//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree
//!
class MAST_CORE_EXPORT PrettyPrinter : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~PrettyPrinter() = default;
  PrettyPrinter()  = default;
  PrettyPrinter(PrettyPrinterOptions options);

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  std::string PrettyPrint()   const { return m_os.str();      } //!< Returns currently visited nodes representation
  bool        IsVerbose()     const { return m_verbose;       } //!< Returns whether verbose mode is active
  bool        UseAutoFormat() const { return m_useAutoFormat; } //!< Returns true if pretty printer is configured to print Register values as hexadecimal string

  void SetVerbose    (bool verbose)       { m_verbose       = verbose;       } //!< Changes verbosity mode
  void UseAutoFormat (bool useAutoFormat) { m_useAutoFormat = useAutoFormat; } //!< Sets whether pretty printer is configured to print Register values as hexadecimal string (except not complete nibbles that are displayed using binary format)


  //! Returns textual model representation starting from a "top" node
  //!
  static std::string PrettyPrint(SystemModelNode& topNode, PrettyPrinterOptions options = PrettyPrinterOptions::Default);

  //! Returns textual model representation starting from a "top" node
  //!
  static std::string PrettyPrint(std::shared_ptr<SystemModelNode> topNode, PrettyPrinterOptions options = PrettyPrinterOptions::Default);


  // ---------------- Private  Methods
  //
  private:
  using pos_type = std::ostringstream::pos_type;

  void PrintChildren (const ParentNode& parentNode);

  void AlignOnNewLine  (pos_type targetPos);
  void AlignRelativeTo (pos_type refPos, pos_type targetPos);

  void StreamDepth()
  {
    m_os << std::string(m_depth, ' ');
  }

  void StreamBinaryVector (std::experimental::string_view name, const BinaryVector&    bits);
  void StreamNodeCommon   (const SystemModelNode&         node);
  void StreamSelector     (const PathSelector&            selector);
  void StreamNodeHeader   (std::experimental::string_view type, const SystemModelNode& node,       std::experimental::string_view notes = "");
  void StreamParentNode   (std::experimental::string_view type, const ParentNode&      parentNode, std::experimental::string_view notes = "");

  // ---------------- Private  Fields
  //
  private:
  uint32_t                      m_depth                  = 0u;    //!< Current nodes tree depth
  std::shared_ptr<PathSelector> m_selector;                       //!< This is used to tell when a child of a linker is selected/active
  uint32_t                      m_childId                = 0u;    //!< When m_selector is valid, it tells what endpoint id is currently beeing "Pretty Printed"
  std::ostringstream            m_os;                             //!< Stream to build up a representation of visited system model nodes
  bool                          m_processingSelector     = false; //!< When true, we are visiting a path selector (while visiting a linker)
  pos_type                      m_startPos               = 0;     //!< Position, in stream, of first character of current line
  bool                          m_useAutoFormat          = false; //!< When true, register values are displayed as hexadecimal string if large enough and not complete nibble as binary
  bool                          m_verbose                = false; //!< When true, more information are printed
  bool                          m_showNodeId             = true;  //!< When true SystemModel nodes identifier are displayed
  bool                          m_showProtocol           = false; //!< When true, protocol kind is displayed for AccessInterface
  bool                          m_showSelectionState     = false; //!< When true, selected/active linker node are reported as so
  bool                          m_showSelectionValue     = false; //!< When true, for all linker children nodes, their selection state is reported
  bool                          m_showSelectorProperties = false; //!< When true, selector options are reported
  bool                          m_showNodeIsIgnored      = false; //!< When true, it is reported if node name is ignored in path composition
  bool                          m_showSelectorTables     = false; //!< When true, Select/Deselect tables of tables based PathSelector are reported
  bool                          m_first                  = true;  //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of PrettyPrinter class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined PRETTYPRINTER_H__4F4E33D0_FA7_4FAA_86BE_FFC320DECD62__INCLUDED_
//===========================================================================
// End of PrettyPrinter.hpp
//===========================================================================
