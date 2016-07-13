//===========================================================================
//                           UT_DefaultNHotPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultNHotPathSelector.cpp
//!
//! Implements test fixture for testing DefaultNHotPathSelector
//!
//===========================================================================


#include "UT_DefaultNHotPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "BinaryVector_Traits.hpp"
#include "Register.hpp"

#include <utility>
#include <memory>
#include <sstream>
using std::ostringstream;
using std::make_shared;
using std::initializer_list;

using namespace mast;

namespace
{

//! Checks that a path is selected and all others are not
//!
void Check_IsSelected (const DefaultNHotPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
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


//! Checks that paths are selected
//!
void Check_IsSelected (const DefaultNHotPathSelector& sut, initializer_list<uint32_t> pathIds)
{
  for (auto pathId : pathIds)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    TSM_ASSERT_TRUE   (message, sut.IsSelected(pathId));
  }
}
//
//  End of: Check_IsSelected
//---------------------------------------------------------------------------


//! Checks that paths are not selected
//!
void Check_IsNotSelected (const DefaultNHotPathSelector& sut, initializer_list<uint32_t> pathIds)
{
  for (auto pathId : pathIds)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    TSM_ASSERT_FALSE(message, sut.IsSelected(pathId));
  }
}
//
//  End of: Check_IsSelected
//---------------------------------------------------------------------------


//! Checks that a path is active and all others are not
//!
void Check_IsActive (const DefaultNHotPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
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


//! Checks that a paths are active
//!
void Check_IsActive (const DefaultNHotPathSelector& sut, initializer_list<uint32_t> pathIds)
{
  for (auto pathId : pathIds)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    TSM_ASSERT_TRUE(message, sut.IsActive(pathId));
  }
}
//
//  End of: Check_IsSelected
//---------------------------------------------------------------------------


//! Checks that a paths are NOT active
//!
void Check_IsNotActive (const DefaultNHotPathSelector& sut, initializer_list<uint32_t> pathIds)
{
  for (auto pathId : pathIds)
  {
    ostringstream os;
    os << "Path id: " << pathId;
    auto message = os.str();

    TSM_ASSERT_FALSE(message, sut.IsActive(pathId));
  }
}
//
//  End of: Check_IsSelected
//---------------------------------------------------------------------------

} // End of unnamed namespace


//! Checks DefaultNHotPathSelector constructor
//!
void UT_DefaultNHotPathSelector::test_Constructor ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("00000");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5);

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


//! Checks DefaultNHotPathSelector constructor when selection bit are inverted (negative logic)
//!
void UT_DefaultNHotPathSelector::test_Constructor_Inverted ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("11111");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u, SelectorProperty::CanSelectNone | SelectorProperty::InvertedBits);

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


//! Checks DefaultNHotPathSelector constructor when requesting reverse order for selection bits
//!
void UT_DefaultNHotPathSelector::test_Constructor_ReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("10010");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u, SelectorProperty::CanSelectNone | SelectorProperty::ReverseOrder);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     2u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_TRUE   (sut.IsActive(5));
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
  TS_ASSERT_FALSE  (sut.IsSelected(3));
  TS_ASSERT_FALSE  (sut.IsSelected(4));
  TS_ASSERT_TRUE   (sut.IsSelected(5));
}


//! Checks DefaultNHotPathSelector constructor when requesting reverse order and inverted selection bits
//!
void UT_DefaultNHotPathSelector::test_Constructor_InvertedReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("10111");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u,    SelectorProperty::CanSelectNone
                                                          | SelectorProperty::ReverseOrder
                                                          | SelectorProperty::InvertedBits);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     1u);
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


//! Checks DefaultNHotPathSelector constructor
//!
void UT_DefaultNHotPathSelector::test_Constructor_Multiple ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("01100");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     2);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_TRUE   (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
}


//! Checks DefaultNHotPathSelector constructor when selection bit are inverted (negative logic)
//!
void UT_DefaultNHotPathSelector::test_Constructor_Multiple_Inverted ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("10010");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u, SelectorProperty::CanSelectNone | SelectorProperty::InvertedBits);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     3u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_TRUE   (sut.IsActive(3));
  TS_ASSERT_FALSE  (sut.IsActive(4));
  TS_ASSERT_TRUE   (sut.IsActive(5));
}


//! Checks DefaultNHotPathSelector constructor when requesting reverse order for selection bits
//!
void UT_DefaultNHotPathSelector::test_Constructor_Multiple_ReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("11010");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u, SelectorProperty::CanSelectNone | SelectorProperty::ReverseOrder);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     3u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_FALSE  (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_TRUE   (sut.IsActive(4));
  TS_ASSERT_TRUE   (sut.IsActive(5));
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
  TS_ASSERT_FALSE  (sut.IsSelected(3));
  TS_ASSERT_TRUE   (sut.IsSelected(4));
  TS_ASSERT_TRUE   (sut.IsSelected(5));
}


//! Checks DefaultNHotPathSelector constructor when requesting reverse order and inverted selection bits
//!
void UT_DefaultNHotPathSelector::test_Constructor_Multiple_InvertedReverseOrder ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("10100");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise
  //
  auto sut = DefaultNHotPathSelector(associatedNode, 5u,    SelectorProperty::CanSelectNone
                                                          | SelectorProperty::ReverseOrder
                                                          | SelectorProperty::InvertedBits);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.ActiveCount(),     3u);
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 5u);
  TS_ASSERT_TRUE   (sut.IsActive(1));
  TS_ASSERT_TRUE   (sut.IsActive(2));
  TS_ASSERT_FALSE  (sut.IsActive(3));
  TS_ASSERT_TRUE   (sut.IsActive(4));
  TS_ASSERT_FALSE  (sut.IsActive(5));
  TS_ASSERT_TRUE   (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
  TS_ASSERT_FALSE  (sut.IsSelected(3));
  TS_ASSERT_TRUE   (sut.IsSelected(4));
  TS_ASSERT_FALSE  (sut.IsSelected(5));
}



//! Checks DefaultNHotPathSelector constructor when associated register is too small
//!
void UT_DefaultNHotPathSelector::test_Constructor_TooSmallRegister ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("000");
  auto associatedNode = make_shared<Register>("Reg", bypassSequence);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (DefaultNHotPathSelector(associatedNode, 5u);, std::exception);
}

//! Checks DefaultNHotPathSelector::Select()
//!
void UT_DefaultNHotPathSelector::test_Select ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(),     0u);
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

//! Checks DefaultNHotPathSelector::Select() when selection table is inverted
//!
void UT_DefaultNHotPathSelector::test_Select_Inverted ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultNHotPathSelector::Select() when requesting reverse order for selection bits
//!
void UT_DefaultNHotPathSelector::test_Select_ReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::ReverseOrder);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultNHotPathSelector::Select() when requesting reverse order and inverted selection bits
//!
void UT_DefaultNHotPathSelector::test_Select_InvertedReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode,
                                                  pathsCount,
                                                    SelectorProperty::ReverseOrder
                                                  | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathToSelect, pathsCount);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, pathsCount);
  };

  auto inputs = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, inputs);
}


//! Checks DefaultNHotPathSelector::Select() of no path (deselect any)
//!
void UT_DefaultNHotPathSelector::test_Select_Path_Zero ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultNHotPathSelector(reg, pathsCount);

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


//! Checks DefaultNHotPathSelector::Select() when several are selected
//!
void UT_DefaultNHotPathSelector::test_Select_Multiple ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Select(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), pathIds.size());

    Check_IsActive   (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathSelector::Select() when several are selected and selection bits are inverted
//!
void UT_DefaultNHotPathSelector::test_Select_Multiple_Inverted ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CanSelectNone
                                                                            | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Select(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), pathIds.size());

    Check_IsActive   (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathSelector::Select() when several are selected and bits order are reversed
//!
void UT_DefaultNHotPathSelector::test_Select_Multiple_ReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CanSelectNone
                                                                            | SelectorProperty::ReverseOrder);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Select(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), pathIds.size());

    Check_IsActive   (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathSelector::Select() when several are selected and bits order are reversed and inverted
//!
void UT_DefaultNHotPathSelector::test_Select_Multiple_InvertedReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CanSelectNone
                                                                            | SelectorProperty::InvertedBits
                                                                            | SelectorProperty::ReverseOrder);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Select(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);

    Check_IsSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), pathIds.size());

    Check_IsActive   (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathSelector::Select() when path identifier is out of range
//!
void UT_DefaultNHotPathSelector::test_Select_OutOfRange ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultNHotPathSelector(reg, pathsCount);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Select(6u), std::exception);
}

//! Checks DefaultNHotPathSelector::Deselect()
//!
void UT_DefaultNHotPathSelector::test_Deselect ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToDeselect)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00010"), true);
    auto sut            = DefaultNHotPathSelector(associatedNode, 5u);

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

//! Checks DefaultNHotPathDeselector::Deselect() when several are selected
//!
void UT_DefaultNHotPathSelector::test_Deselect_Multiple ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Deselect(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u);

    Check_IsNotSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u - pathIds.size());

    Check_IsNotActive   (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathDeselector::Deselect() when several are selected
//!
void UT_DefaultNHotPathSelector::test_Deselect_Multiple_InvertedReverseOrder ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00000"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CanSelectNone
                                                                            | SelectorProperty::ReverseOrder
                                                                            | SelectorProperty::InvertedBits);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Deselect(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u);

    Check_IsNotSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u - pathIds.size());

    Check_IsNotActive (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}, {1u, 5u, 3u, 4u, 2u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks DefaultNHotPathDeselector::Deselect() when configured to forbid not path selection
//!
void UT_DefaultNHotPathSelector::test_Deselect_CannotSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathIds)
  {
    // ---------------- Setup
    //
    auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("11111"));
    uint32_t pathsCount = 5u;
    auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CannotSelectNone);

    // ---------------- Exercise
    //
    for (const auto& pathId : pathIds)
    {
      sut.Deselect(pathId);
    }

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u);

    Check_IsNotSelected (sut, pathIds);

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_EQUALS (sut.ActiveCount(), 5u - pathIds.size());

    Check_IsNotActive (sut, pathIds);
  };

  using  data_t = initializer_list<initializer_list<uint32_t>>;
  data_t data   = {{1u, 2u}, {3u, 4u}, {1u, 5u}, {1u, 3u, 5u}, {1u, 5u, 3u, 4u}};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks DefaultNHotPathDeselector::Deselect() when configured to forbid no path selection
//!                                              and try to deselect last selected path
//!
void UT_DefaultNHotPathSelector::test_Deselect_CannotSelectNone_LastOne ()
{
  // ---------------- Setup
  //
  auto associatedNode = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("00010"));
  uint32_t pathsCount = 5u;
  auto sut            = DefaultNHotPathSelector(associatedNode, pathsCount, SelectorProperty::CannotSelectNone);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Deselect(4u), std::exception);
}



//! Checks DefaultNHotPathSelector::Deselect() when path identifier is out of range
//!
void UT_DefaultNHotPathSelector::test_Deselect_OutOfRange ()
{
  // ---------------- Setup
  //
  auto     reg        = make_shared<Register>("Reg", BinaryVector::CreateFromBinaryString("10000"), true);
  uint32_t pathsCount = 5u;
  auto     sut        = DefaultNHotPathSelector(reg, pathsCount);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Deselect(6u), std::exception);
}

//===========================================================================
// End of UT_DefaultNHotPathSelector.cpp
//===========================================================================
