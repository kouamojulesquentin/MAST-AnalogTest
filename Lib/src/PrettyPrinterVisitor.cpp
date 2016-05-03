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



void PrettyPrinterVisitor::StreamNodeHeader(std::experimental::string_view type, const SystemModelNode& node)
{
  constexpr char nameLead  = '"';
  constexpr char nameTrail = '"';
  StreamDepth();
  m_os << type << '(';
  m_os << node.GetIdentifier() << ", ";
  m_os << nameLead << node.GetName() << nameTrail;
}

//! Appends content of AccessInterface node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  StreamNodeHeader("AccessInterface", accessInterface);
//+  m_os << ", " << reg.GetBypassSequence().DataAsBinaryString();
  m_os << ")"  << std::endl;

  PrintChildren(accessInterface);
}

//! Appends content of Chain node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitChain (Chain& chain)
{
  StreamNodeHeader("Chain", chain);
  m_os << ")"  << std::endl;

  PrintChildren(chain);
}

//! Appends content of Linker node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitLinker (Linker& linker)
{
  StreamNodeHeader("Linker", linker);
//+  m_os << ", " << reg.GetBypassSequence().DataAsBinaryString();
  m_os << ")"  << std::endl;

  PrintChildren(linker);
}

//! Appends content of Register node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitRegister (Register& reg)
{
  StreamNodeHeader("Register", reg);
  m_os << ", " << reg.GetBypassSequence().DataAsBinaryString();
  m_os << ")"  << std::endl;
}

//! Appends content of tap node in text representation and visits
//! sub-nodes
//!
void PrettyPrinterVisitor::VisitTap (Tap& tap)
{
  StreamNodeHeader("Tap", tap);
  m_os << ")"  << std::endl;

  PrintChildren(tap);
}

//===========================================================================
// End of PrettyPrinterVisitor.cpp
//===========================================================================
