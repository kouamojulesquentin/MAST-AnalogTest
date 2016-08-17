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

#include "SystemModelAdapter.h"
#include "Startup.hpp"
#include "SystemModelBuilder.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "GmlPrinterVisitor.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "SystemModelManager.hpp"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "LogFormatter.h"
#include "LoggerSinks.h"
#include "CustomFileSink.h"

#include <iostream>
#include <stdexcept>
#include <memory>
#include <fstream>
#include <sstream>

using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::cout;
using std::ofstream;
using std::ostringstream;

using namespace mast;



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


//! Runs a testcase depending on actual arguments
//!
int main (int /* argc */, char */* argv */[])
{
  std::cout << "Test case 1500 Wrapper" << std::endl;

//+  auto errorCode = RunMast(filePath, pAssociations, )

  auto retCode = ErrorCode::Ok;
  try
  {
    auto logworker = InitializeLogger();

    auto sm               = mast::Startup::GetSystemModel();
    auto builder          = SystemModelBuilder(*sm);
    auto protocol         = make_shared<LoopbackAccessInterfaceProtocol> ();
    auto accessInterface  = builder.Create_JTAG_TAP("Tap", 8u, 2u, protocol);
    auto derivationsCount = uint32_t(4u);
    auto wrapper          = builder.Create_1500_Wrapper("1500", derivationsCount);

    accessInterface->AppendChild(wrapper);
    builder.AppendRegisters(4u, "dynamic_", BinaryVector::CreateFromHexString("ABCD"), wrapper);

    sm->SetRoot(accessInterface);

    auto printGraph  = true;

    auto checkResult = sm->Check();

    if (checkResult.HasIssues())
    {
      cout << "!!!!!! Invalid model !!!!!!" << std::endl;
      cout << checkResult.MakeReport();

      if (checkResult.HasErrors())
      {
        retCode = ErrorCode::InvalidModel;
        return static_cast<int>(retCode);
      }
      printGraph = true;
    }

    if (printGraph)
    {
      ofstream os("Testcase_1500.gml");
      os << GmlPrinterVisitor::Graph(accessInterface);
    }

    if (retCode == ErrorCode::Ok)
    {
      auto manager = mast::Startup::GetManager();
      auto monitor = std::make_shared<SystemModelManagerMonitor>();
      manager->Monitor(monitor);
      manager->Start();

      auto pdlApp = [manager](uint16_t loopCount, uint16_t initialValue)
      {
        while (loopCount)
        {
          manager->iWrite("dynamic_0", initialValue);
          manager->iApply();

          ++initialValue;
          --loopCount;
        }
      };


//+      auto node         = dynamic_pointer_cast<ParentNode>(wrapper->FindNode("SWIR."));
      auto appNode         = wrapper->DeepestChildAppender();

      auto initialValue = uint16_t(0x1000);
      auto loopCount    = uint16_t(10);
      for (uint32_t ii = 0 ; ii < derivationsCount ; ++ii)
      {
        ostringstream os;
        os << "App_" << ii;
        auto appName      = os.str();
        auto appWrapper   = [pdlApp, loopCount, initialValue]() { pdlApp(loopCount, initialValue); };
        manager->CreateApplicationThread(appNode, appWrapper, appName);

        initialValue += 0x1000;
      }

      manager->StartCreatedApplicationThreads();
      manager->WaitForApplicationsEnd();
      manager->Stop();

//+      manager.reset();
    //! @todo [JFC]-[August/17/2016]: In main(): Create then used a MAST session that initialize/cleanup mast
    //!
      mast::Startup::ForgetManager();
      mast::Startup::ForgetSystemModel();

      std::cout << "Test case run successfully" << std::endl;
    }
  }
  catch(std::invalid_argument& exc) { retCode = ErrorCode::InvalidArgument;  std::cout << exc.what(); }
  catch(std::out_of_range&     exc) { retCode = ErrorCode::OutOfRange;       std::cout << exc.what(); }
  catch(std::logic_error&      exc) { retCode = ErrorCode::LogicError;       std::cout << exc.what(); }
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



