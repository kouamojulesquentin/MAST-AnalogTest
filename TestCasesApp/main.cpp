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
#include "Startup.hpp"
#include "ApplicationAssociation_CPP.hpp"
#include "SystemModelAdapter_CPP.hpp"
#include "ErrorCode_C.h"
#include "SystemModelBuilder.hpp"
#include "SystemModelManager.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "I2C_EmulationProtocol.hpp"
#include "OpenOCDProtocol.hpp"
#include "SPI_Protocol.hpp"
#include "GmlPrinter.hpp"
#include "PrettyPrinter.hpp"
#include "g3log/g3log.hpp"
//+#include "g3log/logworker.hpp"
//+#include "LogFormatter.h"
//+#include "LoggerSinks.h"
//+#include "CustomFileSink.h"
//+#include "SIT_reader.hpp"
#include "Options.hpp"
#include "Zybo.hpp"

#include <stdexcept>
#include <vector>
#include <tuple>
#include <initializer_list>
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
using std::tuple;
using std::make_tuple;
using std::initializer_list;
using std::string;
using std::experimental::string_view;
using std::regex;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::cout;
using std::cerr;

using namespace mast;
using namespace test;

namespace
{

//! Creates applications and associate them with some register path
//!
vector<ApplicationAssociation> CreateDefaultAppDescriptors (const string& appTopNodePath, uint16_t appCount, string_view registerNamePrefix, uint16_t loopCount )
{
  auto pdlApp = [](uint16_t loopCount, string registerPath, uint16_t initialValue)
  {
    auto manager = Startup::GetManager();
    while (loopCount)
    {
      manager->iWrite(registerPath, initialValue);
      manager->iApply();

      ++initialValue;
      --loopCount;
    }
  };

  vector<ApplicationAssociation> associations;

  auto initialValue = uint16_t(0x1000);
  for (uint16_t ii = 0 ; ii < appCount ; ++ii)
  {
    ostringstream os_app;
    os_app << "App_" << ii;
    auto appName      = os_app.str();

    ostringstream os_reg;
    os_reg << registerNamePrefix << ii;
    auto registerPath = os_reg.str();

    auto appWrapper   = [pdlApp, loopCount, registerPath, initialValue]() { pdlApp(loopCount, registerPath, initialValue); };

    initialValue += 0x1000;

    associations.emplace_back(appWrapper, appTopNodePath, appName);
  }

  return associations;
}
//
//  End of: CreateDefaultAppDescriptors
//---------------------------------------------------------------------------


//! Builds SystemModel with optional AccessInterfaceProtocol for specified testcase.
//! @param protocol         Optional protocol (supersede one that may be defined by the testcase)
//! @param testcase         Tescase kind
//! @param testcaseOptions  Options for test case (depends on tescase kind)
//!
//! @return Application function and their association with a ParentNode within the SystemModel
vector<ApplicationAssociation> CreateTestcase (shared_ptr<AccessInterfaceProtocol> protocol,
                                               Options::Testcase                   testcase,
                                               const string&                       testcaseOptions)
{
  string_view filePath  = testcaseOptions;  // By default, testcaseOptions may contains a file path
  string_view name      = "default";        // This is testcase "sub-name"
  uint32_t    loopCount = 10u;

  // ---------------- Define switches and associated processing to update Options
  //
  using it_t   = vector<string_view>::const_iterator;
  using data_t = tuple<string_view, std::function<void(it_t&, const it_t&, string_view)>>;
  auto parsingData =
  {
    data_t("-f",
           [&filePath](it_t& it, const it_t& ite, string_view)
           {
             if (it != ite) filePath = *it++;
           }),
    data_t("-lc|--loop_count",
           [&loopCount](it_t& it, const it_t& ite, string_view)
           {
             if (it != ite) loopCount = std::stoul((*it++).data());
           }),
    data_t("-n|--name",
           [&name](it_t& it, const it_t& ite, string_view)
           {
             if (it != ite) name = *it++;
           }),
  };

  // ---------------- Parse specific options
  //
  auto parts = Utility::Split(testcaseOptions, " ");
  auto ite   = parts.cend();
  auto it    = parts.cbegin();
  while (it != ite)
  {
    auto item = *it++;
    for (const auto& data : parsingData)
    {
      auto expr        = std::get<0>(data);
      auto switchRegex = regex(expr.cbegin(), expr.cend(), regex::icase);
      if (regex_match(item.cbegin(), item.cend(), switchRegex))
      {
        std::get<1>(data)(it, ite, item);
        break;
      }
    }
  }

  Utility::TrimBoth(filePath);

  auto sm = Startup::GetSystemModel();

  vector<ApplicationAssociation> associations;
  switch (testcase)
  {
    case Options::Testcase::NotSpecified:
      cerr << "A testcase must be specified !!!" ;
      break;
    case Options::Testcase::SIT_File:
    {
      InitializeMast(filePath);

      if (protocol)
      {
        auto ai = dynamic_pointer_cast<AccessInterface>(sm->Root());
        if (ai)
        {
          ai->SetProtocol(protocol);
        }
      }

      if (name == "zybo")
      {
        associations = test::Zybo_CreateApplicationsDescriptor();
      }
      else
      {
        auto topPath   = "W_1500.SWIR.WIR";
        associations   = CreateDefaultAppDescriptors(topPath, 4u, "reg_", loopCount);
      }
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
      auto registerNamePrefix = "dynamic_";
      builder.AppendRegisters(4u, registerNamePrefix, BinaryVector::CreateFromHexString("ABCD"), wrapper);

      sm->SetRoot(accessInterface);

      auto topPath   = "1500.SWIR.WIR";
      associations   = CreateDefaultAppDescriptors(topPath, 4u, registerNamePrefix, loopCount);
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
      aiProtocol = make_shared<SVF_EmulationProtocol> ();
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
    case Options::Protocol::OpenOCD:
    {
      auto configFilePath = protocolOptions;
      aiProtocol = make_shared<OpenOCDProtocol> (configFilePath, "zybo", 11); // @todo passer la longueur IR (int) - NG
      break;
    }
		case Options::Protocol::SPI:
		{
			initializer_list<uint32_t> readCommands		=	{	0x10, 0x01 };
			initializer_list<uint32_t> writeCommands	=	{ 0x10, 0x02 };
			string_view                commandsPrefix = "";
			aiProtocol = make_shared<SPI_Protocol>(readCommands, writeCommands, commandsPrefix);
			break;
		}
    case Options::Protocol::I2C_Emulation:
    {
      initializer_list<uint32_t> addresses      = { 0x00, 0x01, 0x02 };
      string_view                commandsPrefix = "";

      aiProtocol = make_shared<I2C_EmulationProtocol> (addresses, commandsPrefix);
      break;
    }
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

} // End of unnamed namespace



//! Runs a testcase depending on actual arguments
//!
int main (int argc, char* argv [])
{
  auto retCode = ErrorCode::Ok;
  auto session = Session (std::make_shared<SystemModelManagerMonitor>());

  try
  {
    auto options = Options::ParseArguments(argc, argv);
    cout << options.ToDebugString("Retained options: \n", "   ");

    auto protocol       = GetProtocol(options.protocol, options.protocolOptions);
    auto appAssociation = CreateTestcase(protocol, options.testcase, options.testcaseOptions);

    auto runMastOptions = RunMastOptions::LogManagerActivity | RunMastOptions::CheckModel;

    if (options.printGraph)
    {
      PrintModelGraph(options.graphFilePath);
    }

    RunMast(appAssociation, runMastOptions);

    std::cout << "End of test case (see log file in case of errors)" << std::endl;
  }
  catch(std::invalid_argument& exc) { retCode = ErrorCode::InvalidArgument;  std::cout << exc.what(); }
  catch(std::out_of_range&     exc) { retCode = ErrorCode::OutOfRange;       std::cout << exc.what(); }
  catch(std::logic_error&      exc) { retCode = ErrorCode::LogicError;       std::cout << exc.what(); }
  catch(std::regex_error&      exc) { retCode = ErrorCode::RegexException;   std::cout << exc.what(); }
  catch(std::runtime_error&    exc) { retCode = ErrorCode::RuntimeError;     std::cout << exc.what(); }
  catch(std::exception&        exc) { retCode = ErrorCode::StdException;     std::cout << exc.what(); }
  catch(...)                        { retCode = ErrorCode::UndefinedFailure; std::cout << "Got non std::exception"; }

  return static_cast<int>(retCode);
}
//
//  End of: main
//---------------------------------------------------------------------------



//===========================================================================
// End of main.cpp
//===========================================================================



