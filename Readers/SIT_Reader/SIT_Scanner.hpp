#ifndef __SITSCANNER_HPP__
#define __SITSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "SIT_parser.tab.hh"
#include "location.hh"

namespace SIT{

class SIT_Scanner final: public yyFlexLexer
{
public:

   SIT_Scanner(std::istream *in) : yyFlexLexer(in)
   {
      loc = new SIT::SIT_Parser::location_type();
      loc ->initialize();
   };

   ~SIT_Scanner()
   {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   int yylex(SIT_Parser::semantic_type* const lval, SIT_Parser::location_type* location);


private:
   SIT_Parser::semantic_type* yylval = nullptr;
   SIT_Parser::location_type* loc    = nullptr;
};

} /* end namespace SIT */

#endif /* END __SITSCANNER_HPP__ */
