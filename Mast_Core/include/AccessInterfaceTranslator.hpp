//===========================================================================
//                           AccessInterface.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterface.hpp
//!
//! Declares class AccessInterface
//!
//===========================================================================

#ifndef ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_
  #define ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

#include "ParentNode.hpp"
#include "BinaryVector.hpp"
#include "CallbackRequest.hpp"
#include "MTQueue.hpp"
#include "AccessInterfaceTranslatorProtocol.hpp"
#include <functional>
#include <vector>
#include <queue>          // std::queue
#include "AccessInterface.hpp"

#include "g3log/g3log.hpp"

using namespace std::chrono;
using namespace std::chrono_literals;

namespace mast
{

//! Represents an access point to scan chain (the physical interface to the SUT)
//!
class MAST_CORE_EXPORT AccessInterfaceTranslator : public ParentNode
{
  // ---------------- Public  Methods
  //
  public: 
  ~AccessInterfaceTranslator() = default;
  AccessInterfaceTranslator()  = delete;
  AccessInterfaceTranslator(std::experimental::string_view name,std::shared_ptr<AccessInterfaceTranslatorProtocol> protocol)
    : ParentNode (name)
    , m_protocol(protocol)
  {m_Busy=false; }

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "AccessInterfaceTranslator"; } //!< Returns readable type name
  
  std::shared_ptr<AccessInterfaceTranslatorProtocol> Protocol() {return m_protocol;}
  //Control of message queues 

 uint32_t RegisterInterface(std::shared_ptr<mast::AccessInterface> interface);
 uint32_t RegisterTranslator(std::shared_ptr<mast::AccessInterfaceTranslator> translator);

 void set_Busy(){m_Busy=true;};
 void unset_Busy(){m_Busy=false;};
 bool is_Busy(){return m_Busy;};
 
  CallbackRequest PopRequest(uint32_t n_interface) {  
     std::cv_status   status;
     CallbackRequest item; 
      LOG(DEBUG) << "Node " << this->Name()<<" : Popping a request ...";
  //   status=m_CallbackQueue->Pop(item,m_timeout);
     m_CallbackQueue[n_interface]->Pop(item);
      LOG(DEBUG) << "Node " << this->Name()<<" : Pop done";
     CHECK_PARAMETER_NEQ (status, std::cv_status::timeout,"Error, timeout on CallbackRequestQueue->Pop");
     return item;}; //!<returns the oldest request. NB: it is a BLOCKING call

  void PushfromSut(BinaryVector Result,uint32_t n_interface) {m_fromSutQueue[n_interface]->Push(std::make_pair(Result,*(new std::string))); 
                         LOG(DEBUG) << "Node " << this->Name()<<" : pushed a fromSut";};//!< Queues a new callback result

  void PushPending() {m_Pending.Push(true);};//!< Queues a new toSut Update value
  bool PopPending() { return  m_Pending.Pop();};//!< returns the oldest toSut Update value NB: it is a BLOCKING call

  // ---------------- Private  Fields
  //
  private:
  uint32_t                                 m_numberOfEndPoints = 0; //!< Number of nodes (endpoints) accessible through the access interface
  std::shared_ptr<AccessInterfaceTranslatorProtocol> m_protocol;                //!< Protocol to use to manage Callbacktranslations
  
  std::chrono::duration<int,std::milli> m_timeout = 15ms;
  
  std::vector<std::shared_ptr<MTQueue<CallbackRequest>>> m_CallbackQueue;  //!<DataCycleThread pushes to_SUT data and CallbackId to this queue
  std::vector<std::shared_ptr<MTQueue<std::pair<BinaryVector,std::string>>>> m_fromSutQueue;   //!<DataCycleThread waits on this queue to update its registers
  MTQueue<bool> m_Pending;  //!<DataCycleThread waits on this queue for pending cycles: only used for synchro, data is not important
  bool m_Busy; //!< When TRUE, the sub-tree is being served by a T-2-E translation and must be bypassed by the DataCycle
};
//
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
