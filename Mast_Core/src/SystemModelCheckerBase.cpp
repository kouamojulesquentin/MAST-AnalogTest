//===========================================================================
//                           SystemModelCheckerBase.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckerBase.cpp
//!
//! Implements class SystemModelCheckerBase
//!
//===========================================================================

#include "SystemModelCheckerBase.hpp"
#include "SystemModelNode.hpp"

using namespace mast;
using std::string;
using std::to_string;
using std::experimental::string_view;
using std::ostringstream;


//! Reports an error for a node
//!
void SystemModelCheckerBase::ReportError (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportError(os.str());
}
//
//  End of: SystemModelCheckerBase::ReportError
//---------------------------------------------------------------------------


//! Reports an info for a node
//!
void SystemModelCheckerBase::ReportInfo (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportInfo(os.str());
}
//
//  End of: SystemModelCheckerBase::ReportError
//---------------------------------------------------------------------------


//! Reports a warning for a node
//!
void SystemModelCheckerBase::ReportWarning (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportWarning(os.str());
}
//
//  End of: SystemModelCheckerBase::ReportWarning
//---------------------------------------------------------------------------



//! Helper that streams into os, an header, node name and identifier
//!
ostringstream& SystemModelCheckerBase::Stream (ostringstream& os, string_view header, const SystemModelNode& node)
{
  if (!header.empty())
  {
    os << header << " ";
  }

  os << node.TypeName() << " '" << node.Name() << "' (id: " << node.Identifier() << ")";
  return os;
}
//
//  End of: SystemModelCheckerBase::Stream
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelCheckerBase.cpp
//===========================================================================
