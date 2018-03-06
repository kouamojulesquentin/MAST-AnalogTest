//===========================================================================
//                           T_2_T_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file T_2_T_TranslatorProtocol.hpp
//!
//! Declares class T_2_T_TranslatorProtocol, whose role is to translate between Raw protocols. A Raw protocol does not directly generate transaction, but relies on a 
//! Parent Interface of type AccessInterfaceTranslator to execute the Callbcaks and provide the results
//!
//===========================================================================


#ifndef T_2_T_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define T_2_T_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

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
class MAST_CORE_EXPORT T_2_T_TranslatorProtocol : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~T_2_T_TranslatorProtocol() = default;
  T_2_T_TranslatorProtocol() {};

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

  private:
  std::string m_ParentTranslatorName;
  std::shared_ptr<MTQueue<CallbackRequest>> m_CallbackQueue;                       //!<Callback requests from underlying Raw protocol 
  std::shared_ptr<MTQueue<std::pair<BinaryVector,std::string>>> m_fromSutQueue;    //!<fromSut data results from underlying Raw protocol

  // ---------------- Protected Methods
  //
  protected:
};
//
//  End of T_2_T_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined T_2_T_TranslatorProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of T_2_T_TranslatorProtocol.hpp
//===========================================================================
