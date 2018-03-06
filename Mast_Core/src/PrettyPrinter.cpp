//===========================================================================
//                           PrettyPrinter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PrettyPrinter.cpp
//!
//! Implements class PrettyPrinter
//!
//===========================================================================

#include "PrettyPrinter.hpp"
#include "SystemModelNodes.hpp"
#include "PathSelector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "RegistersAlias.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "T_2_T_TranslatorProtocol.hpp"
#include "T_2_E_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"

using std::string;
using std::shared_ptr;
using std::experimental::string_view;

using namespace mast;



//! Initializes with specified options
//!
PrettyPrinter::PrettyPrinter (PrettyPrinterOptions options)
  : m_useAutoFormat          (IsSet(options, PrettyPrinterOptions::DisplayValueAuto))
  , m_verbose                (IsSet(options, PrettyPrinterOptions::Verbose))
  , m_showNodeId             (IsSet(options, PrettyPrinterOptions::ShowNodesIdentifier))
  , m_showProtocol           (IsSet(options, PrettyPrinterOptions::ShowProtocol))
  , m_showSelectionState     (IsSet(options, PrettyPrinterOptions::ShowSelectionState))
  , m_showSelectionValue     (IsSet(options, PrettyPrinterOptions::ShowSelectionValue))
  , m_showSelectorProperties (IsSet(options, PrettyPrinterOptions::ShowSelectorProperties))
  , m_showNodeIsIgnored      (IsSet(options, PrettyPrinterOptions::ShowNodeIsIgnored))
  , m_showSelectorTables     (IsSet(options, PrettyPrinterOptions::ShowSelectorTables))
{
}
//
//  End of: PrettyPrinter::PrettyPrinter
//---------------------------------------------------------------------------




//! Adds spaces to force next insertion point to be at target position relative
//! to reference position
//!
//! @param refPos       Reference position
//! @param targetPos    Target position relative to refPos
//!
std::ostringstream& PrettyPrinter::AlignRelativeTo (pos_type refPos, pos_type targetPos)
{
  auto curPos      = m_os.tellp();
  auto startLength = curPos - refPos;

  if (startLength < targetPos)
  {
    m_os << string(targetPos - startLength, ' ');
  }
  return m_os;
}
//
//  End of: PrettyPrinter::AlignRelativeTo
//---------------------------------------------------------------------------


//! Inserts new line and align position on target position oin newly added line
//!
//! @param targetPos  Position set after adding a new line
//!
std::ostringstream& PrettyPrinter::AlignOnNewLine (pos_type targetPos)
{
  m_os << std::endl;
  m_os << string(targetPos, ' ');
  return m_os;
}
//
//  End of: PrettyPrinter::AlignOnNewLine
//---------------------------------------------------------------------------


//! Returns textual model representation starting from a "top" node
//!
//! @param topNode    Node from which graph is created
//! @param options    Printer options
//!
string PrettyPrinter::PrettyPrint (SystemModelNode& topNode, PrettyPrinterOptions options)
{
  PrettyPrinter printer(options);
  topNode.Accept(printer);
  return printer.PrettyPrint();
}
//
//  End of: PrettyPrinter::PrettyPrint
//---------------------------------------------------------------------------


//! Returns textual model representation starting from a "top" node
//!
//! @param topNode    Node from which graph is created
//! @param options    Printer options
//!
string PrettyPrinter::PrettyPrint (shared_ptr<SystemModelNode> topNode, PrettyPrinterOptions options)
{
  CHECK_PARAMETER_NOT_NULL(topNode, "Cannot 'pretty print' from nullptr");

  return PrettyPrint(*topNode, options);
}
//
//  End of: PrettyPrinter::PrettyPrint
//---------------------------------------------------------------------------



//! Prints register aliases of a ParentNode
//!
void PrettyPrinter::PrintAliases (const ParentNode& parentNode)
{
  if (parentNode.HasAliases())
  {
    SeparatorInserter separator(m_os, ", ");

    auto aliases = parentNode.RegistersAliases();
    for (const auto& alias : *aliases)
    {
      AlignOnNewLine(m_depth) << "Alias " << alias.Name() << ": ";

      separator.Reset();
      const auto& registers = alias.AliasedRegisters();

      for (const auto& slice : registers)
      {
        separator.Insert() << slice.reg->Name() << "[";

        const auto& range = slice.range;

        m_os << std::to_string(range.left);
        if (range.IsMultiBits())
        {
          m_os << ":" << std::to_string(range.right);
        }
        m_os << "]";
      }
    }
  }
}
//
//  End of: PrettyPrinter::PrintAliases
//---------------------------------------------------------------------------




//! Pretty print childrens of a parent node
//!
void PrettyPrinter::PrintChildren (const ParentNode& parentNode)
{
  // ---------------- Do support exceptions
  //
  auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
  AT_SCOPE_EXIT(restoreDepth);

  ++m_depth;

  // ---------------- Aliases
  //
  PrintAliases(parentNode);

  // ---------------- SystemModelNode children per se
  //
  auto pLinker  = dynamic_cast<const Linker*>(&parentNode);
  auto selector = pLinker ? pLinker->Selector() : nullptr;
  auto childId  = uint32_t(1u);

  auto child = parentNode.FirstChild();
  while (child)
  {
    m_selector = selector;
    m_childId  = childId;

    child->Accept(*this);
    child = child->NextSibling();

    ++childId;
  }
  m_selector = nullptr;
  m_childId  = 0;
}
//
//  End of: PrettyPrinter::PrintChildren
//---------------------------------------------------------------------------


//! Streams content of binary vector, prefixed with given name
//!
//! @param bits  Binary vector to print
//! @param name  Name given to the binary vector (can be empty)
//!
void PrettyPrinter::StreamBinaryVector (std::experimental::string_view name, const BinaryVector& bits)
{
  m_os << ", " << name;

  if (m_useAutoFormat)
  {
    m_os << bits.DataAsMixString(8u, "_", ":");
  }
  else
  {
    m_os << bits.DataAsBinaryString("_", ":");
  }
}
//
//  End of: PrettyPrinter::StreamBinaryVector
//---------------------------------------------------------------------------


//! Prints SystemModelNode data
//!
//! @param node   The node for which header is to be streamed
//!
void PrettyPrinter::StreamNodeCommon (const SystemModelNode& node)
{
  if (m_verbose)
  {
    m_os.setf(std::ios_base::boolalpha);
    m_os << ", pending: "         << node.IsPending();
    m_os << ", has_conditioner: " << node.HasConditioner();
    m_os << ", priority: "        << node.Priority();
  }
}
//
//  End of: PrettyPrinter::StreamNodeCommon
//---------------------------------------------------------------------------



//! Streams node common information: identifier, name and type
//!
//! @param type   Text representation of the node type
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void PrettyPrinter::StreamNodeHeader(string_view type, const SystemModelNode& node, string_view notes)
{
  if (!m_first)
  {
    m_os << std::endl;
  }

  m_startPos = m_os.tellp();
  StreamDepth();

  m_os << "[" << type << "]";

  if (m_showNodeId)
  {
    m_os << '(' << node.Identifier() << ") ";
  }


  AlignRelativeTo(m_startPos, 15u + m_depth);
  m_os << '"' << node.Name()       << '"';


  if (!notes.empty())
  {
    m_os << ", " << notes;
  }


  // ---------------- Display selection/active state(s)
  //
  if (m_selector && (m_showSelectionState || m_showSelectionValue))
  {
    bool isSelected         = m_selector->IsSelected(m_childId);
    bool isActive           = m_selector->IsActive(m_childId);
    bool showSelectionValue = m_showSelectionValue && !dynamic_cast<const Linker*>(&node);
    bool showSomething      = showSelectionValue || isActive || isSelected;

    if (showSomething)                      m_os << ", ", AlignRelativeTo(m_startPos, 30u + m_depth);
    if (showSelectionValue)                 m_os << ":" << m_selector->SelectionValue(m_childId).DataAsMixString(8, "", ":");
    if (m_showSelectionState && isSelected) m_os << ":S";
    if (m_showSelectionState && isActive)   m_os << ":A";
    if (showSomething)                      m_os << ":";
  }

  m_first = false;
}





//! Appends content of parent node in text representation and visits
//! sub-nodes
//!
//! @param type   Text representation of the node type
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void PrettyPrinter::StreamParentNode (std::experimental::string_view type, const ParentNode& parentNode, string_view notes)
{
  StreamNodeHeader(type, parentNode, notes);

  if (m_verbose || m_showNodeIsIgnored)
  {
    if (parentNode.IgnoreForNodePath())
    {
      m_os.setf(std::ios_base::boolalpha);
      m_os << ", ignore_in_path: true";
    }

    StreamNodeCommon(parentNode);
  }

  PrintChildren(parentNode);
}
//
//  End of: PrettyPrinter::StreamParentNode
//---------------------------------------------------------------------------


//! Streams PathSelector information
//!
//! @param selector   PathSelector to stream information for
//!
void PrettyPrinter::StreamSelector(const PathSelector& selector)
{
  m_os << std::endl;

  m_startPos = m_os.tellp();
  StreamDepth();

  m_os << ":Selector:";

  // ---------------- Stream associated VirtualRegister slice(s)
  //
  auto associatedRegisters = selector.AssociatedRegisters();
  if (associatedRegisters != nullptr)
  {
    auto first = true;
    for (const auto& registerSlice : *associatedRegisters)
    {
      auto reg    = registerSlice.reg;
      auto range  = registerSlice.range;

      if (first)
      {
        first = false;
        if (m_showNodeId && (associatedRegisters->SlicesCount() == 1u))  // Only show register id only when there is only one
        {
          m_os << '(' << reg->Identifier() << ") ";
        }
        AlignRelativeTo(m_startPos, 15u + m_depth);
        m_os << '"';
      }
      else
      {
        m_os << ':';    // Separate each register slice
      }

      m_os << reg->Name();
      if (reg->BitsCount() != range.Width())
      {
        m_os << '['  << range.left;
        if (!range.IsSingleBit())
        {
          m_os << ":" << range.right;
        }
        m_os << ']';
      }
    }
    m_os << '"';
  }

  if (m_showSelectorProperties)
  {
    m_os << ", kind: "            << selector.KindName();
    m_os << ", can_select_none: " << std::boolalpha << IsSet(selector.Properties(), SelectorProperty::CanSelectNone);
    m_os << ", inverted_bits: "   << std::boolalpha << IsSet(selector.Properties(), SelectorProperty::InvertedBits);
    m_os << ", reversed_order: "  << std::boolalpha << IsSet(selector.Properties(), SelectorProperty::ReverseOrder);

    if (m_showSelectorTables)
    {
      auto selector = std::dynamic_pointer_cast<DefaultTableBasedPathSelector>(m_selector);
      if (selector)
      {
        auto indent = string(m_depth, ' ');
        selector->StreamTable(m_os, indent, "Selection Table:",   selector->SelectTable());
        selector->StreamTable(m_os, indent, "Deselection Table:", selector->DeselectTable());
      }
    }
  }
}



//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void PrettyPrinter::VisitAccessInterface (AccessInterface& accessInterface)
{
  string note;

  if (m_verbose || m_showProtocol)
  {
    auto protocol = accessInterface.Protocol();
    note = "Protocol: ";
    if (!protocol) note += "Not set";
    else 
     {
     note += protocol->KindName();
     auto protocol_is_raw=std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(protocol);
     if (protocol_is_raw) 
      {
      note += "->" ;
      if (protocol_is_raw->ParentTranslator_is_set())
       note += protocol_is_raw->ParentTranslatorName();
      else note += "Not set";
      }
     }
  }

  StreamParentNode("Access_I", accessInterface, note);
}

//! Appends content of AccessInterfaceTranslator node in text representation and visits
//! sub-nodes
//!
void PrettyPrinter::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  string note;

  if (m_verbose || m_showProtocol)
  {
    auto protocol = accessInterfaceTranslator.Protocol();
    note = "Protocol: ";
    if (!protocol) note += "Not set";
    else 
     {
     note += protocol->KindName();
     auto protocol_is_t2t=std::dynamic_pointer_cast<T_2_T_TranslatorProtocol>(protocol);
     if (protocol_is_t2t) 
      {
      note += "->" ;
      if (protocol_is_t2t->ParentTranslator_is_set())
       note += protocol_is_t2t->ParentTranslatorName();
      else note += "Not set";
      }
     else
      {
     auto protocol_is_t2e=std::dynamic_pointer_cast<T_2_E_TranslatorProtocol>(protocol);
     if (protocol_is_t2e) 
       {
       note += "->" ;
       if (protocol_is_t2e->EventDomain_is_set())
        note += protocol_is_t2e->EventDomainRootNode()->Name();
       else note += "Not set";
       }
      } 
    }
  }
 
  StreamParentNode("Access_T", accessInterfaceTranslator, note);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void PrettyPrinter::VisitChain (Chain& chain)
{
  StreamParentNode("Chain", chain);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void PrettyPrinter::VisitLinker (Linker& linker)
{
  m_selector = linker.Selector();
  StreamNodeHeader("Linker", linker);

  if (m_verbose)
  {
    if (linker.IgnoreForNodePath())
    {
      m_os << ", ignore_in_path: true";
    }

    StreamNodeCommon(linker);
  }

  // ---------------- Deal with path selector
  //
  {
    auto restoreMembers = [this, initialValue = m_depth]()
    {
      this->m_depth              = initialValue;
      this->m_processingSelector = false;
    };
    AT_SCOPE_EXIT(restoreMembers);

    ++m_depth;
    m_processingSelector = true;

    auto selector = linker.Selector();
    if (selector)
    {
      StreamSelector(*selector);
    }
    else
    {
      m_os << ", ERROR: LINKER HAS NO SELECTOR !!!";
    }
  }

  PrintChildren(linker);
}

//! Appends content of Register node in text representation and visits
//! sub-nodes
//!
void PrettyPrinter::VisitRegister (Register& reg)
{
  if (m_processingSelector)
  {
    StreamNodeHeader("", reg);
  }
  else
  {
    StreamNodeHeader("Register", reg);

    m_os << ", length: " << reg.BypassSequence().BitsCount();

    if (reg.HoldValue())
    {
      m_os << ", Hold value: true";
    }

    if (!m_verbose)
    {
      StreamBinaryVector("bypass: ", reg.BypassSequence());
    }
    else
    {
      auto targetPosInLine = m_os.tellp() - m_startPos;

                                       StreamBinaryVector("bypass:            ", reg.BypassSequence());
      AlignOnNewLine(targetPosInLine); StreamBinaryVector("next_to_sut:       ", reg.NextToSut());
      AlignOnNewLine(targetPosInLine); StreamBinaryVector("last_to_sut:       ", reg.LastToSut());
      AlignOnNewLine(targetPosInLine); StreamBinaryVector("last_from_sut:     ", reg.LastFromSut());
      AlignOnNewLine(targetPosInLine); StreamBinaryVector("expected_from_sut: ", reg.ExpectedFromSut());
      AlignOnNewLine(targetPosInLine); StreamNodeCommon(reg);
    }
  }
}

//===========================================================================
// End of PrettyPrinter.cpp
//===========================================================================
