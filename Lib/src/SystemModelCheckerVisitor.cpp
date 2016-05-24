//===========================================================================
//                           SystemModelCheckerVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckerVisitor.cpp
//!
//! Implements class SystemModelCheckerVisitor
//!
//===========================================================================


#include "SystemModelCheckerVisitor.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::vector;
using std::ostringstream;



//! Checks SystemModel coherence
//!
//! @note
//!   - Each node has one and only one parent (except root that has no parent)
//!   - Each parent node has at least one child otherwise a warning is issued
//!   - Each child is only appended once in its parent
//!   - Each node is reachable (no dangling node)
//!   - Unused id generate a warning
//!   - Each linker has a number of chidren that matches its selector or an warning is issued when there are to few
//!     children and an error when there are too much
//!
SystemModelCheckResult SystemModelCheckerVisitor::Check ()
{
  CheckIdentifiers();
//+  CheckTree();

  auto result = MakeCheckResult();
  return result;
}
//
//  End of: SystemModelCheckerVisitor::Check
//---------------------------------------------------------------------------



//! Checks coherence of identifiers:
//!
//! @note - Each used identifier must refere to a node that has the very same identifier
//!       - Unused identifiers are collected as "info"
//!
void SystemModelCheckerVisitor::CheckIdentifiers ()
{
  auto idCount = m_identifierMapping.size();

  for (size_t id = 0 ; id < idCount ; ++id)
  {
    auto node = m_identifierMapping[id];
    if (node)
    {
      if (node->GetIdentifier() != static_cast<SystemModelNode::NodeIdentifier>(id))
      {
        ostringstream os;
        os << "Found node '" << node->GetName() << "' with identifier '" << node->GetIdentifier() << "' while expecting '" << id << "'";
        ReportError(os.str());
      }
    }
    else
    {
      ostringstream os;
      os << "Identifier '" << id << "' is not used";
      ReportInfo(os.str());
    }
  }
}
//
//  End of: SystemModelCheckerVisitor::CheckIdentifiers
//---------------------------------------------------------------------------



//! Builds up a SystemModelCheckResult from currently selected issues
//!
SystemModelCheckResult SystemModelCheckerVisitor::MakeCheckResult ()
{
  SystemModelCheckResult result;

  result.errors        = m_errors.str();
  result.warnings      = m_warnings.str();
  result.infos         = m_infos.str();

  result.errorsCount   = m_errorsCount;
  result.warningsCount = m_warningsCount;
  result.infosCount    = m_infosCount;

  return result;
}
//
//  End of: SystemModelCheckerVisitor::MakeCheckResult
//---------------------------------------------------------------------------


//! Reports an error, warning or info
//!
void SystemModelCheckerVisitor::Report (string_view message, uint32_t& counter, ostringstream& os)
{
  if (counter != 0)
  {
    os << std::endl;
  }

  os << "  - " << message;
  ++counter;
}
//
//  End of: SystemModelCheckerVisitor::ReportError
//---------------------------------------------------------------------------


//! Visits AccessInterface
//!
//! @note Checked point are:
//!  - The node has at least one child
//!  - The node has only one parent
//!  -
void SystemModelCheckerVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
}
//
//  End of: SystemModelCheckerVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Visits Chain
//!
//! @note Checked point are:
//!  - The node has at least one child
//!  - The node has only one parent
//!  -
void SystemModelCheckerVisitor::VisitChain (Chain& chain)
{
}
//
//  End of: SystemModelCheckerVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits Linker
//!
//! @note Checked point are:
//!  - The node has at least one child
//!  - The node has only one parent
//!  -
void SystemModelCheckerVisitor::VisitLinker (Linker& linker)
{
}
//
//  End of: SystemModelCheckerVisitor::VisitLinker
//---------------------------------------------------------------------------


//! Visits Register
//!
//! @note Checked point are:
//!  - The node has at least one child
//!  - The node has only one parent
//!  -
void SystemModelCheckerVisitor::VisitRegister (Register& reg)
{
}
//
//  End of: SystemModelCheckerVisitor::VisitRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelCheckerVisitor.cpp
//===========================================================================
