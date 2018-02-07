//===========================================================================
//                           UT_RegistersAlias.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_RegistersAlias.cpp
//!
//! Implements test fixture for testing RegistersAlias
//!
//===========================================================================

#include "UT_RegistersAlias.hpp"
#include "RegistersAlias.hpp"
#include "VirtualRegister.hpp"
#include "BinaryVector.hpp"
#include "IndexedRange.hpp"

#include "Mast_Core_Traits.hpp"

#include <experimental/string_view>
#include <tuple>
#include <vector>
#include <memory>

using std::tuple;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::experimental::string_view;

using mast::BinaryVector;
using mast::Register;
using mast::VirtualRegister;
using mast::IndexedRange;
using mast::BitsOrdering;

using RegisterSlice = VirtualRegister::RegisterSlice;

using namespace mast;

//! Checks RegistersAlias constructor
//!
void UT_RegistersAlias::test_Constructor ()
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

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING
  (
    RegistersAlias sut("Foo", std::move(virtualRegister));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.Name(), "Foo");

    const auto& gotAliasedRegisters = sut.AliasedRegisters();

    TS_ASSERT_EQUALS (gotAliasedRegisters.SlicesCount(), 2u);
    TS_ASSERT_EQUALS (gotAliasedRegisters.BitsCount(),   9u);
  );
}


//! Checks RegistersAlias constructor when providing an empty name
//!
void UT_RegistersAlias::test_Constructor_empty_name ()
{
  // ---------------- Setup
  //
  auto reg      = make_shared<Register>("reg", BinaryVector::CreateFromString("/xCAFE/b101"), false, BitsOrdering::Downto);
  auto regSlice = RegisterSlice{reg, IndexedRange{12, 8}};

  VirtualRegister virtualRegister;
  virtualRegister.Append(regSlice);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (RegistersAlias("", std::move(virtualRegister)), std::invalid_argument);
}


//! Checks RegistersAlias constructor when providing an empty VirtualRegister
//!
void UT_RegistersAlias::test_Constructor_empty_VirtualRegister ()
{
  // ---------------- Setup
  //
  VirtualRegister virtualRegister;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (RegistersAlias("Foo", std::move(virtualRegister)), std::invalid_argument);
}


//===========================================================================
// End of UT_RegistersAlias.cpp
//===========================================================================
