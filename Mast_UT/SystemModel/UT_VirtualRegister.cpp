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
#include <memory>
#include <tuple>

using std::tuple;
using std::shared_ptr;
using std::make_shared;
using std::experimental::string_view;

using mast::BinaryVector;
using mast::Register;
using mast::VirtualRegister;
using mast::IndexedRange;
using mast::BitsOrdering;

using RegisterSlice = VirtualRegister::RegisterSlice;

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
  );


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
    auto regValue          = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering      = std::get<1>(data);
    auto leftIndex         = std::get<2>(data);
    auto rightIndex        = std::get<3>(data);
    auto expectedWidth     = std::get<4>(data);


    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};
    VirtualRegister sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(),    expectedWidth);
    TS_ASSERT_EQUALS (sut.BitsOrdering(), bitsOrdering);
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
    TS_ASSERT_EQUALS (sut.BitsCount(),    range_1.Width() + range_2.Width());
    TS_ASSERT_EQUALS (sut.BitsOrdering(), bitsOrdering);
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



//! Checks VirtualRegister::LastToSut() when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_LastToSut_1_Register ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue          = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering      = std::get<1>(data);
    auto leftIndex         = std::get<2>(data);
    auto rightIndex        = std::get<3>(data);
    auto expectedLastToSut = BinaryVector::CreateFromString(std::get<4>(data));

    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};
    VirtualRegister sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.LastToSut(), expectedLastToSut);
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



//! Checks VirtualRegister::LastToSut() when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_LastToSut_2_Registers ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue          = std::get<0>(data);
    auto bitsOrdering      = std::get<1>(data);
    auto leftIndex         = std::get<2>(data);
    auto rightIndex        = std::get<3>(data);
    auto expectedLastToSut = BinaryVector::CreateFromString(std::get<4>(data));

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};    // ==> Viewed slice: Upto ==> /b0_1010; Downto ==> /b1_0101

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
    TS_ASSERT_EQUALS (sut.BitsCount(),    range_1.Width() + range_2.Width());
    TS_ASSERT_EQUALS (sut.BitsOrdering(), bitsOrdering);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, string_view>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  "/b0_1010:1"},            // 00
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  "/b0_1010:11"},           // 01
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  "/b0_1010:10"},           // 02
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, "/b0_1010:11_0101"},      // 03
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/b1_0101:/xDEAF"},       // 04
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  "/b1_0101:101/xEA/b111"}, // 05
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  "/b1_0101:1"},            // 06
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, "/b1_0101:1"},            // 07
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  "/b1_0101:0"},            // 08
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  "/b1_0101:1"},            // 09
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  "/b1_0101:10"},           // 10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  "/b1_0101:010"},          // 11
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  "/b1_0101:010:0"},        // 12
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  "/b1_0101:010:00"},       // 13
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  "/b1_0101:010:001"},      // 14
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  "/b1_0101:0010:001"},     // 15
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  "/b1_0101:1_0010:001"},   // 16
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  "/b1_0101:1_0010:0011"},  // 17
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::LastToSut() when no Register are "interfaced" by VirtualRegister
//!
void UT_VirtualRegister::test_LastToSut_when_HasNoRegister ()
{
  // ---------------- Setup
  //
  VirtualRegister sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.LastToSut(), std::runtime_error);
}


//! Checks VirtualRegister::NextToSut() when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_NextToSut_1_Register ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue          = BinaryVector::CreateFromString(std::get<0>(data));
    auto bitsOrdering      = std::get<1>(data);
    auto leftIndex         = std::get<2>(data);
    auto rightIndex        = std::get<3>(data);
    auto expectedNextToSut = BinaryVector::CreateFromString(std::get<4>(data));

    auto reg        = make_shared<Register>("reg", regValue, false, bitsOrdering);
    IndexedRange    range(leftIndex,   rightIndex);
    RegisterSlice   registerSlice{reg, range};
    VirtualRegister sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(registerSlice));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expectedNextToSut);
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



//! Checks VirtualRegister::NextToSut() when there is only 1 "interfaced" Register
//!
void UT_VirtualRegister::test_NextToSut_2_Registers ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto regValue          = std::get<0>(data);
    auto bitsOrdering      = std::get<1>(data);
    auto leftIndex         = std::get<2>(data);
    auto rightIndex        = std::get<3>(data);
    auto expectedNextToSut = BinaryVector::CreateFromString(std::get<4>(data));

    auto range_1    = bitsOrdering == BitsOrdering::Downto ? IndexedRange(7u, 3u) : IndexedRange(3u, 7u);
    auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, bitsOrdering);
    auto regSlice_1 = RegisterSlice{reg_1, range_1};    // ==> Viewed slice: Upto ==> /b0_1010; Downto ==> /b1_0101

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
    TS_ASSERT_EQUALS (sut.BitsCount(),    range_1.Width() + range_2.Width());
    TS_ASSERT_EQUALS (sut.BitsOrdering(), bitsOrdering);
  };

  using data_t = tuple<string_view, BitsOrdering, uint32_t, uint32_t, string_view>;
  auto data = // Register value, Bits ordering, left Index, Right Index, expected result
  {
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  3,  "/b0_1010:1"},            // 00
    data_t{"/xDEAF", BitsOrdering::Upto,   3,  4,  "/b0_1010:11"},           // 01
    data_t{"/xD7AF", BitsOrdering::Upto,   3,  4,  "/b0_1010:10"},           // 02
    data_t{"/xDEAF", BitsOrdering::Upto,   5,  10, "/b0_1010:11_0101"},      // 03
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 0,  "/b1_0101:/xDEAF"},       // 04
    data_t{"/xDEAF", BitsOrdering::Downto, 14, 1,  "/b1_0101:101/xEA/b111"}, // 05
    data_t{"/xDEAF", BitsOrdering::Downto, 0,  0,  "/b1_0101:1"},            // 06
    data_t{"/xDEAF", BitsOrdering::Downto, 15, 15, "/b1_0101:1"},            // 07
    data_t{"/x1234", BitsOrdering::Downto, 8,  8,  "/b1_0101:0"},            // 08
    data_t{"/x1234", BitsOrdering::Downto, 9,  9,  "/b1_0101:1"},            // 09
    data_t{"/x1234", BitsOrdering::Downto, 9,  8,  "/b1_0101:10"},           // 10
    data_t{"/x1234", BitsOrdering::Downto, 10, 8,  "/b1_0101:010"},          // 11
    data_t{"/x1234", BitsOrdering::Downto, 10, 7,  "/b1_0101:010:0"},        // 12
    data_t{"/x1234", BitsOrdering::Downto, 10, 6,  "/b1_0101:010:00"},       // 13
    data_t{"/x1234", BitsOrdering::Downto, 10, 5,  "/b1_0101:010:001"},      // 14
    data_t{"/x1234", BitsOrdering::Downto, 11, 5,  "/b1_0101:0010:001"},     // 15
    data_t{"/x1234", BitsOrdering::Downto, 12, 5,  "/b1_0101:1_0010:001"},   // 16
    data_t{"/x1234", BitsOrdering::Downto, 12, 4,  "/b1_0101:1_0010:0011"},  // 17
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks VirtualRegister::NextToSut() when no Register are "interfaced" by VirtualRegister
//!
void UT_VirtualRegister::test_NextToSut_when_HasNoRegister ()
{
  // ---------------- Setup
  //
  VirtualRegister sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.NextToSut(), std::runtime_error);
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
    auto reg_2      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Upto);
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
    auto reg_2      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAB/b101"), false, BitsOrdering::Downto);
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

//===========================================================================
// End of UT_VirtualRegister.cpp
//===========================================================================
