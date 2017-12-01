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
#include "SystemModelChecker.hpp"
#include "I2C_EmulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "Emulation_TranslatorProtocol.hpp"
#include "I2C_RawPlayer.hpp"
#include "SVF_RawPlayer.hpp"
#include "GmlPrinter.hpp"
#include "PrettyPrinter.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"


#include "BinaryVector_Traits.hpp"
#include "CheckResult_Traits.hpp"

#include <memory>
#include <experimental/string_view>
#include <string>
#include <initializer_list>

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::string;
using std::experimental::string_view;
using std::initializer_list;

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


//! Checks TestModelBuilder::Create_MIB() with 1 EndPoint
//!
void UT_SystemModelBuilder::test_Create_MIB_1_EndPoint ()
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
void UT_SystemModelBuilder::test_Create_MIB_4_EndPoints ()
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


//! Checks SystemModel::DaisyChain_JTAG_TAPS()
//!
void UT_SystemModelBuilder::test_DaisyChain_JTAG_TAPS ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  auto tap1 = sut.Create_JTAG_TAP("TAP1", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("TAP2", 5u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap1);
  sut.AppendRegisters(2u, "reg_", BinaryVector::CreateFromString("0xAB"),  tap2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(tap1, tap2));

  // ---------------- Verify
  //
  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
//+  TS_TRACE (result.MakeReport());

  auto graph         = GmlPrinter::Graph(sm.Root(), "", GmlPrinterOptions::DisplayIdentifiers | GmlPrinterOptions::ShowProtocol);
  auto expectedGraph = "graph\n"
                       "[\n"
                       "   hierarchic 1 directed 1\n"
                       "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 211 h 44 ] LabelGraphics [ text \"(0)\n"
                       "Chained_TAP\n"
                       "Protocol: Loopback\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 13 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 163 h 43 ] LabelGraphics [ text \"(13)\n"
                       "IR_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(1)\n"
                       "TAP1.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(5)\n"
                       "TAP2.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 14 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 201 h 43 ] LabelGraphics [ text \"(14)\n"
                       "DR_Mux_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
                       "TAP1\n"
                       ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(3)\n"
                       "TAP1_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(8)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(9)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(10)\n"
                       "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 6 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(6)\n"
                       "TAP2\n"
                       ":5:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(7)\n"
                       "TAP2_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(11)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(12)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   edge [ source 13 target 1 label \"1\" ]\n"
                       "   edge [ source 13 target 5 label \"2\" ]\n"
                       "   edge [ source 0 target 13 label \"1\" ]\n"
                       "   edge [ source 2 target 3 label \"1\" ]\n"
                       "   edge [ source 2 target 8 label \"2\" ]\n"
                       "   edge [ source 2 target 9 label \"3\" ]\n"
                       "   edge [ source 2 target 10 label \"4\" ]\n"
                       "   edge [ source 14 target 2 label \"1\" ]\n"
                       "   edge [ source 6 target 7 label \"1\" ]\n"
                       "   edge [ source 6 target 11 label \"2\" ]\n"
                       "   edge [ source 6 target 12 label \"3\" ]\n"
                       "   edge [ source 14 target 6 label \"2\" ]\n"
                       "   edge [ source 0 target 14 label \"2\" ]\n"
                       "]"s;

  TS_ASSERT_EQUALS (graph, expectedGraph);
}

//! Checks SystemModel::DaisyChain_JTAG_TAPS() when taps have no name
//!
void UT_SystemModelBuilder::test_DaisyChain_JTAG_TAPS_NoName ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  auto tap1 = sut.Create_JTAG_TAP("", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("", 5u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap1);
  sut.AppendRegisters(2u, "reg_", BinaryVector::CreateFromString("0xAB"),  tap2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(tap1, tap2));

  // ---------------- Verify
  //
  // IRs
  auto ir1     = sm.RegisterWithId(1u);
  auto ir2     = sm.RegisterWithId(5u);
  auto ir1Name = ir1->Name();
  auto ir2Name = ir2->Name();
  TS_ASSERT_DIFFERS (ir1Name, ir2Name);

  // Linkers
  auto mux1     = sm.LinkerWithId(2u);
  auto mux2     = sm.LinkerWithId(6u);
  auto mux1Name = mux1->Name();
  auto mux2Name = mux2->Name();
  TS_ASSERT_DIFFERS (mux1Name, mux2Name);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
}


//! Checks SystemModel::DaisyChain_JTAG_TAPS() that still allow to append properly after having been chained
//!
void UT_SystemModelBuilder::test_DaisyChain_JTAG_TAPS_Append ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  auto tap1 = sut.Create_JTAG_TAP("TAP", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("TAP", 5u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0xAA"), tap1);
  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0xB1"), tap2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(tap1, tap2));

  // ---------------- Verify
  //
  sut.AppendRegisters(2u, "reg_A", BinaryVector::CreateFromString("0xAA2"), "TAP1");
  sut.AppendRegisters(1u, "reg_B", BinaryVector::CreateFromString("0xBB2"), "TAP2");

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
//+  TS_TRACE (result.MakeReport());
//+  TS_TRACE (GmlPrinter::Graph(sm.Root()));
}


//! Checks SystemModel::DaisyChain_JTAG_TAPS() when first AccessInterface is already a Daisy Chain
//!
void UT_SystemModelBuilder::test_DaisyChain_JTAG_TAPS_3rdTap ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  auto tap1 = sut.Create_JTAG_TAP("TAP_A", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("TAP_B", 5u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap3 = sut.Create_JTAG_TAP("TAP_C", 3u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0x1"),   tap1);
  sut.AppendRegisters(2u, "reg_", BinaryVector::CreateFromString("0x22"),  tap2);
  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x333"), tap3);

  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(tap1, tap2));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(tap1, tap3));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());

  auto graph         = GmlPrinter::Graph(sm.Root(), "", GmlPrinterOptions::DisplayIdentifiers | GmlPrinterOptions::ShowProtocol);
  auto expectedGraph =
                      "graph\n"
                      "[\n"
                      "   hierarchic 1 directed 1\n"
                      "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 211 h 44 ] LabelGraphics [ text \"(0)\n"
                      "Chained_TAP\n"
                      "Protocol: Loopback\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 18 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 163 h 43 ] LabelGraphics [ text \"(18)\n"
                      "IR_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(1)\n"
                      "TAP_A.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(5)\n"
                      "TAP_B.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(9)\n"
                      "TAP_C.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 19 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 201 h 43 ] LabelGraphics [ text \"(19)\n"
                      "DR_Mux_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
                      "TAP_A\n"
                      ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(3)\n"
                      "TAP_A_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(12)\n"
                      "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(13)\n"
                      "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(14)\n"
                      "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 6 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(6)\n"
                      "TAP_B\n"
                      ":5:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(7)\n"
                      "TAP_B_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 15 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(15)\n"
                      "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 16 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(16)\n"
                      "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 10 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(10)\n"
                      "TAP_C\n"
                      ":9:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(11)\n"
                      "TAP_C_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   node [ id 17 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(17)\n"
                      "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                      "   edge [ source 18 target 1 label \"1\" ]\n"
                      "   edge [ source 18 target 5 label \"2\" ]\n"
                      "   edge [ source 18 target 9 label \"3\" ]\n"
                      "   edge [ source 0 target 18 label \"1\" ]\n"
                      "   edge [ source 2 target 3 label \"1\" ]\n"
                      "   edge [ source 2 target 12 label \"2\" ]\n"
                      "   edge [ source 2 target 13 label \"3\" ]\n"
                      "   edge [ source 2 target 14 label \"4\" ]\n"
                      "   edge [ source 19 target 2 label \"1\" ]\n"
                      "   edge [ source 6 target 7 label \"1\" ]\n"
                      "   edge [ source 6 target 15 label \"2\" ]\n"
                      "   edge [ source 6 target 16 label \"3\" ]\n"
                      "   edge [ source 19 target 6 label \"2\" ]\n"
                      "   edge [ source 10 target 11 label \"1\" ]\n"
                      "   edge [ source 10 target 17 label \"2\" ]\n"
                      "   edge [ source 19 target 10 label \"3\" ]\n"
                      "   edge [ source 0 target 19 label \"2\" ]\n"
                      "]"s;

  TS_ASSERT_EQUALS (graph, expectedGraph);
}


//! Checks SystemModel::DaisyChain_JTAG_TAPS() with 4 taps
//!
void UT_SystemModelBuilder::test_DaisyChain_JTAG_TAPS_4xTap ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);

  auto tap1 = sut.Create_JTAG_TAP("TAP_A", 7u, 5u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("TAP_B", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap3 = sut.Create_JTAG_TAP("TAP_C", 5u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap4 = sut.Create_JTAG_TAP("TAP_D", 4u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(4u, "reg_", BinaryVector::CreateFromString("0x1"),    tap1);
  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0x22"),   tap2);
  sut.AppendRegisters(2u, "reg_", BinaryVector::CreateFromString("0x33"),   tap3);
  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x4444"), tap4);

  auto taps = {tap1, tap2, tap3, tap4};

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DaisyChain_JTAG_TAPS(taps));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());

  auto graph         = GmlPrinter::Graph(sm.Root(), "", GmlPrinterOptions::DisplayIdentifiers | GmlPrinterOptions::ShowProtocol);
  auto expectedGraph = "graph\n"
                       "[\n"
                       "   hierarchic 1 directed 1\n"
                       "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 211 h 44 ] LabelGraphics [ text \"(0)\n"
                       "Chained_TAP\n"
                       "Protocol: Loopback\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 26 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 163 h 43 ] LabelGraphics [ text \"(26)\n"
                       "IR_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(1)\n"
                       "TAP_A.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(5)\n"
                       "TAP_B.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(9)\n"
                       "TAP_C.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(13)\n"
                       "TAP_D.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 27 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 201 h 43 ] LabelGraphics [ text \"(27)\n"
                       "DR_Mux_DaisyChain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
                       "TAP_A\n"
                       ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(3)\n"
                       "TAP_A_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 16 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(16)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 17 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(17)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 18 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(18)\n"
                       "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 19 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(19)\n"
                       "reg_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 6 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(6)\n"
                       "TAP_B\n"
                       ":5:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(7)\n"
                       "TAP_B_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 20 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(20)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 21 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(21)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 22 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(22)\n"
                       "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 10 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(10)\n"
                       "TAP_C\n"
                       ":9:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(11)\n"
                       "TAP_C_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 23 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(23)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 24 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(24)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 14 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(14)\n"
                       "TAP_D\n"
                       ":13:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 15 graphics [ type \"rectangle\" fill \"#59FF20\" w 85 h 35 ] LabelGraphics [ text \"(15)\n"
                       "TAP_D_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 25 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(25)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   edge [ source 26 target 1 label \"1\" ]\n"
                       "   edge [ source 26 target 5 label \"2\" ]\n"
                       "   edge [ source 26 target 9 label \"3\" ]\n"
                       "   edge [ source 26 target 13 label \"4\" ]\n"
                       "   edge [ source 0 target 26 label \"1\" ]\n"
                       "   edge [ source 2 target 3 label \"1\" ]\n"
                       "   edge [ source 2 target 16 label \"2\" ]\n"
                       "   edge [ source 2 target 17 label \"3\" ]\n"
                       "   edge [ source 2 target 18 label \"4\" ]\n"
                       "   edge [ source 2 target 19 label \"5\" ]\n"
                       "   edge [ source 27 target 2 label \"1\" ]\n"
                       "   edge [ source 6 target 7 label \"1\" ]\n"
                       "   edge [ source 6 target 20 label \"2\" ]\n"
                       "   edge [ source 6 target 21 label \"3\" ]\n"
                       "   edge [ source 6 target 22 label \"4\" ]\n"
                       "   edge [ source 27 target 6 label \"2\" ]\n"
                       "   edge [ source 10 target 11 label \"1\" ]\n"
                       "   edge [ source 10 target 23 label \"2\" ]\n"
                       "   edge [ source 10 target 24 label \"3\" ]\n"
                       "   edge [ source 27 target 10 label \"3\" ]\n"
                       "   edge [ source 14 target 15 label \"1\" ]\n"
                       "   edge [ source 14 target 25 label \"2\" ]\n"
                       "   edge [ source 27 target 14 label \"4\" ]\n"
                       "   edge [ source 0 target 27 label \"2\" ]\n"
                       "]"s;

  TS_ASSERT_EQUALS (graph, expectedGraph);
}


//! Checks SystemModel::Create_Brocade when there is just one tap
//!
void UT_SystemModelBuilder::test_Create_Brocade_1xTAP ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);
  auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u };
  auto masterProtocol = make_shared<I2C_EmulationProtocol>(I2C_Adresses);
  auto slaveProtocol  = make_shared<SVF_EmulationProtocol>();

  auto tap1 = sut.Create_JTAG_TAP("TAP1", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap1);

  shared_ptr<Chain> brocadeChain;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (brocadeChain = sut.Create_Brocade(masterProtocol, slaveProtocol, { tap1 }));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (brocadeChain);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
//+  TS_TRACE (result.MakeReport());

  auto graph         = GmlPrinter::Graph(sm.Root(), "", GmlPrinterOptions::DisplayIdentifiers | GmlPrinterOptions::ShowProtocol);
  auto expectedGraph =  "graph\n"
                        "[\n"
                        "   hierarchic 1 directed 1\n"
                        "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(7)\n"
                        "Brocade\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 8 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 258 h 44 ] LabelGraphics [ text \"(8)\n"
                        "Master_AI\n"
                        "Protocol: I2C_Emulation\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 114 h 35 ] LabelGraphics [ text \"(10)\n"
                        "Brocade_CTRL\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 9 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 258 h 44 ] LabelGraphics [ text \"(9)\n"
                        "Slave_AI\n"
                        "Protocol: SVF_Emulation\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 11 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(11)\n"
                        "IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 13 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(13)\n"
                        "IR_Mux\n"
                        ":10:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(1)\n"
                        "TAP1.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 12 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(12)\n"
                        "DR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 14 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(14)\n"
                        "DR_Mux\n"
                        ":10:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
                        "TAP1\n"
                        ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(3)\n"
                        "TAP1_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 4 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(4)\n"
                        "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(5)\n"
                        "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(6)\n"
                        "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                        "   edge [ source 8 target 10 label \"1\" ]\n"
                        "   edge [ source 7 target 8 label \"1\" ]\n"
                        "   edge [ source 13 target 1 label \"1\" ]\n"
                        "   edge [ source 11 target 13 label \"1\" ]\n"
                        "   edge [ source 9 target 11 label \"1\" ]\n"
                        "   edge [ source 2 target 3 label \"1\" ]\n"
                        "   edge [ source 2 target 4 label \"2\" ]\n"
                        "   edge [ source 2 target 5 label \"3\" ]\n"
                        "   edge [ source 2 target 6 label \"4\" ]\n"
                        "   edge [ source 14 target 2 label \"1\" ]\n"
                        "   edge [ source 12 target 14 label \"1\" ]\n"
                        "   edge [ source 9 target 12 label \"2\" ]\n"
                        "   edge [ source 7 target 9 label \"2\" ]\n"
                        "]";

  TS_ASSERT_EQUALS (graph, expectedGraph);
}


//! Checks SystemModel::Create_Brocade when there are 5 taps to manages
//!
void UT_SystemModelBuilder::test_Create_Brocade_5xTAPs ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);
  auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u };
  auto masterProtocol = make_shared<I2C_EmulationProtocol>(I2C_Adresses);
  auto slaveProtocol  = make_shared<SVF_EmulationProtocol>();

  auto tap1 = sut.Create_JTAG_TAP("Zybo", 6u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = sut.Create_JTAG_TAP("Tap",  6u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap3 = sut.Create_JTAG_TAP("TAP3", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap4 = sut.Create_JTAG_TAP("",     6u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap5 = sut.Create_JTAG_TAP("TAP",  6u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap1);
  sut.AppendRegisters(2u, "R_",   BinaryVector::CreateFromString("0xFDE"), tap2);
  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap3);
  sut.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap4);
  sut.AppendRegisters(2u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap5);

  auto taps = { tap1, tap2, tap3, tap4, tap5 };
  shared_ptr<Chain> brocadeChain;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (brocadeChain = sut.Create_Brocade(masterProtocol, slaveProtocol, taps));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (brocadeChain);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
//+  TS_TRACE (result.MakeReport());

  auto graph         = GmlPrinter::Graph(sm.Root(), "",   GmlPrinterOptions::DisplayIdentifiers
                                                        | GmlPrinterOptions::ShowProtocol
                                                        | GmlPrinterOptions::ShowSelectionValues
                                                        | GmlPrinterOptions::ShowSelectorProperties);
  auto expectedGraph =
                       "graph\n"
                       "[\n"
                       "   hierarchic 1 directed 1\n"
                       "   node [ id 29 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(29)\n"
                       "Brocade\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 30 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 258 h 44 ] LabelGraphics [ text \"(30)\n"
                       "Master_AI\n"
                       "Protocol: I2C_Emulation\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 32 graphics [ type \"rectangle\" fill \"#59FF20\" w 114 h 35 ] LabelGraphics [ text \"(32)\n"
                       "Brocade_CTRL\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 31 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 258 h 44 ] LabelGraphics [ text \"(31)\n"
                       "Slave_AI\n"
                       "Protocol: SVF_Emulation\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 33 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(33)\n"
                       "IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 35 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(35)\n"
                       "IR_Mux\n"
                       ":32:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483647 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 199 h 90 ] LabelGraphics [ text \"Selector :32:\n"
                       "Kind: Brocade\n"
                       "Can_select_none: true\n"
                       "Reversed_order:  true\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(1)\n"
                       "Zybo.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(5)\n"
                       "TAP2.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(9)\n"
                       "TAP3.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(13)\n"
                       "TAP4.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 17 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(17)\n"
                       "TAP5.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 34 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(34)\n"
                       "DR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 36 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(36)\n"
                       "DR_Mux\n"
                       ":32:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
                       "Zybo\n"
                       ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483646 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :1:\n"
                       "Kind: Table_Based\n"
                       "Can_select_none: false\n"
                       "Reversed_order:  false\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(3)\n"
                       "Zybo_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 20 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(20)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 6 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(6)\n"
                       "TAP2\n"
                       ":5:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483645 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :5:\n"
                       "Kind: Table_Based\n"
                       "Can_select_none: false\n"
                       "Reversed_order:  false\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(7)\n"
                       "Tap_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 21 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(21)\n"
                       "R_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 22 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(22)\n"
                       "R_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 10 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(10)\n"
                       "TAP3\n"
                       ":9:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483644 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :9:\n"
                       "Kind: Table_Based\n"
                       "Can_select_none: false\n"
                       "Reversed_order:  false\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(11)\n"
                       "TAP3_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 23 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(23)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 24 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(24)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 25 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(25)\n"
                       "reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 14 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(14)\n"
                       "TAP4\n"
                       ":13:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483643 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :13:\n"
                       "Kind: Table_Based\n"
                       "Can_select_none: false\n"
                       "Reversed_order:  false\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 15 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(15)\n"
                       "TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 26 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(26)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 18 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(18)\n"
                       "TAP5\n"
                       ":17:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 2147483642 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :17:\n"
                       "Kind: Table_Based\n"
                       "Can_select_none: false\n"
                       "Reversed_order:  false\n"
                       "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 19 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(19)\n"
                       "TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 27 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(27)\n"
                       "reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   node [ id 28 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(28)\n"
                       "reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                       "   edge [ source 30 target 32 label \"1\" ]\n"
                       "   edge [ source 29 target 30 label \"1\" ]\n"
                       "   edge [ source 2147483647 target 35 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 35 target 1 label \"1/[0x01]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 35 target 5 label \"2/[0x02]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 35 target 9 label \"3/[0x04]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 35 target 13 label \"4/[0x08]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 35 target 17 label \"5/[0x10]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 33 target 35 label \"1\" ]\n"
                       "   edge [ source 31 target 33 label \"1\" ]\n"
                       "   edge [ source 2147483647 target 36 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2147483646 target 2 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2 target 3 label \"1/[0b111111:S:A]\" ]\n"
                       "   edge [ source 2 target 20 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 36 target 2 label \"1/[0x01]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2147483645 target 6 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 6 target 7 label \"1/[0b111111:S:A]\" ]\n"
                       "   edge [ source 6 target 21 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 6 target 22 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 36 target 6 label \"2/[0x02]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2147483644 target 10 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 10 target 11 label \"1/[0b111111:S:A]\" ]\n"
                       "   edge [ source 10 target 23 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 10 target 24 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 10 target 25 label \"4/[0b000011]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 36 target 10 label \"3/[0x04]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2147483643 target 14 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 14 target 15 label \"1/[0b111111:S:A]\" ]\n"
                       "   edge [ source 14 target 26 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 36 target 14 label \"4/[0x08]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 2147483642 target 18 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 18 target 19 label \"1/[0b111111:S:A]\" ]\n"
                       "   edge [ source 18 target 27 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 18 target 28 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 36 target 18 label \"5/[0x10]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                       "   edge [ source 34 target 36 label \"1\" ]\n"
                       "   edge [ source 31 target 34 label \"2\" ]\n"
                       "   edge [ source 29 target 31 label \"2\" ]\n"
                       "]";

  TS_ASSERT_EQUALS (graph, expectedGraph);
}

//! Checks SystemModel::Create_Brocade when there is just one tap
//!
void UT_SystemModelBuilder::test_Create_Brocade_Emulator_1xTAP ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder sut(sm);
  auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u };
  auto topProtocol = make_shared<Emulation_TranslatorProtocol>();
  auto masterProtocol = make_shared<I2C_RawPlayer>(I2C_Adresses);
  auto slaveProtocol  = make_shared<SVF_RawPlayer>();

  auto tap1 = sut.Create_JTAG_TAP("TAP1", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());

  sut.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0xFDE"), tap1);

  shared_ptr<AccessInterfaceTranslator> brocadeTop;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (brocadeTop = sut.Create_Brocade(topProtocol,masterProtocol, slaveProtocol, { tap1 }));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (brocadeTop);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
  TS_ASSERT_FALSE (result.HasIssues());
//+  TS_TRACE (result.MakeReport());

  
  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);

  auto expectedPretty = string(
"[Access_T](7)  \"Brocade\", Protocol: Emulation_Translator, pending: false, has_conditioner: false, priority: 0\n"
" [Access_I](8)  \"Master_AI\", Protocol: I2C_RAW, ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"  [Register](10) \"Brocade_CTRL\", length: 8, Hold value: true, bypass:            0x00\n"
"                                                            , next_to_sut:       0x00\n"
"                                                            , last_to_sut:       0x00\n"
"                                                            , last_from_sut:     0x00\n"
"                                                            , expected_from_sut: 0x00\n"
"                                                            , pending: false, has_conditioner: false, priority: 0\n"
" [Access_I](9)  \"Slave_AI\", Protocol: SVF_RAW, ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"  [Chain](11)    \"IR\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"   [Linker](13)   \"IR_Mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"    :Selector:(10) \"Brocade_CTRL\", kind: Brocade, can_select_none: true, inverted_bits: false, reversed_order: true\n"
"    [Register](1)  \"TAP1.IR\",     :0x01:, length: 6, Hold value: true, bypass:            0b1111_11\n"
"                                                                     , next_to_sut:       0b1111_11\n"
"                                                                     , last_to_sut:       0b1111_11\n"
"                                                                     , last_from_sut:     0b1111_11\n"
"                                                                     , expected_from_sut: 0b1111_11\n"
"                                                                     , pending: false, has_conditioner: false, priority: 0\n"
"  [Chain](12)    \"DR\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"   [Linker](14)   \"DR_Mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
"    :Selector:(10) \"Brocade_CTRL\", kind: Brocade, can_select_none: true, inverted_bits: false, reversed_order: true\n"
"    [Linker](2)    \"TAP1\",        :S:A:, pending: false, has_conditioner: false, priority: 0\n"
"     :Selector:(1)  \"TAP1.IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
"     [Register](3)  \"TAP1_BPY\",    :0b111111:S:A:, length: 1, bypass:            0b1\n"
"                                                            , next_to_sut:       0b1\n"
"                                                            , last_to_sut:       0b1\n"
"                                                            , last_from_sut:     0b1\n"
"                                                            , expected_from_sut: 0b1\n"
"                                                            , pending: false, has_conditioner: false, priority: 0\n"
"     [Register](4)  \"reg_0\",       :0b000001:, length: 12, bypass:            0xFDE\n"
"                                                         , next_to_sut:       0xFDE\n"
"                                                         , last_to_sut:       0xFDE\n"
"                                                         , last_from_sut:     0xFDE\n"
"                                                         , expected_from_sut: 0xFDE\n"
"                                                         , pending: false, has_conditioner: false, priority: 0\n"
"     [Register](5)  \"reg_1\",       :0b000010:, length: 12, bypass:            0xFDE\n"
"                                                         , next_to_sut:       0xFDE\n"
"                                                         , last_to_sut:       0xFDE\n"
"                                                         , last_from_sut:     0xFDE\n"
"                                                         , expected_from_sut: 0xFDE\n"
"                                                         , pending: false, has_conditioner: false, priority: 0\n"
"     [Register](6)  \"reg_2\",       :0b000011:, length: 12, bypass:            0xFDE\n"
"                                                         , next_to_sut:       0xFDE\n"
"                                                         , last_to_sut:       0xFDE\n"
"                                                         , last_from_sut:     0xFDE\n"
"                                                         , expected_from_sut: 0xFDE\n"
"                                                         , pending: false, has_conditioner: false, priority: 0"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);

  auto graph         = GmlPrinter::Graph(sm.Root(), "", GmlPrinterOptions::DisplayIdentifiers | GmlPrinterOptions::ShowProtocol);


  auto expectedGraph =  "graph\n"
"[\n"
"   hierarchic 1 directed 1\n"
"   node [ id 7 graphics [ type \"octagon\" fill \"#10FFFF\" w 325 h 44 ] LabelGraphics [ text \"(7)\n"
"Brocade\n"
"Protocol: Emulation_Translator\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 8 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 201 h 44 ] LabelGraphics [ text \"(8)\n"
"Master_AI\n"
"Protocol: I2C_RAW\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 114 h 35 ] LabelGraphics [ text \"(10)\n"
"Brocade_CTRL\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 9 graphics [ type \"octagon\" fill \"#10FFFF\" outlineStyle \"dashed\" w 201 h 44 ] LabelGraphics [ text \"(9)\n"
"Slave_AI\n"
"Protocol: SVF_RAW\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 11 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(11)\n"
"IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 13 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(13)\n"
"IR_Mux\n"
":10:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(1)\n"
"TAP1.IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 12 graphics [ type \"ellipse\" fill \"#FFCC20\" outlineStyle \"dashed\" w 90 h 43 ] LabelGraphics [ text \"(12)\n"
"DR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 14 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 97 h 44 ] LabelGraphics [ text \"(14)\n"
"DR_Mux\n"
":10:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 90 h 44 ] LabelGraphics [ text \"(2)\n"
"TAP1\n"
":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"(3)\n"
"TAP1_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 4 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(4)\n"
"reg_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(5)\n"
"reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(6)\n"
"reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
"   edge [ source 8 target 10 label \"1\" ]\n"
"   edge [ source 7 target 8 label \"1\" ]\n"
"   edge [ source 13 target 1 label \"1\" ]\n"
"   edge [ source 11 target 13 label \"1\" ]\n"
"   edge [ source 9 target 11 label \"1\" ]\n"
"   edge [ source 2 target 3 label \"1\" ]\n"
"   edge [ source 2 target 4 label \"2\" ]\n"
"   edge [ source 2 target 5 label \"3\" ]\n"
"   edge [ source 2 target 6 label \"4\" ]\n"
"   edge [ source 14 target 2 label \"1\" ]\n"
"   edge [ source 12 target 14 label \"1\" ]\n"
"   edge [ source 9 target 12 label \"2\" ]\n"
"   edge [ source 7 target 9 label \"2\" ]\n"
"]"
;

  TS_ASSERT_EQUALS (graph, expectedGraph);
}


//===========================================================================
// End of UT_SystemModelBuilder.cpp
//===========================================================================
