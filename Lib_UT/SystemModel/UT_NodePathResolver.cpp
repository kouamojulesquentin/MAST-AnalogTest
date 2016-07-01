//===========================================================================
//                           UT_NodePathResolver.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_NodePathResolver.cpp
//!
//! Implements test fixture for testing NodePathResolver
//!
//===========================================================================

#include "UT_NodePathResolver.hpp"
#include "NodePathResolver.hpp"
#include "Chain.hpp"
#include "SystemModel.hpp"
#include "GmlPrinterVisitor.hpp"  // To get a visual representation of testcase

#include <memory>
#include <tuple>
#include <string>
#include <cxxtest/ValueTraits.h>

using std::shared_ptr;
using std::make_shared;
using std::tuple;
using std::make_tuple;
using std::experimental::string_view;
using namespace std::string_literals;
using namespace mast;

namespace
{
//! Creates a system model for testing NodePathResolver
//!
shared_ptr<ParentNode> CreateSystemModel (SystemModel& sm, bool reportGml = false)
{
  auto tap    = sm.CreateTap("Tap", 6u, 2u);
  auto tapMux = sm.LinkerWithId(2u);
  tapMux->IgnoreForNodePath(true);

  auto chain_0 = sm.CreateChain("Chain_0", tap);

  // ---------------- Level 1
  //
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0001_1"), chain_0);
  auto chain_1_1 = sm.CreateChain("Chain", chain_0);
  auto chain_1_2 = sm.CreateChain("Chain", chain_0);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1000"), chain_0);
  chain_1_1->IgnoreForNodePath(true);

  // ---------------- Level 2
  //
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0010_1"),  chain_1_2);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("0010_10"), chain_1_2);
  auto chain_2_3 = sm.CreateChain("Chain", chain_1_2);
  auto chain_2_4 = sm.CreateChain("Chain_2", chain_1_1);

  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("0010"), chain_1_1);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("0011"), chain_1_1);

  // ---------------- Level 3
  //
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1000"), chain_2_3);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1001"), chain_2_3);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0011_11"), chain_2_4);

  // ---------------- Another unique in level two
  //
  sm.CreateRegister("Reg_5", BinaryVector::CreateFromBinaryString("0010_101"), chain_1_2);

  if (reportGml)
  {
    GmlPrinterVisitor gmlPrinter("NodePathResolver", GmlPrinterOptions::Std);
    tap->Accept(gmlPrinter);
    TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");
  }

  sm.Check();
  return tap;
}
//
//  End of: CreateSystemModel
//---------------------------------------------------------------------------
} // End of unnamed namespace



//! Checks testcase model
//!
void UT_NodePathResolver::test_testcaseModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = CreateSystemModel(sm, false);

  // ---------------- Verify
  //
  auto checkResult = sm.Check();

  TS_ASSERT_FALSE  (checkResult.HasIssues());
  if (checkResult.HasIssues())
  {
    TS_FAIL (checkResult.MakeReport());
  }
}


//! Initializes test (called for each test)
void UT_NodePathResolver::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks NodePathResolver constructor
//!
void UT_NodePathResolver::test_constructor ()
{
  // ---------------- Setup
  //
  auto referenceNode = make_shared<Chain>("chain");

  // ---------------- Exercise
  //
  NodePathResolver sut(referenceNode);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.Prefix().empty());
}

//! Checks NodePathResolver constructor passing a nullptr as reference node
//!
void UT_NodePathResolver::test_constructor_nullptr ()
{
  // ---------------- Setup
  //
  shared_ptr<Chain> nullptrReferenceNode;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (auto sut = NodePathResolver(nullptrReferenceNode), std::exception);
}



//! Checks NodePathResolver::Resolve() giving valid path
//!
void UT_NodePathResolver::test_Resolve_ValidPaths ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto referenceNode = CreateSystemModel(sm);
    auto sut           = NodePathResolver(referenceNode);

    auto path       = std::get<0>(data);
    auto expectedId = std::get<1>(data);

    // ---------------- Exercise
    //
    auto node = sut.Resolve(path);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (node);
    TS_ASSERT_EQUALS      (node->Identifier(), expectedId);
  };

  auto data =
  {
    make_tuple(".",                         0u),  // 00
    make_tuple("Tap_IR",                    1u),  // 01
    make_tuple("Tap_BPY",                   3u),  // 02
    make_tuple("Chain_0",                   4u),  // 03
    make_tuple("Chain_0.Reg_1",             5u),  // 04
    make_tuple("Chain_0.Chain",             7u),  // 05
    make_tuple("Chain_0.Reg_4",             8u),  // 06
    make_tuple("Chain_0.Chain.Reg_1",       9u),  // 07
    make_tuple("Chain_0.Chain.Reg_2",       10u), // 08
    make_tuple("Chain_0.Chain.Chain",       11u), // 09
    make_tuple("Chain_0.Chain_2",           12u), // 10
    make_tuple("Chain_0.Reg_2",             13u), // 11
    make_tuple("Chain_0.Reg_3",             14u), // 12
    make_tuple("Chain_0.Chain.Chain.Reg_1", 15u), // 13
    make_tuple("Chain_0.Chain.Chain.Reg_2", 16u), // 14
    make_tuple("Chain_0.Chain_2.Reg_1",     17u), // 15
    make_tuple("Chain_0.Chain.Reg_5",       18u), // 16
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks NodePathResolver::Resolve() giving not valid path
//!
void UT_NodePathResolver::test_Resolve_NotValidPaths ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](string_view path)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto referenceNode = CreateSystemModel(sm);
    auto sut           = NodePathResolver(referenceNode);

    // ---------------- Exercise
    //
    auto node = sut.Resolve(path);

    // ---------------- Verify
    //
    TS_ASSERT_NULLPTR (node);
  };

  auto data =
  {
    "..",                  // 00
    "Tap_iR",              // 01
    "Chain",               // 02
    "Reg_1",               // 03
    "Chain_2.Chain",       // 04
    "Chain.Reg_3",         // 05
    "Chain.Reg_1",         // 06
    "Tap_DR_Mux.Chain_0",  // 07
    "Tap_DR_Mux.tap_BPY",  // 08
    "Chain_0/Chain.Chain", // 09
    "Chain_0..Reg_1",      // 10
    "Chain_0.Reg_5",       // 11
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks NodePathResolver::SetPrefix() just after construction and passing an empty string (resetting it)
//!
void UT_NodePathResolver::test_SetPrefix_FirstTime_Empty ()
{
  // ---------------- Setup
  //
  auto referenceNode = make_shared<Chain>("chain");
  auto sut           = NodePathResolver(referenceNode);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetPrefix(""));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.Prefix().empty());
}


//! Checks NodePathResolver::SetPrefix() just after construction and passing '.' (resetting it)
//!
void UT_NodePathResolver::test_SetPrefix_FirstTime_Dot ()
{
  // ---------------- Setup
  //
  auto referenceNode = make_shared<Chain>("chain");
  auto sut           = NodePathResolver(referenceNode);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetPrefix("."));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.Prefix().empty());
}

//! Checks NodePathResolver::Resolve() giving valid path in context of a prefix
//!
void UT_NodePathResolver::test_Resolve_Prefix_ValidPaths ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto referenceNode = CreateSystemModel(sm);
    auto sut           = NodePathResolver(referenceNode);

    auto path       = std::get<0>(data);
    auto expectedId = std::get<1>(data);

    sut.SetPrefix("Chain_0.Chain");

    // ---------------- Exercise
    //
    auto node = sut.Resolve(path);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (node);
    TS_ASSERT_EQUALS      (node->Identifier(), expectedId);
  };

  auto data =
  {
    make_tuple(".",           7u),  // 00
    make_tuple("Reg_1",       9u),  // 01
    make_tuple("Reg_2",       10u), // 02
    make_tuple("Chain",       11u), // 03
    make_tuple("Chain.Reg_1", 15u), // 04
    make_tuple("Chain.Reg_2", 16u), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks NodePathResolver::Resolve() giving a prefix and not valid path
//!
void UT_NodePathResolver::test_Resolve_Prefix_NotValidPaths ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](string_view path)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto referenceNode = CreateSystemModel(sm);
    auto sut           = NodePathResolver(referenceNode);

    sut.SetPrefix("Chain_0.Chain");

    // ---------------- Exercise
    //
    auto node = sut.Resolve(path);

    // ---------------- Verify
    //
    TS_ASSERT_NULLPTR (node);
  };

  auto data =
  {
    "..",         // 00
    "Reg_3",      // 01
    "Reg_4",      // 02
    "..Chain_0",  // 03
    "../Chain_0", // 04
    "../Reg_4",   // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//===========================================================================
// End of UT_NodePathResolver.cpp
//===========================================================================
