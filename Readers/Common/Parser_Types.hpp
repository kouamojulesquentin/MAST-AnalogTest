//===========================================================================
//                           Parser_Types.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Parser_Types.hpp
//!
//! Declares types shared by parsers
//!
//===========================================================================

#ifndef PARSER_TYPES_H__5A3DC243_5770_4F28_48BF_29298BCCAB8F__INCLUDED_
  #define PARSER_TYPES_H__5A3DC243_5770_4F28_48BF_29298BCCAB8F__INCLUDED_

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

namespace mast
{
  class SystemModelNode;
  class Linker;
  class AccessInterfaceTranslator;
  enum class SelectorProperty;
}


struct name_type
{
  std::uint8_t is_transparent;
  std::string  name;
};

struct node_list_type
{
  std::string                                         name;
  std::vector<std::shared_ptr<mast::SystemModelNode>> nodes;
};

//! Informations to create PathSelector associated with linker
//!
//! @note Register driving the selector may be yet unknown when the linker is created
//!
struct linker_information
{
  std::shared_ptr<mast::Linker> linker_node;
  uint32_t                      line              = 0;
  uint32_t                      beginColumn       = 0;
  uint32_t                      endColumn         = 0;
  uint32_t                      max_derivations   = 0;
  std::string                   selector_kind_name;
  std::string                   selector_reg_name;   //!< Name of register associated with the selector (driving the mux)
  std::string                   selector_parameters; //!< Optional parameters (for custom PathSelector)
  mast::SelectorProperty        selector_property = static_cast<mast::SelectorProperty>(0);
};

//! Informations to create a T-2-E translator protocol associated with a Translator node
//!
//! @note Register driving the selector may be yet unknown when the linker is created
//!
struct T_2_E_information
{
  std::shared_ptr<mast::AccessInterfaceTranslator> translator_node; //Translator node
  uint32_t                      line              = 0;
  uint32_t                      beginColumn       = 0;
  uint32_t                      endColumn         = 0;
  std::string                   translator_reg_name;   //!< Name of register associated with the protocol
};


#endif  // not defined PARSER_TYPES_H__5A3DC243_5770_4F28_48BF_29298BCCAB8F__INCLUDED_
//===========================================================================
// End of Parser_Types.hpp
//===========================================================================



