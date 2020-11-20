//===========================================================================
//                           DefaultS2IBPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultS2IBPathSelector.cpp
//!
//! Implements class DefaultS2IBPathSelector
//!
//! It is based on a simplified version of the DefaultTableBased with just one
//! bit, but with a connection to an SSAK controller PathLinker:
//!
//! Constructor makes the link with the SSAK controller
//! Select() triggers an authentication before opening the SIB
//! Deselect() Closes the SSAK 
//===========================================================================

#include "DefaultS2IBPathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"
#include "SystemModel.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>
#include <experimental/string_view>

using std::ostringstream;
using std::shared_ptr;
using std::experimental::string_view;
using std::dynamic_pointer_cast;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

namespace
{
  auto REQUIRED_REGISTER_MESSAGE = "A valid associated register is mandatory to create DefaultS2IBPathSelector"sv;
}

//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer (must be not nullptr)
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultS2IBPathSelector::DefaultS2IBPathSelector (std::vector<std::shared_ptr<SystemModelNode>> associatedNodes,
                                uint32_t               pathsCount,
				const std::string& parameters)
  : PathSelector    (SelectorProperty::CanSelectNone)
{

   LOG(INFO)<<"S2IB Parameter string: " << parameters;
     auto results     = Utility::Split(parameters, " ");

   //Parse and Check Parameters
   int paramcount=0;
   for (const auto& param_n : results)
    LOG(INFO)<<"S2IB Parameter "<< paramcount++ << " : " << param_n;
   
   m_cardinality = std::stoull(std::string(results.front()));

   if ((associatedNodes.size()< 2) ||(associatedNodes.size()> 3))
     {
      ostringstream os; os << "S2IB needs 2 or 3 registers, " << associatedNodes.size() << " given ";
      throw std::logic_error(os.str());
     }
     
   if (  pathsCount!=1)
    {
      ostringstream os; os << "S2IB can only support 1 segment";
      throw std::logic_error(os.str());
    }

   auto S2IB_AuthenticationLinker =  std::dynamic_pointer_cast<Linker> (associatedNodes.front());
   CHECK_PARAMETER_NOT_NULL(S2IB_AuthenticationLinker,"First parameter of S2IB must be a Linker Node");

   std::shared_ptr<Register> associatedRegister = std::dynamic_pointer_cast<Register> (associatedNodes.back());
   CHECK_PARAMETER_NOT_NULL(associatedRegister,"Second parameter of S2IB must be a Register Node");
   
   
   auto S2IB_AuthenticationSelector = S2IB_AuthenticationLinker->Selector();
   m_S2IB_SSAK_PathSelector = std::dynamic_pointer_cast<SSAK_PathSelector>(S2IB_AuthenticationSelector);
   CHECK_PARAMETER_NOT_NULL(m_S2IB_SSAK_PathSelector,"S2IB must reference an SSAK Path Selector");
   
   m_muxRegisters = VirtualRegister(associatedRegister);
   m_pathsCount = 1;
   m_SelectValue = BinaryVector::CreateFromBinaryString("/b1");
   m_DeSelectValue = BinaryVector::CreateFromBinaryString("/b0");
   
   LOG(DEBUG) << "Select Value set to  " << m_SelectValue.DataAsBinaryString();
   LOG(DEBUG) << "DeSelect Value set to  " << m_DeSelectValue.DataAsBinaryString();
   
}

//
//  End of: DefaultS2IBPathSelector::DefaultS2IBPathSelector
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool DefaultS2IBPathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto selectValue = m_SelectValue;
  auto  lastToSut   = AssociatedRegisters()->LastToSut();

  bool  isActive    = lastToSut == selectValue;

  return isActive;
}
//
//  End of: DefaultS2IBPathSelector::IsActive
//---------------------------------------------------------------------------

//! Returns the number of paths that are currently active
//!
uint32_t DefaultS2IBPathSelector::ActiveCount () const
{
  if (IsActive(1u))
    return 1;
  return 0;
}
//
//  End of: DefaultS2IBPathSelector::ActiveCount
//---------------------------------------------------------------------------

//! Forwards call to any driving Register(s)
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void DefaultS2IBPathSelector::Accept (SystemModelVisitor& visitor)
{
  const auto muxRegisters = AssociatedRegisters();

  for (const auto& sliceReg : *muxRegisters)
  {
    sliceReg.reg->Accept(visitor);
  }
}
//
//  End of: DefaultS2IBPathSelector::Accept
//---------------------------------------------------------------------------


//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void DefaultS2IBPathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  (void) pathIdentifier; //Dummy instruction to avoid warning
}
//
//  End of: DefaultS2IBPathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------

//! Returns true when the specified path is already selected
//!
bool DefaultS2IBPathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto selectValue = m_SelectValue;
  auto  nextToSut   = AssociatedRegisters()->NextToSut();

  bool  isSelected  = nextToSut == selectValue;
  
  LOG(DEBUG) << "Comparing SelectValue  " << m_SelectValue.DataAsBinaryString() << " and nextToSut " << nextToSut.DataAsBinaryString();

  
  LOG(DEBUG)<< isSelected;

  return isSelected;
}
//
//  End of: DefaultS2IBPathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool DefaultS2IBPathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto      selectValue  = m_SelectValue;
  const auto muxRegisters = AssociatedRegisters();
  auto       lastToSut    = muxRegisters->LastToSut();
  auto       nextToSut    = muxRegisters->NextToSut();

  bool  isSelected  = nextToSut == selectValue;
  bool  isActive    = lastToSut == selectValue;

  return isSelected && isActive;
}
//
//  End of: DefaultS2IBPathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------

//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultS2IBPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
  

   auto& selectValue  = m_DeSelectValue;
  const auto  muxRegisters = AssociatedRegisters();

  if (muxRegisters->NextToSut() != selectValue)
  {
    muxRegisters->SetToSut(selectValue);
    muxRegisters->SetPending();
    m_S2IB_SSAK_PathSelector->CloseSSAK();
  }
}
//
//  End of: DefaultS2IBPathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultS2IBPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  LOG(INFO) << "Entering Select for S2IB ";
    //BinaryVector config(m_interfaceSize);
  //Example;=: in reality should be computed from S2IB cardinality (position)
  // pathIdentifier is useless because each S2IB has 1 derivation

  if (! IsSelected(1u)) //S2IB not selected yet
    {
      LOG(INFO) << "Triggering SSAK Authentication";
     auto SSAK_State=  m_S2IB_SSAK_PathSelector->DoAuthentication(m_cardinality);
     if (SSAK_State==SSAK_PathSelector::SSAK_SelectorState::OPEN)
      //Authentication successfull, open S2IB
       {
        //---Control of Selection Bit ---start
        const auto selectValue  = m_SelectValue;
        const auto  muxRegisters = AssociatedRegisters();

        if (muxRegisters->NextToSut() != selectValue)
        {
          muxRegisters->SetToSut(selectValue);
          muxRegisters->SetPending();
	  LOG(DEBUG) << "Setting S2IB to " << selectValue.DataAsBinaryString();
	  LOG(DEBUG) << "NextToSut: " << muxRegisters->NextToSut().DataAsBinaryString();
        }
        //---Control of Selection Bit ---end
      }
}
}
//
//  End of: DefaultS2IBPathSelector::Select
//---------------------------------------------------------------------------



//! Returns some value associated with specified path selection
//!
//! @note When cannot provide a value, an empty BinaryVector is returned
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
const BinaryVector& DefaultS2IBPathSelector::SelectionValue (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  
  return m_SelectValue;
}
//
//  End of: DefaultS2IBPathSelector::SelectionValue
//---------------------------------------------------------------------------

//! Returns readable information about selector (ex: select and deselect tables)
//!
//! @param onlyProperties When true, only properties are return, otherwise content
//!                       of select/deselect tables are return as well
//!
std::string DefaultS2IBPathSelector::DebugSelectorInfo (bool onlyProperties) const
{
  auto debugInfo = PathSelector::DebugSelectorInfo(onlyProperties);


  if (!onlyProperties)
  {
    ostringstream os;

    os<< "" << "\nSelection Value :" <<   m_SelectValue.DataAsBinaryString();

    debugInfo.append(os.str());
  }

  return debugInfo;
}
//===========================================================================
// End of DefaultS2IBPathSelector.cpp
//===========================================================================
