#include <cctype>
#include <experimental/string_view>
#include <fstream>
#include <cassert>
#include <memory>

#include "SystemModelBuilder.hpp"
#include "ICL_Reader.hpp"
#include "ICL_Parser.tab.hh"
#include "ICL_Scanner.hpp"
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


ICL::ICL_Reader::ICL_Reader(std::shared_ptr<mast::SystemModel> sm)
  : systemModel (sm)
//+  , builder     (*systemModel)
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

void ICL::ICL_Reader::Parse(string_view filename)
{
  CHECK_PARAMETER_NOT_EMPTY(filename, "Must specify a valid file path");

  std::ifstream iclFileStream(filename.data());

  CHECK_TRUE  (iclFileStream.good(), "Cannot open file: "s            .append(filename.cbegin(), filename.cend()));
  CHECK_FALSE (iclFileStream.eof(),  "Cannot parse empty ICL file: "s .append(filename.cbegin(), filename.cend()));

  try
  {
    Parse_Impl(iclFileStream);
  }
  catch(ParserException& exc)
  {
    if (exc.filePath.empty())
    {
      exc.filePath.append(filename.cbegin(), filename.cend());
      throw exc;
    }
    throw;
  }
}

void ICL::ICL_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid ICL stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse ICL from empty stream");

  ICL_Scanner scanner(stream);
  ICL_Parser  parser(scanner, *this /* driver */);

  auto succeeded = parser.parse() == 0;

  CHECK_TRUE(succeeded, "Failed to parse ICL stream");

  LOG(INFO) << "ICL has been parsed successfully";
}

//===========================================================================
// End of ICL_Reader.cpp
//===========================================================================
