#include <cctype>
#include <experimental/string_view>
#include <fstream>
#include <cassert>
#include <memory>

#include "SystemModelBuilder.hpp"
#include "SIT_Reader.hpp"
#include "SIT_Parser.tab.hh"
#include "SIT_Scanner.hpp"
#include "ParserException.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "Utility.hpp"

#include "g3log/g3log.hpp"

using std::experimental::string_view;
using std::string;

using std::make_shared;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;


SIT::SIT_Reader::SIT_Reader(std::shared_ptr<mast::SystemModel> sm)
  : systemModel (sm)
  , builder (make_shared<mast::SystemModelBuilder>(*systemModel))
{
  #define MAKE_LAMBDA(selectorClass, forcedProperty)                                                                          \
  [model = systemModel](const string& selectorRegName, uint32_t pathsCount, SelectorProperty selectorProperty)                    \
  {                                                                                                                           \
    auto registerInitialValue = selectorClass::AssociatedRegisterInitialValue(pathsCount, selectorProperty | forcedProperty); \
    return model->CreateRegister (selectorRegName, registerInitialValue, true);                                               \
  }

  selector_register_creator["Binary"]         = MAKE_LAMBDA(DefaultBinaryPathSelector, SelectorProperty::CanSelectNone);
  selector_register_creator["Binary_noidle"]  = MAKE_LAMBDA(DefaultBinaryPathSelector, SelectorProperty::CannotSelectNone);
  selector_register_creator["One_Hot"]        = MAKE_LAMBDA(DefaultOneHotPathSelector, SelectorProperty::CanSelectNone);
  selector_register_creator["One_Hot_noidle"] = MAKE_LAMBDA(DefaultOneHotPathSelector, SelectorProperty::CannotSelectNone);
  selector_register_creator["N_Hot"]          = MAKE_LAMBDA(DefaultNHotPathSelector,   SelectorProperty::CanSelectNone);
  selector_register_creator["N_Hot_noidle"]   = MAKE_LAMBDA(DefaultNHotPathSelector,   SelectorProperty::CannotSelectNone);

  #undef MAKE_LAMBDA
}

//! Parses a SIT file to construct a SystemModel
//!
//! @param filePath SIT file path
//!
void SIT::SIT_Reader::Parse(string_view filePath)
{
  CHECK_PARAMETER_NOT_EMPTY(filePath, "Must specify a valid file path");

  std::ifstream sitFileStream(filePath.data());

  CHECK_TRUE  (sitFileStream.good(), "Cannot open file: "s            .append(filePath.cbegin(), filePath.cend()));
  CHECK_FALSE (sitFileStream.eof(),  "Cannot parse empty SIT file: "s .append(filePath.cbegin(), filePath.cend()));

  try
  {
    Parse_Impl(sitFileStream);
  }
  catch(ParserException& exc)
  {
    if (exc.filePath.empty())
    {
      exc.filePath.append(filePath.cbegin(), filePath.cend());
      m_errorMessage = exc.Message();
      throw exc;
    }

    m_errorMessage = exc.Message();
    throw;
  }
}


void SIT::SIT_Reader::ParseExcerpt(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid stream: ");
  CHECK_FALSE (stream.eof(),  "Cannot parse empty SIT: ");

  try
  {
    Parse_Impl(stream);
  }
  catch(ParserException& exc)
  {
    m_errorMessage = exc.Message();
    throw;
  }
}


//! Parses a SIT formatted excerpt to construct a SystemModel
//!
//! @param excerpt  SIT formatted excerpt to parse
//!
void SIT::SIT_Reader::ParseExcerpt(const string& excerpt)
{
  CHECK_PARAMETER_NOT_EMPTY(excerpt, "Cannot parse empty string");

  try
  {
    std::stringstream stream(excerpt);
    Parse_Impl(stream);
  }
  catch(ParserException& exc)
  {
    m_errorMessage = exc.Message();
    throw;
  }
}



//! Parses a SIT formatted excerpt to construct a SystemModel
//!
//! @param stream   Input stream to get SIT excerpt to parse
//!
void SIT::SIT_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid SIT stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse SIT from empty stream");

  SIT_Scanner scanner(stream);
  SIT_Parser  parser(scanner, *this /* driver */);

  auto succeeded = parser.parse() == 0;

  CHECK_TRUE(succeeded, "Failed to parse SIT stream");

  LOG(INFO) << "SIT has been parsed successfully";
}

//===========================================================================
// End of SIT_Reader.cpp
//===========================================================================
