//===========================================================================
//                           AST_Number.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Number.hpp
//!
//! Declares AST_Number class
//!
//===========================================================================

#ifndef AST_NUMBER_H__9A9DFE82_138C_40D5_A1A1_A14F2F1EE4C5__INCLUDED_
  #define AST_NUMBER_H__9A9DFE82_138C_40D5_A1A1_A14F2F1EE4C5__INCLUDED_

#include "AST_SimpleNode.hpp"

#include <string>

namespace Parsers
{
//! Represents a number that is either an integer expression or a "based" number (vector)
//!
//! @note In ICL grammar, a number expression (excerpt) is:
//!         number :         unsized_number | sized_number | integer_expr ;
//!         unsized_number : pos_int | UNSIZED_DEC_NUM | UNSIZED_BIN_NUM | UNSIZED_HEX_NUM ;
//!         sized_number :   sized_dec_num | sized_bin_num | sized_hex_num;
//!
class AST_Number : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_Number() = default;
  AST_Number()  = delete;

  //! Returns true when a size (bits count) is defined for the number
  //!
  virtual bool IsSized() const { return false; }

  //! Returns true when the number representation is inverted
  //!
  //! @note Invertion (i.e. bits toggling) can be used in context of convertion to BinaryVector
  //!
  bool IsInverted() const { return m_isInverted; }

  //! Set whether the number representation is inverted
  //!
  //! @note Invertion (i.e. bits toggling) can be used in context of convertion to BinaryVector
  //!
  void IsInverted (bool isInverted) { m_isInverted = isInverted; }

  // ---------------- Protected Methods
  //

  //! Initializes base class
  //!
  explicit AST_Number(Kind kind)
    : AST_SimpleNode (kind)
  {
  }

  // ---------------- Private Fields
  //
  bool m_isInverted = false;  //!< Tells whether the value should be inverted (when converted to BinaryVector)
};
//
//  End of AST_Number class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_NUMBER_H__9A9DFE82_138C_40D5_A1A1_A14F2F1EE4C5__INCLUDED_
//===========================================================================
// End of AST_Number.hpp
//===========================================================================
