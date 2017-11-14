//===========================================================================
//                           AST_Checker.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Checker.cpp
//!
//! Implements class AST_Checker
//!
//===========================================================================

#include "AST_Checker.hpp"

#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Network.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

#include "Utility.hpp"
#include "g3log/g3log.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::ostringstream;
using std::make_unique;
using std::to_string;

using namespace mast;
using namespace Parsers;

//! Destructs AST_Checker
//!
AST_Checker::~AST_Checker ()
{
}
//
//  End of: AST_Checker::~AST_Checker
//---------------------------------------------------------------------------



//! Constructor...
AST_Checker::AST_Checker (AST_Network* network)
  : m_network (network)
{
  CHECK_PARAMETER_NOT_NULL(m_network, "Cannot check from nullptr AST_Network");
}
//
//  End of: AST_Checker::AST_Checker
//---------------------------------------------------------------------------




//! Checks basic AST consistency
//!
CheckResult AST_Checker::Check ()
{
  Visit_Network(m_network);

  auto result = MakeCheckResult();
  return result;
}
//
//  End of: AST_Checker::Check
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_AccessLink node
//!
void AST_Checker::Visit_AccessLink (AST_AccessLink* accessLink)
{
}
//
//  End of: AST_Checker::Visit_AccessLink
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Instance node
//!
void AST_Checker::Visit_Instance (AST_Instance* instance)
{
}
//
//  End of: AST_Checker::Visit_Instance
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Network node
//!
void AST_Checker::Visit_Network (AST_Network* network)
{
  auto topModule = network->TopModule();

  if (topModule == nullptr)
  {
    ReportError("Network has no top module");
  }

  auto   namespaces   = m_network->NamespacesWithDefinedModules();
  size_t modulesCount = 0u;

  for (const auto& namespaceNode : namespaces)
  {
    auto modules = m_network->ModulesInNamespace(namespaceNode);
    modulesCount += modules.size();

    for (const auto module : modules)
    {
      Visit_Module(module);
    }
  }

  ReportInfo("Network has "s.append(to_string(modulesCount)).append(" defined in ").append(to_string(namespaces.size())).append(" namespace(s)"));
}
//
//  End of: AST_Checker::Visit_Network
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Module node
//!
void AST_Checker::Visit_Module (AST_Module* module)
{
  // ---------------- ScanInPorts
  //
  auto scanInPorts = module->ScanInPorts();
  auto msgPrefix   = "Module \""s.append(module->Name()).append("\" ");

//+  #define SET_MSG_PREFIX(expr)  { ostringstream os; os << expr; msgPrefix = os.str(); }
//+  SET_MSG_PREFIX("Module \"" << module->Name() << "\" ")

  if (scanInPorts.empty())
  {
    ostringstream os; os << msgPrefix << "has no ScanInPorts - It should have at least one";
    ReportError(os.str());
  }
  else if ((scanInPorts.size() != 1u) && module->ScanInterfaces().empty())
  {
    ostringstream os; os << msgPrefix << "has " << scanInPorts.size() << " ScanInPorts - but has no ScanInterfaces";
    ReportError(os.str());
  }

  // ---------------- ScanOutPorts
  //
  auto scanOutPorts = module->ScanOutPorts();

  if (scanOutPorts.empty())
  {
    ostringstream os; os << msgPrefix << "has no ScanOutPorts - It should have at least one";
    ReportError(os.str());
  }
  else if ((scanOutPorts.size() != 1u) && module->ScanInterfaces().empty())
  {
    ostringstream os; os << msgPrefix << "has " << scanOutPorts.size() << " ScanOutPorts - but has no ScanInterfaces";
    ReportError(os.str());
  }
  else
  {
    auto scanOutPort = scanOutPorts.front();
    auto source      = scanOutPort->Source();

    msgPrefix.append("ScanOutPort \"").append(scanOutPort->Name()).append("\" ");

    if (source == nullptr)
    {
      ostringstream os; os << msgPrefix << "has no source";
      ReportError(os.str());
    }
    else
    {
      auto signals = source->Signals();
      if (signals.size() != 1u)
      {
        ostringstream os; os << msgPrefix << "has a source with no signal";
        ReportError(os.str());
      }
      else
      {
        auto signal = signals.front();
        CHECK_VALUE_NOT_NULL(signal, "Expecting not nullptr signal");
        if (signal->IsNumber())
        {
          ostringstream os; os << msgPrefix << "source signal is a number instead of connecting to some port";
          ReportError(os.str());
        }
      }
    }
  }

}
//
//  End of: AST_Checker::Visit_Module
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Port node
//!
void AST_Checker::Visit_Port (AST_Port* port)
{
}
//
//  End of: AST_Checker::Visit_Port
//---------------------------------------------------------------------------



//! Generates SystemModel node from AST_ScanInterface node
//!
void AST_Checker::Visit_ScanInterface (AST_ScanInterface* scanInterface)
{
}
//
//  End of: AST_Checker::Visit_ScanInterface
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_ScanMux node
//!
void AST_Checker::Visit_ScanMux (AST_ScanMux* scanMux)
{
}
//
//  End of: AST_Checker::Visit_ScanMux
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_ScanRegister node
//!
void AST_Checker::Visit_ScanRegister (AST_ScanRegister* scanRegister)
{
}
//
//  End of: AST_Checker::Visit_ScanRegister
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Source node
//!
void AST_Checker::Visit_Source (AST_Source* source)
{
}
//
//  End of: AST_Checker::Visit_Source
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Checker.cpp
//===========================================================================
