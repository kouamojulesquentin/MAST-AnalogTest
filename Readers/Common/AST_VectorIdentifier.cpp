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
#include "AST_Builder.hpp"

#include <sstream>

using std::string;
using std::tuple;
using std::make_tuple;

using namespace Parsers;

//! Text representation of identifier with its index(es)
//!
string AST_VectorIdentifier::AsText () const
{
  std::ostringstream os;

  os << BaseName();

  if (!IsScalar())
  {
    os << RangeAsText();
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
  if (m_right.empty())
  {
    return 1u;
  }

  uint32_t left  = static_cast<uint32_t>(std::stoul(m_left));
  uint32_t right = static_cast<uint32_t>(std::stoul(m_right));

  return (left > right) ? 1u + (left  - right)
                        : 1u + (right - left);
}
//
//  End of: AST_VectorIdentifier::BitsCount
//---------------------------------------------------------------------------



//! Returns true when Parameter is defined using Parameter reference(s)
//!
//! @note This is useful to know when it should be uniquified
bool AST_VectorIdentifier::HasParameterRef () const
{
  return    AST_Parameter::HasParameterRef(m_left)
         || AST_Parameter::HasParameterRef(m_right);
}
//
//  End of: AST_VectorIdentifier::HasParameterRef
//---------------------------------------------------------------------------



//! Returns range indexes
//!
//! @return [has_range, left_index, right_index]
tuple<bool, uint32_t, uint32_t> AST_VectorIdentifier::Range () const
{
  if (m_left.empty())
  {
    return make_tuple(false, 0u, 0u);
  }

  uint32_t left = static_cast<uint32_t>(std::stoul(m_left));

  if (m_right.empty())
  {
    return make_tuple(true, left, left);
  }

  uint32_t right = static_cast<uint32_t>(std::stoul(m_right));

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

  os << '[' << m_left;

  if (!m_right.empty())
  {
    os << ":" << m_right;
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
  if (AST_Parameter::HasParameterRef(m_left))
  {
    m_left = AST_Parameter::ReplacedParameters(m_left, parameters);
  }

  if (AST_Parameter::HasParameterRef(m_right))
  {
    m_right = AST_Parameter::ReplacedParameters(m_right, parameters);
  }
}
//
//  End of: AST_VectorIdentifier::Resolve
//---------------------------------------------------------------------------


//! Returns uniquified clone
//!
AST_VectorIdentifier* AST_VectorIdentifier::UniquifiedClone (AST_Builder& astBuilder) const
{
  return astBuilder.Clone_VectorIdentifier(this);
}
//
//  End of: AST_VectorIdentifier::UniquifiedClone
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_VectorIdentifier.cpp
//===========================================================================
