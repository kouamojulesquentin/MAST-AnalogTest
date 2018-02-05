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

#include "AST_AccessLink.hpp"
#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_BsdlInstructionRef.hpp"
#include "AST_EnumRef.hpp"
#include "AST_Instance.hpp"
#include "AST_FileRef.hpp"
#include "AST_Module.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Network.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanInterfaceRef.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_ConcatNumber.hpp"

#include "Utility.hpp"

#include <sstream>
#include <algorithm>

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
  class HierarchyInserter final
  {
    public:
    ~HierarchyInserter()
    {
      if (m_simpleHierarchy)
      {
        m_printer.m_os << " }";
      }
      else
      {
        --m_printer.m_depth;
        m_printer.m_os << "\n";
        m_printer.StreamDepth() << "}";
      }
    }

    HierarchyInserter(AST_PrettyPrinter& printer, bool simpleHierarchy = false)
      : m_printer         (printer)
      , m_simpleHierarchy (simpleHierarchy)
    {
      if (m_simpleHierarchy)
      {
        m_printer.m_os << " { ";
      }
      else
      {
        m_printer.m_os << "\n";
        m_printer.StreamDepth() << "{";
        ++m_printer.m_depth;
      }
    }

    private:
    AST_PrettyPrinter& m_printer;
    bool               m_simpleHierarchy = false;
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
std::ostringstream& AST_PrettyPrinter::StreamNodeHeader(const AST_NamedNode* node, string_view notes)
{
  if (m_os.tellp() != 0)
  {
    m_os << "\n";
  }

  StreamDepth() << node->KindName() << " " << node->Name();

  if (!notes.empty())
  {
    m_os << notes;
  }
  return m_os;
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
    if (m_os.tellp() != 0)
    {
      m_os << "\n";
    }
    StreamDepth() << node->KindName() << " " << node->AsText() << ";";
  }
}
//
//  End of StreamSimpleNode
//---------------------------------------------------------------------------


//! Appends content of a AccessLink node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_AccessLink (AST_AccessLink* accessLink)
{
  StreamNodeHeader(accessLink, " Of ") << accessLink->TypeAsText();

  HierarchyInserter hierarchyInserter(*this);

  StreamSimpleNode (accessLink->BSDL());

  // ---------------- BSDL Instruction reference
  //
  const auto& bsdlInstructionsRef = accessLink->BsdlInstructionsRef();
  for (const auto bsdlInstruction : bsdlInstructionsRef)
  {
    m_os << "\n";
    StreamDepth() << bsdlInstruction->Name();
    HierarchyInserter bsdlInstructionHierarchyInserter(*this);

    const auto& scanInterfacesRef = bsdlInstruction->ScanInterfacesRef();
    for (const auto scanInterfaceRef : scanInterfacesRef)
    {
      m_os << "\n";
      StreamDepth() << "ScanInterface";

      const auto& interfacesNames       = scanInterfaceRef->ScanInterfaceNames();
      const auto  multipleScanInterface = interfacesNames.size() > 1u;

      HierarchyInserter scanInterfaceHierarchyInserter(*this, !multipleScanInterface);

      for (const auto& interfaceName : interfacesNames)
      {
        const auto  instanceRef       = std::get<0>(interfaceName);
        const auto& scanInterfaceName = std::get<1>(interfaceName);

        if (multipleScanInterface)
        {
          m_os << "\n";
          StreamDepth();
        }

        if (instanceRef != nullptr)
        {
          m_os << instanceRef->AsText() << ".";
        }

        m_os << scanInterfaceName << ";";
      }
    }
  }

  AcceptNodes (accessLink->UndispatchedChildren());
}
//
//  End of: AST_PrettyPrinter::Visit_AccessLink
//---------------------------------------------------------------------------



//! Appends Alias statement
//!
void AST_PrettyPrinter::Visit_Alias (AST_Alias* alias)
{
  // ---------------- Name
  //
  StreamNodeHeader(alias, " = ");

  // ---------------- Signals
  //
  const auto& signals = alias->Signals();
  m_os << AST_SimpleNode::AsText(signals, ", ");

  // ---------------- Optional parts
  //
  const auto  accessTogether = alias->AccessTogether();
  const auto  applyEndState  = alias->ApplyEndState();
  const auto  enumRef        = alias->EnumRef();
  const auto& attributes     = alias->Attributes();

  auto needScope = (enumRef != nullptr) || accessTogether || (applyEndState != nullptr) || !attributes.empty();
  if (needScope)
  {
    HierarchyInserter hierarchyInserter(*this);

    StreamSimpleNodes(attributes);

    if (enumRef != nullptr)
    {
      StreamSimpleNode(enumRef);
    }

    if (accessTogether)
    {
      StreamDepth(INSERT_NEW_LINE_BEFORE) << "AccessTogether;";
    }

    if (applyEndState != nullptr)
    {
      StreamDepth(INSERT_NEW_LINE_BEFORE) << "iApplyEndState " << applyEndState->AsText() << ";";
    }
  }
  else
  {
    m_os << ";";
  }
}
//
//  End of: AST_PrettyPrinter::Visit_Aliases
//---------------------------------------------------------------------------


//! Appends content of a Instance node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Instance (AST_Instance* instance)
{
  auto moduleIdentifier = instance->ModuleIdentifier();

  StreamNodeHeader(instance, " Of ") << moduleIdentifier->AsText();

  HierarchyInserter hierarchyInserter(*this);

  StreamSimpleNodes (instance->Attributes());
  StreamSimpleNodes (instance->Parameters());

  AcceptNodes       (instance->InputPorts());
  AcceptNodes       (instance->UndispatchedChildren());
}
//
//  End of: AST_PrettyPrinter::Visit_Instance
//---------------------------------------------------------------------------


//! Appends content of a Module node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_Network (AST_Network* network)
{
  // ---------------- Get sorted namespaces
  //
  auto namespaces = network->NamespacesWithDefinedModules();
  auto comparator = [](const auto lhs, const auto rhs) { return lhs->Name() < rhs->Name(); };

  std::sort(namespaces.begin(), namespaces.end(), comparator);

  // ---------------- Pretty print modules in each namespaces
  //
  for (const auto namespaceNode : namespaces)
  {
    if (m_os.tellp() != 0)
    {
      m_os << "\n";
    }
    StreamDepth() << namespaceNode->KindName();
    const auto name = namespaceNode->Name();
    if (!name.empty())
    {
      m_os << " " << name;
    }
    m_os << ";";

    // ---------------- Get sorted modules
    //
    auto modules = network->ModulesInNamespace(namespaceNode);
    std::sort(modules.begin(), modules.end(), comparator);

    for (const auto module : modules)
    {
      module->Accept(*this);
    }
  }
}
//
//  End of: AST_PrettyPrinter::Visit_Network
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

  if (module->HasAccessLink())
  {
    AcceptNode (module->AccessLink());
  }

  AcceptNodes (module->Aliases());
  AcceptNodes (module->ScanInterfaces());
  AcceptNodes (module->ScanInPorts());
  AcceptNodes (module->ScanOutPorts());
  AcceptNodes (module->DataInPorts());
  AcceptNodes (module->DataOutPorts());
  AcceptNodes (module->UndispatchedChildren());
  AcceptNodes (module->Instances());
  AcceptNodes (module->ScanMuxes());
  AcceptNodes (module->ScanRegisters());
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
  const auto  portSource   = port->Source();

  if (port->IsKind(Kind::InputPort))
  {
    CHECK_PARAMETER_NOT_NULL(portSource, "InputPort is expected to have valid port source");

    m_os << " = " << portSource->AsText() << ";";
  }
  else if ((portSource != nullptr) || !attributes.empty() || !undispatched.empty())
  {
    auto totalItemsCount = attributes.size() + undispatched.size();
    if (portSource != nullptr)
    {
      ++totalItemsCount;
    }

    auto simpleHierarchy = totalItemsCount == 1u;

    if (simpleHierarchy)
    {
      HierarchyInserter hierarchyInserter(*this, simpleHierarchy);

      if (portSource != nullptr)
      {
        m_os << portSource->KindName() << " " << portSource->AsText() << ";";
      }
      else if (!attributes.empty())
      {
        auto node = attributes.front();
        m_os << node->KindName() << " " << node->AsText() << ";";
      }
      else
      {
        auto node = undispatched.front();

        AcceptNode(node);
      }
    }
    else
    {
      HierarchyInserter hierarchyInserter(*this);

      StreamSimpleNodes(attributes);
      AcceptNodes(undispatched);
    }
  }
  else
  {
    m_os << ";";
  }
}
//
//  End of: AST_PrettyPrinter::Visit_Port
//---------------------------------------------------------------------------



//! Appends content of a ScanInterface node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_ScanInterface (AST_ScanInterface* scanInterface)
{
  StreamNodeHeader(scanInterface, "");

  HierarchyInserter hierarchyInserter(*this);

  StreamSimpleNodes (scanInterface->Attributes());

  AcceptNodes       (scanInterface->Ports());
}
//
//  End of: AST_PrettyPrinter::Visit_ScanInterface
//---------------------------------------------------------------------------


//! Appends content of a ScanMux node in text representation and visits
//! sub-nodes
void AST_PrettyPrinter::Visit_ScanMux (AST_ScanMux* scanMux)
{
  StreamNodeHeader(scanMux, " SelectedBy ");

  const auto& selectors = scanMux->Selectors();
  m_os << AST_SimpleNode::AsText(selectors, ", ");

  HierarchyInserter hierarchyInserter(*this);

  const auto& selections = scanMux->Selections();
  for (const auto& selection : selections)
  {
    m_os << "\n";
    StreamDepth() << selection->AsText() << ";";
  }
}
//
//  End of: AST_PrettyPrinter::Visit_ScanMux
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
