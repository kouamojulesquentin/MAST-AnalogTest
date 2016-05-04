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
#include "Utility.hpp"


using std::string;

using namespace mast;


//! Pretty print childrens of a parent node
//!
void PrettyPrinterVisitor::PrintChildren (const ParentNode& parentNode)
{
  // ---------------- Do support exceptions
  //
  auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
  AT_SCOPE_EXIT(restoreDepth);

  ++m_depth;

  auto child = parentNode.GetFirstChild();
  while (child)
  {
    child->Accept(*this);
    child = child->GetNextSibling();
  }
}
//
//  End of: PrettyPrinterVisitor::PrintChildren
//---------------------------------------------------------------------------


//! Adds spaces to force next insertion point to be at target position relative
//! to reference position
//!
//! @param refPos       Reference position
//! @param targetPos    Target position relative to refPos
//!
void PrettyPrinterVisitor::AlignRelativeTo (std::fpos<int> refPos, std::fpos<int> targetPos)
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
void PrettyPrinterVisitor::AlignOnNewLine (std::fpos<int> targetPos)
{
  m_os << std::endl;
  m_os << string(targetPos, ' ');
}
//
//  End of: PrettyPrinterVisitor::AlignOnNewLine
//---------------------------------------------------------------------------



//! Streams content of binary vector, prefixed with given name
//!
//! @param bits  Binary vector to print
//! @param name  Name given to the binary vector (can be empty)
//!
void PrettyPrinterVisitor::StreamBinaryVector (std::experimental::string_view name, const BinaryVector& bits)
{
  m_os << ", " << name << bits.DataAsBinaryString(":", "_");
}
//
//  End of: PrettyPrinterVisitor::StreamBinaryVector
//---------------------------------------------------------------------------



//! Streams node common information: identifier, name and type
//!
//!
void PrettyPrinterVisitor::StreamNodeHeader(std::experimental::string_view type, const SystemModelNode& node)
{
  if (!m_first)
  {
    m_os << std::endl;
  }

  m_startPos = m_os.tellp();
  StreamDepth();

  m_os << '(' << node.GetIdentifier() << ") ";
  m_os << "[" << type                 << "]";

  AlignRelativeTo(m_startPos, 15 + m_depth);
  m_os << '"' << node.GetName()       << '"';

  m_first = false;
}

//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  StreamNodeHeader("Access_I", accessInterface);

  PrintChildren(accessInterface);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitChain (Chain& chain)
{
  StreamNodeHeader("Chain", chain);

  PrintChildren(chain);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitLinker (Linker& linker)
{
  StreamNodeHeader("Linker", linker);

  PrintChildren(linker);
}

//! Appends content of Register node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitRegister (Register& reg)
{
  StreamNodeHeader("Register", reg);

  m_os << ", length: " << reg.GetBypassSequence().BitsCount();

  if (!m_verbose)
  {
    StreamBinaryVector("bypass: ", reg.GetBypassSequence());
  }
  else
  {
    auto targetPosInLine = m_os.tellp() - m_startPos;

                                     StreamBinaryVector("bypass:            ", reg.GetBypassSequence());
    AlignOnNewLine(targetPosInLine); StreamBinaryVector("next_to_sut:       ", reg.GetNextToSut());
    AlignOnNewLine(targetPosInLine); StreamBinaryVector("last_to_sut:       ", reg.GetLastToSut());
    AlignOnNewLine(targetPosInLine); StreamBinaryVector("last_from_sut:     ", reg.GetLastFromSut());
    AlignOnNewLine(targetPosInLine); StreamBinaryVector("expected_from_sut: ", reg.GetExpectedFromSut());

    AlignOnNewLine(targetPosInLine);
    m_os.setf(std::ios_base::boolalpha);
    m_os << ", pending: "       << reg.IsPending();
    m_os << ", has_condition: " << reg.HasConditions();
    m_os << ", priority: "      << reg.GetPriority();
  }
}

//! Appends content of tap node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitTap (Tap& tap)
{
  StreamNodeHeader("Tap", tap);

  PrintChildren(tap);
}

//===========================================================================
// End of PrettyPrinterVisitor.cpp
//===========================================================================
