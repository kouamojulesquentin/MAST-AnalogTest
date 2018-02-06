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
#include "AST_Alias.hpp"
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
#include "AST_VectorIdentifier.hpp"

#include "Utility.hpp"
#include "g3log/g3log.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::ostringstream;
using std::string;
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


//! Checks nothing for AST_AccessLink
//!
void AST_Checker::Visit_AccessLink (AST_AccessLink* accessLink)
{
}
//
//  End of: AST_Checker::Visit_AccessLink
//---------------------------------------------------------------------------


//! Reports not yet supported alias options
//!
void AST_Checker::Visit_Alias (AST_Alias* alias)
{
  // Lamba: Construct a message with context, alias name and message per se
  //
  auto makeMessage = [this, alias](string_view messageSuffix)
  {
    string message = m_messageContext + "has Alias \"" + alias->Name() + "\" ";
    message += messageSuffix;

    return message;
  };

  if (alias->EnumRef() != nullptr)
  {
    ReportWarning(makeMessage("with RefEnum statement ==> that is not yet supported"));
  }

  if (alias->ApplyEndState() != nullptr)
  {
    ReportWarning(makeMessage("with iApplyEndState statement ==> that is not yet supported"));
  }

  if (alias->AccessTogether())
  {
    ReportWarning(makeMessage("with AccessTogether statement ==> that is not yet supported"));
  }

  // ---------------- Bits count
  //
  const auto& signals = alias->Signals();
  uint32_t signalsBitsCount = 0;
  for (auto signal : signals)
  {

    auto bitsCount    = signal->BitsCount();
    signalsBitsCount += bitsCount;
  }

  auto bitsCount = alias->Identifier()->BitsCount();

  if (bitsCount != signalsBitsCount)
  {
    ReportWarning("defined with "s.append(std::to_string(bitsCount)).append(" bits, ")
                                  .append("although aliased signals appeart to represent ")
                                  .append(std::to_string(signalsBitsCount)).append(" bits"));
  }
}
//
//  End of: AST_Checker::Visit_AccessLink
//---------------------------------------------------------------------------


//! Checks nothing for AST_Instance
//!
void AST_Checker::Visit_Instance (AST_Instance* instance)
{
}
//
//  End of: AST_Checker::Visit_Instance
//---------------------------------------------------------------------------


//! Checks and collects basic statistics of AST_Network
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

  ReportInfo("Network has "s.append(to_string(modulesCount)).append(" module(s) defined in ").append(to_string(namespaces.size())).append(" namespace(s)"));
}
//
//  End of: AST_Checker::Visit_Network
//---------------------------------------------------------------------------




//! Checks AST_Module
//!
void AST_Checker::Visit_Module (AST_Module* module)
{
  // ---------------- ScanInPorts
  //
  auto scanInPorts = module->ScanInPorts();

  m_messageContext = "Module \""s.append(module->Name()).append("\" ");

  if (scanInPorts.empty())
  {
    ostringstream os; os << m_messageContext << "has no ScanInPorts";
    ReportInfo(os.str());
  }
  else if ((scanInPorts.size() != 1u) && module->ScanInterfaces().empty())
  {
    ostringstream os; os << m_messageContext << "has " << scanInPorts.size() << " ScanInPorts - but has no ScanInterfaces";
    ReportWarning(os.str());
  }

  // ---------------- ScanOutPorts
  //
  auto scanOutPorts = module->ScanOutPorts();

  if (scanOutPorts.empty())
  {
    ostringstream os; os << m_messageContext << "has no ScanOutPorts";
    ReportInfo(os.str());
  }
  else if ((scanOutPorts.size() != 1u) && module->ScanInterfaces().empty())
  {
    ostringstream os; os << m_messageContext << "has " << scanOutPorts.size() << " ScanOutPorts - but has no ScanInterfaces";
    ReportWarning(os.str());
  }
  else
  {
    auto scanOutPort = scanOutPorts.front();
    auto source      = scanOutPort->Source();

    m_messageContext.append("ScanOutPort \"").append(scanOutPort->Name()).append("\" ");

    if (source == nullptr)
    {
      ostringstream os; os << m_messageContext << "has no source";
      ReportError(os.str());
    }
    else
    {
      auto signals = source->Signals();
      if (signals.size() != 1u)
      {
        ostringstream os; os << m_messageContext << "has a source with no signal";
        ReportError(os.str());
      }
      else
      {
        auto signal = signals.front();
        CHECK_VALUE_NOT_NULL(signal, "Expecting not nullptr signal");
        if (signal->IsNumber())
        {
          ostringstream os; os << m_messageContext << "source signal is a number instead of connecting to some port";
          ReportError(os.str());
        }
      }
    }
  }

  // ---------------- Aliases
  //
  if (module->IsUniquified())
  {
    for (auto alias : module->Aliases())
    {
      Visit_Alias(alias);
    }
  }
}
//
//  End of: AST_Checker::Visit_Module
//---------------------------------------------------------------------------


//! Checks nothing for AST_Port
//!
void AST_Checker::Visit_Port (AST_Port* port)
{
}
//
//  End of: AST_Checker::Visit_Port
//---------------------------------------------------------------------------



//! Checks nothing for AST_ScanInterface
//!
void AST_Checker::Visit_ScanInterface (AST_ScanInterface* scanInterface)
{
}
//
//  End of: AST_Checker::Visit_ScanInterface
//---------------------------------------------------------------------------


//! Checks nothing for AST_ScanMux
//!
void AST_Checker::Visit_ScanMux (AST_ScanMux* scanMux)
{
}
//
//  End of: AST_Checker::Visit_ScanMux
//---------------------------------------------------------------------------


//! Checks nothing for AST_ScanRegister
//!
void AST_Checker::Visit_ScanRegister (AST_ScanRegister* scanRegister)
{
}
//
//  End of: AST_Checker::Visit_ScanRegister
//---------------------------------------------------------------------------


//! Checks nothing for AST_Source
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
