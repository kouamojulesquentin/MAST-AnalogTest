//===========================================================================
//                           UT_TClap.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_TClap.cpp
//!
//! Implements test fixture for testing tclap
//!
//===========================================================================

#include "UT_TClap.hpp"

#include <tclap/CmdLine.h>
#include <tclap/StreamOutput.h>
#include <cxxtest/ValueTraits.h>
#include <vector>
#include <sstream>
#include <string>
#include <experimental/string_view>

using std::ostringstream;
using std::string;
using std::vector;
using namespace std::string_literals;

#define TS_ASSERT_STR_CONTAINS(text, sub_text)                                                              \
    if ((text).find((sub_text)) == std::string::npos)                                                       \
    {                                                                                                       \
      TS_FAIL(string(#text).append(": ").append((text)).append(", does not contain: ").append((sub_text)));\
    }

#define TS_ASSERT_STR_EMPTY(text)                                           \
    if (!(text).empty())                                                    \
    {                                                                       \
      TS_FAIL(string(#text).append(" is not empty. Got: ").append((text))); \
    }


//! Checks ValueArg constructor with minimal parameters
//!
void UT_TClap::test_ValueArg_Constructor_Minimal ()
{
  // ---------------- Exercise
  //
  TCLAP::ValueArg<std::string> sut("c", "conf", "Define configuration file", false, "mast.cfg", "File path");

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.shortID(),  "[-c <File path>]");
  TS_ASSERT_EQUALS (sut.longID(),   "-c <File path>,  --conf <File path>");
  TS_ASSERT_EQUALS (sut.getValue(), "mast.cfg");
}


//! Checks ValueArg constructor with bad specification
//!
void UT_TClap::test_ValueArg_Constructor_BadSpecification ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (TCLAP::ValueArg<std::string>("-c", "--conf", "Define configuration file", false, "mast.cfg", "File path"),
                    TCLAP::SpecificationException);  // Must not have dashes before "c" and "conf" !
}

//! Checks CmdLine constructor with minimal parameters
//!
void UT_TClap::test_CmdLine_Constructor_Minimal ()
{
  // ---------------- Setup
  //
  auto message = "Test of Cmdline"s;

  // ---------------- Exercise
  //
  TCLAP::CmdLine sut(message);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.getMessage(),     message);
  TS_ASSERT_EQUALS (sut.getVersion(),     "none");
  TS_ASSERT_EQUALS (sut.getProgramName(), "not_set_yet");
  TS_ASSERT_EQUALS (sut.getDelimiter(),   ' ');
  TS_ASSERT_TRUE   (sut.hasHelpAndVersion());

  auto argList = sut.getArgList();
  TS_ASSERT_EQUALS (argList.size(), 3u);  // For -- -v/--version -h/--help

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (sut.getOutput());
}


//! Checks CmdLine constructor with all parameters
//!
void UT_TClap::test_CmdLine_Constructor_all_Params ()
{
  // ---------------- Setup
  //
  auto message        = "Test of Cmdline"s;
  auto version        = "1.0.9"s;
  auto delimiter      = '=';
  auto helpAndVersion = false;

  // ---------------- Exercise
  //
  TCLAP::CmdLine sut(message, delimiter, version, helpAndVersion);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.getMessage(),     message);
  TS_ASSERT_EQUALS (sut.getVersion(),     version);
  TS_ASSERT_EQUALS (sut.getProgramName(), "not_set_yet");
  TS_ASSERT_EQUALS (sut.getDelimiter(),   delimiter);
  TS_ASSERT_FALSE  (sut.hasHelpAndVersion());
}


//! Checks CmdLine::setOutput()
//!
void UT_TClap::test_CmdLine_setOutput ()
{
  // ---------------- Setup
  //
  ostringstream stream;
  TCLAP::StreamOutput streamOutput(stream, stream);
  TCLAP::CmdLine sut("tist_CmdLine_setOutput");

  // ---------------- Exercise
  //
  sut.setOutput(&streamOutput);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  auto pOutput = sut.getOutput();

  TS_ASSERT_NOT_NULLPTR (pOutput);
  TS_ASSERT_EQUALS_PTR  (pOutput, &streamOutput);
}


//! Checks CmdLine::parse() without any parameters (not even program name)
//!
void UT_TClap::test_CmdLine_parse_no_Parameters ()
{
  // ---------------- Setup
  //
  vector<string>      parameters;
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("tist_CmdLine_parse_no_Parameters");

  sut.setOutput(&streamOutput);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sut.parse(parameters), TCLAP::CmdLineParseException);
}


//! Checks CmdLine::parse() with only program name
//!
void UT_TClap::test_CmdLine_parse_only_ExeName ()
{
  // ---------------- Setup
  //
  vector<string>      parameters{ "MyApp" };
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("test_CmdLine_parse_only_ExeName");

  sut.setOutput(&streamOutput);

  // ---------------- Exercise
  //
  sut.parse(parameters);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.shouldExit());

  TS_ASSERT_EQUALS (stdStream.str(), "");
  TS_ASSERT_EQUALS (errStream.str(), "");
}


//! Checks CmdLine::parse() with help request
//!
void UT_TClap::test_CmdLine_parse_Help ()
{
  // ---------------- Setup
  //
  vector<string>      parameters{ "MyApp", "--help" };
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("test_CmdLine_parse_Help");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.shouldExit());

  TS_ASSERT_STR_CONTAINS (stdStream.str(), "USAGE: ");
  TS_ASSERT_EQUALS       (errStream.str(), "");
}


//! Checks CmdLine::parse() with version request
//!
void UT_TClap::test_CmdLine_parse_Version ()
{
  // ---------------- Setup
  //
  vector<string>      parameters {"MySuperApp", "--version"};
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("test_CmdLine_parse_Version", '=', "0.9.x");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.shouldExit());

  TS_ASSERT_STR_CONTAINS(stdStream.str(), "MySuperApp  version: 0.9.x");
  TS_ASSERT_EQUALS (errStream.str(), "");
}


//! Checks CmdLine::parse() with unexpected switch
//!
void UT_TClap::test_CmdLine_parse_UnregisteredSwitch ()
{
  // ---------------- Setup
  //
  vector<string>      parameters {"MyApp", "--foo"};
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("", '=', "0.9.x");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.shouldExit());

  auto stdMessage = stdStream.str();
  auto errMessage = errStream.str();

  TS_ASSERT_STR_CONTAINS(stdMessage, "MyApp");
  TS_ASSERT_STR_CONTAINS(stdMessage, "[--version]");
  TS_ASSERT_STR_CONTAINS(errMessage, "PARSE ERROR: Argument: --foo");
}


//! Checks CmdLine::parse() with unexpected parameter
//!
void UT_TClap::test_CmdLine_parse_UnregisteredParam ()
{
  // ---------------- Setup
  //
  vector<string>      parameters {"MyApp", "foo"};
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("", '=', "0.9.x");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.shouldExit());

  auto stdMessage = stdStream.str();
  auto errMessage = errStream.str();

  TS_ASSERT_STR_CONTAINS(stdMessage, "MyApp");
  TS_ASSERT_STR_CONTAINS(stdMessage, "[--version]");
  TS_ASSERT_STR_CONTAINS(errMessage, "PARSE ERROR: Argument: foo");
}


//! Checks CmdLine::parse() with all required parameter
//!
void UT_TClap::test_CmdLine_parse_all_Parameters ()
{
  // ---------------- Setup
  //
  vector<string>      parameters {"MyApp", "--foo=5"};
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("", '=', "0.9.x");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  TCLAP::ValueArg<int>  fooArg("f", "foo", "Description for foo", false, 12, "foo count", sut);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.shouldExit());
  TS_ASSERT_STR_EMPTY(stdStream.str());
  TS_ASSERT_STR_EMPTY(errStream.str());

  TS_ASSERT_EQUALS (fooArg.getValue(), 5);
}


//! Checks CmdLine::parse() with a missing required parameter
//!
void UT_TClap::test_CmdLine_parse_missing_Required ()
{
  // ---------------- Setup
  //
  vector<string>      parameters {"MyApp", "--foo= 7"};
  ostringstream       stdStream;
  ostringstream       errStream;
  TCLAP::StreamOutput streamOutput(stdStream, errStream);
  TCLAP::CmdLine      sut("", '=', "0.9.x");
  sut.setOutput(&streamOutput);
  sut.automaticExit(false); // Prevent exit of unit tests

  TCLAP::ValueArg<int>   fooArg("f", "foo", "Description for foo", false, 12,  "foo count", sut);
  TCLAP::ValueArg<float> barArg("b", "bar", "Description for bar", true,  1.2, "bar level", sut);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.parse(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.shouldExit());

  auto stdMessage = stdStream.str();
  auto errMessage = errStream.str();

  TS_ASSERT_STR_CONTAINS (stdMessage, "MyApp  -b=<bar level>");
  TS_ASSERT_STR_CONTAINS (errMessage, "Required argument missing: bar");

  TS_ASSERT_EQUALS (fooArg.getValue(), 7);
}


//===========================================================================
// End of UT_TClap.cpp
//===========================================================================
