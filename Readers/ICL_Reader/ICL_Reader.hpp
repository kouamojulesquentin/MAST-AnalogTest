//===========================================================================
//                           ICL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Reader.hpp
//!
//! Declares  class for managing parsing of ICL files
//!
//===========================================================================


#ifndef ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
  #define ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_

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
#include "Parser_Types.h"

namespace mast
{
  class SystemModelBuilder;
  class SystemModel;
  class SystemModelNode;
  class Register;
}

namespace ICL
{
class ICL_Reader final
{
public:
   ~ICL_Reader() = default;
   ICL_Reader() = delete;
   ICL_Reader(std::shared_ptr<mast::SystemModel> sm);


   //! Parses a ICL file to construct a SystemModel
   //!
  void Parse(std::experimental::string_view filePath);

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

private:  // Part used by ICL_Parser
  friend class ICL_Parser;

  std::vector<mast::AppFunctionNameAndNode>              namesAndNodes;      //!< Associations of algorithms name a node
  std::vector<mast::SubModelPlaceHolder>                 placeHolders;       //!< Represents sub-model to instantiate and splice in created (sub)model (from file or exerpt)
  std::map<std::string, std::shared_ptr<mast::Register>> declared_registers; //!< Created registers - kept to potentially associate to PathSelector (at end of parsing)
  std::queue<linker_information>                         unresolved_linkers; //!< Informations to create PathSelector associated with linker (register driving the selector may be yet unknown when the linker is created)

  std::shared_ptr<mast::SystemModelNode>    parsedTopNode;    //!< SystemModel tree build from ICL file
  std::shared_ptr<mast::SystemModel>        systemModel;
//+  mast::SystemModelBuilder                  builder;

  using RegisterCreator_t = std::function<std::shared_ptr<mast::Register>(const std::string&     selectorRegName,
                                                                          uint32_t               pathsCount,
                                                                          mast::SelectorProperty properties)>; //!< Function to create a register for a PathSelector

  std::map<std::string, RegisterCreator_t>  selector_register_creator;  //!< Creators to create a register for a PathSelector

private:

  void Parse_Impl(std::istream& stream);

  std::string m_errorMessage; //!< Error message build while parsing (empty when successful)
};

} // End of: namespace ICL


#endif  // not defined ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
