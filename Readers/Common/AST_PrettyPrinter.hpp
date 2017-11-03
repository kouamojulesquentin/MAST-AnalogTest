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
#include <vector>
#include <sstream>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
class AST_Node;
class AST_SimpleNode;
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
  virtual void Visit_Port         (AST_Port*         port)         override;
  virtual void Visit_ScanRegister (AST_ScanRegister* scanRegister) override;
  virtual void Visit_Source       (AST_Source*       source)       override;

  std::string PrettyPrint() const { return m_os.str(); } //!< Returns currently visited nodes representation

  //! Returns textual model representation starting from a "top" node
  //!
  static std::string PrettyPrint(AST_Node* topNode);

  // ---------------- Private  Methods
  //
  private:
  friend class HierarchyInserter; //!< Helper class to insert hierarchy open/close sequences
  using pos_type = std::ostringstream::pos_type;

  std::ostringstream& StreamDepth()
  {
    m_os << std::string(2u * m_depth, ' ');
    return m_os;
  }

  void StreamNodeHeader  (const AST_NamedNode* node, std::experimental::string_view notes);
  void StreamSimpleNode  (const AST_SimpleNode*               node);
  void StreamSimpleNodes (const std::vector<AST_SimpleNode*>& nodes);

  template<typename T>
  void StreamSimpleNodes(const std::vector<T*>& nodes)
  {
    for (const auto node : nodes)
    {
      StreamSimpleNode(node);
    }
  }

  template<typename T>
  void AcceptNode (T* node)
  {
    if (node != nullptr)
    {
      node->Accept(*this);
    }
  }

  template<typename T>
  void AcceptNodes (const std::vector<T*>& nodes)
  {
    for (const auto node : nodes)
    {
      AcceptNode(node);
    }
  }


  // ---------------- Private  Fields
  //
  private:
  std::ostringstream m_os;           //!< Stream to build up a representation of visited nodes
  uint32_t           m_depth = 0u;   //!< Current nodes tree depth
  bool               m_first = true; //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of AST_PrettyPrinter class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined AST_PRETTYPRINTER_H__3E520FCA_560A_44AF_5085_A5DE42AD51FD__INCLUDED_

//===========================================================================
// End of AST_PrettyPrinter.hpp
//===========================================================================
