#ifndef __SITDRIVER_HPP__
#define __SITDRIVER_HPP__ 1

#include "AppFunctionNameAndNode.hpp"
#include "PathSelector.hpp"

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
#include "SIT_types.h"

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

//! Exception thrown internally by SIT_Parser
//!
//! @note Should not yet been used externally
class ParserException : public std::runtime_error
{
  public:
  ParserException(const std::string& what_arg)
   : std::runtime_error(what_arg)
  { }

  ParserException(const char* what_arg)
   : std::runtime_error(what_arg)
  { }
};

class SIT_Reader
{
public:
   virtual ~SIT_Reader() = default;
   SIT_Reader() = delete;
   SIT_Reader( std::shared_ptr<mast::SystemModel> sm);


   /**
    * parse - parse from a file
    * @param filename - valid string with input file
    */
   bool parse(std::experimental::string_view filename);

   /**
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
   bool parse(std::istream &iss);

   //! Returns SystemModelNode build from SIT
   //!
   std::shared_ptr<mast::SystemModelNode> ParsedSystemModel() { return parsed_sut; }

   //! Returns error message in case of failure, empty string otherwise
   //!
   std::string ErrorMessage() const { return error_message; }

   std::vector<mast::AppFunctionNameAndNode> namesAndNodes; //!< Associations of algorithms name a node

private:  // Part used by SIT_Parser
  friend class SIT_Parser;


  std::map<std::string, std::shared_ptr<mast::Register>> declared_registers; //!< Created registers - kept to potentially associate to PathSelector (at end of parsing)
  std::queue<linker_information>                         unresolved_linkers; //!< Informations to create PathSelector associated with linker (register driving the selector may be yet unknown when the linker is created)

  std::shared_ptr<mast::SystemModelNode>    parsed_sut;    //!< SystemModel tree build from SIT file
  std::shared_ptr<mast::SystemModel>        main_sm;
  std::shared_ptr<mast::SystemModelBuilder> builder;

  using RegisterCreator_t = std::function<std::shared_ptr<mast::Register>(const std::string&     selectorRegName,
                                                                          uint32_t               pathsCount,
                                                                          mast::SelectorProperty properties)>; //!< Function to create a register for a PathSelector

  std::map<std::string, RegisterCreator_t>  selector_register_creator;  //!< Creators to create a register for a PathSelector

private:

   void add_newline();
   void add_column();
   bool parse_helper( std::istream &stream );

   std::size_t                  column = 0;
   std::size_t                  line   = 0;
   std::shared_ptr<SIT_Parser>  parser;
   std::shared_ptr<SIT_Scanner> scanner;
   std::string                  error_message;  //!< Error message build while parsing SIT (empty when successful)
};

} /* end namespace SIT */
#endif /* END __SITDRIVER_HPP__ */
