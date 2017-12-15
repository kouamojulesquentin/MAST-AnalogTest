//===========================================================================
//                           UT_MastEnvironment.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastEnvironment.cpp
//!
//! Implements test fixture for testing MastEnvironment
//!
//===========================================================================

#include "UT_MastEnvironment.hpp"
#include "MastEnvironment.hpp"
#include "AccessInterfaceProtocolFactory.hpp"
#include "PDL_AlgorithmsRepository.hpp"
#include "PDL_Adapter_CPP.hpp"
#include "Startup.hpp"
#include "SystemModel.hpp"
#include "SystemModelManager.hpp"
#include "Utility.hpp"

#include "TestUtilities.hpp"
#include "SpiedProtocolsCommands.hpp"
#include "Spy_SVF_Protocol.hpp"

#include <cxxtest/ValueTraits.h>
#include <cxxtest/traits/STL11_Traits.h>
#include <vector>
#include <array>
#include <string>
#include <experimental/string_view>

using std::vector;
using std::string;
using std::experimental::string_view;
using namespace std::experimental::literals::string_view_literals;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::make_unique;

using namespace mast;
using namespace std::chrono_literals;

namespace
{
uint32_t g_Fake_PDL_AlgorithmCallCount = 0;

//! Returns path of a file accessible by test runner
//!
//! @param fileName   File name used by test
//!
string GetTestFilePath (string_view fileName, bool checkExists = true)
{
  return test::GetTestFilePath({"Mast_API"s, "UT_TestFiles"s, string(fileName)}, checkExists);
}
//
//  End of: GetTestFilePath
//---------------------------------------------------------------------------

//! Builds up some command line arguments
//!
vector<string> MakeCmdlineArguments ()
{
  vector<string> arguments {
                             "Mast.exe",
                             "--min_cycle=12ms",
                             "--max_cycle=36ms",
                             "--sit="        + GetTestFilePath("UT_MastEnvironment.sit"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                             "--plugin=Fake_Plugin_1",
                           };
  return arguments;
}
//
//  End of: MakeCmdlineArguments
//---------------------------------------------------------------------------

//! Fake PDL algorithm used for tests (to avoid loading a real plugin)
//!
//! @note Does nothing to avoid blocking the test
void Fake_PDL_Algorithm ()
{
  ++g_Fake_PDL_AlgorithmCallCount;
}
//
//  End of: PDL_Algorithm
//---------------------------------------------------------------------------

//! PDL algorithm used for tests (to avoid loading a real plugin)
void PDL_Algorithm ()
{
  auto     registerPath = "reg";
  auto     loopCount    = 5u;
  uint16_t initialValue = 1u;

  for (auto cycle = 0u ; cycle < loopCount ; ++cycle)
  {
    iWrite(registerPath, initialValue++);
    iGet<uint16_t>(registerPath);

    iApply();
  }
}
//
//  End of: PDL_Algorithm
//---------------------------------------------------------------------------


//! Returns expected SVF command for PDL_Algorithm function using "last_lazy" configuration file
//!
vector<string> PDLAlgoExpectedSVFCommands_Lazy ()
{
  vector<string> expectedCommands {
                                    "SIR 4 TDI(01);",
                                    "SDR 12 TDI(0001);",
                                    "SDR 12 TDI(0002);",
                                    "SDR 12 TDI(0003);",
                                    "SDR 12 TDI(0004);",
                                    "SDR 12 TDI(0005);",
                                  };
  return expectedCommands;
}
//
//  End of: PDLAlgoExpectedSVFCommands
//---------------------------------------------------------------------------




} // End of unnamed namespace



//! Initializes test (called for each test)
void UT_MastEnvironment::setUp ()
{
  Startup::ForgetSystemModel();
  Startup::ForgetManager();     //!< Gets rid of common SystemModelManager singleton
  SystemModelNode::ResetNodeIdentifier();

  g_Fake_PDL_AlgorithmCallCount = 0;
  auto& algos = PDL_AlgorithmsRepository::Instance();
  algos.Clear();
}


//! Checks MastEnvironment construction for unit tests context
//!
void UT_MastEnvironment::test_Constructor_UnitTest ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (MastEnvironment sut(true));
}



//! Checks MastEnvironment::ParseOptions() with no arguments (just program name)
//!
void UT_MastEnvironment::test_ParseOptions_NoOptions ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  vector<string>  options {"Mast.exe"};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseOptions(options));
}


//! Checks MastEnvironment::ParseOptions() with bad argument
//!
void UT_MastEnvironment::test_ParseOptions_bad_Args ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  vector<string>  arguments {"Mast.exe", "--badArg"};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.ParseOptions(arguments), std::runtime_error);
}



//! Checks MastEnvironment::ParseOptions() with no YAML configuration file
//!
void UT_MastEnvironment::test_ParseOptions_no_Yaml ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  vector<string>  arguments {
                              "Mast.exe",
                              "--sit=" + GetTestFilePath("UT_MastEnvironment.sit"),
                              "--check",
                            };

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseOptions(arguments));
}


//! Checks MastEnvironment::ParseOptions() with YAML configuration file
//!
void UT_MastEnvironment::test_ParseOptions_with_Yaml ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  vector<string>  arguments {
                              "Mast.exe",
                              "--conf=" + GetTestFilePath("UT_MastEnvironment.yml"),
                              "--sit="  + GetTestFilePath("UT_MastEnvironment.sit"),
                              "--check",
                            };

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseOptions(arguments));
}

//! Checks MastEnvironment::ParseOptions() with with arc, argv
//!
void UT_MastEnvironment::test_ParseOptions_Argc_Argv ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& arg)
  {
    // ---------------- Setup
    //
    const char* argv[] = { "Mast.exe", arg.c_str() };
    auto argc = std::extent<decltype(argv)>::value;

    MastEnvironment sut(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseOptions(argc, argv));
  };

  auto data =
  {
    "--sit="s.append(GetTestFilePath("UT_MastEnvironment.sit")),
    "--icl="s.append(GetTestFilePath("UT_MastEnvironment.icl")),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks MastEnvironment::LoadPlugins() before requesting to parse options
//!
void UT_MastEnvironment::test_LoadPlugins_BeforeParsingOptions ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.LoadPlugins(), std::exception);
}


//! Checks MastEnvironment::LoadPlugins() after requesting to parse options
//!
void UT_MastEnvironment::test_LoadPlugins_AfterParsingOptions ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.LoadPlugins());
}


//! Checks MastEnvironment::CreateSystemModel() before requesting to loading plugins
//!
void UT_MastEnvironment::test_CreateSystemModel_BeforeLoadPlugins ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateSystemModel(), std::runtime_error);
}


//! Checks MastEnvironment::CreateSystemModel() after requesting to loading plugins
//!
void UT_MastEnvironment::test_CreateSystemModel_AfterLoadPlugins ()
{
  // ---------------- Setup
  //
  auto checkResultFile = GetTestFilePath("Model_Check.txt", false);
  Utility::ClearFile(checkResultFile);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();

  // Fake plugin registation
  auto& algos = PDL_AlgorithmsRepository::Instance();

  algos.RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateSystemModel());

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (Startup::GetSystemModel_NoCreate());

  TS_ASSERT_FILE_CONTENT (checkResultFile, "No issue found in model");
}


//! Checks MastEnvironment::CreateSystemModel() when there is an error in the model
//!
void UT_MastEnvironment::test_CreateSystemModel_SIT_Error ()
{
  // ---------------- Setup
  //
  auto checkResultFile = GetTestFilePath("Model_Check.txt", false);
  Utility::ClearFile(checkResultFile);


  vector<string> arguments {
                             "Mast.exe",
                             "--sit="        + GetTestFilePath("UT_MastEnvironment_with_error.sit"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                           };
  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();

  // Fake plugin registation
  auto& algos = PDL_AlgorithmsRepository::Instance();

  algos.RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateSystemModel(), std::runtime_error);

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (Startup::GetSystemModel_NoCreate());

  auto expectedReport = "Errors   (2):\n"
                        "  - Linker 'TAP_DR_Mux' (id: 2) has 3 children, even though it can only select 2 paths\n"
                        "  - Register 'reg' (id: 5) Has same name as a previous sibling\n"
                        "Warnings (0):\n"
                        "Infos    (0):\n"s;

  TS_ASSERT_FILE_CONTENT (checkResultFile, expectedReport);
}


//! Checks MastEnvironment::CreateSystemModel() when there is an error in the model
//! and error is not reported in a file
//!
void UT_MastEnvironment::test_CreateSystemModel_SIT_Error_no_CheckFile ()
{
  // ---------------- Setup
  //
  auto checkResultFile = GetTestFilePath("Model_Check.txt", false);
  Utility::ClearFile(checkResultFile);

  vector<string> arguments {
                             "Mast.exe",
                             "--sit=" + GetTestFilePath("UT_MastEnvironment_with_error.sit"),
                             "--check",
                           };
  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();

  // Fake plugin registation
  auto& algos = PDL_AlgorithmsRepository::Instance();

  algos.RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateSystemModel(), std::runtime_error);

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR    (Startup::GetSystemModel_NoCreate());
  TS_ASSERT_FILE_EMPTY (checkResultFile);
}


//! Checks MastEnvironment::CreateSystemModel() when parsed SIT model
//! is exported as GML and pretty print
//!
void UT_MastEnvironment::test_CreateSystemModel_SIT_Model_Exports ()
{
  // ---------------- Setup
  //
  auto sitExportExpectedFile   = GetTestFilePath("UT_MastEnvironment_ExpectedExport.sit");
  auto gmlExpectedFile         = GetTestFilePath("UT_MastEnvironment_MastModel.gml");
  auto prettyPrintExpectedFile = GetTestFilePath("UT_MastEnvironment_MastModel.txt");
  auto sitExportFile           = "MastModel_Export.sit";
  auto gmlFile                 = "MastModel.gml";
  auto prettyPrintFile         = "MastModel.txt";

  Utility::ClearFile(sitExportFile);
  Utility::ClearFile(gmlFile);
  Utility::ClearFile(prettyPrintFile);

  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  vector<string> arguments {
                             "Mast.exe",
                             "--conf=" + GetTestFilePath("UT_MastEnvironment.yml"),
                             "--sit="  + GetTestFilePath("UT_MastEnvironment.sit"),
                           };
  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();


  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateSystemModel());

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (Startup::GetSystemModel_NoCreate());
  TS_ASSERT_SAME_FILES  (gmlFile,         gmlExpectedFile);
  TS_ASSERT_SAME_FILES  (prettyPrintFile, prettyPrintExpectedFile);
  TS_ASSERT_SAME_FILES  (sitExportFile,   sitExportExpectedFile);
}



//! Checks MastEnvironment::CreateSystemModel() with change of protocol
//!
void UT_MastEnvironment::test_CreateSystemModel_ChangeProtocol ()
{
  // ---------------- Setup
  //
  // Spy protocol
  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  vector<string> arguments {
                             "Mast.exe",
                             "--protocol_name=Spy",
                             "--sit="        + GetTestFilePath("UT_MastEnvironment.sit"),
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateSystemModel());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  auto model = Startup::GetSystemModel_NoCreate();
  TS_ASSERT_NOT_NULLPTR (model);
  auto root = model->Root();
  TS_ASSERT_NOT_NULLPTR (root);
  auto rootAsAi = dynamic_pointer_cast<AccessInterface>(root);
  TS_ASSERT_NOT_NULLPTR (rootAsAi);
  auto protocol = rootAsAi->Protocol();
  TS_ASSERT_NOT_NULLPTR (protocol);
  auto spyProtocol = dynamic_pointer_cast<test::Spy_SVF_Protocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (spyProtocol);
}


//! Checks MastEnvironment::CreateSystemModel() with change of protocol while top node is a Chain
//!
void UT_MastEnvironment::test_CreateSystemModel_ChangeProtocol_TopIsChain ()
{
  // ---------------- Setup
  //
  // Spy protocol
  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  vector<string> arguments {
                             "Mast.exe",
                             "--protocol_name=Spy",
                             "--sit="        + GetTestFilePath("UT_MastEnvironment_top_chain.sit"),
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateSystemModel());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  auto model = Startup::GetSystemModel_NoCreate();
  TS_ASSERT_NOT_NULLPTR (model);
  auto root = model->Root();
  TS_ASSERT_NOT_NULLPTR (root);
  auto rootAsChain = dynamic_pointer_cast<Chain>(root);
  TS_ASSERT_NOT_NULLPTR (rootAsChain);
  auto chainFirstChild = rootAsChain->FirstChild();
  TS_ASSERT_NOT_NULLPTR (chainFirstChild);
  auto firstAi = dynamic_pointer_cast<AccessInterface>(chainFirstChild);
  TS_ASSERT_NOT_NULLPTR (firstAi);
  auto protocol = firstAi->Protocol();
  TS_ASSERT_NOT_NULLPTR (protocol);
  auto spyProtocol = dynamic_pointer_cast<test::Spy_SVF_Protocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (spyProtocol);
}



//! Checks MastEnvironment::CreateManager() before creating SystemModel
//!
void UT_MastEnvironment::test_CreateManager_BeforeCreateSystemModel ()
{
  // ---------------- Setup
  //
  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateManager(), std::runtime_error);
}


//! Checks MastEnvironment::CreateManager() after creating SystemModel
//!
void UT_MastEnvironment::test_CreateManager_AfterCreateSystemModel ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateManager());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  auto manager = Startup::GetManager_NoCreate();

  TS_ASSERT_NOT_NULLPTR (manager);
  TS_ASSERT_EQUALS      (manager->SleepTimeBetweenConfigurations(), 12ms); // See MakeCmdlineArguments for value
  TS_ASSERT_EQUALS      (manager->DataCycleLoopTimeout(),           36ms); // See MakeCmdlineArguments for value
}


//! Checks MastEnvironment::CreateManager() when activating manager monitoring
//!
void UT_MastEnvironment::test_CreateManager_Monitoring ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  vector<string> arguments {
                             "Mast.exe",
                             "--conf=" + GetTestFilePath("UT_MastEnvironment.yml"),
                             "--sit="  + GetTestFilePath("UT_MastEnvironment.sit"),
                           };

  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();
  sut.CreateSystemModel();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateManager());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  auto manager = Startup::GetManager_NoCreate();

  TS_ASSERT_NOT_NULLPTR (manager);
  TS_ASSERT_EQUALS      (manager->SleepTimeBetweenConfigurations(), 17ms); // See UT_MastEnvironment.yml for value
  TS_ASSERT_EQUALS      (manager->DataCycleLoopTimeout(),           23ms); // See UT_MastEnvironment.yml for value
}




//! Checks MastEnvironment::CreateApplications() before creating the manager
//!
void UT_MastEnvironment::test_CreateApplications_BeforeCreateManager ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateApplications(), std::runtime_error);
}


//! Checks MastEnvironment::CreateApplications() after creating the manager
//!
void UT_MastEnvironment::test_CreateApplications_AfterCreateManager ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();
  sut.CreateManager();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateApplications());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0);  // Algorithm has not been started yet
}


//! Checks MastEnvironment::CreateApplications() when application associated with a register in SIT description is not registered
//!
void UT_MastEnvironment::test_CreateApplications_NotFoundApp ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();
  sut.CreateManager();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateApplications(), std::runtime_error); // "Incr" PDL algorithm not registered!
}

//! Checks MastEnvironment::Start() before creating applications
//!
void UT_MastEnvironment::test_Start_BeforeCreateApp ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();
  sut.CreateManager();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0);  // Algorithm cannot be started (the manager is not aware of it)
}

//! Checks MastEnvironment::Start() after creating applications
//!
void UT_MastEnvironment::test_Start_AfterCreateApp ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", Fake_PDL_Algorithm);

  MastEnvironment sut(true);
  sut.ParseOptions(MakeCmdlineArguments());
  sut.LoadPlugins();
  sut.CreateSystemModel();
  sut.CreateManager();
  sut.CreateApplications();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 1u);
}


//! Checks MastEnvironment::Start() with a real PDL algorithm
//!
void UT_MastEnvironment::test_Start_RealPDLAlgo ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", PDL_Algorithm);

  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  vector<string> arguments {
                             "Mast.exe",
                             "--protocol_name=Spy",
                             "--config_algo=last_lazy",
                             "--sit="        + GetTestFilePath("UT_MastEnvironment.sit"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);
  sut.ParseOptions(arguments);
  sut.LoadPlugins();
  sut.CreateSystemModel();
  sut.CreateManager();
  sut.CreateApplications();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0u); // Fake algorithm has not been started (not associated with a node)

  auto expectedCommands = PDLAlgoExpectedSVFCommands_Lazy();  // Lazy algorithm should not be thread dependent
  TS_ASSERT_EQUALS (spiedCommands->Commands(), expectedCommands);
}


//! Checks MastEnvironment::Start() with arguments and a real PDL algorithm
//!
void UT_MastEnvironment::test_Start_Arguments_RealPDLAlgo_SIT ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", PDL_Algorithm);

  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  vector<string> arguments {
                             "Mast.exe",
                             "--config_algo=last_or_default",
                             "--protocol_name=Spy",
                             "--sit="        + GetTestFilePath("test_Start_Arguments_RealPDLAlgo_SIT.sit"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0u); // Fake algorithm has not been started (not associated with a node)

  const auto& effectiveCommands = spiedCommands->Commands();
  auto minimalExpectedCommands  = PDLAlgoExpectedSVFCommands_Lazy();  // Last_or_default algorithm depends on threads scheduling
  for (const auto& command : minimalExpectedCommands)
  {
    TS_ASSERT_CONTAINS (effectiveCommands, command);
  }
}


//! Checks MastEnvironment::Start() with arguments and a real PDL algorithm for an ICL define test network
//!
void UT_MastEnvironment::test_Start_Arguments_RealPDLAlgo_ICL ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", PDL_Algorithm);

  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  vector<string> arguments {
                             "Mast.exe",
                             "--config_algo=last_or_default",
                             "--protocol_name=Spy",
                             "--icl="        + GetTestFilePath("test_Start_Arguments_RealPDLAlgo_ICL.icl"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0u); // Fake algorithm has not been started (not associated with a node)

  const auto& effectiveCommands = spiedCommands->Commands();
  auto minimalExpectedCommands  = PDLAlgoExpectedSVFCommands_Lazy();  // Last_or_default algorithm depends on threads scheduling
  for (const auto& command : minimalExpectedCommands)
  {
    TS_ASSERT_CONTAINS (effectiveCommands, command);
  }
}


//! Checks MastEnvironment::Start() with arguments and a real PDL algorithm for an ICL define test network
//!
void UT_MastEnvironment::test_Start_Arguments_RealPDLAlgo_ICL_List ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", PDL_Algorithm);

  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  vector<string> arguments {
                             "Mast.exe",
                             "--config_algo=last_or_default",
                             "--protocol_name=Spy",
                             "--icl_list=" + GetTestFilePath("test_Start_Arguments_RealPDLAlgo_ICL_List.txt"),
                             "--check",
                             "--check_file=" + GetTestFilePath("Model_Check.txt", false), // Do not check it exists
                             "--plugin=Fake_Plugin_1",
                           };

  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0u); // Fake algorithm has not been started (not associated with a node)

  const auto& effectiveCommands = spiedCommands->Commands();
  auto minimalExpectedCommands  = PDLAlgoExpectedSVFCommands_Lazy();  // Last_or_default algorithm depends on threads scheduling
  for (const auto& command : minimalExpectedCommands)
  {
    TS_ASSERT_CONTAINS (effectiveCommands, command);
  }
}



//! Checks MastEnvironment::Start() with arc, argv and a real PDL algorithm
//!
void UT_MastEnvironment::test_Start_Argc_Argv_RealPDLAlgo_SIT ()
{
  // ---------------- Setup
  //
  // Fake plugin registation
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Fake", Fake_PDL_Algorithm);
  PDL_AlgorithmsRepository::Instance().RegisterAlgorithm("Incr", PDL_Algorithm);

  auto spiedCommands = make_shared<test::SpiedProtocolsCommands>();
  auto creator       = [spiedCommands](const string& /*params*/) { return make_unique<test::Spy_SVF_Protocol>(spiedCommands); };
  AccessInterfaceProtocolFactory::Instance().RegisterCreator("Spy", creator);

  auto sitArg       = "--sit="        + GetTestFilePath("test_Start_Arguments_RealPDLAlgo_SIT.sit");
  auto checkFileArg = "--check_file=" + GetTestFilePath("Model_Check.txt", false); // Do not check it exists

  const char* argv[] = {
                         "Mast.exe",
                         "--protocol_name=Spy",
                         sitArg.data(),
                         "--check",
                         checkFileArg.data(),
                         "--plugin=Fake_Plugin_1",
                       };
  auto argc = std::extent<decltype(argv)>::value;

  MastEnvironment sut(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.Start(argc, argv));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (g_Fake_PDL_AlgorithmCallCount, 0u); // Fake algorithm has not been started (not associated with a node)

  const auto& effectiveCommands = spiedCommands->Commands();
  auto minimalExpectedCommands  = PDLAlgoExpectedSVFCommands_Lazy();  // Last_or_default algorithm depends on threads scheduling
  for (const auto& command : minimalExpectedCommands)
  {
    TS_ASSERT_CONTAINS (effectiveCommands, command);
  }
}



//===========================================================================
// End of UT_MastEnvironment.cpp
//===========================================================================
