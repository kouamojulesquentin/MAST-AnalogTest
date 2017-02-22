//===========================================================================
//                           UT_Remote_Protocol_Proxy.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Remote_Protocol_Proxy.cpp
//!
//! Implements test fixture for testing Remote_Protocol_Proxy
//!
//===========================================================================


#include "UT_Remote_Protocol_Proxy.hpp"
#include "Remote_Protocol_Proxy.hpp"
#include "Remote_Protocol_Client.hpp"

#include <memory>
#include <cxxtest/ValueTraits.h>

using mast::Remote_Protocol_Proxy;
using mast::Remote_Protocol_Client;

using std::unique_ptr;
using std::make_unique;
using std::pair;
using std::make_pair;
using std::string;
using std::vector;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

namespace
{
  //! Spies call to a Remote_Protocol_Client
  //!
  class ClientProtocol : public Remote_Protocol_Client
  {
    public:
    using CapturedCall_t = vector<pair<string, vector<unsigned char>>>;
    CapturedCall_t  capturedCall; //!< This saved parameters used when calling SendScanVector

    ~ClientProtocol() {}

    ClientProtocol()
      : Remote_Protocol_Client("http://localhost:8080/Test")
    {}

    ClientProtocol(const std::string& parameters)
      : Remote_Protocol_Client(parameters)
    {
    }

    virtual string KindName() const { return "Remote_Spy"; };


    //! Sends scan vector to System Under Test
    //!
    //! @param commandName  Command name (SIR, SDR, RST...)
    //! @param scanVector   Binary data to send to SUT (default is right aligned)
    //!
    //! @return Error code (0 means no error)
    virtual int SendScanVector(const std::string& commandName, const std::vector<unsigned char>& scanVector)
    {
      capturedCall.emplace_back(make_pair(commandName, scanVector));
      return 0;
    }
  };
} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_Remote_Protocol_Proxy::setUp ()
{
}


//! Cleanups test (called for each test)
void UT_Remote_Protocol_Proxy::tearDown ()
{
}


//! Checks Remote_Protocol_Proxy constructor giving it an ClientProtocol
//!
void UT_Remote_Protocol_Proxy::test_Constructor_RemoteClient ()
{
  // ---------------- Setup
  //
  auto clientProtocol = make_unique<ClientProtocol>();
  auto commands       = vector<string>{"RST"s, "SIR"s, "SDR"s};

  unique_ptr<Remote_Protocol_Proxy> proxy;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (proxy = make_unique<Remote_Protocol_Proxy>(std::move(clientProtocol), std::move(commands)));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (proxy->MaxSupportedDerivations(), 3u);
  TS_ASSERT_EQUALS (proxy->KindName(),                "Remote_Spy"sv);
}



//===========================================================================
// End of UT_Remote_Protocol_Proxy.cpp
//===========================================================================
