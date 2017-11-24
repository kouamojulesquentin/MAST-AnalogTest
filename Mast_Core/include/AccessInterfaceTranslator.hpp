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
  AccessInterfaceTranslator()  = default;
  AccessInterfaceTranslator(std::experimental::string_view name)
    : ParentNode (name)
  {}

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "AccessInterfaceTranslator"; } //!< Returns readable type name
  
  //Control of message queues 

  void PushRequest(CallbackRequest Request) {m_CallbackQueue.Push(Request);}; //!<Queues a new Callback Request
  CallbackRequest PopRequest() {return m_CallbackQueue.Pop();}; //!<returns the oldest request. NB: it is a BLOCKING call

  void PushfromSut(BinaryVector Result) {m_fromSutQueue.Push(std::make_pair(Result,*(new std::string)));};//!< Queues a new callback result
  BinaryVector PopfromSut() { return  m_fromSutQueue.Pop().first;};//!< returns the oldest callback result. NB: it is a BLOCKING call
  std::string PopFormattedfromSut() { auto tmp=m_fromSutQueue.Pop(); if (!tmp.second.empty()) return tmp.second; 
                                      else return tmp.first.DataAsBinaryString();};//!< returns the Formatted Data of the oldest callback result. NB: it is a BLOCKING call

  void PushPending() {m_Pending.Push(true);};//!< Queues a new toSut Update value
  bool PopPending() { return  m_Pending.Pop();};//!< returns the oldest toSut Update value NB: it is a BLOCKING call

  // ---------------- Private  Fields
  //
  private:
  uint32_t                                 m_numberOfEndPoints = 0; //!< Number of nodes (endpoints) accessible through the access interface
  
  MTQueue<CallbackRequest> m_CallbackQueue;  //DataCycleThread pushes to_SUT data and CallbackId to this queue
  MTQueue<std::pair<BinaryVector,std::string>> m_fromSutQueue;   //DataCycleThread waits on this queue to update its registers
  MTQueue<bool> m_Pending;  //DataCycleThread waits on this queue for pending cycles: only used for synchro, data is not important
};
//
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACETRANSLATOR_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
