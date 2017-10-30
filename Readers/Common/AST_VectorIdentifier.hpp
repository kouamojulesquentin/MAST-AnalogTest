//===========================================================================
//                           AST_VectorIdentifier.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_VectorIdentifier.hpp
//!
//! Declares AST_VectorIdentifier class
//!
//===========================================================================


#ifndef AST_VECTORIDENTIFIER_H__2EFCBB8C_1366_48A4_A89B_AF16972E3E__INCLUDED_
  #define AST_VECTORIDENTIFIER_H__2EFCBB8C_1366_48A4_A89B_AF16972E3E__INCLUDED_

#include "AST_Identifier.hpp"
#include <experimental/string_view>

namespace Parsers
{
//! Specifies name with single or range index(es)
//!
class AST_VectorIdentifier final : public AST_Identifier
{
  // ---------------- Public Methods
  //
  public:
  ~AST_VectorIdentifier() = default;
  AST_VectorIdentifier()  = delete;

  std::experimental::string_view LeftIndex()  const { return m_left;  } //!< Returns identifier left index
  std::experimental::string_view RightIndex() const { return m_right; } //!< Returns identifier right index

  std::experimental::string_view BaseName()    const { return m_identifier; } //!< Text representation of identifier base name - without index(es)
  std::string                    RangeAsText() const;                         //!< Text representation of only index(es)
  std::string                    AsText()      const;                         //!< Text representation of identifier with its index(es)


  bool IsScalar()    const { return  m_right.empty() && m_left.empty(); }      //!< Returns true when both left and right indexes are empty (equivalent to ScalarIdentifier)
  bool IsSingleBit() const { return  m_right.empty() || (m_left == m_right); } //!< Returns true when it represents a single bit
  bool IsMultiBits() const { return !m_right.empty() && (m_left != m_right); } //!< Returns true when it represents a multiple bits

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_VectorIdentifier)(std::experimental::string_view&,
                                              std::experimental::string_view&,
                                              std::experimental::string_view&); // AST currently uses make_unit<T>() to create nodes

  AST_VectorIdentifier(std::experimental::string_view identifier,
                       std::experimental::string_view leftIndex,
                       std::experimental::string_view rightIndex = "")
    : AST_Identifier (Kind::VectorIdentifier)
    , m_identifier   (identifier)
    , m_left         (leftIndex)
    , m_right        (rightIndex)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string m_identifier; //!< Textual representation of identifier
  std::string m_left;       //!< Left index
  std::string m_right;      //!< Right index (can be empty for single bit identifier)
};
//
//  End of AST_VectorIdentifier class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_VECTORIDENTIFIER_H__2EFCBB8C_1366_48A4_A89B_AF16972E3E__INCLUDED_

//===========================================================================
// End of AST_VectorIdentifier.hpp
//===========================================================================
