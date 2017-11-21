//===========================================================================
//                           UT_AST_Checker.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_Checker.cpp
//!
//! Implements test fixture for testing AST_Checker
//!
//===========================================================================

#include "UT_AST_Checker.hpp"
#include "AST_Checker.hpp"
#include "AST.hpp"

#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Network.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

#include "Mast_Core_Traits.hpp"

#include <vector>
#include <string>
#include <experimental/string_view>

using std::vector;
using std::string;
using std::experimental::string_view;

using namespace Parsers;


//! Checks AST_Checker constructor
//!
void UT_AST_Checker::test_Constructor ()
{
  // ---------------- Setup
  //
  AST  ast;
  auto network = ast.Network();

  // ---------------- Exercise
  //
  AST_Checker sut(network);

  // ---------------- Verify
  //
  auto checkResult = sut.MakeCheckResult();

  TS_ASSERT_FALSE  (checkResult.HasIssues());
  TS_ASSERT_EQUALS (checkResult.infosCount, 0);
}


//! Checks AST_Checker constructor when giving nullptr AST_Network
//!
void UT_AST_Checker::test_Constructor_Nullptr_Network ()
{
  TS_ASSERT_THROWS (AST_Checker(nullptr), std::invalid_argument);
}



//! Checks AST_Checker::Check() when there is no module at all
//!
void UT_AST_Checker::test_Check_NoModule ()
{
  // ---------------- Setup
  //
  AST         ast;
  auto        network = ast.Network();
  AST_Checker sut(network);

  // ---------------- Exercise
  //
  auto checkResult = sut.Check();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS   (checkResult.errorsCount, 1u);
  TS_ASSERT_CONTAINS (checkResult.errors,      "no top module");
}


//! Checks AST_Checker::Check() when module is ok (at its level)
//!
void UT_AST_Checker::test_Check_Module_Ok ()
{
  // ---------------- Setup
  //
  AST  ast;
  auto scanInPortId   = ast.Create_VectorIdentifier("SI", "0", "0");
  auto scanOutPortId  = ast.Create_VectorIdentifier("SO", "0", "0");
  auto sourceSignal   = ast.Create_Signal(scanInPortId);
  auto outSource      = ast.Create_Source(Kind::Source, sourceSignal); // Connect input to output;
  auto scanInPort     = ast.Create_Port(Kind::ScanInPort,  scanInPortId);
  auto scanOutPort    = ast.Create_Port(Kind::ScanOutPort, scanOutPortId, { outSource });
  auto moduleId       = ast.Create_ScalarIdentifier("Test");
  auto moduleChildren = vector<AST_Node*>{scanInPort, scanOutPort};

  auto module = ast.Create_Module(moduleId, std::move(moduleChildren));

  auto network = ast.Network();
  AST_Checker sut(network);

  // ---------------- Exercise
  //
  auto checkResult = sut.Check();

  // ---------------- Verify
  //
  TS_ASSERT_EMPTY (checkResult.errors);
  TS_ASSERT_EMPTY (checkResult.warnings);
}


//! Checks AST_Checker::Check() when module has no ScanOutPort
//!
void UT_AST_Checker::test_Check_Module_NoScanOutPorts ()
{
  // ---------------- Setup
  //
  AST  ast;
  auto moduleId       = ast.Create_ScalarIdentifier("Test");
  auto scanInPortId   = ast.Create_VectorIdentifier("SI", "0", "0");
  auto scanInPort     = ast.Create_Port(Kind::ScanInPort,  scanInPortId);
  auto children       = vector<AST_Node*>({ scanInPort });
  auto module         = ast.Create_Module(moduleId, std::move(children));

  auto        network = ast.Network();
  AST_Checker sut(network);

  // ---------------- Exercise
  //
  auto checkResult    = sut.Check();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS   (checkResult.infosCount,          2u);  // 1 for no ScanOutPorts +1 for modules count
  TS_ASSERT_CONTAINS (checkResult.InformativeReport(), "Module \"Test\" has no ScanOutPorts");
}


//===========================================================================
// End of UT_AST_Checker.cpp
//===========================================================================
