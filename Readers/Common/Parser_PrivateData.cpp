//===========================================================================
//                           Parser_PrivateData.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Parser_PrivateData.cpp
//!
//! Implements helper to ease usage of Parser_PrivateData
//!
//===========================================================================

#include "Parser_PrivateData.hpp"
#include "PathSelector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
//+#include "Utility.hpp"

using std::string;

using Parsers::Parser_PrivateData;
using namespace mast;

//! Initializes parsers "private" data
//!
Parser_PrivateData::Parser_PrivateData (std::shared_ptr<mast::SystemModel> p_systemModel)
  : systemModel (p_systemModel)
  , builder     (*systemModel)
{
  #define MAKE_LAMBDA(selectorClass, forcedProperty)                                                                          \
  [model = systemModel](const string& selectorRegName, uint32_t pathsCount, SelectorProperty selectorProperty)                \
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
//
//  End of: Parser_PrivateData::Parser_PrivateData
//---------------------------------------------------------------------------






//===========================================================================
// End of Parser_PrivateData.cpp
//===========================================================================
