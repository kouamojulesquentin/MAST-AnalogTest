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
#include "RemoteProtocolFactory.hpp"

#include <memory>
#include <tuple>
#include <cxxtest/ValueTraits.h>


using std::unique_ptr;
using std::make_unique;
using std::make_shared;
using std::pair;
using std::make_pair;
using std::tuple;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using std::vector;

using mast::RemoteProtocolFactory;
using mast::Remote_Protocol_Proxy;
using mast::Remote_Protocol_Client;
using mast::BinaryVector;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

namespace
{
  //! Spies call to a Remote_Protocol_Client
  //!
  class Spy_ClientProtocol : public Remote_Protocol_Client
  {
    public:
    using          CapturedCall_t = vector<tuple<string, uint32_t, vector<unsigned char>>>;

    CapturedCall_t capturedCall;       //!< This saved parameters used when calling SendScanVector
    uint32_t       asyncReset     = 0; //!< Tally request for asynchronous reset
    uint32_t       syncReset      = 0; //!< Tally request for synchronous reset

    ~Spy_ClientProtocol() {}

    Spy_ClientProtocol()
      : Remote_Protocol_Client("http://localhost:8080/Test")
    {}

    Spy_ClientProtocol(const std::string& parameters)
      : Remote_Protocol_Client(parameters)
    {
    }

    virtual string KindName() const { return "Remote_Spy"; };

    using ScanVector_t = Remote_Protocol_Client::ScanVector_t;

    //! Sends scan vector to System Under Test
    //!
    //! @param commandName  Command name (SIR, SDR, RST...)
    //! @param scanVector   Binary data to send to SUT (default is right aligned)
    //!
    //! @return Error code (0 means no error)
    virtual ScanVector_t SendScanVector(const std::string& commandName, uint32_t bitsCount, const ScanVector_t& scanVector)
    {
      capturedCall.emplace_back(make_tuple(commandName, bitsCount, scanVector));
      return scanVector;
    }

    virtual void SendDoReset(bool doSynchronousReset)
    {
      if (doSynchronousReset)
      {
        ++syncReset;
      }
      else
      {
        ++asyncReset;
      }
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
  auto clientProtocol  = make_unique<Spy_ClientProtocol>();
  auto pSpy            = clientProtocol.get(); // This is to retrieve spied data
  auto commands        = vector<string>{"RST"s, "SIR"s, "SDR"s};

  unique_ptr<Remote_Protocol_Proxy> proxy;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (proxy = make_unique<Remote_Protocol_Proxy>(std::move(clientProtocol), std::move(commands)));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (proxy->MaxSupportedDerivations(), 3u);
  TS_ASSERT_EQUALS (proxy->KindName(),                "Remote_Spy"sv);
  TS_ASSERT_EQUALS (pSpy->syncReset,                  0);
  TS_ASSERT_EQUALS (pSpy->asyncReset,                 0);
  TS_ASSERT_EQUALS (pSpy->capturedCall.size(),        0);
}


//! Checks Remote_Protocol_Proxy constructor giving it some string parameters
//!
void UT_Remote_Protocol_Proxy::test_Constructor_StringParameters ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);
  auto& factory = RemoteProtocolFactory::Instance();

  TS_ASSERT_THROWS_NOTHING(factory.RegisterCreator("SpyProtocol", [](const string& /* parameters */) { return make_shared<Spy_ClientProtocol>(); }));

  unique_ptr<Remote_Protocol_Proxy> proxy;
  string parameters("SpyProtocol, RST, SIR, SDR");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (proxy = make_unique<Remote_Protocol_Proxy>(parameters));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (proxy);
  TS_ASSERT_EQUALS (proxy->MaxSupportedDerivations(), 3u);
  TS_ASSERT_EQUALS (proxy->KindName(),                "Remote_Spy"sv);
}



//! Checks Remote_Protocol_Proxy::DoReset requesting for a synchronous reset
//!
void UT_Remote_Protocol_Proxy::test_DoReset_Sync ()
{
  // ---------------- Setup
  //
  auto clientProtocol  = make_unique<Spy_ClientProtocol>();
  auto pSpy            = clientProtocol.get(); // This is to retrieve spied data
  auto commands        = vector<string>{"RST"s, "SIR"s, "SDR"s};

  Remote_Protocol_Proxy sut(std::move(clientProtocol), std::move(commands));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(true));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (pSpy->syncReset,           1u);
  TS_ASSERT_EQUALS (pSpy->asyncReset,          0);
  TS_ASSERT_EQUALS (pSpy->capturedCall.size(), 0);
}


//! Checks Remote_Protocol_Proxy::DoReset requesting for an asynchronous reset
//!
void UT_Remote_Protocol_Proxy::test_DoReset_ASync ()
{
  // ---------------- Setup
  //
  auto clientProtocol  = make_unique<Spy_ClientProtocol>();
  auto pSpy            = clientProtocol.get(); // This is to retrieve spied data
  auto commands        = vector<string>{"RST"s, "SIR"s, "SDR"s};

  Remote_Protocol_Proxy sut(std::move(clientProtocol), std::move(commands));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoReset(false));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (pSpy->syncReset,           0);
  TS_ASSERT_EQUALS (pSpy->asyncReset,          1u);
  TS_ASSERT_EQUALS (pSpy->capturedCall.size(), 0);
}



//! Checks Remote_Protocol_Proxy::DoAction()
//!
void UT_Remote_Protocol_Proxy::test_DoAction ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto         derivationId    = std::get<0>(data);
    const auto&  toSutData       = BinaryVector::CreateFromString(std::get<1>(data));
    auto         expectedCommand = std::get<2>(data);
    const auto&  expectedBinary  = std::get<3>(data);
    BinaryVector fromSutData;
    auto         clientProtocol  = make_unique<Spy_ClientProtocol>();
    auto         pSpy            = clientProtocol.get(); // This is to retrieve spied data
    auto         commands        = vector<string>{"RST"s, "SIR"s, "SDR"s};

    Remote_Protocol_Proxy sut(std::move(clientProtocol), std::move(commands));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (fromSutData = sut.DoAction(derivationId, nullptr, toSutData));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (fromSutData, toSutData);

    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (pSpy->capturedCall.size(),    1u);

    const auto& captured     = pSpy->capturedCall[0];
    const auto& gotCommand   = std::get<0>(captured);
    const auto  gotBitsCount = std::get<1>(captured);
    const auto& gotBinary    = std::get<2>(captured);

    TS_ASSERT_EQUALS (gotCommand,   expectedCommand);
    TS_ASSERT_EQUALS (gotBitsCount, toSutData.BitsCount());
    TS_ASSERT_EQUALS (gotBinary,    expectedBinary);
  };

  using data_t = tuple<uint32_t, string_view, string_view, vector<unsigned char>>;
  auto data =
  {
    data_t(0u, ""sv,               "RST"sv, { }),
    data_t(1u, "/xA5/b1011"sv,     "SIR"sv, { 0x0A, 0x5B  }),
    data_t(2u, "/x80FF5A/b1001"sv, "SDR"sv, { 0x08, 0x0F, 0xF5, 0xA9 }),
  };

  CxxTest::setDisplayUnsignedAsHex(true);

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Remote_Protocol_Proxy::DoAction() with invalid derivation id
//!
void UT_Remote_Protocol_Proxy::test_DoAction_invalid_DerivationId ()
{
  // ---------------- Setup
  //
  Remote_Protocol_Proxy sut(make_unique<Spy_ClientProtocol>(), vector<string>{"RST"s, "SIR"s, "SDR"s});

  const auto toSutData = BinaryVector::CreateFromString("/x01234FACE");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DoAction(3, nullptr, toSutData), std::exception);
}


//! Checks Remote_Protocol_Proxy::DoAction() with invalid (not nullptr) interface data
//!
//! @note Remote protocols cannot use interfaceData pointer has there is not sharing of address space
//!
void UT_Remote_Protocol_Proxy::test_DoAction_invalid_InterfaceData ()
{
  // ---------------- Setup
  //
  Remote_Protocol_Proxy sut(make_unique<Spy_ClientProtocol>(), vector<string>{"RST"s, "SIR"s, "SDR"s});

  const auto toSutData     = BinaryVector::CreateFromString("/x01234FACE");
  auto       myData        = { 0x01, 0x02 };      // There is no meaning in those data
  auto       interfaceData = reinterpret_cast<void*>(&myData);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DoAction(2, interfaceData, toSutData), std::exception);
}





//===========================================================================
// End of UT_Remote_Protocol_Proxy.cpp
//===========================================================================
