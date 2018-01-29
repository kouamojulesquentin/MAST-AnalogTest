//===========================================================================
//                           AST_BasedNumber.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_BasedNumber.hpp
//!
//! Declares AST_BasedNumber class
//!
//===========================================================================

#ifndef AST_BASEDNUMBER_H__A06E236E_68C8_441E_20AA_9390800D766C__INCLUDED_
  #define AST_BASEDNUMBER_H__A06E236E_68C8_441E_20AA_9390800D766C__INCLUDED_

#include "AST_Number.hpp"
#include "BinaryVector.hpp"

#include <string>

namespace Parsers
{
class AST_IntegerExpr;


//! Represents a number that is either an integer expression or a "based" number (vector)
//!
//! @note In ICL grammar, a based number (excerpt) is:
//!
//!         unsized_number : pos_int | UNSIZED_DEC_NUM | UNSIZED_BIN_NUM | UNSIZED_HEX_NUM
//!         sized_number :   sized_dec_num | sized_bin_num | sized_hex_num
//!         sized_dec_num :  size UNSIZED_DEC_NUM
//!         sized_bin_num :  size UNSIZED_BIN_NUM
//!         sized_hex_num :  size UNSIZED_HEX_NUM
//!
class AST_BasedNumber final : public AST_Number
{
  // ---------------- Public Methods
  //
  public:
  ~AST_BasedNumber() = default;
  AST_BasedNumber()  = delete;


  //! Build text representation of integer expression
  //!
  std::string AsText() const override;

  //! Returns number as a BinaryVector
  //!
  mast::BinaryVector AsBinaryVector(uint8_t targetCount = 0) const;

  //! Returns true when a size (bits count) is defined for the number
  //!
  bool IsSized() const override { return m_sizeExpr != nullptr; }

  //! Returns size expression
  //!
  AST_IntegerExpr*  SizeExpr() const { return m_sizeExpr; }

  //! Sets size expression
  //!
  void  SizeExpr (AST_IntegerExpr* sizeExpr) { m_sizeExpr = sizeExpr; }

  //! Returns size (only when sized)
  //!
  uint32_t Size() const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                                   // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_BasedNumber)(Parsers::Kind& kind, std::string&& digits);  // AST currently uses make_unit<T>() to create nodes

  // ---------------- Protected Methods
  //

  //! Initializes base class
  //!
  AST_BasedNumber(Kind kind, std::string&& digits);

  // ---------------- Private Fields
  //
  AST_IntegerExpr* m_sizeExpr = nullptr; //!< When not nullptr defines number bits count
  std::string      m_digits;             //!< Number base and digits
};
//
//  End of AST_BasedNumber class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_BASEDNUMBER_H__A06E236E_68C8_441E_20AA_9390800D766C__INCLUDED_
//===========================================================================
// End of AST_BasedNumber.hpp
//===========================================================================
