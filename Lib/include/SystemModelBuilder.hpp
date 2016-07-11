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
  Binary,                        //!< Basic binary encoding where value '00' select path '0'
  Binary_Inverted,               //!< Binary encoding where value 'FF' select path '0'
  Binary_CanSelectNone,          //!< Binary encoding where value '00' select no path and "01" path '0'
  Binary_Inverted_CanSelectNone, //!< Binary encoding where value 'FF' select no path and "FE" path '0'
  One_Hot,                       //!< One hot encoding where value '000' select no path and "001" path '0'
  N_Hot,                         //!< One hot encoding where value '000' select no path and "001" path '0'
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


  //! Appends several registers (with same length and initial content) to a parent
  //!
  void AppendRegisters (uint32_t                          count,
                        const std::string&                baseName,
                        const mast::BinaryVector&         bypass,
                        std::shared_ptr<mast::ParentNode> parent);


  std::shared_ptr<mast::Chain> Create_MIB (std::experimental::string_view      name,
                                           std::shared_ptr<mast::PathSelector> selector,
                                           std::shared_ptr<mast::Register>     selectorRegister,
                                           MuxRegPlacement                     muxRegPlacement
                                          );

  std::shared_ptr<mast::Chain> Create_MIB (std::experimental::string_view  name,
                                           SelectorKind                    selectorKind,
                                           std::shared_ptr<mast::Register> selectorRegister,
                                           MuxRegPlacement                 muxRegPlacement
                                  );


  std::shared_ptr<mast::Chain> Create_SIB (std::experimental::string_view      name,
                                           std::shared_ptr<mast::PathSelector> selector,
                                           std::shared_ptr<mast::Register>     selectorRegister,
                                           MuxRegPlacement                     muxRegPlacement
                                          )
  {
    return Create_MIB(name, selector, selectorRegister, muxRegPlacement);
  }

  std::shared_ptr<mast::Chain> Create_SIB (std::experimental::string_view      name,
                                           SelectorKind                        selectorKind,
                                           std::shared_ptr<mast::Register>     selectorRegister,
                                           MuxRegPlacement                     muxRegPlacement
                                          )
  {
    return Create_MIB(name, selectorKind, selectorRegister, muxRegPlacement);
  }


  std::shared_ptr<mast::Chain>           Create_1500_Wrapper (std::experimental::string_view name, uint32_t maxDerivations);

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
