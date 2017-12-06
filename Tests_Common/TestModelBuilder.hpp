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
#include "AccessInterfaceProtocol.hpp"
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

  using string_view = std::experimental::string_view;

  //! Appends several chains (with same name prefix) to a parent
  //!
  void AppendChains (uint32_t                          count,
                     const std::string&                baseName,
                     std::shared_ptr<mast::ParentNode> parent);

  //! Appends several registers (with same length and initial content) to a parent
  //!
  void AppendRegisters (uint32_t                          count,
                        const std::string&                baseName,
                        const mast::BinaryVector&         bypass,
                        std::shared_ptr<mast::ParentNode> parent)
  {
    m_builder.AppendRegisters(count, baseName, bypass, parent);
  }

  std::shared_ptr<mast::AccessInterface> Create_TestCase_AccessInterface     (string_view name = "TAP");
  std::shared_ptr<mast::AccessInterfaceTranslator> Create_TestCase_Emulation_Translator   (string_view name = "TAP");
  std::shared_ptr<mast::AccessInterfaceTranslator> Create_TestCase_I2C_Emulation_Translator (string_view name = "I2C");
  std::shared_ptr<mast::AccessInterfaceTranslator>  Create_TestCase_JTAG_to_I2C_Translator(string_view name = "JTAG_to_I2C");
  std::shared_ptr<mast::AccessInterface> Create_TestCase_1687                (string_view name = "");
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB                 (string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES, uint32_t regsBitsCount = DYNAMIC_TDR_LEN);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB_Multichain_Pre  (string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES, uint32_t regsBitsCount = DYNAMIC_TDR_LEN);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB_Multichain_Post (string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES, uint32_t regsBitsCount = DYNAMIC_TDR_LEN);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_1500                (string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES, uint32_t regsBitsCount = DYNAMIC_TDR_LEN);

  std::shared_ptr<mast::AccessInterface> Create_UnitTestCase_6_Levels();

  //! Creates a new Tap sub-tree
  //!
  std::shared_ptr<mast::AccessInterface> Create_JTAG_TAP    (string_view name, uint32_t irBitsCount, uint32_t muxPathsCount);
  std::shared_ptr<mast::AccessInterface> Create_JTAG_TAP    (string_view name, uint32_t irBitsCount, uint32_t muxPathsCount,std::shared_ptr<mast::AccessInterfaceProtocol> protocol);
  std::shared_ptr<mast::AccessInterface> Create_Default_SUT (string_view name);
  std::shared_ptr<mast::Chain>           Create_Default_SIB (string_view name);
  std::shared_ptr<mast::Chain>           Create_Default_MIB (string_view name,  uint32_t maxEndPoints);
  std::shared_ptr<mast::Chain>           Create_Simple_MIB  (string_view rootName = "Sut");
  std::shared_ptr<mast::AccessInterfaceTranslator> Create_Brocade (std::shared_ptr<mast::AccessInterfaceTranslatorProtocol>  TopProtocol,
                                                      std::shared_ptr<mast::AccessInterfaceProtocol>           masterProtocol,
                                                      std::shared_ptr<mast::AccessInterfaceProtocol>           slaveProtocol,
                                                      std::initializer_list<std::shared_ptr<mast::AccessInterface>> taps);

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
