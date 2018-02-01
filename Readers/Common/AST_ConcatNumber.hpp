//===========================================================================
//                           AST_ConcatNumber.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ConcatNumber.hpp
//!
//! Declares AST_ConcatNumber class
//!
//===========================================================================

#ifndef AST_CONCATNUMBER_H__5C92AC14_5B83_407D_E29E_F5C87CC92F91__INCLUDED_
  #define AST_CONCATNUMBER_H__5C92AC14_5B83_407D_E29E_F5C87CC92F91__INCLUDED_

#include "AST_SimpleNode.hpp"
#include "BinaryVector.hpp"
#include <string>
#include <vector>

namespace Parsers
{
class AST_Builder;
class AST_Number;
class AST_Parameter;

//! Represents a concatenation of numbers that can be either "based" number
//! or integer expression.
//!
class AST_ConcatNumber final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ConcatNumber() = default;

  //! Returns BinaryVector resulting of concatenation of each numbers
  //!
  mast::BinaryVector AsBinaryVector(uint32_t targetCount = 0) const;

  //! Builds text representation of concatenated numbers
  //!
  std::string AsText() const override;

  //! Returns true when a size (bits count) is defined for each of concatenated number
  //!
  bool IsFullySized() const;

  //! Appends a number after previous ones
  //!
  void Append(AST_Number* number) { m_numbers.push_back(number); }

  //! Returns concatenated numbers
  //!
  const std::vector<AST_Number*>& Numbers() const { return m_numbers; }

  //! Returns true when there is a parameter reference in any of concatenated number
  //!
  bool HasParameterRef () const override;

  //! Replaces parameter references by their actual number
  //!
  void Resolve (const std::vector<AST_Parameter*>& parameters);

  //! Returns uniquified clone
  //!
  AST_ConcatNumber* UniquifiedClone(AST_Builder& astBuilder) const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                              // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ConcatNumber)(std::vector<Parsers::AST_Number*>&&);  // AST currently uses make_unit<T>() to create nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ConcatNumber)();                                     // AST currently uses make_unit<T>() to create nodes

  explicit AST_ConcatNumber(std::vector<AST_Number*>&& numbers)
    : AST_SimpleNode (Kind::ConcatenatedNumbers)
    , m_numbers      (std::move(numbers))
  {
  }


  //! Creates a yet empty (numbers must then be appended)
  //!
  AST_ConcatNumber()
    : AST_SimpleNode (Kind::ConcatenatedNumbers)
  {
  }

  // ---------------- Private Fields
  //
  private:

  std::vector<AST_Number*> m_numbers; //!< Concatenated numbers
};
//
//  End of AST_ConcatNumber class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_CONCATNUMBER_H__5C92AC14_5B83_407D_E29E_F5C87CC92F91__INCLUDED_

//===========================================================================
// End of AST_ConcatNumber.hpp
//===========================================================================
