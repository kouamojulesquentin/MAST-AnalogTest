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
#include "SystemModelNodes.hpp"
#include "GmlPrinterVisitor.hpp"
#include "PrettyPrinterVisitor.hpp"

#include "SystemModelCheckResult_Traits.hpp"

#include <cxxtest/ValueTraits.h>

using std::string;
using std::experimental::string_view;

using test::SystemModelBuilder;
using namespace mast;

//! Initializes test (called for each test)
void UT_SystemModelBuilder::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModelBuilder::Create_UnitTestCase_6_Levels()
//!
void UT_SystemModelBuilder::test_Create_UnitTestCase_6_Levels ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  // ---------------- Exercise
  //
  auto tap = builder.Create_UnitTestCase_6_Levels();

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (tap);

  // Check coherency
  auto checkResult = sm.Check();

  TS_ASSERT_FALSE  (checkResult.HasIssues());
  if (checkResult.HasIssues())
  {
    TS_FAIL (checkResult.MakeReport());
  }

  // Export gml
//+  GmlPrinterVisitor gmlPrinter("UnitTestCase_6_Levels", GmlPrinterOptions::Std);
//+  tap->Accept(gmlPrinter);
//+  TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");
}


//! Checks SystemModelBuilder::Create_TestCase_AccessInterface()
//!
void UT_SystemModelBuilder::test_Create_TestCase_AccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  // ---------------- Exercise
  //
  auto tap = builder.Create_TestCase_AccessInterface();

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (tap);

  // Check with SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);

  // Check with GmlPrinterVisitor
  GmlPrinterVisitor printer("", GmlPrinterOptions::ShowSelectorWithEdge);
  printer.DisplayIdentifier(true);
  printer.DisplayRegisterValue(true);
  printer.DisplayValueAuto(true);

  tap->Accept(printer);

  auto gotGraph = printer.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 90 h 43 ] LabelGraphics [ text \"(0)\n"
                         "TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\n"
                         "Width: 8\n"
                         "Hold value: true\n"
                         "Bypass:    0xFF\n"
                         "Next to:   0xFF\n"
                         "Last to:   0xFF\n"
                         "Last from: 0xFF\n"
                         "Expected:  0xFF\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 133 h 126 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\n"
                         "Width: 1\n"
                         "Bypass:    0b1\n"
                         "Next to:   0b1\n"
                         "Last to:   0b1\n"
                         "Last from: 0b1\n"
                         "Expected:  0b1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(4)\n"
                         "sut_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(5)\n"
                         "static_1\n"
                         "Width: 16\n"
                         "Bypass:    0x0000\n"
                         "Next to:   0x0000\n"
                         "Last to:   0x0000\n"
                         "Last from: 0x0000\n"
                         "Expected:  0x0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(6)\n"
                         "sut_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(7)\n"
                         "static_2\n"
                         "Width: 16\n"
                         "Bypass:    0x0000\n"
                         "Next to:   0x0000\n"
                         "Last to:   0x0000\n"
                         "Last from: 0x0000\n"
                         "Expected:  0x0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 6 target 7 label \"1\" ]\n"
                         "   edge [ source 2 target 6 label \"3\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);
}


//! Checks SystemModelBuilder::Create_TestCase_MIB()
//!
void UT_SystemModelBuilder::test_Create_TestCase_MIB_4_Chains ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  // ---------------- Exercise
  //
  auto tap = builder.Create_TestCase_MIB("TAP", 4u);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (tap);
  CxxTest::setAbortTestOnFail(false);

  // With SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);

  // Check with GmlPrinterVisitor

  // With GML printer
  GmlPrinterVisitor printer("", GmlPrinterOptions::ShowSelectorWithEdge);
  printer.DisplayIdentifier(true);
  printer.DisplayRegisterValue(true);
  printer.DisplayValueAuto(true);

  tap->Accept(printer);

  auto gotGraph = printer.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 90 h 43 ] LabelGraphics [ text \"(0)\n"
                         "TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\n"
                         "Width: 8\n"
                         "Hold value: true\n"
                         "Bypass:    0xFF\n"
                         "Next to:   0xFF\n"
                         "Last to:   0xFF\n"
                         "Last from: 0xFF\n"
                         "Expected:  0xFF\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 133 h 126 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\n"
                         "Width: 1\n"
                         "Bypass:    0b1\n"
                         "Next to:   0b1\n"
                         "Last to:   0b1\n"
                         "Last from: 0b1\n"
                         "Expected:  0b1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(4)\n"
                         "sut\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(5)\n"
                         "static\n"
                         "Width: 16\n"
                         "Bypass:    0x0000\n"
                         "Next to:   0x0000\n"
                         "Last to:   0x0000\n"
                         "Last from: 0x0000\n"
                         "Expected:  0x0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(7)\n"
                         "MIB\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(6)\n"
                         "MIB_ctrl\n"
                         "Width: 2\n"
                         "Hold value: true\n"
                         "Bypass:    0b00\n"
                         "Next to:   0b00\n"
                         "Last to:   0b00\n"
                         "Last from: 0b00\n"
                         "Expected:  0b00\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"trapezoid\" fill \"#FF3060\" w 106 h 43 ] LabelGraphics [ text \"(8)\n"
                         "MIB_mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(9)\n"
                         "dynamic_0\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(10)\n"
                         "dynamic_1\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(11)\n"
                         "dynamic_2\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(12)\n"
                         "dynamic_3\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   edge [ source 7 target 6 label \"1\" ]\n"
                         "   edge [ source 8 target 9 label \"1\" ]\n"
                         "   edge [ source 8 target 10 label \"2\" ]\n"
                         "   edge [ source 8 target 11 label \"3\" ]\n"
                         "   edge [ source 8 target 12 label \"4\" ]\n"
                         "   edge [ source 8 target 6 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 7 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 7 label \"2\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);

  // With "Pretty" printer
  PrettyPrinterVisitor prettyPrinter;
  tap->Accept(prettyPrinter);

  auto gotPretty      = prettyPrinter.PrettyPrint();
  auto expectedPretty = string("[Access_I](0)  \"TAP\"\n"
                               " [Register](1)  \"TAP_IR\", length: 8, Hold value: true, bypass: 1111_1111\n"
                               " [Linker](2)    \"TAP_DR_Mux\"\n"
                               "  :Selector:(1)  \"TAP_IR\"\n"
                               "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1\n"
                               "  [Chain](4)     \"sut\"\n"
                               "   [Register](5)  \"static\", length: 16, bypass: 0000_0000:0000_0000\n"
                               "   [Chain](7)     \"MIB\"\n"
                               "    [Register](6)  \"MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n"
                               "    [Linker](8)    \"MIB_mux\"\n"
                               "     :Selector:(6)  \"MIB_ctrl\"\n"
                               "     [Register](9)  \"dynamic_0\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "     [Register](10) \"dynamic_1\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "     [Register](11) \"dynamic_2\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "     [Register](12) \"dynamic_3\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}


//! Checks SystemModelBuilder::Create_TestCase_1687()
//!
void UT_SystemModelBuilder::test_Create_TestCase_1687 ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  // ---------------- Exercise
  //
  auto tap = builder.Create_TestCase_1687("sut");

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (tap);
  CxxTest::setAbortTestOnFail(false);

  // With SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);

  // Check with GmlPrinterVisitor

  // With GML printer
  GmlPrinterVisitor printer("", GmlPrinterOptions::ShowSelectorWithEdge);
  printer.DisplayIdentifier(true);
  printer.DisplayRegisterValue(true);
  printer.DisplayValueAuto(true);

  tap->Accept(printer);

  auto gotGraph = printer.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 90 h 43 ] LabelGraphics [ text \"(0)\n"
                         "sut\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(1)\n"
                         "sut_IR\n"
                         "Width: 8\n"
                         "Hold value: true\n"
                         "Bypass:    0xFF\n"
                         "Next to:   0xFF\n"
                         "Last to:   0xFF\n"
                         "Last from: 0xFF\n"
                         "Expected:  0xFF\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "sut_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 133 h 126 ] LabelGraphics [ text \"(3)\n"
                         "sut_BPY\n"
                         "Width: 1\n"
                         "Bypass:    0b1\n"
                         "Next to:   0b1\n"
                         "Last to:   0b1\n"
                         "Last from: 0b1\n"
                         "Expected:  0b1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(4)\n"
                         "sut\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(5)\n"
                         "static\n"
                         "Width: 16\n"
                         "Bypass:    0x0000\n"
                         "Next to:   0x0000\n"
                         "Last to:   0x0000\n"
                         "Last from: 0x0000\n"
                         "Expected:  0x0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(7)\n"
                         "SIB\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(6)\n"
                         "SIB_ctrl\n"
                         "Width: 1\n"
                         "Hold value: true\n"
                         "Bypass:    0b0\n"
                         "Next to:   0b0\n"
                         "Last to:   0b0\n"
                         "Last from: 0b0\n"
                         "Expected:  0b0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"trapezoid\" fill \"#FF3060\" w 106 h 43 ] LabelGraphics [ text \"(8)\n"
                         "SIB_mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(9)\n"
                         "dynamic\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   edge [ source 7 target 6 label \"1\" ]\n"
                         "   edge [ source 8 target 9 label \"1\" ]\n"
                         "   edge [ source 8 target 6 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 7 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 7 label \"2\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);

  // With "Pretty" printer
  PrettyPrinterVisitor prettyPrinter;
  tap->Accept(prettyPrinter);

  auto gotPretty      = prettyPrinter.PrettyPrint();
  auto expectedPretty = string("[Access_I](0)  \"sut\"\n"
                               " [Register](1)  \"sut_IR\", length: 8, Hold value: true, bypass: 1111_1111\n"
                               " [Linker](2)    \"sut_DR_Mux\"\n"
                               "  :Selector:(1)  \"sut_IR\"\n"
                               "  [Register](3)  \"sut_BPY\", length: 1, bypass: 1\n"
                               "  [Chain](4)     \"sut\"\n"
                               "   [Register](5)  \"static\", length: 16, bypass: 0000_0000:0000_0000\n"
                               "   [Chain](7)     \"SIB\"\n"
                               "    [Register](6)  \"SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n"
                               "    [Linker](8)    \"SIB_mux\"\n"
                               "     :Selector:(6)  \"SIB_ctrl\"\n"
                               "     [Register](9)  \"dynamic\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}

//! Checks SystemModelBuilder::Create_TestCase_1500()
//!
void UT_SystemModelBuilder::test_Create_TestCase_1500_3_Chains ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  // ---------------- Exercise
  //
  auto tap = builder.Create_TestCase_1500("TAP", 3u);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (tap);
  CxxTest::setAbortTestOnFail(false);

  // With SystemModel checker
  auto result = sm.Check();
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);

  // Check with GmlPrinterVisitor

  // With GML printer
  GmlPrinterVisitor printer("", GmlPrinterOptions::ShowSelectorWithEdge);
  printer.DisplayIdentifier(true);
  printer.DisplayRegisterValue(true);
  printer.DisplayValueAuto(true);

  tap->Accept(printer);

  auto gotGraph = printer.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 90 h 43 ] LabelGraphics [ text \"(0)\n"
                         "TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\n"
                         "Width: 8\n"
                         "Hold value: true\n"
                         "Bypass:    0xFF\n"
                         "Next to:   0xFF\n"
                         "Last to:   0xFF\n"
                         "Last from: 0xFF\n"
                         "Expected:  0xFF\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 133 h 126 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\n"
                         "Width: 1\n"
                         "Bypass:    0b1\n"
                         "Next to:   0b1\n"
                         "Last to:   0b1\n"
                         "Last from: 0b1\n"
                         "Expected:  0b1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(4)\n"
                         "sut\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(5)\n"
                         "static\n"
                         "Width: 16\n"
                         "Bypass:    0x0000\n"
                         "Next to:   0x0000\n"
                         "Last to:   0x0000\n"
                         "Last from: 0x0000\n"
                         "Expected:  0x0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"ellipse\" fill \"#FFCC20\" w 154 h 43 ] LabelGraphics [ text \"(6)\n"
                         "1500_wrapper\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(8)\n"
                         "SWIR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(7)\n"
                         "SWIR_ctrl\n"
                         "Width: 1\n"
                         "Hold value: true\n"
                         "Bypass:    0b0\n"
                         "Next to:   0b0\n"
                         "Last to:   0b0\n"
                         "Last from: 0b0\n"
                         "Expected:  0b0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"trapezoid\" fill \"#FF3060\" w 116 h 43 ] LabelGraphics [ text \"(9)\n"
                         "SWIR_mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"(11)\n"
                         "WIR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"trapezoid\" fill \"#FF3060\" w 106 h 43 ] LabelGraphics [ text \"(12)\n"
                         "WIR_mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 133 h 126 ] LabelGraphics [ text \"(13)\n"
                         "WBY\n"
                         "Width: 1\n"
                         "Bypass:    0b0\n"
                         "Next to:   0b0\n"
                         "Last to:   0b0\n"
                         "Last from: 0b0\n"
                         "Expected:  0b0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(14)\n"
                         "dynamic_0\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 15 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(15)\n"
                         "dynamic_1\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 16 graphics [ type \"rectangle\" fill \"#59FF20\" w 209 h 126 ] LabelGraphics [ text \"(16)\n"
                         "dynamic_2\n"
                         "Width: 32\n"
                         "Bypass:    0x0000_0000\n"
                         "Next to:   0x0000_0000\n"
                         "Last to:   0x0000_0000\n"
                         "Last from: 0x0000_0000\n"
                         "Expected:  0x0000_0000\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 152 h 144 ] LabelGraphics [ text \"(10)\n"
                         "WIR_reg\n"
                         "Width: 2\n"
                         "Hold value: true\n"
                         "Bypass:    0b00\n"
                         "Next to:   0b00\n"
                         "Last to:   0b00\n"
                         "Last from: 0b00\n"
                         "Expected:  0b00\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   edge [ source 8 target 7 label \"1\" ]\n"
                         "   edge [ source 12 target 13 label \"1\" ]\n"
                         "   edge [ source 12 target 14 label \"2\" ]\n"
                         "   edge [ source 12 target 15 label \"3\" ]\n"
                         "   edge [ source 12 target 16 label \"4\" ]\n"
                         "   edge [ source 12 target 10 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 11 target 12 label \"1\" ]\n"
                         "   edge [ source 9 target 11 label \"1\" ]\n"
                         "   edge [ source 9 target 10 label \"2\" ]\n"
                         "   edge [ source 9 target 7 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 8 target 9 label \"2\" ]\n"
                         "   edge [ source 6 target 8 label \"1\" ]\n"
                         "   edge [ source 4 target 6 label \"2\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);

  // With "Pretty" printer
  PrettyPrinterVisitor prettyPrinter;
  tap->Accept(prettyPrinter);

  auto gotPretty      = prettyPrinter.PrettyPrint();
  auto expectedPretty = string("[Access_I](0)  \"TAP\"\n"
                               " [Register](1)  \"TAP_IR\", length: 8, Hold value: true, bypass: 1111_1111\n"
                               " [Linker](2)    \"TAP_DR_Mux\"\n"
                               "  :Selector:(1)  \"TAP_IR\"\n"
                               "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1\n"
                               "  [Chain](4)     \"sut\"\n"
                               "   [Register](5)  \"static\", length: 16, bypass: 0000_0000:0000_0000\n"
                               "   [Chain](6)     \"1500_wrapper\"\n"
                               "    [Chain](8)     \"SWIR\"\n"
                               "     [Register](7)  \"SWIR_ctrl\", length: 1, Hold value: true, bypass: 0\n"
                               "     [Linker](9)    \"SWIR_mux\"\n"
                               "      :Selector:(7)  \"SWIR_ctrl\"\n"
                               "      [Chain](11)    \"WIR\"\n"
                               "       [Linker](12)   \"WIR_mux\"\n"
                               "        :Selector:(10) \"WIR_reg\"\n"
                               "        [Register](13) \"WBY\", length: 1, bypass: 0\n"
                               "        [Register](14) \"dynamic_0\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "        [Register](15) \"dynamic_1\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "        [Register](16) \"dynamic_2\", length: 32, bypass: 0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "      [Register](10) \"WIR_reg\", length: 2, Hold value: true, bypass: 00"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}


//===========================================================================
// End of UT_SystemModelBuilder.cpp
//===========================================================================
