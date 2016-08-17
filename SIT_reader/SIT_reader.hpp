#ifndef __SITDRIVER_HPP__
#define __SITDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "SIT_scanner.hpp"
#include "SIT_parser.tab.hh"
#include "SystemModelBuilder.hpp"

using namespace mast;
using std::shared_ptr;

namespace SIT{

class SIT_Reader{
public:
   SIT_Reader( std::shared_ptr<mast::SystemModel> sm);

   virtual ~SIT_Reader();
   
   /** 
    * parse - parse from a file
    * @param filename - valid string with input file
    */
   bool parse( const char * const filename );
   /** 
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
   bool parse( std::istream &iss );

   void add_upper();
   void add_lower();
   void add_word( const std::string &word );
   void add_newline();
   void add_char();

   std::ostream& print(std::ostream &stream);

   std::shared_ptr<mast::SystemModel> main_sm;
   std::shared_ptr<mast::SystemModelNode> parsed_sut;
   mast::SystemModelBuilder *builder;

private:

   void parse_helper( std::istream &stream );

   std::size_t  chars      = 0;
   std::size_t  words      = 0;
   std::size_t  lines      = 0;
   std::size_t  uppercase  = 0;
   std::size_t  lowercase  = 0;
   SIT::SIT_Parser  *parser  = nullptr;
   SIT::SIT_Scanner *scanner = nullptr;
   
   const std::string red   = "\033[1;31m";
   const std::string blue  = "\033[1;36m";
   const std::string norm  = "\033[0m";
};

} /* end namespace SIT */
#endif /* END __SITDRIVER_HPP__ */
