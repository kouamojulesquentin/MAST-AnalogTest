//===========================================================================
//                           UT_AliasRepository.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AliasRepository.cpp
//!
//! Implements test fixture for testing AliasRepository
//!
//===========================================================================

#include "UT_AliasRepository.hpp"
#include "AliasRepository.hpp"
#include "VirtualRegister.hpp"
#include "Mast_Core_Traits.hpp"

#include <tuple>

using std::tuple;
using std::make_tuple;
using std::make_shared;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;


namespace
{
  //! Creates two aliases (Foo & Bar) for two virtual registers
  //!
  tuple<RegistersAlias, RegistersAlias> CreateAliases()
  {
    // Create first alias
    VirtualRegister virtualRegister_1;
    {
      auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, BitsOrdering::Downto);
      auto regSlice_1 = RegisterSlice{reg_1, IndexedRange{12, 8}};

      auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xB0F"), false, BitsOrdering::Downto);
      auto regSlice_2 = RegisterSlice{reg_2, IndexedRange{7, 4}};

      virtualRegister_1.Append(regSlice_1);
      virtualRegister_1.Append(regSlice_2);
    }
    RegistersAlias alias_1("Foo", std::move(virtualRegister_1));

    // Create second alias
    VirtualRegister virtualRegister_2;
    {
      auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xFADE/b101"), false, BitsOrdering::Downto);
      auto regSlice_1 = RegisterSlice{reg_1, IndexedRange{11, 9}};

      auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xBAF"), false, BitsOrdering::Downto);
      auto regSlice_2 = RegisterSlice{reg_2, IndexedRange{4, 3}};

      virtualRegister_2.Append(regSlice_1);
      virtualRegister_2.Append(regSlice_2);
    }
    RegistersAlias alias_2("Bar", std::move(virtualRegister_2));

    return make_tuple(alias_1, alias_2);
  }
  //
  //  End of CreateAliases
  //---------------------------------------------------------------------------


} // End of unnamed namespace

//! Checks AliasRepository default constructor
//!
void UT_AliasRepository::test_Constructor ()
{
  // ---------------- Exercise
  //
  AliasRepository sut;

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.HasAliases());

  const auto& aliases = sut.RegistersAliases();
  TS_ASSERT_EMPTY (aliases);
}



//! Checks AliasRepository::Append() when this is the first appended alias
//!
void UT_AliasRepository::test_Append_first ()
{
  // ---------------- Setup
  //
  auto reg_1      = make_shared<Register>("reg_1", BinaryVector::CreateFromString("/xCAFE/b101"), false, BitsOrdering::Downto);
  auto regSlice_1 = RegisterSlice{reg_1, IndexedRange{12, 8}};

  auto reg_2      = make_shared<Register>("reg_2", BinaryVector::CreateFromString("/xB0F"), false, BitsOrdering::Downto);
  auto regSlice_2 = RegisterSlice{reg_2, IndexedRange{7, 4}};

  VirtualRegister virtualRegister;
  virtualRegister.Append(regSlice_1);
  virtualRegister.Append(regSlice_2);

  RegistersAlias alias("Foo", std::move(virtualRegister));

  AliasRepository sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Append(std::move(alias)));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.HasAliases());

  const auto& aliases = sut.RegistersAliases();

  TS_ASSERT_EQUALS (aliases.size(), 1u);
  const auto& firstAlias = aliases.front();

  TS_ASSERT_EQUALS (firstAlias.Name(), "Foo");

  const auto& gotAliasedRegisters = firstAlias.AliasedRegisters();

  TS_ASSERT_EQUALS (gotAliasedRegisters.SlicesCount(), 2u);
  TS_ASSERT_EQUALS (gotAliasedRegisters.BitsCount(),   9u);
}


//! Checks AliasRepository::Append() when this is the second appended alias
//!
void UT_AliasRepository::test_Append_second ()
{
  // ---------------- Setup
  //
  auto  createAliases = CreateAliases();   // See that function for expecting results !
  auto& alias_1 = std::get<0>(createAliases);
  auto& alias_2 = std::get<1>(createAliases);

  AliasRepository sut;
  sut.Append(std::move(alias_1));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Append(std::move(alias_2)));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.HasAliases());

  const auto& aliases = sut.RegistersAliases();

  TS_ASSERT_EQUALS (aliases.size(), 2u);

  // Check first alias
  {
    const auto& firstAlias = aliases[0];

    TS_ASSERT_EQUALS (firstAlias.Name(), "Foo");

    const auto& gotAliasedRegisters = firstAlias.AliasedRegisters();

    TS_ASSERT_EQUALS (gotAliasedRegisters.SlicesCount(), 2u);
    TS_ASSERT_EQUALS (gotAliasedRegisters.BitsCount(),   9u);
  }

  // Check second alias
  {
    const auto& secondAlias = aliases[1];

    TS_ASSERT_EQUALS (secondAlias.Name(), "Bar");

    const auto& gotAliasedRegisters = secondAlias.AliasedRegisters();

    TS_ASSERT_EQUALS (gotAliasedRegisters.SlicesCount(), 2u);
    TS_ASSERT_EQUALS (gotAliasedRegisters.BitsCount(),   5u);
  }
}



//! Checks AliasRepository::FindRegisterAlias()
//!
void UT_AliasRepository::test_FindRegisterAlias ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto aliasName         = std::get<0>(data);
    auto expectedBitsCount = std::get<1>(data);

    auto aliases = CreateAliases();   // See that function for expecting results !

    AliasRepository sut;
    sut.Append(std::move(std::get<0>(aliases)));
    sut.Append(std::move(std::get<1>(aliases)));

    // ---------------- Exercise
    //
    auto alias = sut.FindRegisterAlias(aliasName);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (alias);
    TS_ASSERT_EQUALS      (alias->Name(), aliasName);

    const auto& gotAliasedRegisters = alias->AliasedRegisters();

    TS_ASSERT_EQUALS (gotAliasedRegisters.BitsCount(), expectedBitsCount);
  };

  auto data =
  {
    make_tuple("Foo"sv, 9u),
    make_tuple("Bar"sv, 5u),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AliasRepository::FindRegisterAlias() when requested alias is not found
//!
void UT_AliasRepository::test_FindRegisterAlias_not_exist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& aliasName)
  {
    // ---------------- Setup
    //
    auto aliases = CreateAliases();

    AliasRepository sut;
    sut.Append(std::move(std::get<0>(aliases)));
    sut.Append(std::move(std::get<1>(aliases)));

    // ---------------- Exercise
    //
    auto alias = sut.FindRegisterAlias(aliasName);

    // ---------------- Verify
    //
    TS_ASSERT_NULLPTR (alias);
  };

  auto data =
  {
    "Foot",   // 0
    "Bad",    // 1
    "",       // 2
    " ",      // 3
    " Foo",   // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks AliasRepository::FindRegister()
//!
void UT_AliasRepository::test_FindRegister ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto aliasName         = std::get<0>(data);
    auto expectedBitsCount = std::get<1>(data);

    auto aliases = CreateAliases();   // See that function for expecting results !

    AliasRepository sut;
    sut.Append(std::move(std::get<0>(aliases)));
    sut.Append(std::move(std::get<1>(aliases)));

    // ---------------- Exercise
    //
    auto reg = sut.FindRegister(aliasName);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (reg);
    TS_ASSERT_EQUALS      (reg->BitsCount(), expectedBitsCount);
  };

  auto data =
  {
    make_tuple("Foo"sv, 9u),
    make_tuple("Bar"sv, 5u),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AliasRepository::FindRegister() when requested alias is not found
//!
void UT_AliasRepository::test_FindRegister_not_exist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& aliasName)
  {
    // ---------------- Setup
    //
    auto aliases = CreateAliases();

    AliasRepository sut;
    sut.Append(std::move(std::get<0>(aliases)));
    sut.Append(std::move(std::get<1>(aliases)));

    // ---------------- Exercise
    //
    auto reg = sut.FindRegister(aliasName);

    // ---------------- Verify
    //
    TS_ASSERT_NULLPTR (reg);
  };

  auto data =
  {
    "Foot",   // 0
    "Bad",    // 1
    " Foo",   // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//===========================================================================
// End of UT_AliasRepository.cpp
//===========================================================================
