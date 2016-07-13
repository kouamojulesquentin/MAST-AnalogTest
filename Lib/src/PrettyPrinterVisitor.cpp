//===========================================================================
//                           PrettyPrinterVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PrettyPrinterVisitor.cpp
//!
//! Implements class PrettyPrinterVisitor
//!
//===========================================================================

#include "PrettyPrinterVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "PathSelector.hpp"
#include "Utility.hpp"

using std::string;
using std::shared_ptr;

using namespace mast;



//! Initializes with specified options
//!
PrettyPrinterVisitor::PrettyPrinterVisitor (PrettyPrinterOptions options)
  : m_useAutoFormat (IsSet(options, PrettyPrinterOptions::AutoFormat))
  , m_verbose       (IsSet(options, PrettyPrinterOptions::Verbose))
{
}
//
//  End of: PrettyPrinterVisitor::PrettyPrinterVisitor
//---------------------------------------------------------------------------




//! Adds spaces to force next insertion point to be at target position relative
//! to reference position
//!
//! @param refPos       Reference position
//! @param targetPos    Target position relative to refPos
//!
void PrettyPrinterVisitor::AlignRelativeTo (pos_type refPos, pos_type targetPos)
{
  auto curPos      = m_os.tellp();
  auto startLength = curPos - refPos;

  if (startLength < targetPos)
  {
    m_os << string(targetPos - startLength, ' ');
  }
}
//
//  End of: PrettyPrinterVisitor::AlignRelativeTo
//---------------------------------------------------------------------------


//! Inserts new line and align position on target position oin newly added line
//!
//! @param targetPos  Position set after adding a new line
//!
void PrettyPrinterVisitor::AlignOnNewLine (pos_type targetPos)
{
  m_os << std::endl;
  m_os << string(targetPos, ' ');
}
//
//  End of: PrettyPrinterVisitor::AlignOnNewLine
//---------------------------------------------------------------------------


//! Returns textual model representation starting from a "top" node
//!
//! @param topNode    Node from which graph is created
//! @param options    Printer options
//!
string PrettyPrinterVisitor::PrettyPrint (shared_ptr<ParentNode> topNode, PrettyPrinterOptions options)
{
  CHECK_PARAMETER_NOT_NULL(topNode, "Cannot 'pretty print' from nullptr");

  PrettyPrinterVisitor printer(options);
  topNode->Accept(printer);
  return printer.PrettyPrint();
}
//
//  End of: PrettyPrinterVisitor::PrettyPrint
//---------------------------------------------------------------------------


//! Pretty print childrens of a parent node
//!
void PrettyPrinterVisitor::PrintChildren (const ParentNode& parentNode)
{
  // ---------------- Do support exceptions
  //
  auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
  AT_SCOPE_EXIT(restoreDepth);

  ++m_depth;

  auto child = parentNode.FirstChild();
  while (child)
  {
    child->Accept(*this);
    child = child->NextSibling();
  }
}
//
//  End of: PrettyPrinterVisitor::PrintChildren
//---------------------------------------------------------------------------


//! Streams content of binary vector, prefixed with given name
//!
//! @param bits  Binary vector to print
//! @param name  Name given to the binary vector (can be empty)
//!
void PrettyPrinterVisitor::StreamBinaryVector (std::experimental::string_view name, const BinaryVector& bits)
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
//  End of: PrettyPrinterVisitor::StreamBinaryVector
//---------------------------------------------------------------------------


//! Prints SystemModelNode data
//!
//! @param node   The node for which header is to be streamed
//!
void PrettyPrinterVisitor::StreamNodeCommon (const SystemModelNode& node)
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
//  End of: PrettyPrinterVisitor::StreamNodeCommon
//---------------------------------------------------------------------------



//! Streams node common information: identifier, name and type
//!
//! @param type   Text representation of the node type
//! @param node   The node for which header is to be streamed
//!
void PrettyPrinterVisitor::StreamNodeHeader(std::experimental::string_view type, const SystemModelNode& node)
{
  if (!m_first)
  {
    m_os << std::endl;
  }

  m_startPos = m_os.tellp();
  StreamDepth();

  if (!m_processingSelector)
  {
    m_os << "[" << type                 << "]";
  }
  else
  {
    m_os << ":Selector:";
  }

  m_os << '(' << node.Identifier() << ") ";

  AlignRelativeTo(m_startPos, 15u + m_depth);
  m_os << '"' << node.Name()       << '"';

  m_first = false;
}


//! Appends content of parent node in text representation and visits
//! sub-nodes
//!
//! @param type   Text representation of the node type
//! @param node   The node for which header is to be streamed
//!
void PrettyPrinterVisitor::StreamParentNode (std::experimental::string_view type, const ParentNode& parentNode)
{
  StreamNodeHeader(type, parentNode);

  if (m_verbose)
  {
    StreamNodeCommon(parentNode);
  }
  PrintChildren(parentNode);
}
//
//  End of: PrettyPrinterVisitor::StreamParentNode
//---------------------------------------------------------------------------



//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  StreamParentNode("Access_I", accessInterface);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitChain (Chain& chain)
{
  StreamParentNode("Chain", chain);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void PrettyPrinterVisitor::VisitLinker (Linker& linker)
{
  StreamNodeHeader("Linker", linker);

  if (m_verbose)
  {
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
    selector->Accept(*this);
  }

  PrintChildren(linker);
}

//! Appends content of Register node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitRegister (Register& reg)
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
// End of PrettyPrinterVisitor.cpp
//===========================================================================
