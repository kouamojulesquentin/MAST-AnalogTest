//===========================================================================
//                           DefaultTableBasedPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultTableBasedPathSelector.cpp
//!
//! Implements class DefaultTableBasedPathSelector
//!
//===========================================================================

#include "DefaultTableBasedPathSelector.hpp"
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


//! Streams content of a selector table
//!
//! @param os       Output stream
//! @param prefix   What is inserted before each line
//! @param title    Title that is streamed just before table content
//! @param table    Table to stream content
//!
void DefaultTableBasedPathSelector::StreamTable (std::ostream& os, string_view prefix, string_view title, const TablesType& table)
{
  os << std::endl;

  if (!title.empty())
  {
    os << prefix << title;
  }

  uint32_t pathID = 0;
  for (const auto& bv : table)
  {
    os << std::endl << prefix;
    os << "  [" << pathID++ << "] " << bv.DataAsMixString();
  }
}
//
//  End of: DefaultTableBasedPathSelector::StreamTable
//---------------------------------------------------------------------------


//! Returns readable information about selector (ex: select and deselect tables)
//!
//! @param onlyProperties When true, only properties are return, otherwise content
//!                       of select/deselect tables are return as well
//!
string DefaultTableBasedPathSelector::DebugSelectorInfo (bool onlyProperties) const
{
  auto debugInfo = PathSelector::DebugSelectorInfo(onlyProperties);


  if (!onlyProperties)
  {
    ostringstream os;

    StreamTable(os, "", "\nSelection Table:",   m_selectTable);
    StreamTable(os, "", "\nDeselection Table:", m_deselectTable);

    debugInfo.append(os.str());
  }

  return debugInfo;
}
//
//  End of: DefaultTableBasedPathSelector::DebugSelectorInfo
//---------------------------------------------------------------------------



//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegisters  Register(s) that is/are used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param selectTable          Table to use for selecting a path
//! @param deselectTable        Table to use for deselecting a path
//! @param properties           Properties of the selector (mainly to report that it can select no path or not)
//!
DefaultTableBasedPathSelector::DefaultTableBasedPathSelector (const VirtualRegister& associatedRegisters,
                                                              uint32_t               pathsCount,
                                                              TablesType&&           selectTable,
                                                              TablesType&&           deselectTable,
                                                              SelectorProperty       properties)
  : PathSelector    (properties)
  , m_selectTable   (std::move(FixTable(selectTable)))
  , m_deselectTable (std::move(FixTable(deselectTable)))
  , m_pathsCount    (pathsCount)
  , m_muxRegisters  (associatedRegisters)
{
  if (m_selectTable.size() != m_deselectTable.size())
  {
    ostringstream os;
    os << "(de)Selection tables size differ, got " << m_selectTable.size();
    os << " for select and "                       << m_deselectTable.size() << " for deselect";
    THROW_INVALID_ARGUMENT(os.str());
  }

  if (m_deselectTable.size() <= m_pathsCount)
  {
    ostringstream os;
    os << "Selection and deselection table must have an entry for not used path identifier zero";
    THROW_INVALID_ARGUMENT(os.str());
  }
}
//
//  End of: DefaultTableBasedPathSelector::DefaultTableBasedPathSelector
//---------------------------------------------------------------------------


//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param selectTable          Table to use for selecting a path
//! @param deselectTable        Table to use for deselecting a path
//! @param properties           Properties of the selector (mainly to report that it can select no path or not)
//!
DefaultTableBasedPathSelector::DefaultTableBasedPathSelector (shared_ptr<Register> associatedRegister,
                                                              uint32_t         pathsCount,
                                                              TablesType&&     selectTable,
                                                              TablesType&&     deselectTable,
                                                              SelectorProperty properties)
  : DefaultTableBasedPathSelector (VirtualRegister(associatedRegister),
                                   pathsCount,
                                   std::move(selectTable),
                                   std::move(deselectTable),
                                   properties)
{
}
//
//  End of: DefaultTableBasedPathSelector::DefaultTableBasedPathSelector
//---------------------------------------------------------------------------


//! Initializes selector for selection/deselection of a paths
//!
//! @note This version is based upon string defined tables values (for description by text file)
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector are currently ignored (but are saved for report purpose)
//! @param tables               String encoded select/deselect table with coma separated values
//!                             First half values are for the selection table,
//!                             second half for the deselection table
//!
DefaultTableBasedPathSelector::DefaultTableBasedPathSelector (shared_ptr<Register> associatedRegister,
                                                              uint32_t             pathsCount,
                                                              SelectorProperty     properties,
                                                              string_view          tables)
  : PathSelector    (properties)
  , m_pathsCount    (pathsCount)
  , m_muxRegisters  (associatedRegister)

{
  auto vectors     = Utility::Split(tables, ",");
  auto valuesCount = vectors.size();
  auto tableDepth  = valuesCount / 2;

  CHECK_PARAMETER_NEQ (valuesCount,      0,          "Missing values for defining select/deselect tables");
  CHECK_PARAMETER_EQ  (valuesCount % 2u, 0,          "There must be same number of select/deselect table value");
  CHECK_PARAMETER_GT  (tableDepth,       pathsCount, "Tables depth must be at least one greater than selectable paths count");

  // ---------------- Fill selection table
  //
  for (size_t ii = 0 ; ii < tableDepth; ++ii)
  {
    auto value        = vectors[ii];
    auto binaryVector = BinaryVector::CreateFromString(value);

    CHECK_PARAMETER_EQ(binaryVector.BitsCount(), associatedRegister->BitsCount(), "Selection table element must have same size as associated REGISTER");
    m_selectTable.emplace_back(std::move(binaryVector));
  }

  // ---------------- Fill deselection table
  //
  for (size_t ii = tableDepth ; ii < valuesCount; ++ii)
  {
    auto value        = vectors[ii];
    auto binaryVector = BinaryVector::CreateFromString(value);

    CHECK_PARAMETER_EQ(binaryVector.BitsCount(), associatedRegister->BitsCount(), "Deselection table element must have same size as associated REGISTER");
    m_deselectTable.emplace_back(std::move(binaryVector));
  }
}
//
//  End of: DefaultTableBasedPathSelector::DefaultTableBasedPathSelector
//---------------------------------------------------------------------------


//! Forwards call to any driving Register(s)
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void DefaultTableBasedPathSelector::Accept (SystemModelVisitor& visitor)
{
  const auto muxRegisters = AssociatedRegisters();

  for (const auto& sliceReg : *muxRegisters)
  {
    sliceReg.reg->Accept(visitor);
  }
}
//
//  End of: DefaultTableBasedPathSelector::Accept
//---------------------------------------------------------------------------



//! Returns the number of paths that are currently active
//!
uint32_t DefaultTableBasedPathSelector::ActiveCount () const
{
  uint32_t activeCount = 0u;

  for (uint32_t pathId = 1u ; pathId < m_selectTable.size() ; ++pathId)
  {
    if (IsActive(pathId))
    {
      ++activeCount;
    }
  }
  return activeCount;
}
//
//  End of: DefaultTableBasedPathSelector::ActiveCount
//---------------------------------------------------------------------------



//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void DefaultTableBasedPathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier >= m_selectTable.size())
  {
    ostringstream os; os << "pathIdentifier must be < " << m_selectTable.size() << ", got: " << pathIdentifier;
    THROW_OUT_OF_RANGE(os.str());
  }
}
//
//  End of: DefaultTableBasedPathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------


//! Inverts all bits of a LUT
//!
DefaultTableBasedPathSelector::TablesType& DefaultTableBasedPathSelector::InvertTable (TablesType& table)
{
  for (auto& elem : table)
  {
    elem = ~elem;
  }
  return table;
}
//
//  End of: DefaultTableBasedPathSelector::InvertTable
//---------------------------------------------------------------------------


//! Makes table entry fix (cannot modify size)
//!
DefaultTableBasedPathSelector::TablesType& DefaultTableBasedPathSelector::FixTable (TablesType& table)
{
  for (auto& elem : table)
  {
    elem.FixSize(true);
  }
  return table;
}
//
//  End of: DefaultTableBasedPathSelector::FixTable
//---------------------------------------------------------------------------


//! Returns true when the specified path is already active
//!
bool DefaultTableBasedPathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& selectValue = m_selectTable[pathIdentifier];
  auto  lastToSut   = AssociatedRegisters()->LastToSut();

  bool  isActive    = lastToSut == selectValue;

  return isActive;
}
//
//  End of: DefaultTableBasedPathSelector::IsActive
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool DefaultTableBasedPathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& selectValue = m_selectTable[pathIdentifier];
  auto  nextToSut   = AssociatedRegisters()->NextToSut();

  bool  isSelected  = nextToSut == selectValue;

  return isSelected;
}
//
//  End of: DefaultTableBasedPathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool DefaultTableBasedPathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto&      selectValue  = m_selectTable[pathIdentifier];
  const auto muxRegisters = AssociatedRegisters();
  auto       lastToSut    = muxRegisters->LastToSut();
  auto       nextToSut    = muxRegisters->NextToSut();

  bool  isSelected  = nextToSut == selectValue;
  bool  isActive    = lastToSut == selectValue;

  return isSelected && isActive;
}
//
//  End of: DefaultTableBasedPathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------

//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultTableBasedPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
  

   auto& selectValue  = m_deselectTable[pathIdentifier];
  const auto  muxRegisters = AssociatedRegisters();

  if (!CanSelectNone() && (selectValue == m_selectTable[0]))
      selectValue = m_selectTable[1]; //As selector cannot be closed, select first path

  if (muxRegisters->NextToSut() != selectValue)
  {
    muxRegisters->SetToSut(selectValue);
    muxRegisters->SetPending();
  }
}
//
//  End of: DefaultTableBasedPathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultTableBasedPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectValue  = m_selectTable[pathIdentifier];
  const auto  muxRegisters = AssociatedRegisters();

  if (muxRegisters->NextToSut() != selectValue)
  {
    muxRegisters->SetToSut(selectValue);
    muxRegisters->SetPending();
  }
}
//
//  End of: DefaultTableBasedPathSelector::Select
//---------------------------------------------------------------------------



//! Returns some value associated with specified path selection
//!
//! @note When cannot provide a value, an empty BinaryVector is returned
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
const BinaryVector& DefaultTableBasedPathSelector::SelectionValue (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectValue = m_selectTable[pathIdentifier];
  return selectValue;
}
//
//  End of: DefaultTableBasedPathSelector::SelectionValue
//---------------------------------------------------------------------------

//===========================================================================
// End of DefaultTableBasedPathSelector.cpp
//===========================================================================
