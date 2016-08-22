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

#ifndef SYSTEMMODELBUILDER_H__65823246_65BD_4D48_5EB4_23C860E85C36__INCLUDED_
  #define SYSTEMMODELBUILDER_H__65823246_65BD_4D48_5EB4_23C860E85C36__INCLUDED_

#include "SystemModel.hpp"
#include "PathSelector.hpp"
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

//! Specifies where the mux (Linker node) is placed relative to the register that drive it
//!
enum class MuxRegPlacement
{
  Remote,    //!< The register placement is NOT related to the mux node
  AfterMux,  //!< The register is placed after the mux
  BeforeMux, //!< The register is placed before the mux
};


//! Defines what kind of path selector to create to drive a mux
//!
enum class SelectorKind
{
  Binary,  //!< Basic binary encoding where value '00' select path '0'
  One_Hot, //!< One hot encoding where value '000' select no path and "001" path '0'
  N_Hot,   //!< One hot encoding where value '000' select no path, "001" path '0', "011" paths '1' and '0'
};


//! Provides facilities to create SystemModel sub-trees
//!
class DLL_EXPORT SystemModelBuilder final
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelBuilder() = default;
  SystemModelBuilder()  = delete;
  SystemModelBuilder(mast::SystemModel& systemModel) : m_model(systemModel) {}

  using string_view = std::experimental::string_view;

  //! Appends several registers (with same length and initial content) to a parent
  //!
  void AppendRegisters (uint32_t                          count,
                        const std::string&                baseName,
                        const mast::BinaryVector&         bypass,
                        std::shared_ptr<mast::ParentNode> parent);


  //! Creates a JTAG TAP
  //!
  std::shared_ptr<AccessInterface> Create_JTAG_TAP (string_view                              name,
                                                    uint32_t                                 irBitsCount,
                                                    uint32_t                                 muxPathsCount,
                                                    std::shared_ptr<AccessInterfaceProtocol> protocol);

  //! Creates a path selector
  //!
  std::shared_ptr<mast::PathSelector> Create_PathSelector(SelectorKind              selectorKind,
                                                          std::shared_ptr<Register> associatedRegister,
                                                          uint32_t                  pathsCount,
                                                          SelectorProperty          properties = SelectorProperty::Std);

  //! Creates a path selector, creating its associated register
  //!
  std::pair<std::shared_ptr<Register>, std::shared_ptr<mast::PathSelector>>
  Create_PathSelector(SelectorKind     selectorKind,
                      string_view      registerName,
                      uint32_t         pathsCount,
                      SelectorProperty properties = SelectorProperty::Std);


  //! Creates a MIB sub-tree
  //!
  std::shared_ptr<mast::Chain> Create_MIB (string_view                         name,
                                           std::shared_ptr<mast::PathSelector> selector,
                                           std::shared_ptr<mast::Register>     selectorRegister,
                                           MuxRegPlacement                     muxRegPlacement
                                          );


  //! Creates a SIB sub-tree
  //!
  std::shared_ptr<mast::Chain> Create_SIB (string_view                         name,
                                           SelectorProperty sel_properties,
                                           MuxRegPlacement                     muxRegPlacement
                                          );


  //! Creates a "1500" style wrapper
  //!
  std::shared_ptr<mast::Chain> Create_1500_Wrapper (string_view name, uint32_t maxDerivations);

  // ---------------- Private  Fields
  //
  private:
  mast::SystemModel& m_model;
};
//
//  End of SystemModelBuilder class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SYSTEMMODELBUILDER_H__65823246_65BD_4D48_5EB4_23C860E85C36__INCLUDED_
//===========================================================================
// End of SystemModelBuilder.hpp
//===========================================================================
