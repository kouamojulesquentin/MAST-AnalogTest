#include <cctype>
#include <fstream>
#include <cassert>

#include "SystemModelBuilder.hpp"
#include "SIT_reader.hpp"
#include "SIT_parser.tab.hh"
#include "SIT_scanner.hpp"

using std::experimental::string_view;

using namespace mast;


SIT::SIT_Reader::SIT_Reader( std::shared_ptr<mast::SystemModel> sm)
{
 main_sm = sm;
 parsed_sut = nullptr;
 builder = make_shared<mast::SystemModelBuilder>(*main_sm);
}

bool
SIT::SIT_Reader::parse( string_view filename )
{
   assert( filename.data() != nullptr );

   std::ifstream in_file( filename.data() );
   if( ! in_file.good() )
   {
      return false;
   }
   return parse_helper( in_file );
}

bool
SIT::SIT_Reader::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return false;
   }
   //else
   return parse_helper( stream );
}


bool
SIT::SIT_Reader::parse_helper( std::istream &stream )
{
   try
   {
      scanner = make_shared<SIT_Scanner>(&stream);
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      return false;
   }

   try
   {
      parser = make_shared<SIT_Parser>(*scanner /* scanner */, *this /* driver */);
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" <<
         ba.what() << "), exiting!!\n";
      return false;
   }

   constexpr int accept = 0;
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!\n";
      return false;
   }
   return true;
}

void
SIT::SIT_Reader::add_newline()
{
   ++line;
   column=0;
}

void
SIT::SIT_Reader::add_column()
{
   ++column;
}


