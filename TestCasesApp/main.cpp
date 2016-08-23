//===========================================================================
//                           main.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file main.cpp
//!
//! Defines main for testcases application
//===========================================================================

#include "Session.hpp"
#include "SystemModelAdapter.h"
#include "SystemModelBuilder.hpp"
#include "SystemModelManager.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "GmlPrinterVisitor.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "LogFormatter.h"
#include "LoggerSinks.h"
#include "CustomFileSink.h"
#include "Options.hpp"

#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <experimental/string_view>
#include <iostream>
#include <fstream>
#include <regex>

using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::ofstream;
using std::ostringstream;
using std::cout;
using std::cerr;

using namespace mast;

namespace
{
struct ApplicationDescriptor
{
  using Application_t = std::function<void()>;

  Application_t function;
  std::string   topNodePath;
  std::string   debugName;

  ApplicationDescriptor(Application_t p_function, std::string p_topNodePath, std::string p_debugName = "")
    : function    (p_function)
    , topNodePath (std::move(p_topNodePath))
    , debugName   (std::move(p_debugName))
  {}
};


//! Check SystemModel coherency
//!
ErrorCode CheckResult (shared_ptr<SystemModel> sm)
{
  auto retCode = ErrorCode::Ok;

  auto checkResult = sm->Check();

  if (checkResult.HasIssues())
  {
    cout << "!!!!!! Invalid model !!!!!!" << std::endl;
    cout << checkResult.MakeReport();

    if (checkResult.HasErrors())
    {
      retCode = ErrorCode::InvalidModel;
    }
  }
  return retCode;
}
//
//  End of: CheckResult
//---------------------------------------------------------------------------


//! Builds SystemModel with optional AccessInterfaceProtocol for specified testcase.
//! @param sm               Current system model
//! @param protocol         Optional protocol (supersede one that may be defined by the testcase)
//! @param testcase         Tescase kind
//! @param testcaseOptions  Options for test case (depends on tescase kind)
//!
//! @return Application function and their association with a ParentNode within the SystemModel
vector<ApplicationDescriptor> CreateTestcase (shared_ptr<SystemModel>             sm,
                                              shared_ptr<SystemModelManager>      manager,
                                              shared_ptr<AccessInterfaceProtocol> protocol,
                                              Options::Testcase                   testcase,
                                              const string&                       testcaseOptions)
{
  vector<ApplicationDescriptor> associations;

  switch (testcase)
  {
    case Options::Testcase::NotSpecified:
      cerr << "A testcase must be specified !!!" ;
      break;
    case Options::Testcase::SIT_File:
    {
      auto file = testcaseOptions;
      break;
    }
    case Options::Testcase::Wrapper_1500:
    {
      //! @todo [JFC]-[August/22/2016]: In CreateTestcase(): Use function provided by testcase file
      //!
      auto builder          = SystemModelBuilder(*sm);
      auto accessInterface  = builder.Create_JTAG_TAP("Tap", 8u, 2u, protocol);
      auto derivationsCount = uint32_t(4u);
      auto wrapper          = builder.Create_1500_Wrapper("1500", derivationsCount);

      accessInterface->AppendChild(wrapper);
      builder.AppendRegisters(4u, "dynamic_", BinaryVector::CreateFromHexString("ABCD"), wrapper);

      sm->SetRoot(accessInterface);


      auto pdlApp = [manager](uint16_t loopCount, string registerPath, uint16_t initialValue)
      {
        while (loopCount)
        {
          manager->iWrite(registerPath, initialValue);
          manager->iApply();

          ++initialValue;
          --loopCount;
        }
      };

  //+    auto appNode      = wrapper->DeepestChildAppender();
      auto topPath      = "Tap_DR_Mux.1500.SWIR.SWIR_mux.WIR.WIR_mux";
      auto initialValue = uint16_t(0x1000);
//+      auto loopCount    = options.loopCount;
      auto loopCount    = 10;

      for (uint32_t ii = 0 ; ii < derivationsCount ; ++ii)
      {
        ostringstream os_app;
        os_app << "App_" << ii;
        auto appName      = os_app.str();

        ostringstream os_reg;
        os_reg << "dynamic_" << ii;
        auto registerPath = os_reg.str();

        auto appWrapper   = [pdlApp, loopCount, registerPath, initialValue]() { pdlApp(loopCount, registerPath, initialValue); };

        initialValue += 0x1000;

        associations.emplace_back(appWrapper, topPath, appName);
      }
      break;
    }
    default:
      break;
  }


  return associations;
}
//
//  End of: CreateTestcase
//---------------------------------------------------------------------------



//! Creates a AccessInterfaceProtocol corresponding with protocol option
//!
//! @param protocol         Protocol kind
//! @param protocolOptions  Options needed to specific protocol
//!
//! @return An AccessInterfaceProtocol
shared_ptr<AccessInterfaceProtocol> GetProtocol (Options::Protocol protocol, const string& protocolOptions)
{
  auto aiProtocol = shared_ptr<AccessInterfaceProtocol>();

  switch (protocol)
  {
    case Options::Protocol::NotSpecified:
      LOG(DEBUG) << "No specific protocol";
      break;
    case Options::Protocol::LoopBack:
      aiProtocol = make_shared<LoopbackAccessInterfaceProtocol> ();
      break;
    case Options::Protocol::SVF_Emulation:
      THROW_LOGIC_ERROR("Not yet implemented");
      break;
    case Options::Protocol::SVF_Simulation:
    {
      auto paths = Utility::Split(protocolOptions, ",");
      if (paths.size() != 2)
      {
        THROW_LOGIC_ERROR("Expecting 2 files paths for SVF_Simulation protocol");
      }
      auto toSutFilePath   = paths[0]; Utility::TrimBoth(toSutFilePath);
      auto fromSutFilePath = paths[1]; Utility::TrimBoth(fromSutFilePath);
      aiProtocol = make_shared<SVF_SimulationProtocol> (toSutFilePath, fromSutFilePath);
      break;
    }
    case Options::Protocol::I2C_Emulation:
    THROW_LOGIC_ERROR("Not yet implemented");
      break;
    case Options::Protocol::OpenOCD:
    THROW_LOGIC_ERROR("Not yet implemented");
      break;
    case Options::Protocol::Generic:
    THROW_LOGIC_ERROR("Not yet implemented");
      break;
    default:
      THROW_INVALID_ARGUMENT("Unsupported aiProtocol");
      break;
  }

  return aiProtocol;
}
//
//  End of: GetProtocol
//---------------------------------------------------------------------------




//! Initializes logger facility
//!
std::unique_ptr<g3::LogWorker> InitializeLogger ()
{
  auto logworker    = g3::LogWorker::createLogWorker();
  auto logFormatter = g3::LogFormatter();

  logFormatter.ShowDate(false);
  logFormatter.ShowTime(true);
  logFormatter.ShowFileName(false);
  logFormatter.ShowFunctionName(true);
  logFormatter.ShowLineNumber(true);

  auto customSink = std::make_unique<g3::CustomFileSink>("Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, logFormatter);
  customSink->Clear();

  logworker->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);

  g3::initializeLogging(logworker.get());
  g3::logEnabled(true);

  return logworker;
}
//
//  End of: InitializeLogger
//---------------------------------------------------------------------------


} // End of unnamed namespace



//! Runs a testcase depending on actual arguments
//!
int main (int argc, char* argv [])
{
//+  auto errorCode = RunMast(filePath, associations)

  auto retCode = ErrorCode::Ok;
  try
  {
    auto logworker  = InitializeLogger();

    auto options = Options::ParseArguments(argc, argv);
    cout << options.ToDebugString("Retained options: \n", "   ");

    auto session          = Session (std::make_shared<SystemModelManagerMonitor>());
    auto sm               = session.sm;
    auto manager          = session.manager;
    auto protocol         = GetProtocol(options.protocol, options.protocolOptions);
    auto descriptors      = CreateTestcase(sm, manager, protocol, options.testcase, options.testcaseOptions);

    retCode = CheckResult(sm);
    if (retCode != ErrorCode::Ok)
    {
      options.printGraph = true;
    }

    if (options.printGraph)
    {
      ofstream os("Testcase_1500.gml");
      os << GmlPrinterVisitor::Graph(sm->Root());
    }

    if (retCode != ErrorCode::Ok)
    {
      return static_cast<int>(retCode);
    }

//+    RunMast(testcase);

    for (const auto& descriptor : descriptors)
    {
      auto appNode = dynamic_pointer_cast<ParentNode>(sm->Root()->FindNode(descriptor.topNodePath));

      manager->CreateApplicationThread(appNode, descriptor.function, descriptor.debugName);
    }

    manager->Start();
    manager->StartCreatedApplicationThreads();
    manager->WaitForApplicationsEnd();
    manager->Stop();

    std::cout << "End of test case (see log file in case of errors)" << std::endl;
  }
  catch(std::invalid_argument& exc) { retCode = ErrorCode::InvalidArgument;  std::cout << exc.what(); }
  catch(std::out_of_range&     exc) { retCode = ErrorCode::OutOfRange;       std::cout << exc.what(); }
  catch(std::logic_error&      exc) { retCode = ErrorCode::LogicError;       std::cout << exc.what(); }
  catch(std::regex_error&      exc) { retCode = ErrorCode::RegexException;   std::cout << exc.what(); }
  catch(std::runtime_error&    exc) { retCode = ErrorCode::RuntimeError;     std::cout << exc.what(); }
  catch(std::exception&        exc) { retCode = ErrorCode::StdException;     std::cout << exc.what(); }
  catch(...)                        { retCode = ErrorCode::UndefinedFailure; std::cout << "Got non C++ std exception"; }

  return static_cast<int>(retCode);
}
//
//  End of: main
//---------------------------------------------------------------------------



//===========================================================================
// End of main.cpp
//===========================================================================



