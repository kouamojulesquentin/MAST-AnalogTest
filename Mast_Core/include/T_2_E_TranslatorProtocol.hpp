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
#include <experimental/string_view>
#include <memory>

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

  T_2_E_TranslatorProtocol() {
  m_CallbackQueue=std::make_shared<MTQueue<CallbackRequest>>();
  m_fromSutQueue=std::make_shared<MTQueue<std::pair<BinaryVector,std::string>>>();
  m_Translator_launched=false;
  };


  bool ParentTranslator_is_set() {return ((m_CallbackQueue!=nullptr) && (m_fromSutQueue!=nullptr));}
  void SetParentTranslatorName( std::string ParentTranslatorName){m_ParentTranslatorName=ParentTranslatorName;}
  std::string ParentTranslatorName (){return m_ParentTranslatorName;}

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
         //Loopback for testing
         auto fromSut = PDL_translator(request.CallbackId(),request.ToSutVector());
         m_fromSutQueue->Push(std::make_pair(fromSut,*(new std::string)));
        } 
       return;};

  void Start_Translator(){
    //Using a lamba to encapsulate T_2_E_translator and avoid an "invalid use of nonstatic member function" error
     auto my_lamba =[this] () {T_2_E_translator();};
     T_2_E_thread=std::thread(my_lamba);
     T_2_E_thread.detach();
     m_Translator_launched=true;
     }
  bool Translator_Running(){return m_Translator_launched;}
  
  private:
  

  std::string m_ParentTranslatorName;
  std::shared_ptr<MTQueue<CallbackRequest>> m_CallbackQueue;                       //!<Callback requests from underlying Raw protocol
  std::shared_ptr<MTQueue<std::pair<BinaryVector,std::string>>> m_fromSutQueue;   //!<fromSut data results for underlying Raw protocol
  
  
  bool m_Translator_launched;
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
