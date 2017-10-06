//===========================================================================
//                           SIT_Printer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SIT_Printer.cpp
//!
//! Implements class SIT_Printer
//!
//===========================================================================

#include "SIT_Printer.hpp"
#include "SystemModelNodes.hpp"
#include "PathSelector.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"

#include <sstream>

using std::string;
using std::shared_ptr;
using std::experimental::string_view;
using std::ostringstream;

using namespace mast;

//! Adds spaces to force next insertion point to be at target position relative
//! to reference position
//!
//! @param refPos       Reference position
//! @param targetPos    Target position relative to refPos
//!
void SIT_Printer::AlignRelativeTo (pos_type refPos, pos_type targetPos)
{
  auto curPos      = m_os.tellp();
  auto startLength = curPos - refPos;

  if (startLength < targetPos)
  {
    m_os << string(targetPos - startLength, ' ');
  }
}
//
//  End of: SIT_Printer::AlignRelativeTo
//---------------------------------------------------------------------------


//! Inserts new line and align position on target position oin newly added line
//!
//! @param targetPos  Position set after adding a new line
//!
void SIT_Printer::AlignOnNewLine (pos_type targetPos)
{
  m_os << std::endl;
  m_os << string(targetPos, ' ');
}
//
//  End of: SIT_Printer::AlignOnNewLine
//---------------------------------------------------------------------------


//! Returns model representation starting from a "top" node using SIT files syntax
//!
//! @param topNode    Node from which graph is created
//!
string SIT_Printer::MakeSIT (SystemModelNode& topNode)
{
  SIT_Printer printer;
  topNode.Accept(printer);
  return printer.SIT_Text();
}
//
//  End of: SIT_Printer::SIT_Text
//---------------------------------------------------------------------------


//! Returns model representation starting from a "top" node using SIT files syntax
//!
//! @param topNode    Node from which graph is created
//!
string SIT_Printer::MakeSIT (shared_ptr<SystemModelNode> topNode)
{
  CHECK_PARAMETER_NOT_NULL(topNode, "Cannot build SIT representation from nullptr");

  return MakeSIT(*topNode);
}
//
//  End of: SIT_Printer::SIT_Print
//---------------------------------------------------------------------------

//! SIT_ print childrens of a parent node
//!
void SIT_Printer::PrintChildren (const ParentNode& parentNode)
{
  // ---------------- Do support exceptions
  //
  auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
  AT_SCOPE_EXIT(restoreDepth);

  ++m_depth;

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
//  End of: SIT_Printer::PrintChildren
//---------------------------------------------------------------------------


//! Streams SystemModelNode name
//!
//! @note When there is any spaces in the node name, it is surrounded with double quotes
//!
//! @param os     A stream
//! @param node   Node for which header is to be streamed
//!
ostringstream& SIT_Printer::StreamNodeName (ostringstream& os, const SystemModelNode& node)
{
  if (node.Name().find(' ') != string::npos)
  {
    os << " \"" << node.Name() << "\"";
  }
  else
  {
    os << " " << node.Name();
  }
  return os;
}
//
//  End of: SIT_Printer::StreamNodeName
//---------------------------------------------------------------------------



//! Streams node common information: identifier, name and type
//!
//! @param type   Text representation of the node type
//! @param node   The node for which header is to be streamed
//! @param notes  Optional note to add after node name
//!
void SIT_Printer::StreamNodeHeader(string_view type, const SystemModelNode& node, string_view notes)
{
  if (!m_first)
  {
    m_os << "\n";
  }

  m_startPos = m_os.tellp();
  StreamDepth();

  if (!m_processingSelector)
  {
    m_os << type;
  }
  else
  {
    m_os << ":Selector:";
  }

  StreamNodeName(m_os, node);

  if (!notes.empty())
  {
    m_os << " " << notes;
  }

  if (m_selector && m_processingSelector)
  {
    m_os << " Kind: "            << m_selector->KindName();
    m_os << " Can_select_none: " << IsSet(m_selector->Properties(), SelectorProperty::CanSelectNone);
    m_os << " Inverted_bits: "   << IsSet(m_selector->Properties(), SelectorProperty::InvertedBits);
    m_os << " Reversed_order: "  << IsSet(m_selector->Properties(), SelectorProperty::ReverseOrder);
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
void SIT_Printer::StreamParentNode (std::experimental::string_view type, const ParentNode& parentNode, string_view notes)
{
  StreamNodeHeader(type, parentNode, notes);

  if (parentNode.IgnoreForNodePath())
  {
    m_os << " NOT_IN_PATH";
  }

  m_os << "\n";
  StreamDepth() << "{";

  PrintChildren(parentNode);

  m_os << "\n";
  StreamDepth() << "}";
}
//
//  End of: SIT_Printer::StreamParentNode
//---------------------------------------------------------------------------



//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitAccessInterface (AccessInterface& accessInterface)
{
  auto protocol = accessInterface.Protocol();
  auto note     = protocol ? protocol->KindName() : "PROTOCOL_NOT_SET";

  StreamParentNode("ACCESS_INTERFACE", accessInterface, note);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitChain (Chain& chain)
{
  StreamParentNode("CHAIN", chain);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void SIT_Printer::VisitLinker (Linker& linker)
{
  auto selector = linker.Selector();
  m_selector    = selector;

  ostringstream os;
  if (selector)
  {
    auto associatedRegister = selector->AssociatedRegister();
    os << selector->KindName();
    if (associatedRegister)
    {
      StreamNodeName(os, *associatedRegister);
    }
    else
    {
      os << " __NO_ASSOCIATED_REGISTER__";
    }
    os << " " << selector->SelectablePaths();
  }
  else
  {
    os << " __ERROR_LINKER_HAS_NO_SELECTOR__";
  }
  StreamParentNode("LINKER", linker, os.str());
}

//! Appends content of Register node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitRegister (Register& reg)
{
  if (m_processingSelector)
  {
    StreamNodeHeader("", reg);
  }
  else
  {
    StreamNodeHeader("REGISTER", reg);

    m_os << " " << reg.BypassSequence().BitsCount();

    if (reg.HoldValue())
    {
      m_os << " Hold_value";
    }

    m_os << " Bypass: \"" << reg.BypassSequence().DataAsMixString(8u, "_", ":") << "\"";
  }
}

//===========================================================================
// End of SIT_Printer.cpp
//===========================================================================
