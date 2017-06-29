#include <cctype>
#include <experimental/string_view>
#include <fstream>
#include <cassert>
#include <memory>

#include "SystemModelBuilder.hpp"
#include "SIT_reader.hpp"
#include "SIT_parser.tab.hh"
#include "SIT_scanner.hpp"
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

bool SIT::SIT_Reader::parse(string_view filename)
{
  CHECK_PARAMETER_NOT_EMPTY(filename, "Must specify a valid file path");

  std::ifstream sitFile(filename.data());

  CHECK_TRUE(sitFile.good(), "Cannot open file: "s.append(filename.cbegin(), filename.cend()));

  return parse_helper(sitFile);
}

bool SIT::SIT_Reader::parse(std::istream& stream)
{
   if (!stream.good() && stream.eof())
   {
     return false;
   }

   return parse_helper(stream);
}


bool SIT::SIT_Reader::parse_helper(std::istream& stream)
{
  scanner = make_shared<SIT_Scanner>(&stream);
  parser  = make_shared<SIT_Parser>(*scanner, *this /* driver */);

  auto success = parser->parse() == 0;
  if (success)
  {
    LOG(DEBUG) << "SIT has been parsed successfully";
  }

  return success;
}

//===========================================================================
// End of SIT_reader.cpp
//===========================================================================
