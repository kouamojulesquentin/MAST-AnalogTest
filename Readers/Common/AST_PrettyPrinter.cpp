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
#include "AST_Attribute.hpp"
#include "AST_Module.hpp"
#include "AST_Port.hpp"
#include "AST_Value.hpp"
#include "AST_Source.hpp"
#include "AST_ScanRegister.hpp"
#include "Utility.hpp"

#include <sstream>

using std::string;
using std::experimental::string_view;
using namespace Parsers;

namespace Parsers
{

  //! Inserts hierarchy open/close sequence and increases depth
  //!
  //! @note Do 1st half of the job in constructor and the 2nd one in the destructor
  //!
  struct HierarchyInserter final
  {
    ~HierarchyInserter()
    {
      --m_printer.m_depth;
      m_printer.StreamDepth() << "}\n";
    }

    HierarchyInserter(AST_PrettyPrinter& printer)
      : m_printer(printer)
    {
      m_printer.m_os << "\n";
      m_printer.StreamDepth() << "{\n";
      ++m_printer.m_depth;
    }

    AST_PrettyPrinter& m_printer;
  };


} // End of Parsers namespace

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
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void AST_PrettyPrinter::StreamNodeHeader(const AST_NamedNode* node, string_view notes)
{
  m_startPos = m_os.tellp();
  StreamDepth();

  m_os << node->KindName() << " " << node->Name();

  if (!notes.empty())
  {
    m_os << notes;
  }
}

//! Appends content of parent node in text representation and visits
//! sub-nodes
//!
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void AST_PrettyPrinter::StreamParentNode (const AST_ParentNode* parentNode, string_view notes)
{
  StreamNodeHeader(parentNode, notes);
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
  StreamNodeHeader(module, "");

  HierarchyInserter hierarchyInserter(*this);

  // ---------------- ScanInPort
  //
  const auto scanInPort = module->ScanInPort();
  if (scanInPort != nullptr)
  {
    scanInPort->Accept(*this);
  }

  for (const auto& node : module->UndispatchedChildren())
  {
    if (node != nullptr)
    {
      node->Accept(*this);
    }
  }

  // ---------------- Scan registers
  //
  for (const auto& scanRegister : module->ScanRegisters())
  {
    CHECK_VALUE_NOT_NULL(scanRegister, "Unexpected hold nullptr ScanRegister in module");

    scanRegister->Accept(*this);
  }

}
//
//  End of: AST_PrettyPrinter::Visit_Module
//---------------------------------------------------------------------------


//! Appends content of a Port node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Port (AST_Port* port)
{
  StreamNodeHeader(port, "");

  const auto& attributes   = port->Attributes();
  const auto& undispatched = port->UndispatchedChildren();

  if (!attributes.empty() || !undispatched.empty())
  {
    HierarchyInserter hierarchyInserter(*this);

    for (const auto attribute : attributes)
    {
      CHECK_VALUE_NOT_NULL(attribute, "Unexpected nullptr attribute in port");

      StreamDepth() << attribute->AsText() << ";\n";
    }

    // ---------------- Others (unprocessed children)
    //
    for (const auto node : undispatched)
    {
      if (node != nullptr)
      {
        node->Accept(*this);
      }
    }
  }
  else
  {
    m_os << ";\n";
  }
}
//
//  End of: AST_PrettyPrinter::Visit_Port
//---------------------------------------------------------------------------


//! Appends content of a ScanRegister node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_ScanRegister (AST_ScanRegister* scanRegister)
{
  StreamNodeHeader(scanRegister, "");

  HierarchyInserter hierarchyInserter(*this);

  // ---------------- ScanInSource
  //
  auto scanInSource = scanRegister->ScanInSource();
  if (scanInSource != nullptr)
  {
    StreamDepth() << scanInSource->KindName() << " " << scanInSource->AsText() << ";\n";
  }

  // ---------------- ResetValue
  //
  auto resetValue = scanRegister->ResetValue();
  if (resetValue != nullptr)
  {
    StreamDepth() << resetValue->KindName() << " " << resetValue->AsText() << ";\n";
  }

  // ---------------- Others (unprocessed children)
  //
  for (const auto node : scanRegister->UndispatchedChildren())
  {
    if (node != nullptr)
    {
      node->Accept(*this);
    }
  }
}
//
//  End of: AST_PrettyPrinter::Visit_ScanRegister
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_PrettyPrinter.cpp
//===========================================================================
