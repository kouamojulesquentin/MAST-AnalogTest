//===========================================================================
//                           SystemModelManager.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager.cpp
//!
//! Implements class SystemModelManager
//!
//===========================================================================


#include "SystemModelManager.hpp"
#include "SystemModel.hpp"
#include "AccessInterface.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Chain.hpp"
#include "Utility.hpp"

using namespace mast;
using std::shared_ptr;
using std::dynamic_pointer_cast;


//! Does a complete data cycles for SystemModel as long as there are pending nodes
//!
//! @note It does the data cycle over each AccessInterface in the SystemModel
//!
void SystemModelManager::DoDataCycles ()
{
  auto root        = m_sm.Root();
  auto doDataCycle = true;

  do
  {
    root->Accept(m_configurator);
    root->Accept(m_propagator);

    doDataCycle = root->IsPending();

    if (doDataCycle)
    {
      auto nextAccessInterface = m_firstAccessInterface;

      while (nextAccessInterface)
      {
        auto protocol = nextAccessInterface->Protocol();
        CHECK_VALUE_NOT_NULL(protocol, "All AccessInterface must be associated with a valid protocol");

        uint32_t derivationId   = 1u;
        auto     nextDerivation = nextAccessInterface->FirstChild();

        while (nextDerivation)
        {
          m_toSutVisitor.Reset();
          nextDerivation->Accept(m_toSutVisitor);

          const auto& toSutVector = m_toSutVisitor.ToSutVector();
          const auto& activeRegs  = m_toSutVisitor.ActiveRegistersIdentifiers();

          auto fromSutVector = protocol->DoAction(derivationId, nextDerivation->ApplicationData(), toSutVector);

          m_fromSutUpdater.UpdateRegisters(activeRegs, fromSutVector);

          nextDerivation = nextDerivation->NextSibling();
        }

        nextAccessInterface = dynamic_pointer_cast<AccessInterface>(nextAccessInterface->NextSibling());;
      }
    }
  } while (doDataCycle);
}
//
//  End of: SystemModelManager::DoDataCycles
//---------------------------------------------------------------------------


//! Retrieves first AccessInterface from a SystemModel
//! @note SystemModel root node must be the only AccessInterface or a chain for which all children
//!       are AccessInterface
//!
shared_ptr<AccessInterface> SystemModelManager::GetFirstAccessInterface (const SystemModel& sm)
{
  auto root = sm.Root();

  CHECK_VALUE_NOT_NULL(root, "Cannot manage an empty system model");

  auto accessInterface = dynamic_pointer_cast<AccessInterface>(root);
  if (!accessInterface)
  {
    auto asChain = dynamic_pointer_cast<Chain>(root);
    CHECK_VALUE_NOT_NULL(asChain, "SystemModel root must be an AccessInterface or a Chain (for which chidren are AccessInterface)");

    accessInterface = dynamic_pointer_cast<AccessInterface>(asChain->FirstChild());
    CHECK_VALUE_NOT_NULL(accessInterface, "Root (a Chain) must have only AccessInterface children");
  }

  return accessInterface;
}
//
//  End of: SystemModelManager::GetFirstAccessInterface
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelManager.cpp
//===========================================================================
