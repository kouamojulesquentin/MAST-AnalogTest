//===========================================================================
//                           TestModelBuilder.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestModelBuilder.hpp
//!
//! Declares TestModelBuilder class
//!
//===========================================================================

#ifndef TESTMODELBUILDER_H__87ACB445_83CE_49DB_7EBB_7F14B3A1D5D0__INCLUDED_
  #define TESTMODELBUILDER_H__87ACB445_83CE_49DB_7EBB_7F14B3A1D5D0__INCLUDED_

#include "SystemModelBuilder.hpp"
#include <memory>
#include <experimental/string_view>
using namespace std::string_literals;

namespace mast
{
  class AccessInterface;
  class Chain;
  class PathSelector;
  class Register;
  class BinaryVector;
} // End of namespace mast

namespace test
{

constexpr uint32_t STATIC_TDR_LEN        = 16u;
constexpr uint32_t DYNAMIC_TDR_LEN       = 32u;
constexpr uint32_t DEFAULT_WRAPPED_CORES = 3u;
constexpr uint32_t DEFAULT_BPY_LEN       = 1u;
constexpr uint32_t DEFAULT_IR_LEN        = 8u;
constexpr uint32_t DEFAULT_TDR_LEN       = 2u;
constexpr char     DEFAULT_1500_NAME[]   = "1500_wrapper";
constexpr char     DEFAULT_MIB_NAME[]    = "MIB";
constexpr char     DEFAULT_SIB_NAME[]    = "SIB";
constexpr char     MIB_CTRL_EXT[]        = "_ctrl";
constexpr char     MIB_MUX_EXT[]         = "_mux";
constexpr char     SIB_CTRL_EXT[]        = "_ctrl";
constexpr char     SIB_MUX_EXT[]         = "_mux";

//! Provides facilities to create SystemModel tree example
//!
//! @note This is mainly intended to be used in unit tests
class TestModelBuilder final
{
  // ---------------- Public  Methods
  //
  public:
  ~TestModelBuilder() = default;
  TestModelBuilder()  = delete;
  TestModelBuilder(mast::SystemModel& systemModel) : m_model(systemModel), m_builder(systemModel) {}

  std::shared_ptr<mast::AccessInterface> Create_TestCase_AccessInterface     (std::experimental::string_view name = "TAP");
  std::shared_ptr<mast::AccessInterface> Create_TestCase_1687                (std::experimental::string_view name = "");
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB                 (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB_Multichain_Pre  (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB_Multichain_Post (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_1500                (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);

  std::shared_ptr<mast::AccessInterface> Create_UnitTestCase_6_Levels();


  std::shared_ptr<mast::AccessInterface> Create_Default_SUT  (std::experimental::string_view name);
  std::shared_ptr<mast::Chain>           Create_Default_SIB  (std::experimental::string_view name);
  std::shared_ptr<mast::Chain>           Create_Default_MIB  (std::experimental::string_view name, uint32_t maxDerivations);

  std::shared_ptr<mast::Chain>           Create_Simple_MIB   (std::experimental::string_view rootName = "Sut");

  // ---------------- Private  Fields
  //
  private:
  mast::SystemModel&       m_model;   //!< Model to build on
  mast::SystemModelBuilder m_builder; //!< "Official" SystemModel builder

};
//
//  End of TestModelBuilder class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined TESTMODELBUILDER_H__87ACB445_83CE_49DB_7EBB_7F14B3A1D5D0__INCLUDED_
//===========================================================================
// End of TestModelBuilder.hpp
//===========================================================================
