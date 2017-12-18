//===========================================================================
//                           DataCycleVisitor.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataCycleVisitor.cpp
//!
//! Implements class DataCycleVisitor
//!
//===========================================================================

#include "DataCycleVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "SystemModelManager_impl.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "Utility.hpp"
#include "T_2_E_TranslatorProtocol.hpp"

using namespace mast;

using std::shared_ptr;
using std::make_shared;


//! Updates AccessInterface pending flag
//!

void DataCycleVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  VisitChildren(accessInterface);

    auto protocol = accessInterface.Protocol();

    CHECK_VALUE_NOT_NULL(protocol, "All AccessInterface must be associated with a valid protocol");

//  if (accessInterface.IsPending()) //Redundant in normal usage, causes hang in multithreaded operation
  {
     m_manager->DoHierarchicalDataCycle(&accessInterface);
  }
}
//
//  End of: DataCycleVisitor::VisitAccessInterface
//---------------------------------------------------------------------------

//! If Pending, sends a notification to the Raw protocol waiting on the pending queue
//!
void DataCycleVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator&accessInterfaceTranslator)

{
 auto AI_data_cycle = [this] (AccessInterface accessInterface) {
 VisitAccessInterface(accessInterface);
// m_manager->DoHierarchicalDataCycle(&accessInterface);
 };
 auto TR_data_cycle = [this] (AccessInterfaceTranslator& translator) {VisitAccessInterfaceTranslator(translator);
    //Release parent Translator
    CallbackRequest request(NO_MORE_PENDING);
    translator.Protocol()->TransformationCallback(request);
};
 
 if (accessInterfaceTranslator.is_Busy()) return; //Node already been served by an T-2-E translator

  //Is this really needed?
  //VisitChildren(accessInterfaceTranslator);
  
  //SystemModelChecker guarantess the only child is an AccessInterface with a Raw protocol
  auto cur_node = accessInterfaceTranslator.FirstChild();
  uint32_t cur_interface = 0;
  std::shared_ptr<mast::AccessInterface> accessInterface;
  std::shared_ptr<AccessInterfaceTranslator> slaveTranslator;  
  std::shared_ptr<T_2_E_TranslatorProtocol> T_2_E_protocol;
  
  if (accessInterfaceTranslator.IsPending())
  {
  do
   {
    LOG(DEBUG) << "accessInterfaceTranslator.IsPending()";
   accessInterface=std::dynamic_pointer_cast<AccessInterface>(cur_node);
   slaveTranslator=std::dynamic_pointer_cast<AccessInterfaceTranslator>(cur_node);
   T_2_E_protocol= std::dynamic_pointer_cast<T_2_E_TranslatorProtocol>(accessInterfaceTranslator.Protocol());
   if (T_2_E_protocol) {
     //T_2_E_protocol->Start_Translator();
     auto T_2_E_lambda =[T_2_E_protocol] () {T_2_E_protocol->T_2_E_translator();};
     m_manager->CreateApplicationThread(T_2_E_protocol->EventDomainRootNode(),T_2_E_lambda,"T-2-E Translator");
     m_manager->StartCreatedApplicationThreads ();
     T_2_E_protocol->Set_Translator_State(true);
     }
   
   if (accessInterface)
    {  
     //Launch Thread executing Data Cycle on the child Access Interface
    auto protocol =  accessInterface->Protocol();
   //Launch DoHierarchicalDataCycle as a separate thread
    std::thread AI_thread(AI_data_cycle,*accessInterface);
   AI_thread.detach(); //Detach AI thread for a clean exit
   }
   if (slaveTranslator)
    {  
     //Launch Thread executing this Visitors on the child Access Interface Translator
    auto protocol =  accessInterfaceTranslator.Protocol();
   //Launch DoHierarchicalDataCycle as a separate thread
    std::thread TR_thread(TR_data_cycle,std::ref(*slaveTranslator));
   TR_thread.detach(); //Detach AI thread for a clean exit
    }
   
   bool cycle_AI = true;
   while(cycle_AI)
   {
   auto request = accessInterfaceTranslator.PopRequest(cur_interface);
      //At this moment, the AI Callback is waiting on the Result queue

  //Execute callback translator
  //Push fromSut to wake AI_thread up
   cycle_AI = (request.CallbackId() != NO_MORE_PENDING);
    if (cycle_AI)
     {
     accessInterfaceTranslator.set_Busy();
     auto fromSutVector = accessInterfaceTranslator.Protocol()->TransformationCallback(request);
     accessInterfaceTranslator.PushfromSut(fromSutVector,cur_interface);
     accessInterfaceTranslator.unset_Busy();
     }
    }
   cur_node =  cur_node->NextSibling();
   cur_interface++;
  }while (cur_node);
  }
   if (T_2_E_protocol) { //Release and terminate "T-2-E Translator" application thread
    CallbackRequest request(NO_MORE_PENDING);
    T_2_E_protocol->TransformationCallback(request);
   } 
}
//
//  End of: DataCycleVisitor::VisitRegister
//---------------------------------------------------------------------------


//! Updates Chain pending flag
//!
void DataCycleVisitor::VisitChain (Chain& chain)
{
  VisitChildren(chain);
}
//
//  End of: DataCycleVisitor::VisitChain
//---------------------------------------------------------------------------


//! Ignores Register
//!
void DataCycleVisitor::VisitRegister (Register&)
{
}
//
//  End of: DataCycleVisitor::VisitRegister
//---------------------------------------------------------------------------


//! Updates Linker pending flag while selecting a pending register
//!
//! @note Selected register is the last sibling that is pending
//! @note More elaborate register selection may be possible but actual needs must be
//!       defined to provide an appropriate solution
//! @note When no child is pending, path 0 is selected
//!
void DataCycleVisitor::VisitLinker (Linker& linker)
{
  VisitChildren(linker);
}
//
//  End of: DataCycleVisitor::VisitLinker
//---------------------------------------------------------------------------





//===========================================================================
// End of DataCycleVisitor.cpp
//===========================================================================
