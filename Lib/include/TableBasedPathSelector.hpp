//===========================================================================
//                           TableBasedPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TableBasedPathSelector.hpp
//!
//! Declares class TableBasedPathSelector
//!
//===========================================================================


#ifndef TABLEBASEDPATHSELECTOR_H__154B9207_E7A2_474F_BC83_A7D6E097F025__INCLUDED_
  #define TABLEBASEDPATHSELECTOR_H__154B9207_E7A2_474F_BC83_A7D6E097F025__INCLUDED_

#include <vector>

namespace mast
{
//! Abstract base class for implementation of path selector that are based upon a LUTs
//! for selection and deselection values
//!
//! @note Path identifier are defined in range [1..nb_path]
class TableBasedPathSelector  : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~TableBasedPathSelector() = default;
  TableBasedPathSelector()  = delete;

  //! Constructs selection and deselection tables with pathCount entries
  //!
  TableBasedPathSelector(uint32_t pathCount);


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:

};
//
//  End of TableBasedPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined TABLEBASEDPATHSELECTOR_H__154B9207_E7A2_474F_BC83_A7D6E097F025__INCLUDED_

//===========================================================================
// End of TableBasedPathSelector.hpp
//===========================================================================
