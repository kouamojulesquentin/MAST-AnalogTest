//===========================================================================
//                           SystemModelBuilder.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelBuilder.hpp
//!
//! Declares SystemModelBuilder class
//!
//===========================================================================


#ifndef SYSTEMMODELBUILDER_H__90360FCD_A06E_4402_79BF_910CC9F73048__INCLUDED_
  #define SYSTEMMODELBUILDER_H__90360FCD_A06E_4402_79BF_910CC9F73048__INCLUDED_

#include "SystemModel.hpp"
#include <memory>
#include <experimental/string_view>

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

//! Provides facilities to create SystemModel tree example
//!
//! @note This is mainly intended to be used in unit tests
class SystemModelBuilder final
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelBuilder() = default;
  SystemModelBuilder()  = delete;
  SystemModelBuilder(mast::SystemModel& systemModel) : m_model(systemModel) {}

  //! Specifies where the mux (Linker node) is placed relative to the register that drive it
  //!
  enum class MuxRegPlacement
  {
    Remote,    //!< The register placement is NOT related to the mux node
    AfterMux,  //!< The register is placed after the mux
    BeforeMux, //!< The register is placed before the mux
  };

  //! Appends several registers (with same length and initial content) to a parent
  //!
  void AppendRegisters (uint32_t                          count,
                        const std::string&                baseName,
                        const mast::BinaryVector&         bypass,
                        std::shared_ptr<mast::ParentNode> parent);


  std::shared_ptr<mast::Chain>           Create_MIB (std::experimental::string_view      name,
                                                     std::shared_ptr<mast::PathSelector> selector,
                                                     std::shared_ptr<mast::Register>     selectorRegister,
                                                     MuxRegPlacement                     muxRegPlacement
                                                     );

  std::shared_ptr<mast::AccessInterface> Create_TestCase_AccessInterface (std::experimental::string_view name = "TAP");
  std::shared_ptr<mast::AccessInterface> Create_TestCase_MIB             (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);
  std::shared_ptr<mast::AccessInterface> Create_TestCase_1500            (std::experimental::string_view name = "", uint32_t registersCount = DEFAULT_WRAPPED_CORES);


  std::shared_ptr<mast::AccessInterface> Create_Default_SUT  (std::experimental::string_view name);
  std::shared_ptr<mast::Chain>           Create_Default_MIB  (std::experimental::string_view name, uint32_t maxDerivations);
  std::shared_ptr<mast::Chain>           Create_1500_Wrapper (std::experimental::string_view name, uint32_t maxDerivations);


//+  static std::shared_ptr<ParentNode>  Append (std::shared_ptr<SystemModelNode> child, std::shared_ptr<ParentNode> parent);

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  static constexpr uint32_t STATIC_TDR_LEN        = 64u;
  static constexpr uint32_t DYNAMIC_TDR_LEN       = 128u;
  static constexpr uint32_t DEFAULT_WRAPPED_CORES = 3u;
  static constexpr uint32_t DEFAULT_IR_LEN        = 8u;
  static constexpr uint32_t DEFAULT_TDR_LEN       = 2u;
  static constexpr char*    DEFAULT_1500_NAME     = "1500_wrapper";
  static constexpr char*    DEFAULT_MIB_NAME      = "MIB";
  static constexpr char*    DEFAULT_SIB_NAME      = "SIB";
  static constexpr char*    MIB_CTRL_EXT          = "_ctrl";
  static constexpr char*    MIB_MUX_EXT           = "_mux";

  mast::SystemModel& m_model;
};
//
//  End of SystemModelBuilder class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELBUILDER_H__90360FCD_A06E_4402_79BF_910CC9F73048__INCLUDED_

//===========================================================================
// End of SystemModelBuilder.hpp
//===========================================================================
