/*
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

%{
/* C++ string header, for string ops below */
#include <string>
#include <stdio.h>

/* Implementation of yyFlexScanner */
#include "ICL_Scanner.hpp"
#include "ICL_Parser.tab.hh"

#undef  YY_DECL
#define YY_DECL int ICL::ICL_Scanner::yylex( ICL::ICL_Parser::semantic_type * const lval, ICL::ICL_Parser::location_type *location )

/* typedef to make the returns for the tokens shorter */
using token = ICL::ICL_Parser::token;

/* define yyterminate as this instead of NULL */
//+#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
//+#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
/* handle locations */


int nlines = 1;
ICL::ICL_Parser::location_type* my_location;
%}

%option debug
%option warn nodefault
%option yyclass="ICL::ICL_Scanner"
%option noyywrap never-interactive nounistd
%option c++
%x COMMENT

%%
%{          /** Code executed at the beginning of yylex **/
    yylval = lval;
    my_location = loc;
%}

<COMMENT>{
[^*\n]*        /* consume anything that's not a '*' or newline */
"*"+[^*/\n]*   /* this pattern is similar to the previous one but
                           it also consumes 1 or more leading *'s. For example, if
                           the comment were "brad **** smiley" than the
                           first pattern would consume "brad " and this
                           pattern would consume "**** smiley". */
\n            { ++yylineno; }
"*"+"/"        { /* end of comment--resume initial state. Note that
                             this pattern specifies one or more "*"'s followed
                             by a "/", so it picks up comments ended by an
                             arbitrary number of *'s */
                          BEGIN(INITIAL);
                        }
}
<INITIAL>{
	"/*"         BEGIN(COMMENT);
	[ \r\t]+            { /* Skip blanks. */ }
	[/][/][^\n]* { /* Skip comments. */ }
	\n { yylineno++; }

  AccessLink                    return token::ACCESSLINK;
  AccessTogether                return token::ACCESSTOGETHER;
  ActivePolarity                return token::ACTIVEPOLARITY;
  ActiveSignals                 return token::ACTIVESIGNALS;
  AddressPort                   return token::ADDRESSPORT;
  AddressValue                  return token::ADDRESSVALUE;
  Alias                         return token::ALIAS;
  AllowBroadcastOnScanInterface return token::ALLOWBROADCASTONSCANINTERFACE;
  ['][bB][ \t]*[01Xx][01Xx_]*   return token::UNSIZED_BIN_NUM;
  ['][dD][ \t]*[0-9][0-9_]*     return token::UNSIZED_DEC_NUM;
  ['][hH][ \t]*[0-9A-Fa-fXx][0-9A-Fa-fXx_]* return token::UNSIZED_HEX_NUM;
  &                             return token::AND;
  Attribute                     return token::ATTRIBUTE;
  BSDLEntity                    return token::BSDLENTITIY;
  CaptureEnPort                 return token::CAPTUREENPORT;
  CaptureSource                 return token::CAPTURESOURCE;
  Chain                         return token::CHAIN;
  ClockMux                      return token::CLOCKMUX;
  ClockPort                     return token::CLOCKPORT;
  :                             return token::COLON;
  \,                            return token::COMMA;
  DataInPort                    return token::DATAINPORT;
  DataMux                       return token::DATAMUX;
  DataOutPort                   return token::DATAOUTPORT;
  DataRegister                  return token::DATAREGISTER;
  DefaultLoadValue              return token::DEFAULTLOADVALUE;
  DifferentialInvOf             return token::DIFFERENTIALINVOF;
  [$]                           return token::DOLLAR;
  \.                            return token::DOT;
  ::                            return token::DOUBLE_COLON;
  [<]D[>]                       return token::D_SUBST;
  Enable                        return token::ENABLE;
  Enum                          return token::ENUM;
  =                             return token::EQUAL;
  Falling                       return token::FALLING;
  FreqDivider                   return token::FREQDIVIDER;
  FreqMultiplier                return token::FREQMULTIPLIER;
  iApplyEndState                return token::IAPPLYENDSTATE;
  InputPort                     return token::INPUTPORT;
  Instance                      return token::INSTANCE;
  LaunchEdge                    return token::LAUNCHEDGE;
  \{                            return token::LEFT_BRACE;
  \[                            return token::LEFT_BRACKET;
  \(                            return token::LEFT_PAREN;
  LocalParameter                return token::LOCALPARAMETER;
  LogicSignal                   return token::LOGICSIGNAL;
  &&                            return token::LOGIC_AND;
  ==                            return token::LOGIC_EQUAL;
  [|][|]                        return token::LOGIC_OR;
  -                             return token::MINUS;
  Module                        return token::MODULE;
  ms                            return token::MSEC;
  NameSpace                     return token::NAMESPACE;
  !=                            return token::NOT_EQUAL;
  ns                            return token::NSEC;
  Of                            return token::OF;
  0                             return token::ZERO;
  1                             return token::ONE;
  OneHotDataGroup               return token::ONEHOTDATAGROUP;
  OneHotScanGroup               return token::ONEHOTSCANGROUP;
  Parameter                     return token::PARAMETER;
  %                             return token::PERCENT;
  Period                        return token::PERIOD;
  [|]                           return token::PIPE;
  [+]                           return token::PLUS;
  Port                          return token::PORT;
  [0-9][0-9_]*                  return token::POS_INT;
  ps                            return token::PSEC;
  ReadCallBack                  return token::READCALLBACK;
  ReadDataSource                return token::READDATASOURCE;
  ReadEnPort                    return token::READENPORT;
  RefEnum                       return token::REFENUM;
  ResetPort                     return token::RESETPORT;
  ResetValue                    return token::RESETVALUE;
  \}                            return token::RIGHT_BRACE;
  ]                             return token::RIGHT_BRACKET;
  [)]                           return token::RIGHT_PAREN;
  Rising                        return token::RISING;
  [<]R[>]                       return token::R_SUBST;
  ScanInPort                    return token::SCANINPORT;
  ScanInSource                  return token::SCANINSOURCE;
  ScanInterface                 return token::SCANINTERFACE;
  ScanMux                       return token::SCANMUX;
  ScanOutPort                   return token::SCANOUTPORT;
  ScanRegister                  return token::SCANREGISTER;
  s                             return token::SEC;
  SelectedBy                    return token::SELECTEDBY;
  SelectPort                    return token::SELECTPORT;
  ;                             return token::SEMICOLON;
  ShiftEnPort                   return token::SHIFTENPORT;
  [/]                           return token::SLASH;
  Source                        return token::SOURCE;
  \*                            return token::STAR;
  STD_1149_1_2001               return token::STD_1149_1_2001;
  STD_1149_1_2013               return token::STD_1149_1_2013;
  \"(([^\"\\])|\\\\|\\)*\"  {   return token::STRING;}
  TCKPort                       return token::TCKPORT;
  ~                             return token::TILDE;
  !                             return token::LOGIC_NOT;
  TMSPort                       return token::TMSPORT;
  ToCaptureEnPort               return token::TOCAPTUREENPORT;
  ToClockPort                   return token::TOCLOCKPORT;
  ToIRSelectPort                return token::TOIRSELECTPORT;
  ToResetPort                   return token::TORESETPORT;
  ToSelectPort                  return token::TOSELECTPORT;
  ToShiftEnPort                 return token::TOSHIFTENPORT;
  ToTCKPort                     return token::TOTCKPORT;
  ToTMSPort                     return token::TOTMSPORT;
  ToTRSTPort                    return token::TOTRSTPORT;
  ToUpdateEnPort                return token::TOUPDATEENPORT;
  TRSTPort                      return token::TRSTPORT;
  UpdateEnPort                  return token::UPDATEENPORT;
  us                            return token::USEC;
  UseNameSpace                  return token::USENAMESPACE;
  WriteCallBack                 return token::WRITECALLBACK;
  WriteDataSource               return token::WRITEDATASOURCE;
  WriteEnPort                   return token::WRITEENPORT;
  WriteEnSource                 return token::WRITEENSOURCE;
  [a-zA-Z][a-zA-Z0-9_]*         return token::SCALAR_ID;
  \^                            return token::XOR;
  . { LexerError("lex Unknown character"); }
}

%%
// . { printf("lex Unknown character = '%s'", yytext); yyerror("lex Unknown character"); }

