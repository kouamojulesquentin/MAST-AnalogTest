//===========================================================================
//                           UT_VirtualRegister.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_VirtualRegister.cpp
//!
//! Implements test fixture for testing VirtualRegister
//!
//===========================================================================

#include "UT_VirtualRegister.hpp"
#include "VirtualRegister.hpp"
#include "BinaryVector.hpp"
#include "IndexedRange.hpp"

#include "Mast_Core_Traits.hpp"

#include <experimental/string_view>
#include <tuple>
#include <vector>
#include <memory>

using std::tuple;
using std::make_tuple;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::experimental::string_view;
using namespace std::experimental::literals::string_view_literals;

using namespace mast;

//! Initializes test (called for each test)
void UT_VirtualRegister::setUp ()
{
  SystemModelNode::ResetNodeIdentifier(); // To start each test with node identifier equal 0
  CxxTest::setDisplayUnsignedAsHex(true);
}

//! Checks VirtualRegister default constructor
//!
void UT_VirtualRegister::test_Constructor_Default ()
{
  // ---------------- Setup
  //
  BinaryVector emptyVector;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING
  (
    VirtualRegister sut;

    TS_ASSERT_EQUALS (sut.BitsCount(),    0u);
    TS_ASSERT_EQUALS (sut.BitsOrdering(), BitsOrdering::Undefined);
    TS_ASSERT_EQUALS (sut.TypeName(),     "VirtualRegister");
    TS_ASSERT_EQUALS (sut.Identifiers().size(),  0u);
  );
}


//! Checks VirtualRegister constructor from single Register
//!
void UT_VirtualRegister::test_Constructor_FromSingleRegister ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue      = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering  = std::get<1>(data);
    auto expectedWidth = std::get<2>(data);
    auto holdValue     = false;

    auto reg = make_shared<Register>("reg", regValue, holdValue, bitsOrdering);

    // ---------------- Exercise
    //
    VirtualRegister sut(reg);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (sut.BitsOrdering(), bitsOrdering);
    TS_ASSERT_EQUALS (sut.BitsCount(),    expectedWidth);
    TS_ASSERT_EQUALS (sut.Identifiers().size(),  1u);
    TS_ASSERT_EQUALS (sut.Identifiers().front(), 0u);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t>;
  auto data = // Register value, expected Width
  {
    data_t{"/xDEAF",        BitsOrdering::Downto, 16}, // 0
    data_t{"/xDEAF",        BitsOrdering::Upto,   16}, // 1
    data_t{"/x1234",        BitsOrdering::Downto, 16}, // 2
    data_t{"/b0",           BitsOrdering::Downto, 1},  // 3
    data_t{"/b1",           BitsOrdering::Upto,   1},  // 4
    data_t{"/b1_0001",      BitsOrdering::Downto, 5},  // 5
    data_t{"/b1_0111_0001", BitsOrdering::Upto,   9},  // 6
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks VirtualRegister::Append() when providing first and valid RegisterSlice
//!
void UT_VirtualRegister::test_Append_First ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue      = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering  = std::get<1>(data);
    auto leftIndex     = std::get<2>(data);
    auto rightIndex    = std::get<3>(data);
    auto expectedWidth = std::get<4>(data);
    auto holdValue     = false;


    auto reg        = make_shared<Register>("reg", regValue, holdValue, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};
    VirtualRegister sut;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(),           expectedWidth);
    TS_ASSERT_EQUALS (sut.BitsOrdering(),        bitsOrdering);
    TS_ASSERT_EQUALS (sut.Identifiers().size(),  1u);
    TS_ASSERT_EQUALS (sut.Identifiers().front(), 0u);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, uint32_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected Width
  {
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  16}, // 0
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  14}, // 1
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, 6},  // 2
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  1},  // 3
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  1},  // 4
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  1},  // 5
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, 1},  // 6
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks VirtualRegister::Append() when providing second and valid RegisterSlice
//!
void UT_VirtualRegister::test_Append_Second ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue      = std::get<0>(data);
    auto bitsOrdering  = std::get<1>(data);
    auto leftIndex     = std::get<2>(data);
    auto rightIndex    = std::get<3>(data);
    auto expectedWidth = std::get<4>(data);

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};

    auto range_2    = IndexedRange(leftIndex, rightIndex);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString(regValue), false, bitsOrdering);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};

    VirtualRegister sut;
    sut.Append(regSlice_1);

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(regSlice_2));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(),          range_1.Width() + range_2.Width());
    TS_ASSERT_EQUALS (sut.BitsOrdering(),       bitsOrdering);
    TS_ASSERT_EQUALS (sut.Identifiers().size(), 2u);
    TS_ASSERT_EQUALS (sut.Identifiers()[0],     0u);
    TS_ASSERT_EQUALS (sut.Identifiers()[1],     1u);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, uint32_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected Width
  {
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  16}, // 0
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  14}, // 1
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, 6},  // 2
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  1},  // 3
    data_t{"/xDEAF", BitsOrdering::Upto,   1,  1,  1},  // 4
    data_t{"/xDEAF", BitsOrdering::Downto, 9,  9,  1},  // 5
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, 1},  // 6
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks VirtualRegister::Append() when providing first and NOT invalid RegisterSlice
//!
void UT_VirtualRegister::test_Append_First_Invalid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto reg        = std::get<0>(data);
    auto leftIndex  = std::get<1>(data);
    auto rightIndex = std::get<2>(data);

    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};
    VirtualRegister sut;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Append(registerSlice), std::invalid_argument);
  };

  using data_t = tuple<shared_ptr<Register>, uint32_t, uint32_t>;
  auto data = // Register, left Index, Right Index
  {
    data_t{shared_ptr<Register>(),                                                 0,  0},  // 0 ==> nullptr
    data_t{make_shared<Register>("reg", BinaryVector::CreateFromString("")),       0,  0},  // 1 ==> Empty vector
    data_t{make_shared<Register>("reg", BinaryVector::CreateFromString("/xDEAF")), 0,  16}, // 2 ==> Too large range
    data_t{make_shared<Register>("reg", BinaryVector::CreateFromString("/xDEAF")), 6,  16}, // 3 ==> Range partially beyond Register width
    data_t{make_shared<Register>("reg", BinaryVector::CreateFromString("/xDEAF")), 16, 17}, // 4 ==> Range completly beyond Register width
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::Append() when providing second and NOT valid RegisterSlice
//!
void UT_VirtualRegister::test_Append_Second_Invalid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue_1      = std::get<0>(data);
    auto bitsOrdering_1  = std::get<1>(data);
    auto leftIndex_1     = std::get<2>(data);
    auto rightIndex_1    = std::get<3>(data);

    auto regValue_2      = std::get<4>(data);
    auto bitsOrdering_2  = std::get<5>(data);
    auto leftIndex_2     = std::get<6>(data);
    auto rightIndex_2    = std::get<7>(data);

    auto range_1    = IndexedRange(leftIndex_1, rightIndex_1);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString(regValue_1), false, bitsOrdering_1);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};

    auto range_2    = IndexedRange(leftIndex_2, rightIndex_2);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString(regValue_2), false, bitsOrdering_2);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};

    VirtualRegister sut;
    sut.Append(regSlice_1);

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Append(regSlice_2), std::invalid_argument);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, string_view, BitsOrdering, uint32_t, uint32_t>;
  auto data = // Reg val 1, Bits ordering 1, Left 1, Right 1, Reg val 2, Bits ordering 2, Left 2, Right 2
  {
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/x1234/b10", BitsOrdering::Upto,   2,  3},  // 00 ==> BitsOrdering mismatch
    data_t{"/xDEAF", BitsOrdering::Upto,   1,  10, "/x1234/b10", BitsOrdering::Downto, 2,  0},  // 01 ==> BitsOrdering mismatch
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "",           BitsOrdering::Downto, 0,  0},  // 02 ==> Empty vector
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/x1234/b10", BitsOrdering::Downto, 29, 18}, // 03 ==> Range completely out of Register width
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/x1234/b10", BitsOrdering::Downto, 18, 2},  // 04 ==> Range partially out of Register width
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::SetPending()
//!
void UT_VirtualRegister::test_SetPending ()
{
  // ---------------- Setup
  //
  auto range_1    = IndexedRange(7u, 3u);
  auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xFAC"), false, BitsOrdering::Downto);
  auto regSlice_1 = RegisterSlice{reg_1, range_1};

  auto range_2    = IndexedRange(12, 9);
  auto reg_2      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAF/b101"), false, BitsOrdering::Downto);
  auto regSlice_2 = RegisterSlice{reg_2, range_2};

  VirtualRegister sut;
  sut.Append(regSlice_1);
  sut.Append(regSlice_2);

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetPending());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg_1->PendingCount(), 1U);
  TS_ASSERT_EQUALS (reg_2->PendingCount(), 1U);
}


//! Checks VirtualRegister getters when no Register are "interfaced" by VirtualRegister
//!
void UT_VirtualRegister::test_Getters_when_HasNoRegister ()
{
  // ---------------- Setup
  //
  BinaryVector readData_binaryVector;
  uint8_t      readData_uint8;
  uint16_t     readData_uint16;
  uint32_t     readData_uint32;
  uint64_t     readData_uint64;
  int8_t       readData_int8;
  int16_t      readData_int16;
  int32_t      readData_int32;
  int64_t      readData_int64;

  VirtualRegister sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.LastToSut(),                        std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(),                      std::runtime_error);
  TS_ASSERT_THROWS (sut.LastCompareResult(),                std::runtime_error);
  TS_ASSERT_THROWS (sut.NextToSut(),                        std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_binaryVector), std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_uint8),        std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_uint16),       std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_uint32),       std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_uint64),       std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_int8),         std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_int16),        std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_int32),        std::runtime_error);
  TS_ASSERT_THROWS (sut.LastFromSut(readData_int64),        std::runtime_error);
}


//! Checks VirtualRegister value getters when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_Getters_1_Register ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = BinaryVector::CreateFromString(std::get<4>(data));

    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};

    BinaryVector readData;

    VirtualRegister sut;

    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EQUALS (sut.LastToSut(),         expected);
    TS_ASSERT_EQUALS (sut.LastFromSut(),       expected);
    TS_ASSERT_EQUALS (sut.NextToSut(),         expected);
    TS_ASSERT_EQUALS (sut.LastCompareResult(), expected); // ==> Expected is all zero by default

    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData));
    TS_ASSERT_EQUALS         (readData, expected);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, string_view>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  "/b1"},            // 00
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  "/b11"},           // 01
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  "/b10"},           // 02
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, "/b11_0101"},      // 03
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/xDEAF"},         // 04
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  "/b101/xEA/b111"}, // 05
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  "/b1"},            // 06
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, "/b1"},            // 07
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  "/b0"},            // 08
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  "/b1"},            // 09
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  "/b10"},           // 10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  "/b010"},          // 11
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  "/b010:0"},        // 12
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  "/b010:00"},       // 13
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  "/b010:001"},      // 14
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  "/b0010:001"},     // 15
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  "/b1_0010:001"},   // 16
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  "/b1_0010:0011"},  // 17
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks VirtualRegister value getters when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_Getters_2_Registers ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = std::get<0>(data);
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = BinaryVector::CreateFromString(std::get<4>(data));

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};    // ==> Viewed slice: Upto ==> /b0_1010; Downto ==> /b1_1110

    auto range_2    = IndexedRange(leftIndex, rightIndex);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString(regValue), false, bitsOrdering);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};

    VirtualRegister sut;
    sut.Append(regSlice_1);

    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS_NOTHING (sut.Append(regSlice_2));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EQUALS (sut.LastToSut(),         expected);
    TS_ASSERT_EQUALS (sut.LastFromSut(),       expected);
    TS_ASSERT_EQUALS (sut.NextToSut(),         expected);
    TS_ASSERT_EQUALS (sut.LastCompareResult(), expected); // ==> Expected is all zero by default
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, string_view>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  "/b0_1010:1"},            // 00
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  "/b0_1010:11"},           // 01
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  "/b0_1010:10"},           // 02
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, "/b0_1010:11_0101"},      // 03
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/b1_1110:/xDEAF"},       // 04
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  "/b1_1110:101/xEA/b111"}, // 05
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  "/b1_1110:1"},            // 06
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, "/b1_1110:1"},            // 07
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  "/b1_1110:0"},            // 08
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  "/b1_1110:1"},            // 09
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  "/b1_1110:10"},           // 10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  "/b1_1110:010"},          // 11
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  "/b1_1110:010:0"},        // 12
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  "/b1_1110:010:00"},       // 13
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  "/b1_1110:010:001"},      // 14
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  "/b1_1110:0010:001"},     // 15
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  "/b1_1110:1_0010:001"},   // 16
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  "/b1_1110:1_0010:0011"},  // 17
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks VirtualRegister::LastFromSut() for, unsigned, integral values
//!
void UT_VirtualRegister::test_LastFromSut_Unsigned_1_Register ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = std::get<4>(data);

    uint8_t  readData_uint8  = 0;
    uint16_t readData_uint16 = 0;
    uint32_t readData_uint32 = 0;
    uint64_t readData_uint64 = 0;

    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};

    BinaryVector readData;

    VirtualRegister sut;

    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint8));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint16));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint32));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint64));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData_uint8,  expected & 0xFF);
    TS_ASSERT_EQUALS (readData_uint16, expected & 0xFFFF);
    TS_ASSERT_EQUALS (readData_uint32, expected & 0xFFFFFFFF);
    TS_ASSERT_EQUALS (readData_uint64, expected);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, uint64_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   2,  2,  0},      // 00 ==> /b0
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  1},      // 01 ==> /b1
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  3},      // 02 ==> /b11
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  2},      // 03 ==> /b10
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, 53},     // 04 ==> /b11_0101
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  0xDEAF}, // 05 ==> /xDEAF
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  0x2F57}, // 06 ==> /b101/xEA/b111 ==> 101:1110_1010:111 ==> 10_1111:0101_0111
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  1},      // 07 ==> /b1
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, 1},      // 08 ==> /b1
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  0},      // 09 ==> /b0
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  1},      // 10 ==> /b1
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  2},      // 11 ==> /b10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  2},      // 12 ==> /b010
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  4},      // 13 ==> /b010:0
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  8},      // 14 ==> /b010:00
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  17},     // 15 ==> /b010:001
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  17},     // 16 ==> /b0010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  0x91},   // 17 ==> /b1_0010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  0x123},  // 18 ==> /b1_0010:0011
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks VirtualRegister::LastFromSut() for, signed, integral values
//!
void UT_VirtualRegister::test_LastFromSut_Signed_1_Register ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = std::get<4>(data);

    int8_t  readData_int8  = 0;
    int16_t readData_int16 = 0;
    int32_t readData_int32 = 0;
    int64_t readData_int64 = 0;


    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};

    BinaryVector readData;

    VirtualRegister sut;

    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));


    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int8));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int16));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int32));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int64));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData_int8,   static_cast<int8_t>(expected));
    TS_ASSERT_EQUALS (readData_int16,  static_cast<int16_t>(expected));
    TS_ASSERT_EQUALS (readData_int32,  static_cast<int32_t>(expected));
    TS_ASSERT_EQUALS (readData_int64,  expected);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, int64_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   2,  2,  0},                  // 00 ==> /b0
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  -1},                 // 01 ==> /b1
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  -1},                 // 02 ==> /b11
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  -2},                 // 03 ==> /b10
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, -11},                // 04 ==> /b11_0101
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  0xFFFFFFFFFFFFDEAF}, // 05 ==> /xDEAF
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  0xFFFFFFFFFFFFEF57}, // 06 ==> /b101/xEA/b111 ==> 101:1110_1010:111 ==> 10_1111:0101_0111
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  -1},                 // 07 ==> /b1
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, -1},                 // 08 ==> /b1
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  0},                  // 09 ==> /b0
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  -1},                 // 10 ==> /b1
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  -2},                 // 11 ==> /b10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  2},                  // 12 ==> /b010
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  4},                  // 13 ==> /b010:0
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  8},                  // 14 ==> /b010:00
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  17},                 // 15 ==> /b010:001
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  17},                 // 16 ==> /b0010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  0xFFFFFFFFFFFFFF91}, // 17 ==> /b1_0010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  0xFFFFFFFFFFFFFF23}, // 18 ==> /b1_0010:0011
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::LastToSut() for unsigned integer when there are 2 "interfaced" Registers
//!
void UT_VirtualRegister::test_LastFromSut_Unsigned_2_Registers ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = std::get<0>(data);
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = std::get<4>(data);

    uint8_t  readData_uint8  = 0;
    uint16_t readData_uint16 = 0;
    uint32_t readData_uint32 = 0;
    uint64_t readData_uint64 = 0;

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};    // ==> Viewed slice: Upto ==> /b0_1010; Downto ==> /b1_1110

    auto range_2    = IndexedRange(leftIndex, rightIndex);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString(regValue), false, bitsOrdering);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};

    VirtualRegister sut;
    sut.Append(regSlice_1);

    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS_NOTHING (sut.Append(regSlice_2));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint8));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint16));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint32));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_uint64));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData_uint8,  expected & 0xFF);
    TS_ASSERT_EQUALS (readData_uint16, expected & 0xFFFF);
    TS_ASSERT_EQUALS (readData_uint32, expected & 0xFFFFFFFF);
    TS_ASSERT_EQUALS (readData_uint64, expected);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, uint64_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  21},       // 00 ==> /b0_1010:1
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  43},       // 01 ==> /b0_1010:11
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  42},       // 02 ==> /b0_1010:10
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, 0x2B5},    // 03 ==> /b0_1010:11_0101
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  0x1EDEAF}, // 04 ==> /b1_1110:/xDEAF
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  0x7AF57},  // 05 ==> /b1_1110:101/xEA/b111 ==> 1_1110:101:1110_1010:111 ==> 111:1010_1111:0101_0111
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  0x3D},     // 06 ==> /b1_1110:1
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  0x3C},     // 07 ==> /b1_1110:0
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  0x7A},     // 08 ==> /b1_1110:10
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  0x791},    // 09 ==> /b1_1110:010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  0x3D23},   // 10 ==> /b1_1110:1_0010:0011
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::LastToSut() for signed integer when there are 2 "interfaced" Registers
//!
void UT_VirtualRegister::test_LastFromSut_Signed_2_Registers ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue     = std::get<0>(data);
    auto bitsOrdering = std::get<1>(data);
    auto leftIndex    = std::get<2>(data);
    auto rightIndex   = std::get<3>(data);
    auto expected     = std::get<4>(data);

    int8_t  readData_int8  = 0;
    int16_t readData_int16 = 0;
    int32_t readData_int32 = 0;
    int64_t readData_int64 = 0;

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};    // ==> Viewed slice: Upto ==> /b0_1010; Downto ==> /b1_1110

    auto range_2    = IndexedRange(leftIndex, rightIndex);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString(regValue), false, bitsOrdering);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};

    VirtualRegister sut;
    sut.Append(regSlice_1);

    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS_NOTHING (sut.Append(regSlice_2));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int8));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int16));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int32));
    TS_ASSERT_THROWS_NOTHING (sut.LastFromSut(readData_int64));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData_int8,   static_cast<int8_t>(expected));
    TS_ASSERT_EQUALS (readData_int16,  static_cast<int16_t>(expected));
    TS_ASSERT_EQUALS (readData_int32,  static_cast<int32_t>(expected));
    TS_ASSERT_EQUALS (readData_int64,  expected);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, int64_t>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  21},                 // 00 ==> /b0_1010:1
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  43},                 // 01 ==> /b0_1010:11
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  42},                 // 02 ==> /b0_1010:10
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, 0x2B5},              // 03 ==> /b0_1010:11_0101
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  0xFFFFFFFFFFFEDEAF}, // 04 ==> /b1_1110:/xDEAF
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  0xFFFFFFFFFFFFAF57}, // 05 ==> /b1_1110:101/xEA/b111 ==> 1_1110:101:1110_1010:111 ==> 111:1010_1111:0101_0111
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  0xFFFFFFFFFFFFFFFD}, // 06 ==> /b1_1110:1
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  0xFFFFFFFFFFFFFF91}, // 07 ==> /b1_1110:010:001
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  0xFFFFFFFFFFFFFD23}, // 08 ==> /b1_1110:1_0010:0011
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::SetToSut()
//!
void UT_VirtualRegister::test_SetToSut_Upto ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    auto toSutValue = BinaryVector::CreateFromString(data);

    auto range_1    = IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xF6C"), false, BitsOrdering::Upto);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};  // ==> Viewed slice: 1_0110

    auto range_2    = IndexedRange(1, 11);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Upto);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};  // ==> Viewed slice: 100:1010_1011

    VirtualRegister sut;
    sut.Append(regSlice_1); // ==> Viewed slice: 1_0110
    sut.Append(regSlice_2); // ==> Viewed slice: 1_0110:100_1010_1011 (0xB4AB)

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.SetToSut(toSutValue));

    // ---------------- Verify
    //
    auto initialToSut = BinaryVector::CreateFromBinaryString("/b1_0110:100_1010_1011");
    TS_ASSERT_EQUALS (sut.LastToSut(), initialToSut);
    TS_ASSERT_EQUALS (sut.NextToSut(), toSutValue);
  };

  auto data =
  {
    "/x1234", // 0
    "/x5678", // 1
    "/x9ABC", // 2
    "/xEDF0", // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks VirtualRegister::SetToSut()
//!
void UT_VirtualRegister::test_SetToSut_Downto ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    auto toSutValue = BinaryVector::CreateFromString(data);

    auto range_1    = IndexedRange(7u, 3u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xF5C"), false, BitsOrdering::Downto);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};  // ==> Viewed slice: 0101_1

    auto range_2    = IndexedRange(11, 1);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Downto);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};  // ==> Viewed slice: 0:1010_1011:10

    VirtualRegister sut;
    sut.Append(regSlice_1); // ==> Viewed slice: 1010_1
    sut.Append(regSlice_2); // ==> Viewed slice: 0101_1:0_1010_1011_10  (0x5AAE)

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.SetToSut(toSutValue));

    // ---------------- Verify
    //
    auto initialToSut = BinaryVector::CreateFromBinaryString("/b0101_1:0_1010_1011_10");
    TS_ASSERT_EQUALS (sut.LastToSut(), initialToSut);
    TS_ASSERT_EQUALS (sut.NextToSut(), toSutValue);
  };

  auto data =
  {
    "/x1234", // 0
    "/x5678", // 1
    "/x9ABC", // 2
    "/xEDF0", // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks VirtualRegister::SetToSut() when no Register are "interfaced" by VirtualRegister
//!
void UT_VirtualRegister::test_SetToSut_when_HasNoRegister ()
{
  // ---------------- Setup
  //
  auto            value = BinaryVector::CreateFromString("/xDead");
  VirtualRegister sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetToSut(value), std::runtime_error);
}


//! Checks VirtualRegister::SetToSut() when providing empty BinaryVector
//!
void UT_VirtualRegister::test_SetToSut_when_Empty ()
{
  // ---------------- Setup
  //
  BinaryVector    emptyVector;
  VirtualRegister sut;
  auto range    = IndexedRange(7, 0);
  auto reg      = make_shared<Register>("reg", BinaryVector::CreateFromString("/xBEEF"), false, BitsOrdering::Downto);
  auto regSlice = RegisterSlice{reg, range};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetToSut(emptyVector), std::invalid_argument);
}


//! Checks VirtualRegister::SetExpectedFromSut()
//!
void UT_VirtualRegister::test_SetExpectedFromSut_Upto ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    auto expectedFromSut       = BinaryVector::CreateFromString(std::get<0>(data));
    auto expectedMask          = BinaryVector::CreateFromString(std::get<1>(data));
    auto expectedFromSut_reg_1 = BinaryVector::CreateFromString(std::get<2>(data));
    auto expectedFromSut_reg_2 = BinaryVector::CreateFromString(std::get<3>(data));
    auto expectedMask_reg_1    = BinaryVector::CreateFromString(std::get<4>(data));
    auto expectedMask_reg_2    = BinaryVector::CreateFromString(std::get<5>(data));

    auto range_1    = IndexedRange(4u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xF6C"), false, BitsOrdering::Upto);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};  // ==> Viewed slice: /x6

    auto range_2    = IndexedRange(0, 11);
    auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Upto);
    auto regSlice_2 = RegisterSlice{reg_2, range_2};  // ==> Viewed slice: /xCAB

    VirtualRegister sut;
    sut.Append(regSlice_1); // ==> Viewed slice: 1_0110
    sut.Append(regSlice_2); // ==> Viewed slice: 1_0110:100_1010_1011 (0xB4AB)

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.SetExpectedFromSut(expectedFromSut, expectedMask));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (reg_1->ExpectedFromSut(), expectedFromSut_reg_1);
    TS_ASSERT_EQUALS (reg_2->ExpectedFromSut(), expectedFromSut_reg_2);
    TS_ASSERT_EQUALS (reg_1->DontCareMask(),    expectedMask_reg_1);
    TS_ASSERT_EQUALS (reg_2->DontCareMask(),    expectedMask_reg_2);
  };

  auto data =
  { // ExpectedFromSut, Mask, Expected_reg_1, Expected_reg_2, ExpectedMask_reg_1, ExpectedMask_reg_2
    make_tuple("/x1234"sv, ""sv,       "/xF1C"sv, "/x234/b101"sv, ""sv,      ""sv),           // 0
    make_tuple("/x5678"sv, ""sv,       "/xF5C"sv, "/x678/b101"sv, ""sv,      ""sv),           // 1
    make_tuple("/x1234"sv, "/x0000"sv, "/xF1C"sv, "/x234/b101"sv, "/x000"sv, "/x000/b000"sv), // 2
    make_tuple("/x5678"sv, "/xFFFF"sv, "/xF5C"sv, "/x678/b101"sv, "/x0F0"sv, "/xFFF/b000"sv), // 3
    make_tuple("/x5678"sv, "/x7EFC"sv, "/xF5C"sv, "/x678/b101"sv, "/x070"sv, "/xEFC/b000"sv), // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::begin() and VirtualRegister::end()
//!
void UT_VirtualRegister::test_begin_end ()
{
  // ---------------- Setup
  //
  auto range_1    = IndexedRange(7u, 3u);
  auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xF5C"), false, BitsOrdering::Downto);
  auto regSlice_1 = RegisterSlice{reg_1, range_1};  // ==> Viewed slice: 0101_1

  auto range_2    = IndexedRange(11, 1);
  auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Downto);
  auto regSlice_2 = RegisterSlice{reg_2, range_2};  // ==> Viewed slice: 0:1010_1011:10

  auto range_3    = IndexedRange(7, 4);
  auto reg_3      = make_shared<Register>("reg_3", BinaryVector::CreateFromString("/xB6D"), false, BitsOrdering::Downto);
  auto regSlice_3 = RegisterSlice{reg_3, range_3};  // ==> Viewed slice: 0110

  VirtualRegister sut;
  sut.Append(regSlice_1); // ==> Viewed slice: 1010_1
  sut.Append(regSlice_2); // ==> Viewed slice: 0101_1:0_1010_1011_10       (0x5AAE)
  sut.Append(regSlice_3); // ==> Viewed slice: 0101_1:0_1010_1011_10:0110  (0x5AAE6)

  CxxTest::setAbortTestOnFail(true);

  vector<RegisterSlice> capturedSlices;

  // ---------------- Exercise
  //
  for (const auto& sliceReg : sut)
  {
    capturedSlices.emplace_back(sliceReg);
  }

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (capturedSlices.size(), 3u);

  vector<RegisterSlice> expectedSlices { regSlice_1, regSlice_2, regSlice_3  };

  TS_ASSERT_EQUALS (capturedSlices, expectedSlices);
}



//! Checks VirtualRegister::cbegin() and VirtualRegister::cend()
//!
void UT_VirtualRegister::test_cbegin_cend ()
{
  // ---------------- Setup
  //
  auto range_1    = IndexedRange(7u, 3u);
  auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xF5C"), false, BitsOrdering::Downto);
  auto regSlice_1 = RegisterSlice{reg_1, range_1};  // ==> Viewed slice: 0101_1

  auto range_2    = IndexedRange(11, 1);
  auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Downto);
  auto regSlice_2 = RegisterSlice{reg_2, range_2};  // ==> Viewed slice: 0:1010_1011:10

  auto range_3    = IndexedRange(7, 4);
  auto reg_3      = make_shared<Register>("reg_3", BinaryVector::CreateFromString("/xB6D"), false, BitsOrdering::Downto);
  auto regSlice_3 = RegisterSlice{reg_3, range_3};  // ==> Viewed slice: 0110

  VirtualRegister sut;
  sut.Append(regSlice_1); // ==> Viewed slice: 1010_1
  sut.Append(regSlice_2); // ==> Viewed slice: 0101_1:0_1010_1011_10       (0x5AAE)
  sut.Append(regSlice_3); // ==> Viewed slice: 0101_1:0_1010_1011_10:0110  (0x5AAE6)

  CxxTest::setAbortTestOnFail(true);

  vector<RegisterSlice> capturedSlices;

  // ---------------- Exercise
  //
  auto pos = sut.cbegin();
  auto end = sut.cend();

  while (pos != end)
  {
    capturedSlices.emplace_back(*pos++);
  }

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (capturedSlices.size(), 3u);

  vector<RegisterSlice> expectedSlices { regSlice_1, regSlice_2, regSlice_3  };

  TS_ASSERT_EQUALS (capturedSlices, expectedSlices);
}


//===========================================================================
// End of UT_VirtualRegister.cpp
//===========================================================================
