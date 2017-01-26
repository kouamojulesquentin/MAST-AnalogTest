//===========================================================================
//                           UnresolvedPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UnresolvedPathSelector.hpp
//!
//! Declares class UnresolvedPathSelector
//!
//===========================================================================


#ifndef UnresolvedPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_
  #define UnresolvedPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines an Unresolved default path selector to be used during SIT parsing
//!
//! NB: the class is not functional, its members should NEVER be called

class DLL_EXPORT UnresolvedPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~UnresolvedPathSelector() = default;
  UnresolvedPathSelector() = default;

  std::shared_ptr<Register> dummyRegister;           
  BinaryVector dummy;

  bool IsActive (uint32_t /*pathIdentifier*/) const {return false;};
  uint32_t ActiveCount() const {return 0;};
  bool IsSelected (uint32_t /*pathIdentifier*/) const {return false;};
  bool IsSelectedAndActive (uint32_t /*pathIdentifier*/) const {return false;};
  void Accept   (SystemModelVisitor& /*visitor*/) {
  		THROW_LOGIC_ERROR("UnresolvedPathSelector should never be exerciced");
		return;}; 
  uint32_t SelectablePaths() const {return 0;};
  std::shared_ptr<const Register> AssociatedRegister() const {return nullptr;};
  const BinaryVector& SelectionValue (uint32_t /*pathIdentifier*/) const {return dummy;};
  std::string DebugSelectorInfo(bool /*onlyProperties*/) const { return "UnresolvedPathSelector"; };

  static BinaryVector AssociatedRegisterInitialValue(uint32_t /*pathsCount*/, SelectorProperty /*properties*/)
  {return BinaryVector(1, 0, SizeProperty::Fixed);}

  void Select   (uint32_t /*pathIdentifier*/) {return;};
  void Deselect (uint32_t /*pathIdentifier*/) {return;};

  std::experimental::string_view KindName() const { return "Unresolved"; }; //!< Returns readable type of selector

};
//
//  End of UnresolvedPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined UnresolvedPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

//===========================================================================
// End of UnresolvedPathSelector.hpp
//===========================================================================
