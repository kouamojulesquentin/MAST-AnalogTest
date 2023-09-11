//===========================================================================
//                           RVF.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RVF.hpp
//!
//! Declares class Relocatable Vector Format (RVF) classes for Requests and Responses
//!
//===========================================================================


#ifndef RVFRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define RVFRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include <experimental/string_view>
#include <experimental/any>

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
class RVFRequest 
{
  // ---------------- Public  Methods
  //
  public:
  ~RVFRequest() = default;
  RVFRequest() {m_CallbackId="undefined";};
  RVFRequest(std::experimental::string_view CallbackId) 
   : m_CallbackId(CallbackId)
  {}
  
  RVFRequest(std::experimental::string_view CallbackId,
		  void* interfaceData) 
   : m_CallbackId(CallbackId)
   , m_interfaceData(interfaceData)
  {}
  
  RVFRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
		   void* interfaceData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_interfaceData(interfaceData)
  {}

  RVFRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)

  {}

/* Might cause ambiguity with FormattedData constructore
  RVFRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
		    std::experimental::any optionalData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_optionalData(optionalData)
  {}
*/

  RVFRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
                   std::string FormattedData,
		   void* interfaceData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_FormattedData(FormattedData)
   , m_interfaceData(interfaceData)
  {}

  RVFRequest( std::experimental::string_view CallbackId,
                   BinaryVector	   ToSutVector,
                   std::string FormattedData
                  ) 
   : m_CallbackId(CallbackId)
   , m_ToSutVector(ToSutVector)
   , m_FormattedData(FormattedData)
  {}

  RVFRequest( std::experimental::string_view CallbackId,
                   std::string FormattedData
                  ) 
   : m_CallbackId(CallbackId)
   , m_FormattedData(FormattedData)
  {}

  /* Getters*/
  std::string CallbackId() {return m_CallbackId;};
  BinaryVector	ToSutVector() {return m_ToSutVector;}	;
  void* interfaceData() { return m_interfaceData;};
  std::string FormattedData(){if (!m_FormattedData.empty()) return m_FormattedData ;
                              else return m_ToSutVector.DataAsBinaryString();};
  BinaryVector	ExpectedData() {return m_ExpectedData;}	;
  BinaryVector	ExpectedMask() {return m_ExpectedMask;}	;

  /* Setters*/
  void setCallbackId(std::string CallbackId) {m_CallbackId= CallbackId;}
  void SetExpectedData (BinaryVector sequence) { m_ExpectedData          = sequence; } //!< Sets Expected Data from SUT
  void SetExpectedMask (BinaryVector sequence) { m_ExpectedMask         = sequence; } //!< Sets Mask for Expected Data from SUT

  private:
  std::string m_CallbackId;      //!> Callback Primitive to be executed
  BinaryVector  m_ToSutVector;  //!< Data to be sent to SUT
  BinaryVector  m_ExpectedData; //!<  Expected Data from SUT
  BinaryVector  m_ExpectedMask; //!< Mask for Expected Data from SUT
  
  //NB: This should be removed now!!!!
  std::string m_FormattedData;
  
  void* m_interfaceData=nullptr;

  public: 
  //std::any requires speficif casting that can be done only by the interface, so it must be accessible
  std::experimental::any m_optionalData; 
  
};
//
//  End of RVFRequest class declaration
//---------------------------------------------------------------------------

//Response is just the Data: using a typedef to avoid a full code refactoring
// a real Class definition would demand
typedef BinaryVector RVFResponse; 
//---------------------------------------------------------------------------

} // End of namespace mast




#endif  // not defined RVFRequest_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of RVF.hpp
//===========================================================================
