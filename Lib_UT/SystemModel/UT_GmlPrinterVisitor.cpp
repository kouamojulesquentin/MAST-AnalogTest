//===========================================================================
//                           UT_GmlPrinterVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinterVisitor.cpp
//!
//! Implements test fixture for testing GmlPrinterVisitor
//!
//===========================================================================


#include "UT_GmlPrinterVisitor.hpp"
#include "GmlPrinterVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "DefaultBinaryPathSelector.hpp"

#include <cxxtest/ValueTraits.h>
using std::string;
using std::make_shared;

using namespace mast;


//! Initializes tests (called for each test)
//!
void UT_GmlPrinterVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks GmlPrinterVisitor constructor
//!
void UT_GmlPrinterVisitor::test_Constructor ()
{
  // ---------------- Exercise
  //
  GmlPrinterVisitor sut;

  // ---------------- Verify
  //
  string graph    = sut.GetGraph();   // Should return valid empty graph
  string expected = "graph\n"
                    "[\n"
                    "   hierarchic 1 directed 1\n"
                    "]";

  TS_ASSERT_EQUALS (graph, expected);
}



//! Checks GmlPrinterVisitor::VisitAccessInterface() when there are several child beneath
//!
void UT_GmlPrinterVisitor::test_VisitAccessInterface_with_Child ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>    ("Chain name");
  auto reg_1 = make_shared<Register> ("Reg_1",        BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2 = make_shared<Register> ("Reg_2",        BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3 = make_shared<Register> ("Reg_3",        BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a = make_shared<Register> ("R_A",          BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b = make_shared<Register> ("the register", BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto muxNode         = make_shared<Register>("Selector", BinaryVector::CreateFromBinaryString("01"));
  auto pathSelector    = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker          = make_shared<Linker>("Linker name", pathSelector);
  auto accessInterface = make_shared<AccessInterface>("Access interface name", nullptr);

  accessInterface->AppendChild(chain);
  accessInterface->AppendChild(linker);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);


  GmlPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto gotGraph = sut.GetGraph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 8 graphics [ type \"octagon\" fill \"#10FFFF\" w 231 h 63 ] LabelGraphics [ text \"Access interface name\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 0 graphics [ type \"ellipse\" fill \"#FFCC20\" w 110 h 35 ] LabelGraphics [ text \"Chain name\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 55 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   node [ id 2 graphics [ type \"rectangle\" fill \"#59FF20\" w 55 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 55 h 35 ] LabelGraphics [ text \"Reg_3\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 3 label \"3\" ]\n"
                         "   edge [ source 8 target 0 label \"1\" ]\n"
                         "   node [ id 7 graphics [ type \"trapezoid\" fill \"#FF3060\" w 121 h 35 ] LabelGraphics [ text \"Linker name\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 4 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"R_A\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 7 target 4 label \"1\" ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 132 h 36 ] LabelGraphics [ text \"the register\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 7 target 5 label \"2\" ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 88 h 35 ] LabelGraphics [ text \"Selector\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 7 target 6 label \"0\" graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 8 target 7 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);
}

//! Checks GmlPrinterVisitor::VisitAccessInterface() when there are several child beneath
//!
void UT_GmlPrinterVisitor::test_Visit_After_GetGraph ()
{
  // ---------------- Setup
  //
  auto chain           = make_shared<Chain>           ("Chain");
  auto reg_1           = make_shared<Register>        ("Reg_1",  BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2           = make_shared<Register>        ("Reg_2",  BinaryVector::CreateFromBinaryString("1010_10"));
  auto accessInterface = make_shared<AccessInterface> ("Access", nullptr);

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);


  accessInterface->AppendChild(chain);

  GmlPrinterVisitor sut;
  sut.GetGraph();   // Should return valid empty graph

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto gotGraph = sut.GetGraph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 3 graphics [ type \"octagon\" fill \"#10FFFF\" w 66 h 35 ] LabelGraphics [ text \"Access\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 0 graphics [ type \"ellipse\" fill \"#FFCC20\" w 55 h 35 ] LabelGraphics [ text \"Chain\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 55 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   node [ id 2 graphics [ type \"rectangle\" fill \"#59FF20\" w 55 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "   edge [ source 3 target 0 label \"1\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (gotGraph, expected);
}





//===========================================================================
// End of UT_GmlPrinterVisitor.cpp
//===========================================================================
