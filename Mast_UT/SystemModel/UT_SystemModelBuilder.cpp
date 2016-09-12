//===========================================================================
//                           UT_SystemModelBuilder.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelBuilder.cpp
//!
//! Implements test fixture for testing SystemModelBuilder
//!
//===========================================================================


#include "UT_SystemModelBuilder.hpp"
#include "SystemModelBuilder.hpp"
#include "SystemModelNode.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "GmlPrinter.hpp"
#include "PrettyPrinter.hpp"

#include "BinaryVector_Traits.hpp"
#include "SystemModelCheckResult_Traits.hpp"

#include <memory>
#include <experimental/string_view>
#include <string>

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::string;
using std::experimental::string_view;

using namespace mast;


//! Initializes test (called for each test)
void UT_SystemModelBuilder::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModel::Create_JTAG_TAP()
//!
void UT_SystemModelBuilder::test_Create_JTAG_TAP ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;
  auto protocol             = make_shared<LoopbackAccessInterfaceProtocol>();

  // ---------------- Exercise
  //
  auto tapNode = sut.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount, protocol);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (tapNode);
  TS_ASSERT_EQUALS      (tapNode->Name(), DEFAULT_TAP_NAME);

  auto id = tapNode->Identifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sm.Root());
  TS_ASSERT_NOT_NULLPTR (sm.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR  (sm.Root(), sm.NodeWithId(id));

  // IR
  auto irNode = tapNode->FirstChild();
  TS_ASSERT_NOT_NULLPTR (irNode);
  TS_ASSERT_EQUALS      (irNode->Name(), DEFAULT_TAP_IR_NAME);

  auto irAsRegister = dynamic_pointer_cast<Register>(irNode);
  TS_ASSERT_NOT_NULLPTR (irAsRegister);
  TS_ASSERT_EQUALS      (irAsRegister->BypassSequence(), BinaryVector::CreateFromBinaryString("1111_11"));

  // DR MUX
  auto muxNode = irNode->NextSibling();
  TS_ASSERT_NOT_NULLPTR (muxNode);
  auto muxAsLinker = dynamic_pointer_cast<Linker>(muxNode);
  TS_ASSERT_NOT_NULLPTR (muxAsLinker);
  TS_ASSERT_EQUALS      (muxAsLinker->Name(), DEFAULT_TAP_MUX_NAME);

  // DR bypass
  auto bypassNode = muxAsLinker->FirstChild();
  TS_ASSERT_NOT_NULLPTR (bypassNode);
  auto bypassAsRegister = dynamic_pointer_cast<Register>(bypassNode);
  TS_ASSERT_NOT_NULLPTR (bypassAsRegister);
  TS_ASSERT_EQUALS (bypassAsRegister->Name(), DEFAULT_TAP_MUX_BPY_NAME);

  // Check appending nodes to tap
  auto linkerSecondChild = bypassAsRegister->NextSibling();
  TS_ASSERT_NULLPTR (linkerSecondChild);

  auto newReg = sm.CreateRegister("New reg", BinaryVector::CreateFromBinaryString("1010"), tapNode);

  linkerSecondChild = bypassAsRegister->NextSibling();
  TS_ASSERT_NOT_NULLPTR (linkerSecondChild);
  TS_ASSERT_EQUALS      (linkerSecondChild->Name(), "New reg");
}



//! Checks SystemModelBuilder::Create_PathSelector() requesting a Binary version
//!
void UT_SystemModelBuilder::test_Create_PathSelector_Binary ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  // ---------------- Exercise
  //
  auto result = sut.Create_PathSelector(SelectorKind::Binary, "SelectorReg", 3u);

  // ---------------- Verify
  //
  auto reg      = std::get<0>(result);
  auto selector = std::get<1>(result);
  auto bypass   = BinaryVector(2u);

  TS_ASSERT_TRUE   (reg->HoldValue());
  TS_ASSERT_FALSE  (reg->MustCheckExpected());
  TS_ASSERT_EQUALS (reg->Mismatches(),      0U);
  TS_ASSERT_EQUALS (reg->BitsCount(),       2U);
  TS_ASSERT_EQUALS (reg->BypassSequence(),  bypass);
  TS_ASSERT_EQUALS (reg->NextToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->LastToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->ExpectedFromSut(), bypass);
  TS_ASSERT_EQUALS (reg->LastFromSut(),     bypass);

  TS_ASSERT_FALSE  (selector->CanSelectNone());
  TS_ASSERT_EQUALS (selector->ActiveCount(),     1u);
  TS_ASSERT_EQUALS (selector->SelectablePaths(), 3u);
  TS_ASSERT_TRUE   (selector->IsActive(1));
  TS_ASSERT_FALSE  (selector->IsActive(2));
  TS_ASSERT_FALSE  (selector->IsActive(3));

  auto asBinarySelector = dynamic_pointer_cast<DefaultBinaryPathSelector>(selector);
  TS_ASSERT_NOT_NULLPTR (asBinarySelector);
}


//! Checks SystemModelBuilder::Create_PathSelector() requesting a OneHot version that cannot select no path
//!
void UT_SystemModelBuilder::test_Create_PathSelector_OneHot_CannotSelectNone ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  // ---------------- Exercise
  //
  auto result = sut.Create_PathSelector(SelectorKind::One_Hot, "SelectorReg", 3u, SelectorProperty::CannotSelectNone);

  // ---------------- Verify
  //
  auto reg      = std::get<0>(result);
  auto selector = std::get<1>(result);
  auto bypass   = BinaryVector::CreateFromBinaryString("100");

  TS_ASSERT_TRUE   (reg->HoldValue());
  TS_ASSERT_FALSE  (reg->MustCheckExpected());
  TS_ASSERT_EQUALS (reg->Mismatches(),      0U);
  TS_ASSERT_EQUALS (reg->BitsCount(),       3U);
  TS_ASSERT_EQUALS (reg->BypassSequence(),  bypass);
  TS_ASSERT_EQUALS (reg->NextToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->LastToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->ExpectedFromSut(), bypass);
  TS_ASSERT_EQUALS (reg->LastFromSut(),     bypass);

  TS_ASSERT_FALSE  (selector->CanSelectNone());
  TS_ASSERT_EQUALS (selector->ActiveCount(),     1u);
  TS_ASSERT_EQUALS (selector->SelectablePaths(), 3u);
  TS_ASSERT_TRUE   (selector->IsActive(1));
  TS_ASSERT_FALSE  (selector->IsActive(2));
  TS_ASSERT_FALSE  (selector->IsActive(3));

  auto asOneHotSelector = dynamic_pointer_cast<DefaultOneHotPathSelector>(selector);
  TS_ASSERT_NOT_NULLPTR (asOneHotSelector);
}


//! Checks SystemModelBuilder::Create_PathSelector() requesting a OneHot version that can select no path
//!
void UT_SystemModelBuilder::test_Create_PathSelector_OneHot_CanSelectNone ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  // ---------------- Exercise
  //
  auto result = sut.Create_PathSelector(SelectorKind::One_Hot, "SelectorReg", 3u, SelectorProperty::CanSelectNone);

  // ---------------- Verify
  //
  auto reg      = std::get<0>(result);
  auto selector = std::get<1>(result);
  auto bypass   = BinaryVector::CreateFromBinaryString("000");

  TS_ASSERT_TRUE   (reg->HoldValue());
  TS_ASSERT_FALSE  (reg->MustCheckExpected());
  TS_ASSERT_EQUALS (reg->Mismatches(),      0U);
  TS_ASSERT_EQUALS (reg->BitsCount(),       3U);
  TS_ASSERT_EQUALS (reg->BypassSequence(),  bypass);
  TS_ASSERT_EQUALS (reg->NextToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->LastToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->ExpectedFromSut(), bypass);
  TS_ASSERT_EQUALS (reg->LastFromSut(),     bypass);

  TS_ASSERT_TRUE   (selector->CanSelectNone());
  TS_ASSERT_EQUALS (selector->ActiveCount(),     0u);
  TS_ASSERT_EQUALS (selector->SelectablePaths(), 3u);
  TS_ASSERT_FALSE  (selector->IsActive(1));
  TS_ASSERT_FALSE  (selector->IsActive(2));
  TS_ASSERT_FALSE  (selector->IsActive(3));

  auto asOneHotSelector = dynamic_pointer_cast<DefaultOneHotPathSelector>(selector);
  TS_ASSERT_NOT_NULLPTR (asOneHotSelector);
}


//! Checks SystemModelBuilder::Create_PathSelector() requesting a NHot version that cannot select no path
//!
void UT_SystemModelBuilder::test_Create_PathSelector_NHot_CannotSelectNone ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  // ---------------- Exercise
  //
  auto result = sut.Create_PathSelector(SelectorKind::N_Hot, "SelectorReg", 3u, SelectorProperty::CannotSelectNone);

  // ---------------- Verify
  //
  auto reg      = std::get<0>(result);
  auto selector = std::get<1>(result);
  auto bypass   = BinaryVector::CreateFromBinaryString("100");

  TS_ASSERT_TRUE   (reg->HoldValue());
  TS_ASSERT_FALSE  (reg->MustCheckExpected());
  TS_ASSERT_EQUALS (reg->Mismatches(),      0U);
  TS_ASSERT_EQUALS (reg->BitsCount(),       3U);
  TS_ASSERT_EQUALS (reg->BypassSequence(),  bypass);
  TS_ASSERT_EQUALS (reg->NextToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->LastToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->ExpectedFromSut(), bypass);
  TS_ASSERT_EQUALS (reg->LastFromSut(),     bypass);

  TS_ASSERT_FALSE  (selector->CanSelectNone());
  TS_ASSERT_EQUALS (selector->ActiveCount(),     1u);
  TS_ASSERT_EQUALS (selector->SelectablePaths(), 3u);
  TS_ASSERT_TRUE   (selector->IsActive(1));
  TS_ASSERT_FALSE  (selector->IsActive(2));
  TS_ASSERT_FALSE  (selector->IsActive(3));

  auto asNHotSelector = dynamic_pointer_cast<DefaultNHotPathSelector>(selector);
  TS_ASSERT_NOT_NULLPTR (asNHotSelector);
}


//! Checks SystemModelBuilder::Create_PathSelector() requesting a NHot version that can select no path
//!
void UT_SystemModelBuilder::test_Create_PathSelector_NHot_CanSelectNone ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  // ---------------- Exercise
  //
  auto result = sut.Create_PathSelector(SelectorKind::N_Hot, "SelectorReg", 3u, SelectorProperty::CanSelectNone);

  // ---------------- Verify
  //
  auto reg      = std::get<0>(result);
  auto selector = std::get<1>(result);
  auto bypass   = BinaryVector::CreateFromBinaryString("000");

  TS_ASSERT_TRUE   (reg->HoldValue());
  TS_ASSERT_FALSE  (reg->MustCheckExpected());
  TS_ASSERT_EQUALS (reg->Mismatches(),      0U);
  TS_ASSERT_EQUALS (reg->BitsCount(),       3U);
  TS_ASSERT_EQUALS (reg->BypassSequence(),  bypass);
  TS_ASSERT_EQUALS (reg->NextToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->LastToSut(),       bypass);
  TS_ASSERT_EQUALS (reg->ExpectedFromSut(), bypass);
  TS_ASSERT_EQUALS (reg->LastFromSut(),     bypass);

  TS_ASSERT_TRUE   (selector->CanSelectNone());
  TS_ASSERT_EQUALS (selector->ActiveCount(),     0u);
  TS_ASSERT_EQUALS (selector->SelectablePaths(), 3u);
  TS_ASSERT_FALSE  (selector->IsActive(1));
  TS_ASSERT_FALSE  (selector->IsActive(2));
  TS_ASSERT_FALSE  (selector->IsActive(3));

  auto asNHotSelector = dynamic_pointer_cast<DefaultNHotPathSelector>(selector);
  TS_ASSERT_NOT_NULLPTR (asNHotSelector);
}


//! Checks TestModelBuilder::Create_MIB() with 1 Derivation
//!
void UT_SystemModelBuilder::test_Create_MIB_1_Derivation ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto res         = builder.Create_PathSelector(SelectorKind::Binary, "Reg_name", 1u, SelectorProperty::CanSelectNone);
  auto selectorReg = res.first;
  auto selector    = res.second;

  // ---------------- Exercise
  //
  auto mib         = builder.Create_MIB("MIB_name", selector, selectorReg, MuxRegPlacement::AfterMux);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (mib);
  CxxTest::setAbortTestOnFail(false);

  // With SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result.errorsCount,   3u);  // 3 for missing AccessInterface + 2 chidren not AccessInterface
  TS_ASSERT_EQUALS (result.warningsCount, 1u);  // 1 for linker with no child

  // Check with GmlPrinter
  auto gotGraph = GmlPrinter::Graph(sm.Root());
  auto expected = string(
                         "graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 1 graphics [ type \"ellipse\" fill \"#FFCC20\" w 116 h 43 ] LabelGraphics [ text \"(1)\n"
                         "MIB_name\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 154 h 44 ] LabelGraphics [ text \"(2)\n"
                         "MIB_name_mux\n"
                         ":0:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 0 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(0)\n"
                         "Reg_name\n"
                         "Width: 1\n"
                         "Hold value: true\n"
                         "Bypass:    0b0\n"
                         "Next to:   0b0\n"
                         "Last to:   0b0\n"
                         "Last from: 0b0\n"
                         "Expected:  0b0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 1 target 2 label \"1\" ]\n"
                         "   edge [ source 1 target 0 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);

  // With "Pretty" printer

  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);
  auto expectedPretty = string(
                               "[Chain](1)     \"MIB_name\", pending: false, has_conditioner: false, priority: 0\n"
                               " [Linker](2)    \"MIB_name_mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                               "  :Selector:(0)  \"Reg_name\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: false\n"
                               " [Register](0)  \"Reg_name\", length: 1, Hold value: true, bypass:            0b0\n"
                               "                                                       , next_to_sut:       0b0\n"
                               "                                                       , last_to_sut:       0b0\n"
                               "                                                       , last_from_sut:     0b0\n"
                               "                                                       , expected_from_sut: 0b0\n"
                               "                                                       , pending: false, has_conditioner: false, priority: 0"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}


//! Checks TestModelBuilder::Create_MIB() with 4 chains
//!
void UT_SystemModelBuilder::test_Create_MIB_4_Derivations ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto res         = builder.Create_PathSelector(SelectorKind::Binary, "Reg_name", 4u);
  auto selectorReg = res.first;
  auto selector    = res.second;

  // ---------------- Exercise
  //
  auto mib         = builder.Create_MIB("MIB_name", selector, selectorReg, MuxRegPlacement::BeforeMux);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (mib);
  CxxTest::setAbortTestOnFail(false);

  // With SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result.errorsCount,   3u);  // 3 for missing AccessInterface + 2 chidren not AccessInterface
  TS_ASSERT_EQUALS (result.warningsCount, 1u);  // 1 for linker with no child

  // Check with GmlPrinter
  auto gotGraph = GmlPrinter::Graph(sm.Root());
  auto expected = string(
                         "graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 1 graphics [ type \"ellipse\" fill \"#FFCC20\" w 116 h 43 ] LabelGraphics [ text \"(1)\n"
                         "MIB_name\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 0 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(0)\n"
                         "Reg_name\n"
                         "Width: 2\n"
                         "Hold value: true\n"
                         "Bypass:    0b00\n"
                         "Next to:   0b00\n"
                         "Last to:   0b00\n"
                         "Last from: 0b00\n"
                         "Expected:  0b00\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 154 h 44 ] LabelGraphics [ text \"(2)\n"
                         "MIB_name_mux\n"
                         ":0:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 1 target 0 label \"1\" ]\n"
                         "   edge [ source 1 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);

  // With "Pretty" printer

  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);
  auto expectedPretty = string("[Chain](1)     \"MIB_name\", pending: false, has_conditioner: false, priority: 0\n"
                               " [Register](0)  \"Reg_name\", length: 2, Hold value: true, bypass:            0b00\n"
                               "                                                       , next_to_sut:       0b00\n"
                               "                                                       , last_to_sut:       0b00\n"
                               "                                                       , last_from_sut:     0b00\n"
                               "                                                       , expected_from_sut: 0b00\n"
                               "                                                       , pending: false, has_conditioner: false, priority: 0\n"
                               " [Linker](2)    \"MIB_name_mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                               "  :Selector:(0)  \"Reg_name\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}

//===========================================================================
// End of UT_SystemModelBuilder.cpp
//===========================================================================
