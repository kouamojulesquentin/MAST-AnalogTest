#include <cctype>
#include <fstream>
#include <cassert>
#include <memory>

#include "g3log/g3log.hpp"
#include "Utility.hpp"
#include "SystemModelBuilder.hpp"
#include "SIT_reader.hpp"
#include "SIT_parser.tab.hh"
#include "SIT_scanner.hpp"

using std::experimental::string_view;

using std::make_shared;
using namespace mast;


SIT::SIT_Reader::SIT_Reader( std::shared_ptr<mast::SystemModel> sm)
{
 main_sm    = sm;
 parsed_sut = nullptr;
 builder    = make_shared<mast::SystemModelBuilder>(*main_sm);
}

bool
SIT::SIT_Reader::parse(string_view filename)
{
  if (filename.empty())
  {
    LOG(ERROR_LVL) << "Must specify a valid file path";
    return false;
  }

  std::ifstream sitFile(filename.data());
  if (!sitFile.good())
  {
    LOG(WARNING) << "Cannot open file:" << filename;
    return false;
  }

  return parse_helper(sitFile);
}

bool
SIT::SIT_Reader::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return false;
   }

   return parse_helper( stream );
}


bool
SIT::SIT_Reader::parse_helper( std::istream &stream )
{
  try
  {
    try
    {
       scanner = make_shared<SIT_Scanner>(&stream);
    }
    catch( std::bad_alloc &ba )
    {
       std::cerr << "Failed to allocate scanner: ("
                 << ba.what() << "), exiting!!\n";
       return false;
    }

    try
    {
       parser = make_shared<SIT_Parser>(*scanner /* scanner */, *this /* driver */);
    }
    catch( std::bad_alloc &ba )
    {
       std::cerr << "Failed to allocate parser: ("
                 << ba.what() << "), exiting!!\n";
       return false;
    }

    constexpr int accept = 0;
    if (parser->parse() == accept)
    {
      return true;
    }
  }
  catch(ParserException& exc) // Do nothing because it is the normal way parser error are reported
  {
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Got unexpected std::exception while parsing: " << exc.what();
  }
  catch (...)
  {
    LOG(ERROR_LVL) << "Got unknown type of exception";
  }

  LOG(ERROR_LVL) << "Parse failed!!";
  return false;
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


