//===========================================================================
//                           UT_DefaultOneHotPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultOneHotPathSelector.cpp
//!
//! Implements test fixture for testing DefaultOneHotPathSelector
//!
//===========================================================================


#include "UT_DefaultOneHotPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
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
void Check_IsSelected (const DefaultOneHotPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
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
void Check_IsActive (const DefaultOneHotPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
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


//! Checks DefaultOneHotPathSelector constructor
//!
void UT_DefaultOneHotPathSelector::test_Constructor ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("00000");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultOneHotPathSelector(associatedNode, 5);

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


//! Checks DefaultOneHotPathSelector constructor when can select no path
//!
void UT_DefaultOneHotPathSelector::test_Constructor_Inverted ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("11111");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultOneHotPathSelector(associatedNode, 5u);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     0);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_FALSE  (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
}


//! Checks DefaultOneHotPathSelector constructor when requesting reverse order and inverted selection bits
//!
void UT_DefaultOneHotPathSelector::test_Constructor_ReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("10111");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultOneHotPathSelector(associatedNode, 5u,    SelectorProperty::ReverseOrder
                                                            | SelectorProperty::InvertedBits);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
  TS_ASSERT_FALSE  (sut.IsSelected(3));
  TS_ASSERT_FALSE  (sut.IsSelected(4));
  TS_ASSERT_FALSE  (sut.IsSelected(5));
}


//! Checks DefaultOneHotPathSelector constructor when requesting reverse order for selection bits
//!
void UT_DefaultOneHotPathSelector::test_Constructor_InvertedReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("00010");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultOneHotPathSelector(associatedNode, 5u, SelectorProperty::ReverseOrder);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_FALSE  (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_TRUE   (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_FALSE  (sut.IsSelected(2));
  TS_ASSERT_FALSE  (sut.IsSelected(3));
  TS_ASSERT_TRUE   (sut.IsSelected(4));
  TS_ASSERT_FALSE  (sut.IsSelected(5));
}



//! Checks DefaultOneHotPathSelector constructor when associated register is too small
//!
void UT_DefaultOneHotPathSelector::test_Constructor_TooSmallRegister ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("000");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (DefaultOneHotPathSelector(associatedNode, 5u);, std::exception);
}

//! Checks DefaultOneHotPathSelector::Select()
//!
void UT_DefaultOneHotPathSelector::test_Select ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("01000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultOneHotPathSelector(associatedNode, pathsCount);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(),     1u);
    TS_ASSERT_EQUALS (sut.SelectablePaths(), pathsCount);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}

//! Checks DefaultOneHotPathSelector::Select() when selection table is inverted
//!
void UT_DefaultOneHotPathSelector::test_Select_Inverted ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("01111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultOneHotPathSelector(associatedNode, pathsCount, SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 1u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultOneHotPathSelector::Select() when requesting reverse order for selection bits
//!
void UT_DefaultOneHotPathSelector::test_Select_ReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("01000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultOneHotPathSelector(associatedNode, pathsCount, SelectorProperty::ReverseOrder);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 1u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultOneHotPathSelector::Select() when requesting reverse order and inverted selection bits
//!
void UT_DefaultOneHotPathSelector::test_Select_InvertedReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11101"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultOneHotPathSelector(associatedNode,
                                                    pathsCount,
                                                      SelectorProperty::ReverseOrder
                                                    | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 1u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultOneHotPathSelector::Select() of no path (deselect any)
//!
void UT_DefaultOneHotPathSelector::test_Select_Path_Zero ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultOneHotPathSelector(reg, pathsCount);

  // ---------------- Exercise
  //
  sut.Select(0u);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), pathsCount);

  TS_ASSERT_TRUE   (sut.IsSelected(0u));
  TS_ASSERT_FALSE  (sut.IsSelected(1u));
  TS_ASSERT_FALSE  (sut.IsSelected(2u));
  TS_ASSERT_FALSE  (sut.IsSelected(3u));
  TS_ASSERT_FALSE  (sut.IsSelected(4u));
  TS_ASSERT_FALSE  (sut.IsSelected(5u));
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->BypassSequence());
}


//! Checks DefaultOneHotPathSelector::Select() when path identifier is out of range
//!
void UT_DefaultOneHotPathSelector::test_Select_OutOfRange ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultOneHotPathSelector(reg, pathsCount);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Select(6u), std::exception);
}

//! Checks DefaultOneHotPathSelector::Deselect()
//!
void UT_DefaultOneHotPathSelector::test_Deselect ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToDeselect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00010"), true);
    auto sut            = DefaultOneHotPathSelector(associatedNode, 5u);

    // ---------------- Exercise
    //
    sut.Deselect(pathToDeselect);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsSelected(pathToDeselect));

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_FALSE  (sut.IsActive(pathToDeselect));
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}

//! Checks DefaultOneHotPathSelector::Deselect() when path identifier is out of range
//!
void UT_DefaultOneHotPathSelector::test_Deselect_OutOfRange ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultOneHotPathSelector(reg, pathsCount);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Deselect(6u), std::exception);
}

//===========================================================================
// End of UT_DefaultOneHotPathSelector.cpp
//===========================================================================
