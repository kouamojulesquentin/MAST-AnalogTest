#ifndef __SITSCANNER_HPP__
#define __SITSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "SIT_parser.tab.hh"
#include "location.hh"

namespace SIT{

class SIT_Scanner : public yyFlexLexer{
public:
   
   SIT_Scanner(std::istream *in) : yyFlexLexer(in)
   {
      loc = new SIT::SIT_Parser::location_type();
   };
   virtual ~SIT_Scanner() {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( SIT::SIT_Parser::semantic_type * const lval, 
              SIT::SIT_Parser::location_type *location );
   // YY_DECL defined in SIT_lexer.l
   // Method body created by flex in SIT_lexer.yy.cc


private:
   /* yyval ptr */
   SIT::SIT_Parser::semantic_type *yylval = nullptr;
   /* location ptr */
   SIT::SIT_Parser::location_type *loc    = nullptr;
};

} /* end namespace SIT */

#endif /* END __SITSCANNER_HPP__ */
