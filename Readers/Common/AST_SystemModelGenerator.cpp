//===========================================================================
//                           AST_SystemModelGenerator.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_SystemModelGenerator.cpp
//!
//! Implements class AST_SystemModelGenerator
//!
//===========================================================================

#include "AST_SystemModelGenerator.hpp"

#include "AST_AccessLink.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_Network.hpp"
#include "AST_Port.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Source.hpp"

#include "SystemModel.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::make_unique;

using namespace mast;
using namespace Parsers;

//! Destructs AST_SystemModelGenerator
//!
AST_SystemModelGenerator::~AST_SystemModelGenerator ()
{
}
//
//  End of: AST_SystemModelGenerator::~AST_SystemModelGenerator
//---------------------------------------------------------------------------


//! Constructor...
AST_SystemModelGenerator::AST_SystemModelGenerator (shared_ptr<mast::SystemModel> systemModel)
  : m_systemModel (CHECK_PARAMETER_NOT_NULL(systemModel, "Expect valid, not nullptr SystemModel"))
  , m_builder     (make_unique<SystemModelBuilder>(*m_systemModel))
{
}
//
//  End of: AST_SystemModelGenerator::AST_SystemModelGenerator
//---------------------------------------------------------------------------



//! Generates a SystemModel (sub-)tree from AST_Network
//!
shared_ptr<mast::SystemModelNode> AST_SystemModelGenerator::Generate (AST_Network* network)
{
  m_network = CHECK_PARAMETER_NOT_NULL(network, "Expect valid, not nullptr AST_Network");

  shared_ptr<mast::SystemModelNode> topNode;
  network->Accept(*this);

  return m_parsedTopNode;
}
//
//  End of: AST_SystemModelGenerator::Generate
//---------------------------------------------------------------------------




//! Generates SystemModel node from AST_AccessLink node
//!
void AST_SystemModelGenerator::Visit_AccessLink (AST_AccessLink* accessLink)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_AccessLink
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Instance node
//!
void AST_SystemModelGenerator::Visit_Instance (AST_Instance* instance)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_Instance
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Network node
//!
void AST_SystemModelGenerator::Visit_Network (AST_Network* network)
{
  auto topModule = network->TopModule();
  CHECK_VALUE_NOT_NULL(topModule, "Cannot generate SystemModel nodes when network has no modules");

  if (topModule->HasAccessLink())
  {
    auto accessLink = topModule->AccessLink();
    auto type       = accessLink->Type();
    switch (type)
    {
      case AccessLinkType::STD_1149_1_2001:
      case AccessLinkType::STD_1149_1_2013:
        LOG(INFO) << "Creating STD_1149 AccessLink";
        break;
      case AccessLinkType::Generic:
        LOG(INFO) << "Creating Generic" << accessLink->GenericIdentifier()->AsText() << " AccessLink";
        break;
      default:
        CHECK_FAILED("Unexpected AccessLink type");
        break;
    }
  }
  else
  {
    auto name = topModule->Name();

    m_parsedTopNode = m_systemModel->CreateChain(name);

    auto scanOutPorts = topModule->ScanOutPorts();
    CHECK_VALUE_NOT_EMPTY(scanOutPorts, "A module must have at least one ScanOutPorts");
    CHECK_TRUE((scanOutPorts.size() == 1u) || !topModule->ScanInterfaces().empty(), "Only support a single ScanOutPort unless there is at least one ScanInterface");

  }
}
//
//  End of: AST_SystemModelGenerator::Visit_Network
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Module node
//!
void AST_SystemModelGenerator::Visit_Module (AST_Module* module)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_Module
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Port node
//!
void AST_SystemModelGenerator::Visit_Port (AST_Port* port)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_Port
//---------------------------------------------------------------------------



//! Generates SystemModel node from AST_ScanInterface node
//!
void AST_SystemModelGenerator::Visit_ScanInterface (AST_ScanInterface* scanInterface)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_ScanInterface
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_ScanMux node
//!
void AST_SystemModelGenerator::Visit_ScanMux (AST_ScanMux* scanMux)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_ScanMux
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_ScanRegister node
//!
void AST_SystemModelGenerator::Visit_ScanRegister (AST_ScanRegister* scanRegister)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_ScanRegister
//---------------------------------------------------------------------------


//! Generates SystemModel node from AST_Source node
//!
void AST_SystemModelGenerator::Visit_Source (AST_Source* source)
{
}
//
//  End of: AST_SystemModelGenerator::Visit_Source
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_SystemModelGenerator.cpp
//===========================================================================
