//===========================================================================
//                           SIT_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SIT_Reader.hpp
//!
//! Declares  class for managing parsing of SIT files
//!
//===========================================================================


#ifndef SIT_READER_H__947F16CC_2E70_465F_4486_B6BDF227A8A3__INCLUDED_
  #define SIT_READER_H__947F16CC_2E70_465F_4486_B6BDF227A8A3__INCLUDED_

#include "AppFunctionNameAndNode.hpp"
#include "PathSelector.hpp"
#include "SubModelPlaceHolder.hpp"

#include <string>
#include <vector>
#include <experimental/string_view>
#include <cstddef>
#include <istream>
#include <memory>
#include <iostream>
#include <map>
#include <queue>
#include <functional>
#include "Parser_Types.hpp"

namespace mast
{
  class SystemModelBuilder;
  class SystemModel;
  class SystemModelNode;
  class Register;
}

namespace SIT
{
class SIT_Parser;
class SIT_Scanner;

class SIT_Reader final
{
public:
  ~SIT_Reader() = default;

  SIT_Reader() = delete;
  SIT_Reader(std::shared_ptr<mast::SystemModel> sm);

  //! Parses a SIT file to construct a SystemModel
  //!
  void Parse(std::experimental::string_view filePath);

  //! Parses a SIT formatted excerpt to construct a SystemModel
  //!
  void ParseExcerpt(std::istream& stream);

  //! Parses a SIT formatted excerpt to construct a SystemModel
  //!
  void ParseExcerpt(const std::string& excerpt);

   //! Returns SystemModelNode build by the actual parser
   //!
   std::shared_ptr<mast::SystemModelNode> ParsedSystemModel() { return parsedTopNode; }

   //! Associations of PDL algorithm names to nodes
   //!
   const std::vector<mast::AppFunctionNameAndNode>& PDLAlgorithmNameToNodeAssociation() { return namesAndNodes; };

   //! Returns error message in case of failure, empty string otherwise
   //!
   std::string ErrorMessage() const { return m_errorMessage; }

   //! Identifies parts of the model that are not instanciated yet and where to insert (splice) them
   //!
   const std::vector<mast::SubModelPlaceHolder>& PlaceHolders() const { return placeHolders; }

private:  // Part used by SIT_Parser
  friend class SIT_Parser;

  std::vector<mast::AppFunctionNameAndNode>              namesAndNodes;      //!< Associations of algorithms name a node
  std::vector<mast::SubModelPlaceHolder>                 placeHolders;       //!< Represents sub-model to instantiate and splice in created (sub)model (from file or exerpt)
  std::map<std::string, std::shared_ptr<mast::Register>> declared_registers; //!< Created registers - kept to potentially associate to PathSelector (at end of parsing)
  std::queue<linker_information>                         unresolved_linkers; //!< Informations to create PathSelector associated with linker (register driving the selector may be yet unknown when the linker is created)

  std::shared_ptr<mast::SystemModelNode>    parsedTopNode;    //!< SystemModel tree build from SIT file
  std::shared_ptr<mast::SystemModel>        systemModel;
  std::shared_ptr<mast::SystemModelBuilder> builder;

  using RegisterCreator_t = std::function<std::shared_ptr<mast::Register>(const std::string&     selectorRegName,
                                                                          uint32_t               pathsCount,
                                                                          mast::SelectorProperty properties)>; //!< Function to create a register for a PathSelector

  std::map<std::string, RegisterCreator_t>  selector_register_creator;  //!< Creators to create a register for a PathSelector

private:

  void Parse_Impl(std::istream& stream);

  std::string m_errorMessage; //!< Error message build while parsing (empty when successful)
};

} // End of: namespace SIT


#endif  // not defined SIT_READER_H__947F16CC_2E70_465F_4486_B6BDF227A8A3__INCLUDED_
