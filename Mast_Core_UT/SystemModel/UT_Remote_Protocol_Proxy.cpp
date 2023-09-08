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
#include "Cpp_11_Traits.hpp"
#include <cxxtest/ValueTraits.h>


using std::unique_ptr;
using std::make_unique;
using std::make_unique;
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

    static Spy_ClientProtocol* pLastCreated;  //!< To track Spy_ClientProtocol created by factory

    using CapturedCall_t = vector<tuple<string, uint32_t, vector<unsigned char>>>;

    CapturedCall_t capturedCall;          //!< This saved parameters used when calling SendScanVector
    uint32_t       asyncReset = 0;        //!< Tally request for asynchronous reset
    uint32_t       syncReset  = 0;        //!< Tally request for synchronous reset
    string         constructorParameters; //!< Parameters given to constructor taking a string

    ~Spy_ClientProtocol()
    {
      pLastCreated = nullptr; // It can no more been used
    }

    Spy_ClientProtocol()
      : Remote_Protocol_Client("http://localhost:8080/Test")
    {
      pLastCreated = this;
    }

    Spy_ClientProtocol(const std::string& parameters)
      : Remote_Protocol_Client(parameters)
    {
      pLastCreated          = this;
      constructorParameters = parameters;
    }

    virtual string KindName() const { return "Remote_Spy"; };

    using ScanVector_t = Remote_Protocol_Client::ScanVector_t;

    //! Sends scan vector to System Under Test
    //!
    //! @param commandName  Command name (SIR, SDR, RST...)
    //! @param scanVector   Binary data to send to SUT (default is right aligned)
    //!
    //! @return Error code (0 means no error)
    virtual SendScanVectorReturn_t SendScanVector(const string&       commandName,
                                                  uint32_t            bitsCount,
                                                  const ScanVector_t& scanVector)
    {
      capturedCall.emplace_back(make_tuple(commandName, bitsCount, scanVector));
      return make_pair(bitsCount, scanVector);
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

  Spy_ClientProtocol* Spy_ClientProtocol::pLastCreated = nullptr; // Initialize the static variable

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
  TS_ASSERT_EQUALS (proxy->MaxSupportedChannels(), 3u);
  TS_ASSERT_EQUALS (proxy->KindName(),                "Remote_Spy"sv);
  TS_ASSERT_EQUALS (pSpy->syncReset,                  0);
  TS_ASSERT_EQUALS (pSpy->asyncReset,                 0);
  TS_ASSERT_EQUALS (pSpy->capturedCall.size(),        0);
}


//! Checks Remote_Protocol_Proxy constructor giving it some (well formated) string parameters
//!
void UT_Remote_Protocol_Proxy::test_Constructor_StringParameters_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto parameters        = string(std::get<0>(data));
    const auto expectedKind      = std::get<1>(data);
    const auto expectedSpyParams = std::get<2>(data);

    CxxTest::setAbortTestOnFail(true);
    auto& factory = RemoteProtocolFactory::Instance();

    TS_ASSERT_THROWS_NOTHING(factory.RegisterCreator("SpyProtocol", [](const string& parameters) { return make_unique<Spy_ClientProtocol>(parameters); }));

    unique_ptr<Remote_Protocol_Proxy> proxy;
    Spy_ClientProtocol::pLastCreated = nullptr;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (proxy = make_unique<Remote_Protocol_Proxy>(parameters));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (proxy);
    TS_ASSERT_EQUALS      (proxy->MaxSupportedChannels(), 3u);
    TS_ASSERT_EQUALS      (proxy->KindName(),                expectedKind);

    // Check a Client Protocol has been created and use as appropriate
    TS_ASSERT_NOT_NULLPTR (Spy_ClientProtocol::pLastCreated);
    TS_ASSERT_EQUALS      (Spy_ClientProtocol::pLastCreated->constructorParameters, expectedSpyParams);
    proxy->DoReset(true);
    TS_ASSERT_EQUALS      (Spy_ClientProtocol::pLastCreated->syncReset, 1u);

  };

  auto data =
  {          // Parameters,                                                            Expected kind,  Expected Spy parameters
    make_tuple("RST, SIR, SDR, |ID:SpyProtocol|"sv,                                    "Remote_Spy"sv, ""sv),                  // 00
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy|, |ID:SpyProtocol|"sv,                    "Foo_Spy"sv,    ""sv),                  // 01
    make_tuple("RST, SIR, SDR, |ID:SpyProtocol|, 192.168.1.45:8080"sv,                 "Remote_Spy"sv, "192.168.1.45:8080"sv), // 02
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy|, |ID:SpyProtocol|, 192.168.1.45:8080"sv, "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 03
    make_tuple("RST, SIR, SDR  |KIND:Foo_Spy|, |ID:SpyProtocol|"sv,                    "Foo_Spy"sv,    ""sv),                  // 04
    make_tuple("RST, SIR, SDR, |ID:SpyProtocol| 192.168.1.45:8080"sv,                  "Remote_Spy"sv, "192.168.1.45:8080"sv), // 05
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy|, |ID:SpyProtocol| 192.168.1.45:8080"sv,  "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 06
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy| |ID:SpyProtocol| 192.168.1.45:8080"sv,   "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 07
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy||ID:SpyProtocol| 192.168.1.45:8080"sv,    "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 08
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy| |ID:SpyProtocol|192.168.1.45:8080"sv,    "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 09
    make_tuple("RST, SIR, SDR, |KIND:Foo_Spy| |ID:SpyProtocol|192.168.1.45:8080   "sv, "Foo_Spy"sv,    "192.168.1.45:8080"sv), // 10
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Remote_Protocol_Proxy constructor giving it some badly formated parameters
//!
void UT_Remote_Protocol_Proxy::test_Constructor_StringParameters_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string parameters(data);
    auto& factory = RemoteProtocolFactory::Instance();

    factory.RegisterCreator("SpyProtocol", [](const string& parameters) { return make_unique<Spy_ClientProtocol>(parameters); });

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (Remote_Protocol_Proxy proxy(parameters), std::exception);
  };

  auto data =
  {
    "RST, ID:SpyProtocol|"sv,                                              // 0 - not enough commands
    "|KIND:Foo_Spy|, |ID:SpyProtocol|"sv,                                  // 1 - no command at all
    "RST, SIR, SDR, 192.168.1.45:8080"sv,                                  // 2 - no creator id
    "RST, SIR, SDR  |KIND:Foo_Spy| |ID:SpyProtocol"sv,                     // 4 - missing |
    "RST, SIR, SDR, |SpyProtocol| 192.168.1.45:8080"sv,                    // 5 - missing ID:
    "RST, SIR, SDR, |KIND:Foo_Spy|, |ID:Spy| 192.168.1.45:8080"sv,         // 6 - unregistered creator id
    "RST, SIR, SDR, |KIND:Foo_Spy|, |ID:| 192.168.1.45:8080"sv,            // 7 - empty creator id
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
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



//! Checks Remote_Protocol_Proxy::DoCallback()
//!
void UT_Remote_Protocol_Proxy::test_DoCallback ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto         channelId    = std::get<0>(data);
    const auto&  toSutData       = BinaryVector::CreateFromString(std::get<1>(data));
    auto         expectedCommand = std::get<2>(data);
    const auto&  expectedBinary  = std::get<3>(data);
    BinaryVector fromSutData;
    auto         clientProtocol  = make_unique<Spy_ClientProtocol>();
    auto         pSpy            = clientProtocol.get(); // This is to retrieve spied data
    auto         commands        = vector<string>{"RST"s, "SIR"s, "SDR"s};
    auto Primitive     = "Primitive"s;

    Remote_Protocol_Proxy sut(std::move(clientProtocol), std::move(commands));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (fromSutData = sut.DoCallback(Primitive,channelId, nullptr, toSutData));

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


//! Checks Remote_Protocol_Proxy::DoCallback() with invalid channel id
//!
void UT_Remote_Protocol_Proxy::test_DoCallback_invalid_ChannelId ()
{
  // ---------------- Setup
  //
  Remote_Protocol_Proxy sut(make_unique<Spy_ClientProtocol>(), vector<string>{"RST"s, "SIR"s, "SDR"s});

  const auto toSutData = BinaryVector::CreateFromString("/x01234FACE");
  auto Primitive     = "Primitive"s;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DoCallback(Primitive,3, nullptr, toSutData), std::exception);
}


//! Checks Remote_Protocol_Proxy::DoCallback() with invalid (not nullptr) interface data
//!
//! @note Remote protocols cannot use interfaceData pointer has there is not sharing of address space
//!
void UT_Remote_Protocol_Proxy::test_DoCallback_invalid_InterfaceData ()
{
  // ---------------- Setup
  //
  Remote_Protocol_Proxy sut(make_unique<Spy_ClientProtocol>(), vector<string>{"RST"s, "SIR"s, "SDR"s});

  const auto toSutData     = BinaryVector::CreateFromString("/x01234FACE");
  auto       myData        = { 0x01, 0x02 };      // There is no meaning in those data
  auto       interfaceData = reinterpret_cast<void*>(&myData);
  auto Primitive     = "Primitive"s;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DoCallback(Primitive,2, interfaceData, toSutData), std::exception);
}





//===========================================================================
// End of UT_Remote_Protocol_Proxy.cpp
//===========================================================================
