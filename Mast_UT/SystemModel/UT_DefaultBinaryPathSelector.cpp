//===========================================================================
//                           UT_DefaultBinaryPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultBinaryPathSelector.cpp
//!
//! Implements test fixture for testing DefaultBinaryPathSelector
//!
//===========================================================================


#include "UT_DefaultBinaryPathSelector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "BinaryVector_Traits.hpp"
#include "Register.hpp"

#include <utility>
#include <memory>
#include <sstream>
using std::ostringstream;
using std::make_shared;

using namespace mast;

namespace
{

//! Checks that a path is selected and all others are not
//!
void Check_IsSelected (const DefaultBinaryPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
{
  for (uint32_t pathId = 1 ; pathId <= maxPaths ; ++pathId)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    if (pathId == pathToSelect)
    {
      TSM_ASSERT_TRUE   (message, sut.IsSelected(pathId));
    }
    else
    {
      TSM_ASSERT_FALSE  (message, sut.IsSelected(pathId));
    }
  }
}
//
//  End of: Check_IsSelected
//---------------------------------------------------------------------------



//! Checks that a path is active and all others are not
//!
void Check_IsActive (const DefaultBinaryPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
{
  for (uint32_t pathId = 1 ; pathId <= maxPaths ; ++pathId)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    if (pathId == pathToSelect)
    {
      TSM_ASSERT_TRUE   (message, sut.IsActive(pathId));
    }
    else
    {
      TSM_ASSERT_FALSE  (message, sut.IsActive(pathId));
    }
  }
}
//
//  End of: Check_IsActive
//---------------------------------------------------------------------------


} // End of unnamed namespace

//! Checks DefaultBinaryPathSelector constructor when cannot select no path
//!
void UT_DefaultBinaryPathSelector::test_Constructor_CannotSelectNone ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("000");
  auto associatedNode = make_shared<Register>("My register name", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultBinaryPathSelector(associatedNode, 5, SelectorProperty::CannotSelectNone);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);
  TS_ASSERT_TRUE   (sut.IsActive(1));
  TS_ASSERT_FALSE  (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
}

//! Checks DefaultBinaryPathSelector constructor when can select no path
//!
void UT_DefaultBinaryPathSelector::test_Constructor_CanSelectNone ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("000");
  auto associatedNode = make_shared<Register>("My register name", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultBinaryPathSelector(associatedNode, 5, SelectorProperty::CanSelectNone);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     0);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_FALSE  (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
}


//! Checks DefaultBinaryPathSelector::Select() when cannot select no path
//!
void UT_DefaultBinaryPathSelector::test_Select_CannotSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto bypassSequence = BinaryVector::CreateFromBinaryString("100");
    auto associatedNode = make_shared<Register>("", bypassSequence);
    auto maxPath        = 5u;

    auto sut = DefaultBinaryPathSelector(associatedNode, maxPath, SelectorProperty::CannotSelectNone);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(),     1);
    TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);

    Check_IsSelected (sut, pathToSelect, maxPath);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, maxPath);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultBinaryPathSelector::Select() when can select no path
//!
void UT_DefaultBinaryPathSelector::test_Select_CanSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto bypassSequence = BinaryVector::CreateFromBinaryString("010");
    auto associatedNode = make_shared<Register>("Reg", bypassSequence);
    auto maxPath        = 5u;

    auto sut = DefaultBinaryPathSelector(associatedNode, maxPath,  SelectorProperty::CanSelectNone
                                                                 | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(),     1);
    TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);

    Check_IsSelected (sut, pathToSelect, maxPath);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, maxPath);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}

//! Checks DefaultBinaryPathSelector::Select() when can select no path and selection table is inverted
//!
void UT_DefaultBinaryPathSelector::test_Select_CanSelectNoneInverted ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto bypassSequence = BinaryVector::CreateFromBinaryString("110");
    auto associatedNode = make_shared<Register>("My register name", bypassSequence);
    auto maxPath        = 5u;

    auto sut = DefaultBinaryPathSelector(associatedNode, maxPath, SelectorProperty::CanSelectNone
                                                                 | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(),     1);

    Check_IsSelected (sut, pathToSelect, maxPath);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, maxPath);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}
//! Checks DefaultBinaryPathSelector::Select() when can select no path
//!
void UT_DefaultBinaryPathSelector::test_Select_Path_Zero ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("010");
  auto reg            = make_shared<Register>("Reg", bypassSequence, true);
  auto maxPath        = 5u;

  auto sut = DefaultBinaryPathSelector(reg, maxPath, SelectorProperty::CanSelectNone);

  // ---------------- Exercise
  //
  sut.Select(0u);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);

  TS_ASSERT_TRUE   (sut.IsSelected(0u));
  TS_ASSERT_FALSE  (sut.IsSelected(1u));
  TS_ASSERT_FALSE  (sut.IsSelected(2u));
  TS_ASSERT_FALSE  (sut.IsSelected(3u));
  TS_ASSERT_FALSE  (sut.IsSelected(4u));
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->BypassSequence());
}


//! Checks DefaultBinaryPathSelector::Deselect() when can select no path
//!
void UT_DefaultBinaryPathSelector::test_Deselect ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToDeselect)
  {
    // ---------------- Setup
    //
    auto bypassSequence = BinaryVector::CreateFromBinaryString("010");
    auto associatedNode = make_shared<Register>("Reg", bypassSequence);
    auto maxPath        = 5u;

    auto sut = DefaultBinaryPathSelector(associatedNode, maxPath, SelectorProperty::CanSelectNone);

    // ---------------- Exercise
    //
    sut.Deselect(pathToDeselect);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsSelected(pathToDeselect));

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_FALSE  (sut.IsActive(pathToDeselect));

    if (pathToDeselect != 2u)
    {
      TS_ASSERT_FALSE  (sut.IsActive(2u));
    }
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//===========================================================================
// End of UT_DefaultBinaryPathSelector.cpp
//===========================================================================
