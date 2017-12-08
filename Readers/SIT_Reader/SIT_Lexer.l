%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */
#include "SIT_Scanner.hpp"
#include "SIT_Parser.tab.hh"

#undef  YY_DECL
#define YY_DECL int SIT::SIT_Scanner::yylex( SIT::SIT_Parser::semantic_type * const lval, SIT::SIT_Parser::location_type *location )

/* typedef to make the returns for the tokens shorter */
using token = SIT::SIT_Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
/* handle locations */
/*int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yyloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;
*/

namespace
{
  struct keywords
  {
   static constexpr size_t KEYWORD_MAXSIZE = 20u;

   char name[KEYWORD_MAXSIZE];
   int  keyword;
  };

  static keywords keywords_table []=
  {
    {"CHAIN",            token::t_CHAIN},             // 01
    {"REGISTER",         token::t_REGISTER},          // 02
    {"LINKER",           token::t_LINKER},            // 03
    {"ACCESS_INTERFACE", token::t_ACCESS_INTERFACE},  // 04
    {"NOT_IN_PATH",      token::t_TRANSPARENT},       // 05
    {"Hold_value",       token::t_HOLD_VALUE},        // 06
    {"Bypass",           token::t_BYPASS},            // 07
    {"SIB",              token::t_SIB},               // 08
    {"POST",             token::t_POST},              // 09
    {"PRE",              token::t_PRE},               // 10
    {"HIGH",             token::t_HIGH},              // 11
    {"LOW",              token::t_LOW},               // 12
    {"REVERSE",          token::t_REVERSE},           // 13
    {"MIB",              token::t_MIB},               // 14
    {"WRAPPER_1500",     token::t_1500_WRAPPER},      // 15
    {"JTAG_TAP",         token::t_JTAG_TAP},          // 16
    {"PDL",              token::t_PDL},               // 17
    {"INSTANCE",         token::t_INSTANCE},          // 18
    {"OF",               token::t_OF},                // 19
    {"TRANSLATOR",       token::t_TRANSLATOR},        // 20
  };
}


static int find_keyword(const char* word)
{
  for (const auto& entry : keywords_table)
  {
    if (strcmp(entry.name, word) == 0)
    {
 //    std::cout << "Found Token " << entry.name<<" \n";
      return entry.keyword;
    }
  }
 //    std::cerr << "Unrecognized Token: " << word << "\n";
  return -1;
}

int nlines = 1;
SIT::SIT_Parser::location_type *my_location;

%}
upper_case_letter         [A-Z]
digit                     [0-9]
special_character         [\#\&\'\\\=\_\|]
space_character           [ \t]
format_effector           [\t\v\r\l\f]
end_of_line               \n
lower_case_letter         [a-z]
other_special_character   [\$\@\`]
operators                 [\/\*\+\-\^\~\%]
punctuations              [\!\?\,\.\;\:]
angle_brackets            [\<\>]
square_brackets           [\[\]]
round_brackets            [\(\)]
curly_brackets            [\{\}]
brackets                  ({angle_brackets}|{square_brackets}|{round_brackets}|{curly_brackets})

graphic_character         ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character   ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter                    ({upper_case_letter}|{lower_case_letter})
letter_or_digit           ({letter}|{digit})
decimal_literal           {integer}(\.{integer})?({exponent})?
integer                   {digit}(_?{digit})*
exponent                  ([eE][-+]?{integer})
base                      {integer}
extended_digit            ({digit}|[a-fA-F])
base_specifier            ("0B"|"0b"|"0O"|"0o"|"0X"|"0x")
based_integer             {extended_digit}(_?{extended_digit})*
extended_letter_or_digit  ({letter}|{digit}|"/"|"\_"|"\:")
string_content            ({brackets}|{punctuations}|{operators}|{graphic_character})
/*MAST Binary vector*/
BV_base_specifier ("/B"|"/b"|"/O"|"/o"|"/X"|"/x")
BV_value    ({extended_digit}|{BV_base_specifier})




%option debug
%option nodefault
%option yyclass="SIT::SIT_Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
      my_location = loc;
%}

{integer}     {
      yylval->build< std::uint32_t> (atoi(yytext));
      return ( token::t_DecimalLiteral );
      }

{letter}(_?{letter_or_digit})*   {
               /**
                * Section 10.1.5.1 of the 3.0.2 Bison Manual says the
                * following should work:
                * yylval.build( yytext );
                * but it doesn't.
                * ref: http://goo.gl/KLn0w2
                */
               yylval->build< std::string >( yytext );
         int itoken;

         itoken = find_keyword(yytext);
         if (itoken == -1) return ( token::t_WORD );
          else return itoken;
    }
{base_specifier}({extended_digit}(_?{extended_digit}))*  {
         yylval->build< std::string >( yytext );
         return( token::t_BASED_INTEGER );
}
\"({string_content})*\" {
         yylval->build< std::string >( yytext );
         return( token::t_QUOTED_STRING );
 }

"{"  { return( token::t_START_HIERARCHY); }
"}"  { return( token::t_END_HIERARCHY);   }
\:   { return( token::t_SEMICOLON);       }
"."  { return( token::t_Dot);             }
"\\" { return( token::t_Backslash);       }
"/"  { return( token::t_Slash);           }
\n  {
      nlines++;
      loc->lines();
    }
\[  { return( token::t_LeftBracket);}
\]  { return( token::t_RightBracket);}
\(  { return (token::t_LeftParenthesis);}
\)  { return (token::t_RightParenthesis);}
\,  { return( token::t_Comma);}
.   { }

%%

/*{BV_value}(_?{BV_element})*  {
              yylval->build< std::string >( yytext );
        std::cout << "Found Binary Vector \n";
               return( token::t_BINARY_VECTOR);
             }
*/

/*{base_specifier}({extended_digit}(_?{extended_digit}))*  {
              yylval->build< std::string >( yytext );
        std::cout << "Found Based integer\n";
         return( token::t_BASED_INTEGER );
            }*/
