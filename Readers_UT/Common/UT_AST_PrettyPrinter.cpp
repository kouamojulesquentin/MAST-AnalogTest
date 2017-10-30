//===========================================================================
//                           UT_AST_PrettyPrinter.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_PrettyPrinter.cpp
//!
//! Implements test fixture for testing AST_PrettyPrinter
//!
//===========================================================================

#include "UT_AST_PrettyPrinter.hpp"
#include "AST.hpp"
#include "AST_VectorIdentifier.hpp"
#include "AST_PrettyPrinter.hpp"

#include <cxxtest/ValueTraits.h>

using Parsers::AST;
using Parsers::AST_Node;
using Parsers::AST_Module;
using Parsers::AST_PrettyPrinter;
using std::vector;


//! Initializes test (called for each test)
void UT_AST_PrettyPrinter::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}


//! Checks AST_PrettyPrinter::Visit_Module() when the module is empty (no children)
//!
void UT_AST_PrettyPrinter::test_Visit_Module_empty ()
{
  // ---------------- Setup
  //
  AST  ast;
  auto identifier = ast.Create_ScalarIdentifier("Dibona");
  auto node       = ast.Create_Module(identifier, vector<AST_Node*>());

  AST_PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Visit_Module(node));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = "Module Dibona\n"
                  "{\n"
                  "}\n";

  TS_ASSERT_EQUALS (got, expected);
}


//! Checks AST_PrettyPrinter::Visit_ScanRegister() when the module is empty (no children)
//!
void UT_AST_PrettyPrinter::test_Visit_ScanRegister_empty ()
{
  // ---------------- Setup
  //
  AST  ast;
  auto identifier = ast.Create_VectorIdentifier("Ecrin", "", "");
  auto node       = ast.Create_ScanRegister(identifier);

  AST_PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Visit_ScanRegister(node));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = "ScanRegister Ecrin\n"
                  "{\n"
                  "}\n";

  TS_ASSERT_EQUALS (got, expected);
}




//===========================================================================
// End of UT_AST_PrettyPrinter.cpp
//===========================================================================
