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
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_Value.hpp"

#include "Utility.hpp"

#include <sstream>

using std::vector;
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
  StreamDepth() << node->KindName() << " " << node->Name();

  if (!notes.empty())
  {
    m_os << notes;
  }
}
//
//  End of StreamNodeHeader
//---------------------------------------------------------------------------


//! Streams children (not parent) node
//!
//! @param node   Node to stream textual representation
//!
void AST_PrettyPrinter::StreamSimpleNode(const AST_SimpleNode* node)
{
  if (node != nullptr)
  {
    StreamDepth() << node->KindName() << " " << node->AsText() << ";\n";
  }
}
//
//  End of StreamSimpleNode
//---------------------------------------------------------------------------



//! Appends content of a Module node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Module (AST_Module* module)
{
  StreamNodeHeader(module, "");

  HierarchyInserter hierarchyInserter(*this);

  StreamSimpleNodes (module->Attributes());
  StreamSimpleNodes (module->Parameters());
  StreamSimpleNodes (module->LocalParameters());

  AcceptNode        (module->ScanInPort());
  AcceptNode        (module->ScanOutPort());
  AcceptNodes       (module->UndispatchedChildren());
  AcceptNodes       (module->ScanRegisters());
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

    StreamSimpleNodes(attributes);

    AcceptNodes(undispatched);
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

  StreamSimpleNode (scanRegister->ScanInSource());
  StreamSimpleNode (scanRegister->ResetValue());
  AcceptNodes      (scanRegister->UndispatchedChildren());
}
//
//  End of: AST_PrettyPrinter::Visit_ScanRegister
//---------------------------------------------------------------------------


//! Appends content of a Source node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Source (AST_Source* source)
{
  StreamSimpleNode(source);
}
//
//  End of: AST_PrettyPrinter::Visit_Source
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_PrettyPrinter.cpp
//===========================================================================
