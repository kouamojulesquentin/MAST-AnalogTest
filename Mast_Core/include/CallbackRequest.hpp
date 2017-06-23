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
  CallbackRequest() {m_KindName="unnamed";};
  CallbackRequest(std::experimental::string_view KindName) 
   : m_KindName(KindName)
  {};
  
  std::experimental::string_view KindName() {return m_KindName;};

  protected:
  BinaryVector		ToSutVector;
  void* interfaceData;
  std::string CallbackId;
  
  private:
  std::experimental::string_view m_KindName;
   
};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of CallbackRequest.hpp
//===========================================================================
