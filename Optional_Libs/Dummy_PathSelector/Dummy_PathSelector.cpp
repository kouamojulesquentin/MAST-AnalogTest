//===========================================================================
//                           Dummy_PathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dummy_PathSelector.cpp
//!
//! Implements class Dummy_PathSelector
//!It is a model to build a custom PathSelector which implements all virtual
//! functions but does nothing: its behavior is the same as a chain node
//! 
//===========================================================================

#include "Dummy_PathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>

using std::ostringstream;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;

using namespace mast;



//! Returns readable information about selector (ex: select and deselect tables)
//!
//! @param onlyProperties When true, only properties are return, otherwise content
//!                       of select/deselect tables are return as well
//!
string Dummy_PathSelector::DebugSelectorInfo (bool onlyProperties) const
{
  auto debugInfo = PathSelector::DebugSelectorInfo(onlyProperties);


  if (!onlyProperties)
  {
    ostringstream os;
    
    os << "\nDummy Selector";

    debugInfo.append(os.str());
  }

  return debugInfo;
}
//
//  End of: Dummy_PathSelector::DebugSelectorInfo
//---------------------------------------------------------------------------



//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegisters  Register(s) that is/are used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param selectTable          Table to use for selecting a path
//! @param deselectTable        Table to use for deselecting a path
//! @param properties           Properties of the selector (mainly to report that it can select no path or not)
//!
Dummy_PathSelector::Dummy_PathSelector (std::shared_ptr<Register> associatedRegister,
                                                              uint32_t               pathsCount,
                                                              const std::string& parameters)
  : PathSelector    (SelectorProperty::None)
  , m_pathsCount    (pathsCount)
  , m_muxRegisters  (associatedRegister)
{
 (void) parameters; //Useless line to avoid Unused Parameter warning
}
//
//  End of: Dummy_PathSelector::Dummy_PathSelector
//---------------------------------------------------------------------------


//! Forwards call to any driving Register(s)
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void Dummy_PathSelector::Accept (SystemModelVisitor& visitor)
{
  const auto muxRegisters = AssociatedRegisters();

  for (const auto& sliceReg : *muxRegisters)
  {
    sliceReg.reg->Accept(visitor);
  }
}
//
//  End of: Dummy_PathSelector::Accept
//---------------------------------------------------------------------------



//! Returns the number of paths that are currently active
//!
uint32_t Dummy_PathSelector::ActiveCount () const
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
//  End of: Dummy_PathSelector::ActiveCount
//---------------------------------------------------------------------------



//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void Dummy_PathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier > m_pathsCount)
  {
    ostringstream os; os << "pathIdentifier must be < " << m_pathsCount << ", got: " << pathIdentifier;
    THROW_OUT_OF_RANGE(os.str());
  }
}
//
//  End of: Dummy_PathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------

//! Returns true when the specified path is already selected
//!
bool Dummy_PathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

    bool  isActive    = true; //All paths are always active

  return isActive;
}
//
//  End of: Dummy_PathSelector::IsActive
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool Dummy_PathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  bool  isSelected   = true; //All paths are always selected

  return isSelected;
}
//
//  End of: Dummy_PathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool Dummy_PathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  bool  isSelected  = true; //All paths are always selected
  bool  isActive    = true; //All paths are always active

  return isSelected && isActive;
}
//
//  End of: Dummy_PathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------

//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void Dummy_PathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
  
  return; //Paths can never be deselected
}
//
//  End of: Dummy_PathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void Dummy_PathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  return; //Paths are always selected

}
//
//  End of: Dummy_PathSelector::Select
//---------------------------------------------------------------------------



//! Returns some value associated with specified path selection
//!
//! @note When cannot provide a value, an empty BinaryVector is returned
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
const BinaryVector& Dummy_PathSelector::SelectionValue (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  return  m_DummySelectorInformation;
}
//
//  End of: Dummy_PathSelector::SelectionValue
//---------------------------------------------------------------------------

//===========================================================================
// End of Dummy_PathSelector.cpp
//===========================================================================
