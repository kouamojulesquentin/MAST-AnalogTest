//===========================================================================
//                           Options.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Options.cpp
//!
//! Implements class Options
//!
//===========================================================================

#include "Options.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <experimental/string_view>
#include <regex>
#include <functional>

using namespace mast;

using namespace std::string_literals;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::cout;
using std::cerr;
using std::regex;
using std::tuple;
using std::make_tuple;


//! Parses main function arguments
//!
//! @note Syntax is:
//!
//! TestCasesApp model [options]
//!   model:           -sf  "sit_file_path"
//!                 |  -tc tescase_name
//!   tescase_name:    1500
//!                 |  1687
//!                 |  Multi_Chain
//!                 |  Daisy_Chain
//!                 |  BSCAN2
//!                 |  Brocade
//!
//! options: [protocol] [loop_count] [print_graph]
//! protocol:         -p loopback | SVF_Simu[lation] | SVF_Emu[lation]
//! loop_count:       -lc value
//! print_graph:      -pg
//!
//! Examples:
//!   TestCasesApp  -sf "c:\models\1500_wrapper.sit" -p SVF_Simu -lc 10
//!   TestCasesApp  -tc 1500 -p SVF_Emulation -lc 100 -pg
//!
//! Default loop count is: 10
//! Default protocol is loopback
//! All options are case insensitive except for file paths on Linux
//!
Options Options::ParseArguments (int argc, char* argv [])
{
  Options options;

  auto printSyntax = false;
  auto getNextItem = [argc, argv, &printSyntax](int& index, string_view context)
  {
    ++index;
    if (index >= argc)
    {
      cerr << "Missing argument after: \"" << context << "\"" << std::endl;
      printSyntax = true;
      return ""s;
    }
    return string(argv[index]);
  };

  // ---------------- Define switches and associated processing to update Options
  //
  using data_t = tuple<string_view, string_view, std::function<void(int& ii, string& item)>>;
  auto parsingData =
  {
    data_t("SIT file", "-sf|--sit_file",
           [&options, &getNextItem](int& ii, string& item)
           {
             options.testcase = Testcase::SIT_File;
             options.sitFile  = getNextItem(ii, item);
           }),
    data_t("Test case", "-tc|--test_case",
           [&options, &getNextItem](int& ii, string& item)
           {
             options.testcase = ParseTestcase(getNextItem(ii, item));
           }),
    data_t("Protocol", "-p|--protocol",
           [&options, &getNextItem](int& ii, string& item)
           {
             options.protocol = ParseProtocol(getNextItem(ii, item));
           }),
    data_t("Loop count", "-lc|--loop_count",
           [&options, &getNextItem](int& ii, string& item)
           {
             options.loopCount = stoul(getNextItem(ii, item));
           }),
    data_t("Print graph", "-pg|--print_graph",
           [&options, &argv](int&, string&)
           {
             options.printGraph    = true;
             options.graphFilePath = ReplaceFilePathExtension(argv[0], ".gml");
           }),
    data_t("Print graph file", "-pgf|--print_graph_file",
           [&options, &getNextItem](int& ii, string& item)
           {
             options.printGraph    = true;
             options.graphFilePath = getNextItem(ii, item);
           }),
    data_t("Help", "-h|--help|-\\?",
           [&printSyntax](int&, string&)
           {
             printSyntax = true;
           }),
  };


  if (argc == 1)
  {
    printSyntax = true;
  }
  else
  {
    for (int ii = 1 ; ii < argc ; ++ii)
    {
      auto item = string(argv[ii]);

      auto parsed = false;
      for (const auto& data : parsingData)
      {
        auto switchRegex = regex("^"s + string(std::get<1>(data)) + "$"s, regex::icase);
        if (regex_search(item, switchRegex))
        {
          std::get<2>(data)(ii, item);
          parsed = true;
          break;
        }
      }

      if (!parsed)
      {
        cerr << "Unsuported argument: " << item << std::endl;
        printSyntax = true;
      }
    }
  }

  if (printSyntax)
  {
    cout << std::endl << "Syntax is: " << argv[0] << " <Switches>"<< std::endl;

    cout << "   Switches: " << std::endl;

    for (const auto& data : parsingData)
    {
      auto switchRegex = std::get<1>(data);
      auto description = string(std::get<0>(data)) + ": ";
      description.resize(20);
      cout << "      " << description << switchRegex << std::endl;
    }
    cout << std::endl;
  }
  return options;
}
//
//  End of: Options::ParseArguments
//---------------------------------------------------------------------------



//! Parses protocol option provided by user to a Protocol enum
//!
Options::Protocol Options::ParseProtocol (string_view protocolOption)
{
  Protocol protocol = Protocol::LoopBack;

  return protocol;
}
//
//  End of: Options::ParseProtocol
//---------------------------------------------------------------------------


//! Parses test case option provided by user to a Testcase enum
//!
Options::Testcase Options::ParseTestcase (string_view testcaseOption)
{
  Testcase testcase = Testcase::Wrapper_1500;

  return testcase;
}
//
//  End of: Options::ParseTestcase
//---------------------------------------------------------------------------


//! Provides a string
//!
string Options::ToDebugString (string_view header, string_view linePrefix) const
{
  ostringstream os;

  os << header;
  os << linePrefix << "Print graph: " << std::boolalpha << printGraph << std::endl;
  os << linePrefix << "Graph file:  " << graphFilePath  << std::endl;
  os << linePrefix << "Loop count:  " << loopCount      << std::endl;

  os << linePrefix << "Test case:   ";
  switch (testcase)
  {
    case Testcase::SIT_File:
      os << "SIT File \"" << sitFile << "\"";
      break;
    case Testcase::Wrapper_1500:
      os << "1500 wrapper";
      break;
    default:
      os << "???";
      break;
  }
  os << std::endl;

  os << linePrefix << "Protocol:    ";
  switch (protocol)
  {
    case Protocol::LoopBack:
      os << "LoopBack";
      break;
    default:
      os << "???";
      break;
  }
  os << std::endl;

  auto asString = os.str();
  return asString;
}
//
//  End of: Options::ToDebugString
//---------------------------------------------------------------------------


//! Replaces file path extension with replacement or removes it when the default value of replacement is used.
//! Firstly, if this path has an extension, it is removed.
//! Then, a dot character is appended if replacement is not empty or does not begin with a dot character.
//! Then replacement is appended to the path.
//!
//! @param filePath      Original file path (with or without an extension)
//! @param replacement   The extension to replace with (may be empty)
//!
string Options::ReplaceFilePathExtension (string filePath, const string& replacement)
{
  constexpr auto dot = '.';

  auto dotPos = filePath.rfind(dot);

  if (dotPos != string::npos)
  {
    filePath.resize(dotPos);
  }

  if (replacement.front() != dot)
  {
    filePath.push_back(dot);
  }

  filePath += replacement;

  return filePath;
}
//
//  End of: Options::ReplaceFilePathExtension
//---------------------------------------------------------------------------



//===========================================================================
// End of Options.cpp
//===========================================================================
