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
  : main_sm (sm)
  , builder (make_shared<mast::SystemModelBuilder>(*main_sm))
{
  #define MAKE_LAMBDA(selectorClass, forcedProperty)                                                                          \
  [model = main_sm](const string& selectorRegName, uint32_t pathsCount, SelectorProperty selectorProperty)                    \
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

void SIT::SIT_Reader::parse(string_view filename)
{
  CHECK_PARAMETER_NOT_EMPTY(filename, "Must specify a valid file path");

  std::ifstream sitFileStream(filename.data());

  CHECK_TRUE  (sitFileStream.good(), "Cannot open file: "s            .append(filename.cbegin(), filename.cend()));
  CHECK_FALSE (sitFileStream.eof(),  "Cannot parse empty SIT file: "s .append(filename.cbegin(), filename.cend()));

  try
  {
    Parse_Impl(sitFileStream);
  }
  catch(ParserException& exc)
  {
    if (exc.filePath.empty())
    {
      exc.filePath.append(filename.cbegin(), filename.cend());
      error_message = exc.Message();
      throw exc;
    }

    error_message = exc.Message();
    throw;
  }
}


void SIT::SIT_Reader::parse(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid stream: ");
  CHECK_FALSE (stream.eof(),  "Cannot parse empty SIT: ");

  try
  {
    Parse_Impl(stream);
  }
  catch(ParserException& exc)
  {
    error_message = exc.Message();
    throw;
  }
}


void SIT::SIT_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid SIT stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse SIT from empty stream");

  SIT_Scanner scanner(&stream);
  SIT_Parser  parser(scanner, *this /* driver */);

  auto succeeded = parser.parse() == 0;

  CHECK_TRUE(succeeded, "Failed to parse SIT stream");

  LOG(INFO) << "SIT has been parsed successfully";
}

//===========================================================================
// End of SIT_Reader.cpp
//===========================================================================
