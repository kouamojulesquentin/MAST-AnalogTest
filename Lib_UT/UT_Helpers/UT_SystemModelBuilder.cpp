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
  GmlPrinterVisitor printer;
  tap->Accept(printer);

  auto gotGraph = printer.GetGraph();
  auto expected = string(
                         "graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 50 h 35 ] LabelGraphics [ text \"TAP\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 110 h 35 ] LabelGraphics [ text \"TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 77 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 55 h 35 ] LabelGraphics [ text \"sut_1\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 88 h 35 ] LabelGraphics [ text \"static_1\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   node [ id 6 graphics [ type \"ellipse\" fill \"#FFCC20\" w 55 h 35 ] LabelGraphics [ text \"sut_2\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 88 h 35 ] LabelGraphics [ text \"static_2\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 6 target 7 label \"1\" ]\n"
                         "   edge [ source 2 target 6 label \"3\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);
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

  // With GML printer
  GmlPrinterVisitor printer;
  tap->Accept(printer);

  auto gotGraph = printer.GetGraph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 50 h 35 ] LabelGraphics [ text \"TAP\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" w 110 h 35 ] LabelGraphics [ text \"TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 77 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 50 h 35 ] LabelGraphics [ text \"sut\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"static\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 4 target 5 label \"1\" ]\n"
                         "   node [ id 6 graphics [ type \"ellipse\" fill \"#FFCC20\" w 132 h 36 ] LabelGraphics [ text \"1500_wrapper\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 8 graphics [ type \"ellipse\" fill \"#FFCC20\" w 50 h 35 ] LabelGraphics [ text \"SWIR\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 7 graphics [ type \"rectangle\" fill \"#59FF20\" w 99 h 35 ] LabelGraphics [ text \"SWIR_ctrl\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 8 target 7 label \"1\" ]\n"
                         "   node [ id 9 graphics [ type \"trapezoid\" fill \"#FF3060\" w 88 h 35 ] LabelGraphics [ text \"SWIR_mux\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 11 graphics [ type \"ellipse\" fill \"#FFCC20\" w 50 h 35 ] LabelGraphics [ text \"WIR\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 12 graphics [ type \"trapezoid\" fill \"#FF3060\" w 77 h 35 ] LabelGraphics [ text \"WIR_mux\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"WBY\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 12 target 13 label \"1\" ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 99 h 35 ] LabelGraphics [ text \"dynamic_0\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 12 target 14 label \"2\" ]\n"
                         "   node [ id 15 graphics [ type \"rectangle\" fill \"#59FF20\" w 99 h 35 ] LabelGraphics [ text \"dynamic_1\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 12 target 15 label \"3\" ]\n"
                         "   node [ id 16 graphics [ type \"rectangle\" fill \"#59FF20\" w 99 h 35 ] LabelGraphics [ text \"dynamic_2\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 12 target 16 label \"4\" ]\n"
                         "   edge [ source 12 target 10 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 11 target 12 label \"1\" ]\n"
                         "   edge [ source 9 target 11 label \"1\" ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 77 h 35 ] LabelGraphics [ text \"WIR_reg\" fontSize 13 fontStyle \"bold\" ] ]\n"
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

  auto gotPretty      = prettyPrinter.GetPrettyPrint();
  auto expectedPretty = string(
                               "[Access_I](0)  \"TAP\"\n"
                               " [Register](1)  \"TAP_IR\", length: 8, bypass: 1111_1111\n"
                               " [Linker](2)    \"TAP_DR_Mux\"\n"
                               "  :Selector:(1)  \"TAP_IR\"\n"
                               "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1\n"
                               "  [Chain](4)     \"sut\"\n"
                               "   [Register](5)  \"static\", length: 64, bypass: 0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "   [Chain](6)     \"1500_wrapper\"\n"
                               "    [Chain](8)     \"SWIR\"\n"
                               "     [Register](7)  \"SWIR_ctrl\", length: 2, bypass: 01\n"
                               "     [Linker](9)    \"SWIR_mux\"\n"
                               "      :Selector:(7)  \"SWIR_ctrl\"\n"
                               "      [Chain](11)    \"WIR\"\n"
                               "       [Linker](12)   \"WIR_mux\"\n"
                               "        :Selector:(10) \"WIR_reg\"\n"
                               "        [Register](13) \"WBY\", length: 1, bypass: 0\n"
                               "        [Register](14) \"dynamic_0\", length: 128, bypass: 0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "        [Register](15) \"dynamic_1\", length: 128, bypass: 0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "        [Register](16) \"dynamic_2\", length: 128, bypass: 0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000:0000_0000\n"
                               "      [Register](10) \"WIR_reg\", length: 3, bypass: 000"
                              );
  TS_ASSERT_EQUALS (gotPretty, expectedPretty);
}


//===========================================================================
// End of UT_SystemModelBuilder.cpp
//===========================================================================
