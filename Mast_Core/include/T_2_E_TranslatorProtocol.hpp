//===========================================================================
//                           T_2_E_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file T_2_E_TranslatorProtocol.hpp
//!
//! Declares class T_2_E_TranslatorProtocol, whose role is to translate between a protocols and a PDL function. 
//! Parent Interface of type AccessInterfaceTranslator to execute the Callbcaks and provide the results
//!
//===========================================================================


#ifndef T_2_E_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define T_2_E_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "AccessInterfaceTranslator.hpp" //temporary, only to force lone hpp compilation 
#include "BinaryVector.hpp"
#include "CallbackIds.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include <experimental/string_view>
#include <memory>

#define DEFAULT_BB_REG_NAME "BB_Reg"

using Application_t = std::function<void()>;

namespace mast
{
//! General type to Transaction - to - Transaction transformations
//!
class MAST_CORE_EXPORT T_2_E_TranslatorProtocol : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~T_2_E_TranslatorProtocol() = default;
  virtual BinaryVector PDL_translator( std::experimental::string_view CallbackId, BinaryVector	   ToSutVector) = 0;

  T_2_E_TranslatorProtocol() = delete;
  T_2_E_TranslatorProtocol(std::shared_ptr<ParentNode> EventDomainRootNode) {
  m_CallbackQueue=std::make_shared<MTQueue<CallbackRequest>>();
  m_fromSutQueue=std::make_shared<MTQueue<std::pair<BinaryVector,std::string>>>();
  m_EventDomainRootNode=EventDomainRootNode;
  n_BB_Reg_Name = DEFAULT_BB_REG_NAME;
  m_Translator_launched=false;
  };

  T_2_E_TranslatorProtocol(std::experimental::string_view BB_Reg_Name) {
  m_CallbackQueue=std::make_shared<MTQueue<CallbackRequest>>();
  m_fromSutQueue=std::make_shared<MTQueue<std::pair<BinaryVector,std::string>>>();
  m_EventDomainRootNode=nullptr;
  n_BB_Reg_Name =BB_Reg_Name;
  m_Translator_launched=false;
  };

  bool EventDomain_is_set() {return (m_EventDomainRootNode!=nullptr) ;}
  void SetEventDomain( std::shared_ptr<SystemModelNode> EventDomainRootNode){m_EventDomainRootNode=std::dynamic_pointer_cast<ParentNode>(EventDomainRootNode);}
  std::shared_ptr<ParentNode> EventDomainRootNode (){return m_EventDomainRootNode;}

  void SetCallbackQueue(std::shared_ptr<MTQueue<CallbackRequest>> CallbackQueue) {m_CallbackQueue=CallbackQueue;}
   void SetfromSutQueue(std::shared_ptr<MTQueue<std::pair<BinaryVector,std::string>>> fromSutQueue) {m_fromSutQueue=fromSutQueue;}
 
    void PushRequest(CallbackRequest Request) {
     m_CallbackQueue->Push(Request);
     LOG(DEBUG) << "Protocol " << this->KindName()<<" : pushed request for Callback "<<Request.CallbackId();
     }; //!<Queues a new Callback Request

  BinaryVector PopfromSut() { auto result=  m_fromSutQueue->Pop().first; 
                             LOG(DEBUG) << "Node " << this->KindName()<<" : popped a fromSut";
                              return result;   };//!< returns the oldest callback result. NB: it is a BLOCKING call
  std::string PopFormattedfromSut() { auto tmp=m_fromSutQueue->Pop(); if (!tmp.second.empty()) return tmp.second; 
                                      else return tmp.first.DataAsBinaryString();};//!< returns the Formatted Data of the oldest callback result. NB: it is a BLOCKING call

  
  void T_2_E_translator() {
         /*Executes the PDL translator when there is one request*/
       auto request = m_CallbackQueue->Pop();
       while (request.CallbackId() != NO_MORE_PENDING)
        {
         auto fromSut = PDL_translator(request.CallbackId(),request.ToSutVector());
         m_fromSutQueue->Push(std::make_pair(fromSut,*(new std::string)));
       request = m_CallbackQueue->Pop();
        } 
       return;};
       
  bool Translator_Running(){return m_Translator_launched;}
  void Set_Translator_State(bool new_state){m_Translator_launched=new_state;}
  

//! Registers the translator algorithm functions in this file
//!
//! @note Names used from registration must be the same as found in SIT file
bool RegisterAlgorithms ()
{
  // ---------------- Get an handle on PDL algorithm repository
  //
  auto& repo = PDL_AlgorithmsRepository::Instance();
  auto T_2_E_translator_lambda =[this] () {T_2_E_translator();};

  // ---------------- Do register algorithm(s) with a name
  //
  repo.RegisterAlgorithm("T_2_E_translator", T_2_E_translator_lambda);
   //Compiles, but is it correct?

  return true;
}
//
//  End of: RegisterAlgorithms
//---------------------------------------------------------------------------


//! Make PDL algorithm functions in this file to be registered
//!
//! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
//!
bool registrated = RegisterAlgorithms();

  private:
  

  std::experimental::string_view n_BB_Reg_Name;
  std::shared_ptr<ParentNode> m_EventDomainRootNode;                                 //!>Retargeting domain root node where the events must be generated 
  std::shared_ptr<MTQueue<CallbackRequest>> m_CallbackQueue;                       //!<Callback requests from underlying Raw protocol
  std::shared_ptr<MTQueue<std::pair<BinaryVector,std::string>>> m_fromSutQueue;   //!<fromSut data results for underlying Raw protocol
  
  
  bool m_Translator_launched=true;
  std::thread T_2_E_thread;

  // ---------------- Protected Methods
  //
  protected:
};
//
//  End of T_2_E_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined T_2_E_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of T_2_E_TranslatorProtocol.hpp
//===========================================================================
