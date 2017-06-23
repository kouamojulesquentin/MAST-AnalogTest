//===========================================================================
//                           AccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol.hpp
//!
//! Declares class AccessInterfaceProtocol
//!
//===========================================================================


#ifndef CALLBACKTRANSLATOR_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define CALLBACKTRANSLATOR_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "BinaryVector.hpp"
#include <experimental/string_view>

namespace mast
{
//! Defines Callbacks for AccessInterface Translators
//!
class CallbackTranslator 
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~CallbackTranslator() = default;

  virtual std::experimental::string_view KindName() const = 0;

  protected:
  CallbackTranslator()          = default;
  BinaryVector		ToSutVector;
  void* interfaceData;
  std::string CallbackId;
   
};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CALLBACKTRANSLATOR_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of CallbackTranslator.hpp
//===========================================================================
