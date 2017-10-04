//===========================================================================
//                           ICL_Scanner.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Scanner.hpp
//!
//! Declares ICL_Scanner class
//!
//===========================================================================


#ifndef ICL_SCANNER_H__6C28041B_784C_4118_28A3_422FDD91C7A__INCLUDED_
  #define ICL_SCANNER_H__6C28041B_784C_4118_28A3_422FDD91C7A__INCLUDED_

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "ICL_Parser.tab.hh"
#include "location.hh"

namespace ICL
{
//! Implements Cpp interface for ICL lexer
//!
//! @note It avoids having renamed copy of yyFlexLexer
//!
class ICL_Scanner final : public yyFlexLexer
{
  public:

  ~ICL_Scanner()
  {
    delete loc;
  }

  ICL_Scanner(std::istream& stream)
    : yyFlexLexer(&stream)
    , loc(new ICL::ICL_Parser::location_type())
  {
     loc->initialize();
  };

  // Import base class yylex methods (==> get rid of override virtual function warning)
  using FlexLexer::yylex;

  int yylex(ICL_Parser::semantic_type* const lval, ICL_Parser::location_type* location);

  private:
  ICL_Parser::semantic_type* yylval = nullptr;
  ICL_Parser::location_type* loc    = nullptr;
};
//
//  End of ICL_Scanner
//---------------------------------------------------------------------------


} /* end namespace ICL */

#endif  // not defined ICL_SCANNER_H__6C28041B_784C_4118_28A3_422FDD91C7A__INCLUDED_
