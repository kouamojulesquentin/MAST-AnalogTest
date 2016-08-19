%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "SIT_scanner.hpp"
#include "SIT_parser.tab.hh"

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


#define KEYWORD_MAXSIZE 	20
#define N_KEYWORDS 		15

typedef struct {
        char name[KEYWORD_MAXSIZE];
        int kval;
        } keywords_type;

static keywords_type keywords_table []=
  {
    {"CHAIN"			,token::t_CHAIN}  /*1*/
   ,{"REGISTER"			,token::t_REGISTER}
   ,{"LINKER"			,token::t_LINKER}
   ,{"ACCESS_INTERFACE"		,token::t_ACCESS_INTERFACE}
   ,{"NO_iPrefix"		,token::t_TRANSPARENT}
   ,{"Hold_value"		,token::t_HOLD_VALUE}
   ,{"Bypass"			,token::t_BYPASS}
   ,{"SIB"			,token::t_SIB}
   ,{"POST"			,token::t_POST}
   ,{"PRE"			,token::t_PRE} /*10*/
   ,{"HIGH"			,token::t_HIGH}
   ,{"LOW"			,token::t_LOW} 
   ,{"REVERSE"			,token::t_REVERSE} 
   ,{"MIB"			,token::t_MIB} 
   ,{"WRAPPER_1500"		,token::t_1500_WRAPPER} 
   ,{"JTAG_TAP"			,token::t_JTAG_TAP} 

  };

static int find_keyword(char *s)
{
  int   l;
  keywords_type *pt;

  pt = NULL;
   
 for (l=0;l<N_KEYWORDS;l++)
   {
    if (strcmp(keywords_table[l].name,s)==0) 
     {
     pt=&(keywords_table[l]);
//    std::cout << "Found Token "<< keywords_table[l].name<<" \n";
     break;
     }
   }
  if (pt==NULL) return(-1);
  else return(pt->kval);
}

%}
upper_case_letter 	  [A-Z]
digit 			  [0-9]
special_character   	  [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character 	  [ \t]
format_effector		  [\t\v\r\l\f]
end_of_line		  \n
lower_case_letter 	  [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character	  ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character	  ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  {digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
extended_digit	   	  ({digit}|[a-fA-F])
base_specifier	  	  ("0B"|"0b"|"0O"|"0o"|"0X"|"0x")
based_integer	   	  {extended_digit}(_?{extended_digit})*
extended_letter_or_digit	  ({letter}|{digit}|"/"|"\_"|"\:")

/*MAST Binary vector*/
BV_base_specifier	("/B"|"/b"|"/O"|"/o"|"/X"|"/x")
BV_value		({extended_digit}|{BV_base_specifier})




%option debug
%option nodefault
%option yyclass="SIT::SIT_Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

{integer}			{
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
	      std::cout << "Found Based integer\n";
	       return( token::t_BASED_INTEGER );
            }
\"({extended_letter_or_digit})*\" {
              yylval->build< std::string >( yytext );
	       return( token::t_QUOTED_STRING );
 }

"{"	{
           return( token::t_START_HIERARCHY);
	 }
"}"	{
           return( token::t_END_HIERARCHY);
	 }
\:	{
           return( token::t_SEMICOLON);
	 }

\n          {
               loc->lines();
            }

\[	{ return( token::t_LeftBracket);}
\]	{ return( token::t_RightBracket);}
\,	{ return( token::t_Comma);}

.           {
            }
	    
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
