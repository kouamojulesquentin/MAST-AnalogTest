#include <cctype>
#include <fstream>
#include <cassert>

#include "SIT_reader.hpp"
#include "SystemModelBuilder.hpp"
using namespace mast;

SIT::SIT_Reader::~SIT_Reader()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

SIT::SIT_Reader::SIT_Reader( std::shared_ptr<mast::SystemModel> sm)
{
 main_sm = sm;
 parsed_sut = nullptr;
}

bool
SIT::SIT_Reader::parse( const char * const filename )
{
   assert( filename != nullptr );
   std::ifstream in_file( filename );
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
   
   delete(scanner);
   try
   {
      SystemModelBuilder builder_obj(*main_sm);
      builder = &builder_obj;
      scanner = new SIT::SIT_Scanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      return false;
   }
   
   delete(parser); 
   try
   {
      parser = new SIT::SIT_Parser( (*scanner) /* scanner */, 
                                  (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" << 
         ba.what() << "), exiting!!\n";
      return false;
   }
   const int accept( 0 );
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
   line++; 
   column=0; 
}

void 
SIT::SIT_Reader::add_column()
{ 
   column++; 
}


