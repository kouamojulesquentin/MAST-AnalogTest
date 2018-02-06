//===========================================================================
//                           AST_VectorIdentifier.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_VectorIdentifier.cpp
//!
//! Implements class AST_VectorIdentifier
//!
//===========================================================================

#include "AST_VectorIdentifier.hpp"
#include "AST_Parameter.hpp"
#include "AST_IntegerExpr.hpp"
#include "AST_Builder.hpp"

#include <sstream>

using std::string;
using std::tuple;
using std::make_tuple;

using namespace Parsers;


//! Constructor...
AST_VectorIdentifier::AST_VectorIdentifier (std::string&& identifier, AST_IntegerExpr* leftIndex, AST_IntegerExpr* rightIndex)
  : AST_Identifier (Kind::VectorIdentifier, std::move(identifier))
  , m_left         (leftIndex)
  , m_right        (rightIndex)
{
  // ---------------- Make sure that left side define the index (or none for scalar identifier)
  //
  if ((m_left == nullptr) && (m_right != nullptr))
  {
    std::swap(m_left, m_right);
  }
}
//
//  End of: AST_VectorIdentifier::AST_VectorIdentifier
//---------------------------------------------------------------------------



//! Returns text representation of identifier with its index(es)
//!
//! @note If it is, still, defined with parameter reference, then they are displayed as in ICL,
//!       otherwise expression are evaluated for left/right indices
string AST_VectorIdentifier::AsText () const
{
  std::ostringstream os;

  os << BaseName();

  if (!IsScalar())
  {
    if (HasParameterRef())
    {
      os << RangeAsText();
    }
    else
    {
      os << '[' << std::to_string(m_left->Evaluate());

      if (m_right != nullptr)
      {
        os << ":" << std::to_string(m_right->Evaluate());
      }

      os << ']';
    }
  }

  return os.str();
}

//
//  End of: AST_VectorIdentifier::AsText
//---------------------------------------------------------------------------



//! Returns bits count
//!
uint32_t AST_VectorIdentifier::BitsCount () const
{
  if (IsMultiBits())
  {
    uint32_t left  = m_left->Evaluate();
    uint32_t right = m_right->Evaluate();

    return (left > right) ? 1u + (left  - right)
                          : 1u + (right - left);
  }

  return 1u;
}
//
//  End of: AST_VectorIdentifier::BitsCount
//---------------------------------------------------------------------------



//! Returns true when Parameter is defined using Parameter reference(s)
//!
//! @note This is useful to know when it should be uniquified
bool AST_VectorIdentifier::HasParameterRef () const
{
  if ((m_left != nullptr) && m_left->HasParameterRef())
  {
    return true;
  }

  if ((m_right != nullptr) && m_right->HasParameterRef())
  {
    return true;
  }

  return false;
}
//
//  End of: AST_VectorIdentifier::HasParameterRef
//---------------------------------------------------------------------------



//! Returns identifier left index
//!
//! @note Do not call when it is a scalar identifier
//!
uint32_t AST_VectorIdentifier::LeftIndex () const
{
  CHECK_VALUE_NOT_NULL(m_left, "Cannot request left index when none is defined");

  return m_left->Evaluate();
}
//
//  End of: AST_VectorIdentifier::LeftIndex
//---------------------------------------------------------------------------


//! Returns identifier right index
//!
//! @note Do not call when it is a scalar identifier
//!
uint32_t AST_VectorIdentifier::RightIndex () const
{
  CHECK_VALUE_NOT_NULL(m_right, "Cannot request right index when none is defined");

  return m_right->Evaluate();
}
//
//  End of: AST_VectorIdentifier::RightIndex
//---------------------------------------------------------------------------



//! Returns range indexes
//!
//! @return [has_range, left_index, right_index]
tuple<bool, uint32_t, uint32_t> AST_VectorIdentifier::Range () const
{
  if (IsScalar())
  {
    return make_tuple(false, 0u, 0u);
  }

  uint32_t left = m_left->Evaluate();

  if (IsSingleBit())
  {
    return make_tuple(true, left, left);
  }

  uint32_t right = m_right->Evaluate();

  return make_tuple(true, left, right);
}
//
//  End of: AST_VectorIdentifier::Range
//---------------------------------------------------------------------------


//! Text representation of only index(es)
//!
string AST_VectorIdentifier::RangeAsText () const
{
  std::ostringstream os;

  CHECK_FALSE(IsScalar(), "Vector identifier must be initialized at least with one index (representing a single bit) to get its range");

  os << '[' << m_left->AsText();

  if (m_right != nullptr)
  {
    os << ":" << m_right->AsText();
  }

  os << ']';
  return os.str();
}
//
//  End of: AST_VectorIdentifier::RangeAsText
//---------------------------------------------------------------------------



//! Replaces parameter references with their actual value, then resolve range expressions
//!
void AST_VectorIdentifier::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  if ((m_left != nullptr) && m_left->HasParameterRef())
  {
    m_left->Resolve(parameters);
  }

  if ((m_right != nullptr) && m_right->HasParameterRef())
  {
    m_right->Resolve(parameters);
  }
}
//
//  End of: AST_VectorIdentifier::Resolve
//---------------------------------------------------------------------------


//! Returns uniquified clone
//!
AST_VectorIdentifier* AST_VectorIdentifier::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_VectorIdentifier(this);

  if ((m_left != nullptr) && m_left->HasParameterRef())
  {
    clone->m_left = m_left->UniquifiedClone(astBuilder);
  }

  if ((m_right != nullptr) && m_right->HasParameterRef())
  {
    clone->m_right = m_right->UniquifiedClone(astBuilder);
  }

  return clone;
}
//
//  End of: AST_VectorIdentifier::UniquifiedClone
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_VectorIdentifier.cpp
//===========================================================================
