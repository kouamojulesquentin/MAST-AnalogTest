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


//===========================================================================
// End of AST_VectorIdentifier.cpp
//===========================================================================
