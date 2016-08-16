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

SIT::SIT_Reader::SIT_Reader()
: parsed_sut ( std::make_unique<mast::SystemModel>())
{
}

bool
SIT::SIT_Reader::parse( const char * const filename )
{
   assert( filename != nullptr );
   std::ifstream in_file( filename );
   if( ! in_file.good() )
   {
       exit( EXIT_FAILURE );
   }
   parse_helper( in_file );
   return true;
}

bool
SIT::SIT_Reader::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return false;
   }
   //else
   parse_helper( stream ); 
   return true;
}


void 
SIT::SIT_Reader::parse_helper( std::istream &stream )
{
   
   delete(scanner);
   try
   {
      SystemModelBuilder builder_obj(*parsed_sut);
      builder = &builder_obj;
      scanner = new SIT::SIT_Scanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
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
      exit( EXIT_FAILURE );
   }
   const int accept( 0 );
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!\n";
   }
   return;
}

void 
SIT::SIT_Reader::add_upper()
{ 
   uppercase++; 
   chars++; 
   words++; 
}

void 
SIT::SIT_Reader::add_lower()
{ 
   lowercase++; 
   chars++; 
   words++; 
}

void 
SIT::SIT_Reader::add_word( const std::string &word )
{
   words++; 
   chars += word.length();
   for(const char &c : word ){
      if( islower( c ) )
      { 
         lowercase++; 
      }
      else if ( isupper( c ) ) 
      { 
         uppercase++; 
      }
   }
}

void 
SIT::SIT_Reader::add_newline()
{ 
   lines++; 
   chars++; 
}

void 
SIT::SIT_Reader::add_char()
{ 
   chars++; 
}


std::ostream& 
SIT::SIT_Reader::print( std::ostream &stream )
{
   stream << red  << "Results: " << norm << "\n";
   stream << blue << "Uppercase: " << norm << uppercase << "\n";
   stream << blue << "Lowercase: " << norm << lowercase << "\n";
   stream << blue << "Lines: " << norm << lines << "\n";
   stream << blue << "Words: " << norm << words << "\n";
   stream << blue << "Characters: " << norm << chars << "\n";
   return(stream);
}
