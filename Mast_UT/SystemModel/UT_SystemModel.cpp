//===========================================================================
//                           UT_SystemModel.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModel.cpp
//!
//! Implements test fixture for testing SystemModel
//!
//===========================================================================

#include "UT_SystemModel.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "TestModelBuilder.hpp"
#include "BinaryVector_Traits.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using namespace mast;
using namespace test;


//! Initializes test (called for each test)
void UT_SystemModel::setUp ()
{
//+  CxxTest::setStringResultsOnNewLine(true);
//+  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModel constructor
//!
void UT_SystemModel::test_Constructor ()
{
  // ---------------- Exercise
  //
  SystemModel sut;

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (sut.Root());
  TS_ASSERT_EQUALS  (sut.RegistersCount(), 0);
}


//! Checks SystemModel::NodeWithId() when empty
//!
void UT_SystemModel::test_NodeWithId_When_Empty ()
{
  // ---------------- Setup
  //
  SystemModel                     sut;
  SystemModelNode::NodeIdentifier id = 0;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.NodeWithId(id), std::exception);
}



//! Checks SystemModel::CreateAccessInterface()
//!
void UT_SystemModel::test_CreateAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AI name";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterface(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.Root());
  TS_ASSERT_NOT_NULLPTR (sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR  (sut.Root(), sut.NodeWithId(id));
}



//! Checks SystemModel::CreateRegister() without providing a parent node
//!
void UT_SystemModel::test_CreateRegister_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name   = "Reg name";
  auto        bypass = BinaryVector::CreateFromBinaryString("1010_1");

  sut.CreateAccessInterface("", nullptr); // Necessary to have a root node

  // ---------------- Exercise
  //
  auto node = sut.CreateRegister(name, bypass);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), node);
}


//! Checks SystemModel::CreateChain() without providing a parent node
//!
void UT_SystemModel::test_CreateChain_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto node = sut.CreateChain(name);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), node);
}


//! Checks SystemModel::CreateChain() providing a parent node
//!
void UT_SystemModel::test_CreateChain_With_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai   = sut.CreateAccessInterface("AI name", nullptr);
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto node = sut.CreateChain(name, ai);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (node, sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR (node, ai->FirstChild());
}



//! Checks SystemModel::CreateLinker() without providing a parent node
//!
void UT_SystemModel::test_CreateLinker_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai           = sut.CreateAccessInterface ("ai name", nullptr);
  auto        muxNode      = sut.CreateRegister        ("Selector", BinaryVector::CreateFromBinaryString("01"), ai);
  auto        pathSelector = make_shared<DefaultBinaryPathSelector>(muxNode, 2);

  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto linker = sut.CreateLinker(name, pathSelector);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (linker);
  TS_ASSERT_EQUALS      (linker->Name(), name);

  auto id = linker->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), linker);
}


//! Checks SystemModel::CreateLinker() providing a parent node
//!
void UT_SystemModel::test_CreateLinker_With_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai           = sut.CreateAccessInterface ("ai name", nullptr);
  auto        muxNode      = sut.CreateRegister        ("Selector", BinaryVector::CreateFromBinaryString("01"), ai);
  auto        pathSelector = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto linker = sut.CreateLinker(name, pathSelector, ai);


  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (linker);
  TS_ASSERT_EQUALS      (linker->Name(), name);

  auto id            = linker->Identifier();
  auto aiFirstChild = ai->FirstChild();
  auto muxSibling    = muxNode->NextSibling();

  TS_ASSERT_EQUALS_PTR (linker,  sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR (muxNode, aiFirstChild);
  TS_ASSERT_EQUALS_PTR (linker,  muxSibling);
}


//! Checks SystemModel "No auto root registration mode"
//!
void UT_SystemModel::test_NotAutoRoot ()
{
  // ---------------- Setup
  //
  SystemModel      sut(false);     // Auto root node is disabled
  TestModelBuilder builder(sut);

  // ---------------- Exercise
  //
  auto tapNode = builder.Create_JTAG_TAP("", 6u, 5u);   // Indirectly use sut in "No auto root registration mode"

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (tapNode);
  TS_ASSERT_EQUALS      (tapNode->Name(),       DEFAULT_TAP_NAME);
  TS_ASSERT_EQUALS      (tapNode->Identifier(), 0);
  TS_ASSERT_EQUALS_PTR  (sut.NodeWithId(0),     tapNode);
  TS_ASSERT_NULLPTR     (sut.Root());            // Still no root node managed by SystemModel

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

  auto newReg = sut.CreateRegister("New reg", BinaryVector::CreateFromBinaryString("1010"), tapNode);

  linkerSecondChild = bypassAsRegister->NextSibling();
  TS_ASSERT_NOT_NULLPTR (linkerSecondChild);
  TS_ASSERT_EQUALS      (linkerSecondChild->Name(), "New reg");
}


//! Checks SystemModel::CreateAccessInterfaceTranslator()
//!
void UT_SystemModel::test_CreateAccessInterfaceTranslator ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AT name";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterfaceTranslator(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.Root());
  TS_ASSERT_NOT_NULLPTR (sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR  (sut.Root(), sut.NodeWithId(id));
}

//! Checks SystemModel::CreateCallbackRequest()
//!
void UT_SystemModel::test_CreateCallbackRequest ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "undefined";//"Callback_name";

  // ---------------- Exercise
  //


  //--Undefined Callback
  auto  test = new CallbackRequest();

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS  (test->ToSutVector().BitsCount(),  0);
  TS_ASSERT_EQUALS  (test->ToSutVector().BytesCount(), 0);
  TS_ASSERT_TRUE    (test->ToSutVector().IsEmpty());
  TS_ASSERT_FALSE   (test->ToSutVector().HasFixedSize());
  TS_ASSERT_NULLPTR (test->ToSutVector().DataLeftAligned());

  free(test);
  //--Named CT
  
    name = "Callback_name";

  test = new CallbackRequest(name);
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS  (test->ToSutVector().BitsCount(),  0);
  TS_ASSERT_EQUALS  (test->ToSutVector().BytesCount(), 0);
  TS_ASSERT_TRUE    (test->ToSutVector().IsEmpty());
  TS_ASSERT_FALSE   (test->ToSutVector().HasFixedSize());
  TS_ASSERT_NULLPTR (test->ToSutVector().DataLeftAligned());
  free(test);

  //--Named CT with additional data
  int dummy=3;
  test = new CallbackRequest(name,(void *)&dummy);
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NOT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
  TS_ASSERT_EQUALS  (*(int *)test->interfaceData(),  dummy);
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
  TS_ASSERT_EQUALS  (test->ToSutVector().BitsCount(),  0);
  TS_ASSERT_EQUALS  (test->ToSutVector().BytesCount(), 0);
  TS_ASSERT_TRUE    (test->ToSutVector().IsEmpty());
  TS_ASSERT_FALSE   (test->ToSutVector().HasFixedSize());
  TS_ASSERT_NULLPTR (test->ToSutVector().DataLeftAligned());
  free(test);

  auto toSutVector = BinaryVector::CreateFromBinaryString("01");
  test = new CallbackRequest(name,toSutVector);
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS      (test->FormattedData(),"01");
    TS_ASSERT_EQUALS (test->ToSutVector().BitsCount(),  2);
    TS_ASSERT_EQUALS (test->ToSutVector().BytesCount(), 1);
    TS_ASSERT_FALSE (test->ToSutVector().IsEmpty());
    TS_ASSERT_FALSE  (test->ToSutVector().HasFixedSize());
    
  free(test);

  test = new CallbackRequest(name,toSutVector,(void *)&dummy);
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NOT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
  TS_ASSERT_EQUALS  (*(int *)test->interfaceData(),  dummy);
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
  TS_ASSERT_EQUALS      (test->FormattedData(),"01");
    TS_ASSERT_EQUALS (test->ToSutVector().BitsCount(),  2);
    TS_ASSERT_EQUALS (test->ToSutVector().BytesCount(), 1);
    TS_ASSERT_FALSE (test->ToSutVector().IsEmpty());
    TS_ASSERT_FALSE  (test->ToSutVector().HasFixedSize());

  free(test);
 
 //--Named CT with formatted data
  test = new CallbackRequest(name,toSutVector,"XY");
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS      (test->FormattedData(),"XY");
    TS_ASSERT_EQUALS (test->ToSutVector().BitsCount(),  2);
    TS_ASSERT_EQUALS (test->ToSutVector().BytesCount(), 1);
    TS_ASSERT_FALSE (test->ToSutVector().IsEmpty());
    TS_ASSERT_FALSE  (test->ToSutVector().HasFixedSize());
  free(test);
 
  test = new CallbackRequest(name,toSutVector,"SQ",(void *)&dummy);
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_EQUALS      (test->FormattedData(),"SQ");
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
  TS_ASSERT_EQUALS  (*(int *)test->interfaceData(),  dummy);
  TS_ASSERT_EQUALS  (test->interfaceData(),  (void *)&dummy);
    TS_ASSERT_EQUALS (test->ToSutVector().BitsCount(),  2);
    TS_ASSERT_EQUALS (test->ToSutVector().BytesCount(), 1);
    TS_ASSERT_FALSE (test->ToSutVector().IsEmpty());
    TS_ASSERT_FALSE  (test->ToSutVector().HasFixedSize());
  free(test);

 //--Named CT with only formatted data
  test = new CallbackRequest(name,"XY");
  TS_ASSERT_NOT_NULLPTR (test);
  TS_ASSERT_EQUALS      (test->CallbackId(), name);
  TS_ASSERT_NULLPTR (test->interfaceData());
  TS_ASSERT_EQUALS      (test->FormattedData(),"XY");
    TS_ASSERT_EQUALS (test->ToSutVector().BitsCount(),  0);
  free(test);

 
}

//! Checks SystemModel::CreateAccessInterfaceTranslator() capability
//! of handling Request queues in Non-blocking situations
//! Blocking behaviour checked in UT_MTQueue
//!
void UT_SystemModel::test_CreateAccessInterfaceTranslator_Request_Queues_NB ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AT name";
  string_view Request = "Request_";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterfaceTranslator(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  // One Request
  auto  test = CallbackRequest(Request+"1");
  node->PushRequest(test);
  auto result = node->PopRequest();
  TS_ASSERT_NOT_NULLPTR (&result);
  TS_ASSERT_EQUALS      (result.CallbackId(), test.CallbackId());


  // Multiple Request
  test = CallbackRequest(Request+"1");
  node->PushRequest(test);
  result = node->PopRequest();
  TS_ASSERT_NOT_NULLPTR (&result);
  TS_ASSERT_EQUALS      (result.CallbackId(), test.CallbackId());


  for (int i=0;i<10;i++)
     {
     test = CallbackRequest(Request+std::to_string(i));
      node->PushRequest(test);
     }

  for (int i=0;i<10;i++)
     {
     result = node->PopRequest();
     TS_ASSERT_NOT_NULLPTR (&result);
     TS_ASSERT_EQUALS      (result.CallbackId(), Request+std::to_string(i));
     }

 //Formatted data
  test = CallbackRequest(Request,"100");
  TS_ASSERT_NULLPTR(test.interfaceData());
  TS_ASSERT_EQUALS (test.FormattedData(),"100");
  TS_ASSERT_EQUALS (test.ToSutVector().BitsCount(),  0);

}

//! Checks SystemModel::CreateAccessInterfaceTranslator() capability
//! of handling Result queues in Non-blocking situations
//! Blocking behaviour checked in UT_MTQueue
//!
void UT_SystemModel::test_CreateAccessInterfaceTranslator_Result_Queues_NB ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AT name";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterfaceTranslator(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  // One Request
  auto  test = BinaryVector::CreateFromBinaryString("01");
  node->PushResult(test);
  auto result = node->PopResult();
  TS_ASSERT_NOT_NULLPTR (&result);
  TS_ASSERT_TRUE      (result.CompareEqualTo(test));


  // Multiple Request

  for (int i=0;i<10;i++)
     {
     test = BinaryVector::CreateFromHexString("01"+std::to_string(i));
      node->PushResult(test);
     }

  for (int i=0;i<10;i++)
     {
     result = node->PopResult();
     TS_ASSERT_NOT_NULLPTR (&result);
    TS_ASSERT_TRUE      (result.CompareEqualTo(BinaryVector::CreateFromHexString("01"+std::to_string(i))));
     }

}


//! Checks SystemModel::CreateAccessInterfaceTranslator() capability
//! of handling Update queues in Non-blocking situations
//! Blocking behaviour checked in UT_MTQueue
//!
void UT_SystemModel::test_CreateAccessInterfaceTranslator_Update_Queues_NB ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AT name";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterfaceTranslator(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  // One Request
  auto  test = BinaryVector::CreateFromBinaryString("01");
  node->PushUpdate(test);
  auto Update = node->PopUpdate();
  TS_ASSERT_NOT_NULLPTR (&Update);
  TS_ASSERT_TRUE      (Update.CompareEqualTo(test));


  // Multiple Request

  for (int i=0;i<10;i++)
     {
     test = BinaryVector::CreateFromHexString("01"+std::to_string(i));
      node->PushUpdate(test);
     }

  for (int i=0;i<10;i++)
     {
     Update = node->PopUpdate();
     TS_ASSERT_NOT_NULLPTR (&Update);
    TS_ASSERT_TRUE      (Update.CompareEqualTo(BinaryVector::CreateFromHexString("01"+std::to_string(i))));
     }

}

//! Checks SystemModel::SetRoot()
//!
void UT_SystemModel::test_SetRoot ()
{
  // ---------------- Setup
  //
  SystemModel sut(false); // No auto root node registration

  auto chain = sut.CreateChain("Chain");

  TS_ASSERT_NULLPTR (sut.Root());

  // ---------------- Exercise
  //
  sut.SetRoot(chain);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.Root(), chain);
}


//! Checks SystemModel::DisconnectNode() with a node down the hierarchy
//!
void UT_SystemModel::test_DisconnectNode_Bottom ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  TestModelBuilder builder(sm);

  builder.Create_UnitTestCase_6_Levels();

  auto chain = sm.ChainWithId(11u);
  auto reg   = sm.RegisterWithId(16u);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm.DisconnectNode(reg));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (chain->DirectChildrenCount(), 1u);
  TS_ASSERT_FALSE  (chain->HasDirectChild(reg));
}


//! Checks SystemModel::DisconnectNode() with a node in the middle of the hierarchy
//!
void UT_SystemModel::test_DisconnectNode_Middle ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  TestModelBuilder builder(sm);

  builder.Create_UnitTestCase_6_Levels();

  auto chain_11 = sm.ChainWithId(11u);
  auto chain_7  = sm.ChainWithId(7u);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm.DisconnectNode(chain_11));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (chain_7->DirectChildrenCount(), 3u);
  TS_ASSERT_FALSE  (chain_7->HasDirectChild(chain_11));
}


//! Checks SystemModel::DisconnectNode() with a node down the hierarchy
//!
void UT_SystemModel::test_DisconnectNode_Top ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_UnitTestCase_6_Levels();
  auto mux = sm.LinkerWithId(2u);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm.DisconnectNode(mux));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (tap->DirectChildrenCount(), 1u);
  TS_ASSERT_FALSE  (tap->HasDirectChild(mux));
}



//! Checks SystemModel::ReplaceRoot() without removing it from SystemModel
//!
void UT_SystemModel::test_ReplaceRoot_NotRemoved ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP("Tap", 6u, 3u);
  auto newTap = sm.CreateAccessInterface("New Tap", nullptr);

  // ---------------- Exercise
  //
  auto oldTap = sm.ReplaceRoot(newTap, false);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (oldTap,    tap);
  TS_ASSERT_EQUALS_PTR (sm.Root(), newTap);
  TS_ASSERT_EQUALS_PTR (sm.NodeWithId(tap->Identifier()), tap);
}


//! Checks SystemModel::ReplaceRoot() removing it from SystemModel
//!
void UT_SystemModel::test_ReplaceRoot_Removed ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP("Tap", 6u, 3u);
  auto newTap = sm.CreateAccessInterface("New Tap", nullptr);

  // ---------------- Exercise
  //
  auto oldTap = sm.ReplaceRoot(newTap, true);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (oldTap,    tap);
  TS_ASSERT_EQUALS_PTR (sm.Root(), newTap);
  TS_ASSERT_THROWS     (sm.NodeWithId(tap->Identifier()), std::exception);
}


//! Checks SystemModel::ReplaceRoot() giving a nullptr
//!
void UT_SystemModel::test_ReplaceRoot_WithNullptr ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("Tap", 6u, 3u);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sm.ReplaceRoot(nullptr, false), std::exception);
}


//===========================================================================
// End of UT_SystemModel.cpp
//===========================================================================
