//===========================================================================
//                           SSAK_PathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SSAK_PathSelector.cpp
//!
//! Implements class SSAK_PathSelector
//! While the Linker itself is a pass-through, the PathSelector instantiates
//! and configures the SSAK controller. It also exports methods to trigger
//! authentication and close the controller, to be used by an S2IB Path 
//! Selector
//! 
//===========================================================================

#include "SSAK_PathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include "microAES.h"
#include "SSAKplugin.h"
#include "g3log/g3log.hpp"
#include "SystemModel.hpp"
#include <stdexcept>
#include <limits>
#include <iterator>

using std::ostringstream;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace mast;



//! Returns readable information about selector (ex: select and deselect tables)
//!
//! @param onlyProperties When true, only properties are return, otherwise content
//!                       of select/deselect tables are return as well
//!
string SSAK_PathSelector::DebugSelectorInfo (bool onlyProperties) const
{
  auto debugInfo = PathSelector::DebugSelectorInfo(onlyProperties);


  if (!onlyProperties)
  {
    ostringstream os;
    
    os << "\nSSAK Selector";

    debugInfo.append(os.str());
  }

  return debugInfo;
}
//
//  End of: SSAK_PathSelector::DebugSelectorInfo
//---------------------------------------------------------------------------



//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegisters  Register(s) that is/are used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param selectTable          Table to use for selecting a path
//! @param deselectTable        Table to use for deselecting a path
//! @param properties           Properties of the selector (mainly to report that it can select no path or not)
//!
SSAK_PathSelector::SSAK_PathSelector (std::shared_ptr<Register> associatedRegister,
                                                              uint32_t               pathsCount,
                                                              const std::string& parameters)
  : PathSelector    (SelectorProperty::CanSelectNone)
  , m_pathsCount    (pathsCount)
  , m_muxRegisters  (associatedRegister)
{
  try{
   

 /* Example Senario:
  Crypto-processeir : AES128
  Amount of Secure SIB : 13;
  NB: this information should come from parsing the Parameters string
*/

LOG(INFO)<<"SSAK Parameter string: " << parameters;

   auto results     = Utility::Split(parameters, " ");
   
LOG(INFO)<<"Found "<< results.size()<< " Parameters";

  CHECK_PARAMETER_EQ(results.size(),2,"SSAK needs two parameters: Key (in hexadecimal) and max suppported S2IB");

  m_SSAK_Key = BinaryVector::CreateFromHexString(results.at(0));
  m_max_S2SIB =   std::stoull(std::string(results.at(1)));

  LOG(INFO)<<"SSAK Key:  "<<m_SSAK_Key.DataAsHexString();
  LOG(INFO)<<"Max_S2SIB "<< m_max_S2SIB; 

  //Instantiating sahed pointer to derived class microAES
  auto local_microaes = make_shared<microAES>();
  // casting back to base class cryptoProc
  m_CryptoProcesseur= std::dynamic_pointer_cast<cryptoProc>(local_microaes);
    
    m_SSAKdriver = make_shared<SSAKplugin>(*m_CryptoProcesseur,m_max_S2SIB);
    m_interfaceSize = m_SSAKdriver->getInterfaceSize();

    LOG(INFO)<<"InterfaceSize :"<< m_interfaceSize;

   std::string tmp_string(m_interfaceSize, '0');
   tmp_string[m_interfaceSize-1]='1';
   m_SSAKAuthenticationSuccess = BinaryVector::CreateFromBinaryString(tmp_string);
   tmp_string[m_interfaceSize-1]='0';
   tmp_string[m_interfaceSize-2]='1';
   m_SSAKAuthenticationFailure = BinaryVector::CreateFromBinaryString(tmp_string);
    
    m_SelectorState = SSAK_SelectorState::CLOSED;
    
   LOG(INFO)<<"SSAK Instantiation Successful";
   m_attempts = 0;
  }
  catch(std::exception& e)
  {
    ostringstream os; os << "Error while instantiating SSAK cryptoprocessor: " << e.what();
    THROW_RUNTIME_ERROR(os.str() );
  }
}
//
//  End of: SSAK_PathSelector::SSAK_PathSelector
//---------------------------------------------------------------------------


//! Forwards call to any driving Register(s)
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void SSAK_PathSelector::Accept (SystemModelVisitor& visitor)
{
  const auto muxRegisters = AssociatedRegisters();

  for (const auto& sliceReg : *muxRegisters)
  {
    sliceReg.reg->Accept(visitor);
  }
}
//
//  End of: SSAK_PathSelector::Accept
//---------------------------------------------------------------------------



//! Linker is always selected
//!
uint32_t SSAK_PathSelector::ActiveCount () const
{
  uint32_t activeCount = 1u;

  return activeCount;
}
//
//  End of: SSAK_PathSelector::ActiveCount
//---------------------------------------------------------------------------



//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void SSAK_PathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier > m_pathsCount)
  {
    ostringstream os; os << "pathIdentifier must be < " << m_pathsCount << ", got: " << pathIdentifier;
    THROW_OUT_OF_RANGE(os.str());
  }
}
//
//  End of: SSAK_PathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------

//! Returns true when the specified path is already selected
//!
bool SSAK_PathSelector::IsActive (uint32_t pathIdentifier) const
{
  //SSAK makes no difference between selected and active
  
  return IsSelected(pathIdentifier);
}
//
//  End of: SSAK_PathSelector::IsActive
//---------------------------------------------------------------------------


//! Linker is always selected
//!
bool SSAK_PathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  bool  isSelected = true;
  
  return isSelected;
}
//
//  End of: SSAK_PathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool SSAK_PathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  bool  isSelected  = true; //All paths are always selected
  bool  isActive    = true; //All paths are always active

  return isSelected && isActive;
}
//
//  End of: SSAK_PathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------

//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void SSAK_PathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
  
  return; //Paths can never be deselected
}
//
//  End of: SSAK_PathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! SSAK is a multi-step process: this method will be called multiple times
//! so states are used to trace the authentication steps and act accordingly
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void SSAK_PathSelector::Select (uint32_t pathIdentifier)
{
 (void)pathIdentifier; //Dummy instruction to avoid warning
  return; //Paths Is always selected
}
//
//  End of: SSAK_PathSelector::Select
//---------------------------------------------------------------------------


//! Triggers an authentication for given Configuration
//!
//! SSAK is a multi-step process: this method will be called multiple times
//! so states are used to trace the authentication steps and act accordingly
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
SSAK_PathSelector::SSAK_SelectorState SSAK_PathSelector::DoAuthentication (uint32_t Cardinality)
{
  
  bool pendingWrite = AssociatedRegisters()->NextToSut() != AssociatedRegisters()->LastToSut();
  BinaryVector AuthenticationResult;
  
  std::string config_string(m_interfaceSize, '0');
  config_string[m_interfaceSize-Cardinality]='1';

 auto config = BinaryVector::CreateFromBinaryString(config_string);

  switch (m_SelectorState)
   {
    case CLOSED: 
          //Write configuration to control register
	  AssociatedRegisters()->SetToSut(std::move(config));
	  m_SelectorState=CONFIG_SENT;
	  break;
    case CONFIG_SENT: 
	  if (pendingWrite==false)
	   {
	    //Config has been received, read SSAK challenge
	    //Challenge is given in the same CSU cycle, so no need to set a read request
	     //System Model is blocked during configuration, no need to protect read access 
	    BinaryVector challenge_BV;
	    AssociatedRegisters()->LastFromSut(challenge_BV);
	    LOG(INFO)<<"SSAK received challenge: : "<< challenge_BV.DataAsHexString();
	    	    //Compute response from challenge and write it back to the register
	    const u8* Response=m_SSAKdriver->computeResponse
	    				(m_SSAK_Key.Get_DataVector().data(),
					//m_SSAK_bits,
					challenge_BV.Get_DataVector().data());
	    std::vector<u8> Response_V (Response,Response+m_interfaceSize/8);
	    auto Response_BV
	     = BinaryVector::CreateFromRightAlignedBuffer(Response_V,m_interfaceSize);
 	    AssociatedRegisters()->SetToSut(std::move(Response_BV));
            m_SelectorState=AUTHENTICATION_CHECK;
	    AssociatedRegisters()->SetPendingForRead(true);
	    m_attempts = 0;
	    
	   }
	  break;
   case AUTHENTICATION_CHECK: 
	  if (false)//(AssociatedRegisters()->IsPendingForRead()==true)
	   {
	    //Authentication answer has not been read yet
	    m_SelectorState=AUTHENTICATION_CHECK;
	   }
	   else
	   {
	    AssociatedRegisters()->LastFromSut(AuthenticationResult);
	    LOG(INFO)<<"SSAK authentication result: "<< AuthenticationResult.DataAsHexString();
	    LOG(INFO)<<"Success is : "<< m_SSAKAuthenticationSuccess.DataAsHexString();
	    LOG(INFO)<<"Failure is : "<< m_SSAKAuthenticationFailure.DataAsHexString();
	    if (AuthenticationResult == m_SSAKAuthenticationSuccess)
	     {
   	      LOG(INFO)<<"SSAK authentication SUCCESS";
	      //Challenge Successfull
               m_SelectorState=OPEN;
	      break;
	     }
	     else if (AuthenticationResult == m_SSAKAuthenticationFailure)
	      {
	       //Challenge Failed, leave S2B closed and raise error
	       THROW_RUNTIME_ERROR("SSAK Selector : Authentication Failure "); break;
	      }
	     else
	      { 
	      //Authentication has not finished yet
	      m_attempts++;
      	     LOG(INFO)<<"Attempt "<< m_attempts <<" out of a maximum of " << SSAK_CONNECTION_WATCHDOG;
	      if (m_attempts==SSAK_CONNECTION_WATCHDOG)
	       THROW_RUNTIME_ERROR("SSAK Selector : Authentication Timeout"); break;  
	      
	      m_SelectorState=AUTHENTICATION_CHECK;
	      AssociatedRegisters()->SetPendingForRead(true);
	      }
	    }
	    break;
   case OPEN: break; 
   default : THROW_RUNTIME_ERROR("SSAK Selector in unknown state"); break;
   }
  return m_SelectorState; 

}
//
//  End of: SSAK_PathSelector::DoAuthentication
//---------------------------------------------------------------------------


//! Returns some value associated with specified path selection
//!
//! @note When cannot provide a value, an empty BinaryVector is returned
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
const BinaryVector& SSAK_PathSelector::SelectionValue (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  return  m_SSAKSelectorInformation;
}
//
//  End of: SSAK_PathSelector::SelectionValue
//---------------------------------------------------------------------------

//===========================================================================
// End of SSAK_PathSelector.cpp
//===========================================================================
