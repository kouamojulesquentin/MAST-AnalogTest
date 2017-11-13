//===========================================================================
//                           Parser_PrivateData.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Parser_PrivateData.hpp
//!
//! Declares Parser_PublicParams struct
//!
//===========================================================================


#ifndef PARSER_PRIVATEDATA_H__AD36B305_91A8_4CB9_BBBE_7969DEF939C0__INCLUDED_
  #define PARSER_PRIVATEDATA_H__AD36B305_91A8_4CB9_BBBE_7969DEF939C0__INCLUDED_

#include "SystemModelBuilder.hpp"
#include <cctype>
#include <string>
#include <map>
#include <memory>
#include <functional>

namespace mast
{
  class SystemModelBuilder;
  class SystemModel;
  class SystemModelNode;
  class Register;
}


namespace Parsers
{
//! Data used by parsers but created and provided by "Readers"
//!
struct Parser_PrivateData
{
  Parser_PrivateData() = delete;
  Parser_PrivateData(std::shared_ptr<mast::SystemModel> p_systemModel);

  using RegisterCreator_t = std::function<std::shared_ptr<mast::Register>(const std::string&     selectorRegName,
                                                                          uint32_t               pathsCount,
                                                                          mast::SelectorProperty properties)>; //!< Function to create a register for a PathSelector

  std::map<std::string,              RegisterCreator_t> selector_register_creator; //!< Creators to create a register for a PathSelector
  std::shared_ptr<mast::SystemModel> systemModel;                                  //!< SystemModel being built
  mast::SystemModelBuilder           builder;                                      //!< Helper to create sub-model
};
//
//  End of Parser_PrivateData class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined PARSER_PRIVATEDATA_H__AD36B305_91A8_4CB9_BBBE_7969DEF939C0__INCLUDED_
//===========================================================================
// End of Parser_PrivateData.hpp
//===========================================================================



