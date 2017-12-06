//===========================================================================
//                           Parser_PublicData.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Parser_PublicData.hpp
//!
//! Declares Parser_PublicData class
//!
//===========================================================================


#ifndef PARSER_PUBLICDATA_H__73219124_792D_4E2E_5685_7323AE57E04E__INCLUDED_
  #define PARSER_PUBLICDATA_H__73219124_792D_4E2E_5685_7323AE57E04E__INCLUDED_

#include "Parser_Types.hpp"
#include "AppFunctionNameAndNode.hpp"
#include "SubModelPlaceHolder.hpp"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <queue>

namespace mast
{
  class SystemModel;
  class SystemModelNode;
  class Register;
}

namespace Parsers
{
//! Data updated by parsers and used by parsers driver
//!
struct Parser_PublicData final
{
  std::shared_ptr<mast::SystemModel>                     systemModel;        //!< SystemModel currently being built
  std::shared_ptr<mast::SystemModelNode>                 parsedTopNode;      //!< SystemModel tree build by parser
  std::vector<mast::AppFunctionNameAndNode>              namesAndNodes;      //!< Associations of algorithms name to nodes
  std::vector<mast::SubModelPlaceHolder>                 placeHolders;       //!< Represents sub-model to instantiate and splice in this build top node
  std::map<std::string, std::shared_ptr<mast::Register>> declared_registers; //!< Created registers - kept to potentially associate to PathSelector (at end of parsing)
  std::queue<linker_information>                         unresolved_linkers; //!< Informations to create PathSelector associated with linker (register driving the selector may be yet unknown when the linker is created)
};
//
//  End of Parser_PublicData class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined PARSER_PUBLICDATA_H__73219124_792D_4E2E_5685_7323AE57E04E__INCLUDED_
//===========================================================================
// End of Parser_PublicData.hpp
//===========================================================================
