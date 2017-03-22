//===========================================================================
//                           UT_DefaultTableBasedPathSelector.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultTableBasedPathSelector.cpp
//!
//! Implements test fixture for testing DefaultTableBasedPathSelector
//!
//===========================================================================


#include "UT_DefaultTableBasedPathSelector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "BinaryVector_Traits.hpp"
#include "Register.hpp"
#include "Utility.hpp"

#include <utility>
#include <memory>
#include <sstream>
#include <experimental/string_view>

using std::ostringstream;
using std::shared_ptr;
using std::make_shared;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

namespace
{

//! Checks that a path is selected and all others are not
//!
void Check_IsSelected (const DefaultTableBasedPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
{
  for (uint32_t pathId = 1u ; pathId <= maxPaths ; ++pathId)
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
void Check_IsActive (const DefaultTableBasedPathSelector& sut, uint32_t pathToSelect, uint32_t maxPaths)
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



//! Checks DefaultTableBasedPathSelector constructor when cannot select no path
//!
void UT_DefaultTableBasedPathSelector::test_Constructor_CannotSelectNone ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("010");
  auto associatedNode = make_shared<Register>("My register name", bypassSequence);
  auto tables         = "0b000, 0b001, 0b010,"
                        "0b000, 0b110, 0b101"sv;

  // ---------------- Exercise
  //
  auto sut = DefaultTableBasedPathSelector(associatedNode, 2, SelectorProperty::CannotSelectNone, tables);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.SelectablePaths(),  2);
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
  TS_ASSERT_EQUALS (sut.KindName(),         "Table_Based");
  TS_ASSERT_EQUALS (sut.SelectionValue(2u), BinaryVector::CreateFromString("0b010"));
}

//! Checks DefaultTableBasedPathSelector constructor when can select no path
//!
void UT_DefaultTableBasedPathSelector::test_Constructor_CanSelectNone ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("010");
  auto associatedNode = make_shared<Register>("Sel", bypassSequence);
  auto tables         = "0b000, 0b001, 0b010,"
                        "0b000, 0b000, 0b000"sv;

  // ---------------- Exercise
  //
  auto sut = DefaultTableBasedPathSelector(associatedNode, 2, SelectorProperty::CanSelectNone, tables);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.CanSelectNone());
  TS_ASSERT_EQUALS (sut.SelectablePaths(), 2);
  TS_ASSERT_FALSE  (sut.IsSelected(1));
  TS_ASSERT_TRUE   (sut.IsSelected(2));
}


//! Checks DefaultTableBasedPathSelector constructor when associated register is not valid (nullptr)
//!
void UT_DefaultTableBasedPathSelector::test_Constructor_nullptr_Register ()
{
  // ---------------- Setup
  //
  auto associatedNode = shared_ptr<Register>();
  auto tables         = ""sv;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (DefaultTableBasedPathSelector(associatedNode, 5, SelectorProperty::Binary_Default, tables), std::exception);
}


//! Checks DefaultTableBasedPathSelector::Select() when cannot select no path
//!
void UT_DefaultTableBasedPathSelector::test_Select_CannotSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    auto bypassSequence = BinaryVector::CreateFromBinaryString("0_0010");
    auto maxPath        = 5u;
    auto associatedNode = make_shared<Register>("Control", bypassSequence);
    auto tables         = "0b0_0000, 0b0_0001, 0b0_0110, 0b0_0100, 0b0_1000, 0b1_0000,"     // Selection table
                          "0b0_0000, 0b1_1110, 0b1_1101, 0b1_1011, 0b1_0111, 0b0_1111,"sv;  // Deselection table

    auto sut = DefaultTableBasedPathSelector(associatedNode, maxPath, SelectorProperty::CannotSelectNone, tables);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.CanSelectNone());
    TS_ASSERT_EQUALS (sut.SelectablePaths(), maxPath);

    Check_IsSelected (sut, pathToSelect, maxPath);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, maxPath);
  };

  auto pathsToSelect = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, pathsToSelect);
}


//! Checks DefaultTableBasedPathSelector::Select() when can select no path
//!
void UT_DefaultTableBasedPathSelector::test_Select_CanSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToSelect)
  {
    auto bypassSequence = BinaryVector::CreateFromBinaryString("0_0010");
    auto maxPath        = 5u;
    auto associatedNode = make_shared<Register>("Control", bypassSequence);
    auto tables         = "0b0_0000, 0b0_0001, 0b0_0110, 0b0_0100, 0b0_1000, 0b1_0000,"     // Selection table
                          "0b0_0000, 0b1_1110, 0b1_1101, 0b1_1011, 0b1_0111, 0b0_1111,"sv;  // Deselection table

    auto sut = DefaultTableBasedPathSelector(associatedNode, maxPath, SelectorProperty::CanSelectNone, tables);

    // ---------------- Exercise
    //
    sut.Select(pathToSelect);

    // ---------------- Verify
    //
    TS_ASSERT_TRUE   (sut.CanSelectNone());
    TS_ASSERT_EQUALS (sut.SelectablePaths(), maxPath);

    Check_IsSelected (sut, pathToSelect, maxPath);

    associatedNode->UpdateLastToSut();   // Force update
    Check_IsActive   (sut, pathToSelect, maxPath);
  };

  auto pathsToSelect = {0u, 1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, pathsToSelect);
}



//! Checks DefaultTableBasedPathSelector::Select() when can selecting none
//!
void UT_DefaultTableBasedPathSelector::test_Select_Path_Zero ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1_1111");
  auto maxPath        = 4u;
  auto reg            = make_shared<Register>("Control", bypassSequence);
  auto tables         = "0b1_1111, 0b0_0001, 0b0_0110, 0b0_0100, 0b0_1000, 0b1_0000,"     // Selection table
                        "0b1_1111, 0b1_1110, 0b1_1101, 0b1_1011, 0b1_0111, 0b0_1111,"sv;  // Deselection table

  auto sut = DefaultTableBasedPathSelector(reg, maxPath, SelectorProperty::CanSelectNone, tables);

  // ---------------- Exercise
  //
  sut.Select(0u);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ActiveCount(), 0);

  TS_ASSERT_TRUE   (sut.IsSelected(0u));
  TS_ASSERT_FALSE  (sut.IsSelected(1u));
  TS_ASSERT_FALSE  (sut.IsSelected(2u));
  TS_ASSERT_FALSE  (sut.IsSelected(3u));
  TS_ASSERT_FALSE  (sut.IsSelected(4u));
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->BypassSequence());
}


//! Checks DefaultTableBasedPathSelector::Deselect() when can select no path
//!
void UT_DefaultTableBasedPathSelector::test_Deselect ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& pathToDeselect)
  {
    // ---------------- Setup
    //
    auto bypassSequence = BinaryVector::CreateFromBinaryString("0_0010");
    auto maxPath        = 5u;
    auto associatedNode = make_shared<Register>("Control", bypassSequence);
    auto tables         = "0b0_0000, 0b0_0001, 0b0_0010, 0b0_0100, 0b0_1000, 0b1_0000,"     // Selection table
                          "0b0_0000, 0b1_1110, 0b1_1101, 0b1_1011, 0b1_0111, 0b0_1111,"sv;  // Deselection table

    auto sut = DefaultTableBasedPathSelector(associatedNode, maxPath, SelectorProperty::CanSelectNone, tables);

    // ---------------- Exercise
    //
    sut.Deselect(pathToDeselect);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsSelected(pathToDeselect));

    associatedNode->UpdateLastToSut();   // Force update
    TS_ASSERT_FALSE  (sut.IsActive(pathToDeselect));

    TS_ASSERT_FALSE  (sut.IsSelected(2u));
    TS_ASSERT_FALSE  (sut.IsActive(2u));
    TS_ASSERT_EQUALS (sut.ActiveCount(), 0u);
  };

  auto pathsToDeselect = {1u, 2u, 3u, 4u, 5u};

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, pathsToDeselect);
}


//! Checks DefaultTableBasedPathSelector::DebugSelectorInfo
//!
void UT_DefaultTableBasedPathSelector::test_DebugSelectorInfo ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("111");
  auto associatedNode = make_shared<Register>("Ctrl", bypassSequence);
  auto tables         = "0b000, 0b001, 0b010,"
                        "0b000, 0b110, 0b101"sv;

  auto sut = DefaultTableBasedPathSelector(associatedNode, 2, SelectorProperty::CanSelectNone, tables);

  // ---------------- Exercise
  //
  auto debugInfo = sut.DebugSelectorInfo(false);

  // ---------------- Verify
  //
  auto expected = "Kind: Table_Based\n"
                  "Can_select_none: true\n"
                  "Reversed_order:  false\n"
                  "Inverted_bits:   false\n"
                  "\n"
                  "Selection Table:\n"
                  "[0] 0b000\n"
                  "[1] 0b001\n"
                  "[2] 0b010\n"
                  "\n"
                  "Deselection Table:\n"
                  "[0] 0b000\n"
                  "[1] 0b110\n"
                  "[2] 0b101"sv;

  TS_ASSERT_EQUALS (debugInfo, expected);
}

//===========================================================================
// End of UT_DefaultTableBasedPathSelector.cpp
//===========================================================================
