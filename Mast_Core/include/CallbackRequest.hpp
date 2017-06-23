//===========================================================================
//                           AccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol.hpp
//!
//! Declares class AccessInterfaceProtocol
//!
//===========================================================================


#ifndef CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include <experimental/string_view>

namespace mast
{
//! Defines Callbacks for AccessInterface Requests
//!
class CallbackRequest 
{
  // ---------------- Public  Methods
  //
  public:
  ~CallbackRequest() = default;
  CallbackRequest() {m_CallbackId="undefined";};
  CallbackRequest(std::experimental::string_view CallbackId) 
   : m_CallbackId(CallbackId)
  {}
  
  CallbackRequest(std::experimental::string_view CallbackId,
		  void* interfaceData) 
   : m_CallbackId(CallbackId)
   , m_interfaceData(interfaceData)
  {}
  
  CallbackRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
		   void* interfaceData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_interfaceData(interfaceData)
  {}

  CallbackRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
  {}

  std::experimental::string_view CallbackId() {return m_CallbackId;};
  BinaryVector	ToSutVector() {return m_ToSutVector;}	;
  void* interfaceData() { return m_interfaceData;};

  private:
  std::string m_CallbackId;
  BinaryVector  m_ToSutVector;
  void* m_interfaceData=nullptr;
  
  
};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of CallbackRequest.hpp
//===========================================================================
