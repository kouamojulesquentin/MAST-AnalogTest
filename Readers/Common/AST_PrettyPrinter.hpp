//===========================================================================
//                           AST_AST_PrettyPrinter.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_PrettyPrinter.hpp
//!
//! Declares AST_PrettyPrinter class
//!
//===========================================================================

#ifndef AST_PRETTYPRINTER_H__3E520FCA_560A_44AF_5085_A5DE42AD51FD__INCLUDED_
  #define AST_PRETTYPRINTER_H__3E520FCA_560A_44AF_5085_A5DE42AD51FD__INCLUDED_

#include "AST_Visitor.hpp"
#include <string>
#include <sstream>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
class AST_Node;
class AST_NamedNode;
class AST_ParentNode;

//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree
//!
class AST_PrettyPrinter final : public AST_Visitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~AST_PrettyPrinter() = default;
  AST_PrettyPrinter()  = default;

  virtual void Visit_Module       (AST_Module*       module)       override;
  virtual void Visit_ScanRegister (AST_ScanRegister* scanRegister) override;

  std::string PrettyPrint() const { return m_os.str(); } //!< Returns currently visited nodes representation

  //! Returns textual model representation starting from a "top" node
  //!
  static std::string PrettyPrint(AST_Node* topNode);

  // ---------------- Private  Methods
  //
  private:
  friend class HierarchyInserter; //!< Helper class to insert hierarchy open/close sequences
  using pos_type = std::ostringstream::pos_type;

  void AlignOnNewLine  (pos_type targetPos);
  void AlignRelativeTo (pos_type refPos, pos_type targetPos);

  std::ostringstream& StreamDepth()
  {
    m_os << std::string(2u * m_depth, ' ');
    return m_os;
  }

  void StreamNodeHeader (std::experimental::string_view kind, const AST_NamedNode*  node,       std::experimental::string_view notes);
  void StreamParentNode (std::experimental::string_view kind, const AST_ParentNode* parentNode, std::experimental::string_view notes);

  // ---------------- Private  Fields
  //
  private:
  uint32_t           m_depth    = 0u;   //!< Current nodes tree depth
  std::ostringstream m_os;              //!< Stream to build up a representation of visited nodes
  pos_type           m_startPos = 0;    //!< Position, in stream, of first character of current line
  bool               m_first    = true; //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of AST_PrettyPrinter class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined AST_PRETTYPRINTER_H__3E520FCA_560A_44AF_5085_A5DE42AD51FD__INCLUDED_

//===========================================================================
// End of AST_PrettyPrinter.hpp
//===========================================================================
