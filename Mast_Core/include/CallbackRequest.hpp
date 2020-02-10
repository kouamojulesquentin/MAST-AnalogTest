//===========================================================================
//                           CallbackRequest.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CallbackRequest.hpp
//!
//! Declares class CallbackRequest
//!
//===========================================================================


#ifndef CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include <experimental/string_view>

namespace mast
{

//  std::string FormatData(BinaryVector raw_data){(void)raw_data;return "UNSET";};
class DataFormatter_Functor
{
 public: 
  std::string operator()(BinaryVector raw_data)
   {
    (void)raw_data;return "UNSET";
   }
 
};

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

  CallbackRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
                   std::string FormattedData,
		   void* interfaceData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_FormattedData(FormattedData)
   , m_interfaceData(interfaceData)
  {}

  CallbackRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
                   std::string FormattedData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_FormattedData(FormattedData)
  {}

  CallbackRequest( std::experimental::string_view CallbackId,
                   std::string FormattedData
                  ) 
   : m_CallbackId(CallbackId)
   , m_FormattedData(FormattedData)
  {}

  std::experimental::string_view CallbackId() {return m_CallbackId;};
  BinaryVector	ToSutVector() {return m_ToSutVector;}	;
  void* interfaceData() { return m_interfaceData;};
  std::string FormattedData(){if (!m_FormattedData.empty()) return m_FormattedData ;
                              else return m_ToSutVector.DataAsBinaryString();};


  private:
  std::string m_CallbackId;
  BinaryVector  m_ToSutVector;
  
  //NB: This should be removed now!!!!
  std::string m_FormattedData;
  void* m_interfaceData=nullptr;
  
  
};
//
//  End of CallbackRequest class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CALLBACKRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of CallbackRequest.hpp
//===========================================================================
