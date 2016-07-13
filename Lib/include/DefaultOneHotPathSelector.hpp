//===========================================================================
//                           DefaultOneHotPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultOneHotPathSelector.hpp
//!
//! Declares class DefaultOneHotPathSelector
//!
//===========================================================================


#ifndef DEFAULTONEHOTPATHSELECTOR_H__7D498E23_55FC_4C91_E58F_8C35F47D6A65__INCLUDED_
  #define DEFAULTONEHOTPATHSELECTOR_H__7D498E23_55FC_4C91_E58F_8C35F47D6A65__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using one hot encoding
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
class DLL_EXPORT DefaultOneHotPathSelector : public DefaultTableBasedPathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultOneHotPathSelector() = default;
  DefaultOneHotPathSelector()  = delete;
  DefaultOneHotPathSelector(std::shared_ptr<Register> associatedRegister,
                            uint32_t                  pathsCount,
                            SelectorProperty          properties = SelectorProperty::One_Hot_Default);

  virtual void Select (uint32_t pathIdentifier) override; //!< Request activation of the specified path

  using TablesType = DefaultTableBasedPathSelector::TablesType;

  static TablesType CreateSelectTable   (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties);
  static TablesType CreateDeselectTable (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties);

  // ---------------- Protected Methods
  //
  protected:
  static void CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount);
};
//
//  End of DefaultOneHotPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast





#endif  // not defined DEFAULTONEHOTPATHSELECTOR_H__7D498E23_55FC_4C91_E58F_8C35F47D6A65__INCLUDED_
//===========================================================================
// End of DefaultOneHotPathSelector.hpp
//===========================================================================
