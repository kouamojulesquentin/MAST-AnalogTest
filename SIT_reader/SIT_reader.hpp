#ifndef __SITDRIVER_HPP__
#define __SITDRIVER_HPP__ 1

#include <string>
#include <experimental/string_view>
#include <cstddef>
#include <istream>
#include <memory>


namespace mast
{
  class SystemModelBuilder;
  class SystemModel;
  class SystemModelNode;
} // End of namespace mast

namespace SIT
{
class SIT_Parser;
class SIT_Scanner;

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
   bool parse( std::istream &iss );

   std::shared_ptr<mast::SystemModelNode> parsed_sut;

private:  // Part used by SIT_Parser
  friend class SIT_Parser;
  std::shared_ptr<mast::SystemModel>        main_sm;
  std::shared_ptr<mast::SystemModelBuilder> builder;

private:

   void add_newline();
   void add_column();
   bool parse_helper( std::istream &stream );

   std::size_t                  column = 0;
   std::size_t                  line   = 0;
   std::shared_ptr<SIT_Parser>  parser;
   std::shared_ptr<SIT_Scanner> scanner;
};

} /* end namespace SIT */
#endif /* END __SITDRIVER_HPP__ */
