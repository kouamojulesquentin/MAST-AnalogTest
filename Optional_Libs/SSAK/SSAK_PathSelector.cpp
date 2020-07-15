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
//!It is a model to build a custom PathSelector which implements all virtual
//! functions but does nothing: its behavior is the same as a chain node
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
#include <sstream>
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
SSAK_PathSelector::SSAK_PathSelector (std::vector<std::shared_ptr<Register>> associatedRegisters,
                                                              uint32_t               pathsCount,
                                                              const std::string& parameters)
  : PathSelector    (SelectorProperty::CanSelectNone)
  , m_pathsCount    (pathsCount)
  , m_muxRegisters  (associatedRegisters.front())
{
  try{
 //  m_S2IB_Register
   
   if (associatedRegisters.size()!= 2)
     {
      ostringstream os; os << "SSAK needs 2 registers, " << associatedRegisters.size() << " given ";
      throw std::logic_error(os.str());
     }
   std::shared_ptr<Register> associatedRegister = associatedRegisters.front();
   m_S2IB_Register = associatedRegisters.back();

 /* Example Senario:
  Crypto-processeir : AES128
  Amount of Secure SIB : 13;
  NB: this information should come from parsing the Parameters string
*/
LOG(INFO)<<"SSAK Parameter string: " << parameters;

std::istringstream iss(parameters);
std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                 std::istream_iterator<std::string>());
//Parse and Check Parameters
int paramcount=0;
for (const auto& param_n : results)
   LOG(INFO)<<"SSAK Parameter "<< paramcount++ << " : " << param_n;

 auto asSMNode = dynamic_pointer_cast <SystemModelNode>(associatedRegister);
 auto asParentNode = dynamic_pointer_cast <ParentNode>(asSMNode);
// auto parNode=asSMNode->FindParentOfNode(associatedRegister);
 if (asParentNode == nullptr) LOG(INFO)<<"SSAK error in dynamic pinter cast";

#define NUM_OF_S2IB 0x0D // 13

  auto S2SIB_count =  NUM_OF_S2IB;
  //Instantiating sahed pointer to derived class microAES
  auto local_microaes = make_shared<microAES>();
  // casting back to base class cryptoProc
  m_CryptoProcesseur= std::dynamic_pointer_cast<cryptoProc>(local_microaes);
    
    m_SSAKdriver = make_shared<SSAKplugin>(*m_CryptoProcesseur,S2SIB_count);
    m_interfaceSize = m_SSAKdriver->getInterfaceSize();
    
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



//! Returns the number of paths that are currently active
//!
uint32_t SSAK_PathSelector::ActiveCount () const
{
  uint32_t activeCount = 0u;

  for (uint32_t pathId = 1u ; pathId < m_pathsCount; ++pathId)
  {
    if (IsActive(pathId))
    {
      ++activeCount;
    }
  }
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


//! Returns true when the specified path is already selected
//!
bool SSAK_PathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  LOG(DEBUG)<<"SSAK isSelected starting";
  //NB: SSAK counts paths starting from 0
  bool  isSelected = m_SSAKdriver->isSelected(pathIdentifier-1);

  LOG(DEBUG)<<"SSAK isSelected returns "<<isSelected;
  
  //Force Select to unroll FSM
  
  switch (m_SelectorState)
   {
    case CLOSED: isSelected   = false;
	  break;
    case CONFIG_SENT: isSelected   = false;
	  break;
    case READ_CHALLENGE: isSelected   = false;
	  break;
    case AUTHENTICATION_CHECK: isSelected   = false;
	  break;
    case OPEN: isSelected   = true;
	  break;
    default : isSelected   = true; //All paths are always selected
  }
   LOG(DEBUG)<<"SSAK state is "<<m_SelectorState;
 
  
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
  
  //SSAK closes all S2IB at once
  m_SSAKdriver->deselect();
  m_S2IB_Register->SetToSut(BinaryVector::CreateFromBinaryString("0"));
  m_SelectorState = SSAK_SelectorState::CLOSED;
  
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
  CheckPathIdentifier(pathIdentifier);
  
  //BinaryVector config(m_interfaceSize);
  //Example;=: in reality should be computed from S2IB cardinality (position)
  // pathIdentifier is useless because each S2IB has 1 derivation
  auto config = BinaryVector::CreateFromHexString("0x00000000000000000000000000000001");
  bool pendingWrite = AssociatedRegisters()->NextToSut() != AssociatedRegisters()->LastToSut();
  BinaryVector AuthenticationResult;


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
	    m_SelectorState=READ_CHALLENGE;
	    //Challenge is given in the same CSU cycle, so no need to set a read request
//	    AssociatedRegisters()->SetPendingForRead(true);
	   }
	  break;
    case READ_CHALLENGE: 
	  if (AssociatedRegisters()->IsPendingForRead()==true)
	   {
	    //Challenge has not been read yet
	    m_SelectorState=READ_CHALLENGE;
	   }
	   else
	    {
	     //System Model is blocked during configuration, no need to protect read access 
	    BinaryVector challenge_BV;
	    AssociatedRegisters()->LastFromSut(challenge_BV);
	    //Compute response from challenge and write it back to the register
	    const u8* Response=m_SSAKdriver->computeResponse(m_SSAK_bits,challenge_BV.Get_DataVector().data());
	    std::vector<u8> Response_V (Response,Response+m_interfaceSize/8);
	    auto Response_BV = BinaryVector::CreateFromRightAlignedBuffer(Response_V,m_interfaceSize);
 	    AssociatedRegisters()->SetToSut(std::move(Response_BV));
            m_SelectorState=AUTHENTICATION_CHECK;
	    AssociatedRegisters()->SetPendingForRead(true);
	    m_attempts = 0;
	    }
	  break;
   case AUTHENTICATION_CHECK: 
	  if (AssociatedRegisters()->IsPendingForRead()==true)
	   {
	    //Authentication answer has not been read yet
	    m_SelectorState=AUTHENTICATION_CHECK;
	   }
	   else
	   {
	    AssociatedRegisters()->LastFromSut(AuthenticationResult);
	    if (AuthenticationResult == m_SSAKAuthenticationSuccess)
	     {
	      //Challenge Successfull, open S2IB
	        m_S2IB_Register->SetToSut(BinaryVector::CreateFromBinaryString("1"));
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
  return; //Paths are always selected

}
//
//  End of: SSAK_PathSelector::Select
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
