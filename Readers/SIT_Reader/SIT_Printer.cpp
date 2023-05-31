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
#include "VirtualRegister.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"

#include <sstream>
#include <memory>

using std::string;
using std::shared_ptr;
using std::experimental::string_view;
using std::ostringstream;

using namespace mast;


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

//! Inserts childrens of a parent node
//!
//! @param parentNode Parent node for which children are "printed" using SIT file format
//!
void SIT_Printer::PrintChildren (const ParentNode& parentNode)
{
  // ---------------- Do support exceptions
  //
  auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
  AT_SCOPE_EXIT(restoreDepth);

  ++m_depth;

  auto childId = uint32_t(1u);
  auto child   = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    child = child->NextSibling();

    ++childId;
  }
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

  StreamDepth();

  m_os << type;

  StreamNodeName(m_os, node);

  if (!notes.empty())
  {
    m_os << " " << notes;
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

  auto isChain = dynamic_cast<const Chain*>(&parentNode) != nullptr;

  m_os << "\n";
  StreamDepth() << (isChain ? "{" : "(");

  PrintChildren(parentNode);

  m_os << "\n";
  StreamDepth() << (isChain ? "}" : ")");
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

//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  auto protocol = accessInterfaceTranslator.Protocol();
  auto note     = protocol ? protocol->KindName() : "PROTOCOL_NOT_SET";

  StreamParentNode("ACCESS_INTERFACE_TRANSLATOR", accessInterfaceTranslator, note);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitChain (Chain& chain)
{
  StreamParentNode("CHAIN", chain);
}

//! Appends content of Streamer node in text representation and visits
//! sub-nodes
//!
void SIT_Printer::VisitStreamer (Streamer& streamer)
{
  StreamParentNode("STREAMER", streamer);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void SIT_Printer::VisitLinker (Linker& linker)
{
  auto selector = linker.Selector();

  ostringstream os;
  if (selector)
  {
    os << selector->KindName();

    auto associatedRegisters = selector->AssociatedRegisters();
    if (associatedRegisters)
    {
      for (const auto& sliceReg : *associatedRegisters)
      {
        StreamNodeName(os, *sliceReg.reg);
      }
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
  StreamNodeHeader("REGISTER", reg);

  m_os << " " << reg.BypassSequence().BitsCount();

  if (reg.HoldValue())
  {
    m_os << " Hold_value";
  }

  m_os << " Bypass: \"" << reg.BypassSequence().DataAsMixString(8u, "_", ":") << "\"";
}

//===========================================================================
// End of SIT_Printer.cpp
//===========================================================================
