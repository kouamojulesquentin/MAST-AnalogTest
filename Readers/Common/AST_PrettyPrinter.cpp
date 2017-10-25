//===========================================================================
//                           AST_PrettyPrinter.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_PrettyPrinter.cpp
//!
//! Implements class AST_PrettyPrinter
//!
//===========================================================================

#include "AST_PrettyPrinter.hpp"
#include "AST_Module.hpp"
#include "AST_ScanRegister.hpp"

using std::experimental::string_view;
using namespace Parsers;


//! Adds spaces to force next insertion point to be at target position relative
//! to reference position
//!
//! @param refPos       Reference position
//! @param targetPos    Target position relative to refPos
//!
void AST_PrettyPrinter::AlignRelativeTo (pos_type refPos, pos_type targetPos)
{
  auto curPos      = m_os.tellp();
  auto startLength = curPos - refPos;

  if (startLength < targetPos)
  {
    m_os << string(targetPos - startLength, ' ');
  }
}
//
//  End of: AST_PrettyPrinter::AlignRelativeTo
//---------------------------------------------------------------------------


//! Inserts new line and align position on target position on newly added line
//!
//! @param targetPos  Position set after adding a new line
//!
void AST_PrettyPrinter::AlignOnNewLine (pos_type targetPos)
{
  m_os << std::endl;
  m_os << string(targetPos, ' ');
}
//
//  End of: AST_PrettyPrinter::AlignOnNewLine
//---------------------------------------------------------------------------


//! Returns textual model representation starting from a "top" node
//!
//! @param topNode    Node from which pretty is created
//!
string AST_PrettyPrinter::PrettyPrint (AST_Node* topNode)
{
  AST_PrettyPrinter printer;
  topNode->Accept(printer);
  return printer.PrettyPrint();
}
//
//  End of: AST_PrettyPrinter::PrettyPrint
//---------------------------------------------------------------------------


//! Streams node common information: identifier, name and type
//!
//! @param kind   Text representation of the node type
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void AST_PrettyPrinter::StreamNodeHeader(string_view kind, const AST_NamedNode* node, string_view notes)
{
  if (!m_first)
  {
    m_os << std::endl;
  }

  m_startPos = m_os.tellp();
  StreamDepth();

  m_os << kind << " " << node->Name();

  if (!notes.empty())
  {
    m_os << ", " << notes;
  }

  m_first = false;
}

//! Appends content of parent node in text representation and visits
//! sub-nodes
//!
//! @param kind   Text representation of the node type
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void AST_PrettyPrinter::StreamParentNode (std::experimental::string_view kind, const AST_ParentNode* parentNode, string_view notes)
{
  StreamNodeHeader(kind, parentNode, notes);
  m_os << "\n";
  StreamDepth() << "{\n";

//+  PrintChildren(parentNode);

  StreamDepth() << "}\n";
}
//
//  End of: AST_PrettyPrinter::StreamParentNode
//---------------------------------------------------------------------------

//! Appends content of a Module node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Module (AST_Module* module)
{
  StreamParentNode("Module", module, "");
}
//
//  End of: AST_PrettyPrinter::Visit_Module
//---------------------------------------------------------------------------


//! Appends content of a ScanRegister node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_ScanRegister (AST_ScanRegister* scanRegister)
{
  StreamParentNode("ScanRegister", scanRegister, "");
}
//
//  End of: AST_PrettyPrinter::Visit_ScanRegister
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_PrettyPrinter.cpp
//===========================================================================
