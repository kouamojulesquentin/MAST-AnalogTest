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

%skeleton "lalr1.cc"
%require  "3.0"
%debug
%locations
%defines
%define api.namespace {ICL}
%define parser_class_name {ICL_Parser}
%{
%}

%code requires
{
#include <tuple>
namespace ICL
{
  class ICL_Reader;
  class ICL_Scanner;
}
namespace Parsers
{
  class  AST;
  class  AST_Node;
  class  AST_Module;
  struct Parser_PublicData;
  struct Parser_PrivateData;
}

using std::string;

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { ICL_Scanner&                 scanner      }
%parse-param { Parsers::Parser_PublicData&  driverPublic }
%parse-param { Parsers::Parser_PrivateData& myData}
%parse-param { Parsers::AST&                ast}

%code
{
// ---------------- Includes for all driver functions
//
#include "Parser_PublicData.hpp"
#include "Parser_PrivateData.hpp"
#include "ParserException.hpp"
#include "AST.hpp"
#include "AST_ScanRegister.hpp"
#include "ICL_Reader.hpp"
#include "ICL_Scanner.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <iostream>
#include <fstream>
#include <experimental/string_view>

using std::vector;
using std::tuple;
using std::experimental::string_view;

using std::make_unique;
using std::make_tuple;

using namespace std::experimental::literals::string_view_literals;

#define STREAM_LOCATION(line, beginCol, endCol) "Line "  << line << ":" << beginCol << "-" << endCol << ": "
#define STREAM_MY_LOCATION                         STREAM_LOCATION(my_location->begin.line, my_location->begin.column, my_location->end.column)
#define STREAM_NODE_NAME(nodeKind, nodeName)       nodeKind << " node \"" << nodeName << "\" "

#define ERROR_MESSAGE(msg)                  std::ostringstream msg; msg
#define THROW_SYNTAX_ERROR(msg)             throw syntax_error(*my_location, msg.str())
#define THROW_SYNTAX_ERROR_AT_LOC(msg, loc) throw syntax_error(loc, msg.str())

#undef yylex
#define yylex scanner.yylex

#define THROW_PARSER_ERROR(msg) THROW_IMPL(ParserException, msg)

extern int                             nlines;
extern ICL::ICL_Parser::location_type* my_location;

namespace
{
//+  ICL::location MakeLocation(const linker_information& linkerInfo)
//+  {
//+    auto locBegin = ICL::position(nullptr, linkerInfo.line, linkerInfo.beginColumn);
//+    auto locEnd   = ICL::position(nullptr, linkerInfo.line, linkerInfo.endColumn);
//+    auto location = ICL::location(locBegin, locEnd);
//+    return location;
//+  }
} // End of unnamed namespace

} /*end of %code section*/

%define api.value.type variant
%define parse.assert

%type <std::string> SCALAR_ID
%type <std::string> instance_name
%type <std::string> module_name
%type <std::string> namespace_name
%type <std::string> scanInterfaceChain_name
%type <std::string> scanInterface_name

%type <std::string> parameter_ref
%type <std::string> index

%type <std::tuple<string, string>>         range                // Left & Right indexes
%type <std::tuple<string, string>>         index_or_range       // Left & Right indexes (Right can be empty)
%type <std::tuple<string, string, string>> alias_name           // Name, Left & Right indexes (Right can be empty)
%type <std::tuple<string, string, string>> vector_id            // Name, Left & Right indexes (Right can be empty)
%type <std::tuple<string, string, string>> oneHotScanGroup_name // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> port_name            // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> register_name        // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> scanMux_name         // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> scanInPort_name      // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> scanOutPort_name     // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> scanRegister_name    // Name, Left & Right indexes (Left & Right can be empty)
%type <std::tuple<string, string, string>> reg_port_signal_id   // Name, Left & Right indexes (Left & Right can be empty)


%type <std::string> POS_INT
%type <std::string> pos_int
%type <std::string> integer_expr
%type <std::string> integer_expr_arg
%type <std::string> integer_expr_paren
%type <std::string> integer_expr_lvl1
%type <std::string> integer_expr_lvl1_b
%type <std::string> plus_or_minus
%type <std::string> integer_expr_lvl2
%type <std::string> integer_expr_lvl2_b
%type <std::string> star_or_slash_or_percent

%type <Parsers::AST_Module*>            module_def
%type <Parsers::AST_Node*>              accessLink_def
%type <Parsers::AST_Node*>              accessLinkGeneric_def
%type <Parsers::AST_Node*>              accessLink1149_def
%type <Parsers::AST_Node*>              alias_def
%type <Parsers::AST_Node*>              attribute_def
%type <Parsers::AST_Node*>              clockMux_def
%type <Parsers::AST_Node*>              dataMux_def
%type <Parsers::AST_Node*>              dataRegister_def
%type <Parsers::AST_Node*>              enum_def
%type <Parsers::AST_Node*>              instance_def
%type <Parsers::AST_Node*>              localParameter_def
%type <Parsers::AST_Node*>              logicSignal_def
%type <Parsers::AST_Node*>              module_item
%type <Parsers::AST_Node*>              nameSpace_def
%type <Parsers::AST_Node*>              oneHotDataGroup_def
%type <Parsers::AST_Node*>              oneHotScanGroup_def
%type <Parsers::AST_Node*>              parameter_def
%type <Parsers::AST_Node*>              scanInterface_def
%type <Parsers::AST_Node*>              scanMux_def
%type <Parsers::AST_Node*>              scanRegister_def
%type <Parsers::AST_Node*>              useNameSpace_def

%type <Parsers::AST_Node*>              addressPort_def
%type <Parsers::AST_Node*>              captureEnPort_def
%type <Parsers::AST_Node*>              clockPort_def
%type <Parsers::AST_Node*>              dataInPort_def
%type <Parsers::AST_Node*>              dataOutPort_def
%type <Parsers::AST_Node*>              port_def
%type <Parsers::AST_Node*>              readEnPort_def
%type <Parsers::AST_Node*>              resetPort_def
%type <Parsers::AST_Node*>              scanInPort_def
%type <Parsers::AST_Node*>              scanOutPort_def
%type <Parsers::AST_Node*>              selectPort_def
%type <Parsers::AST_Node*>              shiftEnPort_def
%type <Parsers::AST_Node*>              tckPort_def
%type <Parsers::AST_Node*>              tmsPort_def
%type <Parsers::AST_Node*>              toCaptureEnPort_def
%type <Parsers::AST_Node*>              toClockPort_def
%type <Parsers::AST_Node*>              toIRSelectPort_def
%type <Parsers::AST_Node*>              toResetPort_def
%type <Parsers::AST_Node*>              toSelectPort_def
%type <Parsers::AST_Node*>              toShiftEnPort_def
%type <Parsers::AST_Node*>              toTckPort_def
%type <Parsers::AST_Node*>              toTmsPort_def
%type <Parsers::AST_Node*>              toTrstPort_def
%type <Parsers::AST_Node*>              toUpdateEnPort_def
%type <Parsers::AST_Node*>              trstPort_def
%type <Parsers::AST_Node*>              updateEnPort_def
%type <Parsers::AST_Node*>              writeEnPort_def



%type <std::vector<Parsers::AST_Node*>> module_items

%token ACCESSLINK
%token ACCESSTOGETHER
%token ACTIVEPOLARITY
%token ACTIVESIGNALS
%token ADDRESSPORT
%token ADDRESSVALUE
%token ALIAS
%token ALLOWBROADCASTONSCANINTERFACE
%token AND
%token ATTRIBUTE
%token BSDLENTITIY
%token CAPTUREENPORT
%token CAPTURESOURCE
%token CHAIN
%token CLOCKMUX
%token CLOCKPORT
%token COLON
%token COMMA
%token DATAINPORT
%token DATAMUX
%token DATAOUTPORT
%token DATAREGISTER
%token DEFAULTLOADVALUE
%token DIFFERENTIALINVOF
%token DOLLAR
%token DOT
%token DOUBLE_COLON
%token D_SUBST
%token ENABLE
%token ENUM
%token EQUAL
%token FALLING
%token FREQDIVIDER
%token FREQMULTIPLIER
%token IAPPLYENDSTATE
%token INPUTPORT
%token INSTANCE
%token LAUNCHEDGE
%token LEFT_BRACE
%token LEFT_BRACKET
%token LEFT_PAREN
%token LOCALPARAMETER
%token LOGICSIGNAL
%token LOGIC_AND
%token LOGIC_EQUAL
%token LOGIC_OR
%token MINUS
%token MODULE
%token MSEC
%token NAMESPACE
%token NOT_EQUAL
%token NSEC
%token OF
%token ONE
%token ONEHOTDATAGROUP
%token ONEHOTSCANGROUP
%token PARAMETER
%token PERCENT
%token PERIOD
%token PIPE
%token PLUS
%token PORT
%token POS_INT
%token PSEC
%token READCALLBACK
%token READDATASOURCE
%token READENPORT
%token REFENUM
%token RESETPORT
%token RESETVALUE
%token RIGHT_BRACE
%token RIGHT_BRACKET
%token RIGHT_PAREN
%token RISING
%token R_SUBST
%token SCALAR_ID
%token SCANINPORT
%token SCANINSOURCE
%token SCANINTERFACE
%token SCANMUX
%token SCANOUTPORT
%token SCANREGISTER
%token SEC
%token SELECTEDBY
%token SELECTPORT
%token SEMICOLON
%token SHIFTENPORT
%token SLASH
%token SOURCE
%token STAR
%token STD_1149_1_2001
%token STD_1149_1_2013
%token STRING
%token TCKPORT
%token TILDE
%token LOGIC_NOT
%token TMSPORT
%token TOCAPTUREENPORT
%token TOCLOCKPORT
%token TOIRSELECTPORT
%token TORESETPORT
%token TOSELECTPORT
%token TOSHIFTENPORT
%token TOTCKPORT
%token TOTMSPORT
%token TOTRSTPORT
%token TOUPDATEENPORT
%token TRSTPORT
%token UNSIZED_BIN_NUM
%token UNSIZED_DEC_NUM
%token UNSIZED_HEX_NUM
%token UPDATEENPORT
%token USEC
%token USENAMESPACE
%token WRITECALLBACK
%token WRITEDATASOURCE
%token WRITEENPORT
%token WRITEENSOURCE
%token XOR
%token ZERO
%token END 0 "end of file"


%start icl_source

%%

//size : pos_int | parameter_ref ;
pos_int :
  POS_INT { $$ = std::move($1); }
| ONE     { $$ = "1"; }
| ZERO    { $$ = "0"; }
;

number : UNSIZED_DEC_NUM | UNSIZED_BIN_NUM | UNSIZED_HEX_NUM
       | integer_expr UNSIZED_DEC_NUM
       | integer_expr UNSIZED_BIN_NUM
       | integer_expr UNSIZED_HEX_NUM
       | integer_expr ;

// 6.3.12
vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
{
  // vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
  auto& baseName = $[SCALAR_ID];
  auto& range    = $[index_or_range];
  auto  combined = make_tuple(std::move(baseName), std::move(std::get<0>(range)), std::move(std::get<1>(range)));
  $$ = std::move(combined);
}
;

index_or_range :
  index { $$ = make_tuple(std::move($[index]), ""s); }
| range { $$ = std::move($1); }
;

index : integer_expr { $$ = std::move($1); };
range : index[LEFT] COLON index[RIGHT]
{
  // range : index[LEFT] COLON index[RIGHT]
  auto& left  = $[LEFT];
  auto& right = $[RIGHT];
  $$          = make_tuple(std::move(left), std::move(right));
}
;


integer_expr : integer_expr_lvl1 { $$ = std::move($1); };

integer_expr_lvl1 :
  integer_expr_lvl2
  {
    // integer_expr_lvl1 : integer_expr_lvl2
    $$ = std::move($1);
  }
| integer_expr_lvl2 integer_expr_lvl1_b
  {
    // integer_expr_lvl1 : integer_expr_lvl2 integer_expr_lvl1_b
    //
    auto combined = $[integer_expr_lvl2].append($[integer_expr_lvl1]);
    $$ = std::move(combined);
  }
;
integer_expr_lvl1_b : plus_or_minus integer_expr_lvl1
{
  // integer_expr_lvl1_b : plus_or_minus integer_expr_lvl1
  //
  auto combined = $[plus_or_minus].append($[integer_expr_lvl1]);
  $$ = std::move(combined);
}
;

plus_or_minus :
  PLUS  { $$ = " + "; }
| MINUS { $$ = " - "; }
;

integer_expr_lvl2 :
  integer_expr_arg
  {
    // integer_expr_lvl2 : integer_expr_arg
    $$ = std::move($1);
  }
| integer_expr_arg integer_expr_lvl2_b
  {
    // integer_expr_lvl2 : integer_expr_arg integer_expr_lvl2_b
    auto combined = $[integer_expr_arg].append($[integer_expr_lvl2_b]);
    $$ = std::move(combined);
  }
;

integer_expr_lvl2_b : star_or_slash_or_percent integer_expr_lvl2
{
  // integer_expr_lvl2_b : star_or_slash_or_percent integer_expr_lvl2
  //
  auto op   = $[star_or_slash_or_percent];
  auto expr = $[integer_expr_lvl2];
  $$ = op.append(expr);
}
;

star_or_slash_or_percent :
  STAR    { $$ = " * "; }
| SLASH   { $$ = " // "; }
| PERCENT { $$ = " % "; }
;

integer_expr_paren : LEFT_PAREN integer_expr RIGHT_PAREN
{
  // integer_expr_paren : LEFT_PAREN integer_expr RIGHT_PAREN
  //
  auto combined = "("s.append($[integer_expr]).append(")");
  $$ = std::move(combined);
}
;

integer_expr_arg :
  integer_expr_paren { $$ = std::move($1); }
| pos_int            { $$ = std::move($1); }
| parameter_ref      { $$ = std::move($1); }
;

parameter_ref : DOLLAR SCALAR_ID
{
  auto combined = "$"s.append($[SCALAR_ID]);
  $$ = std::move(combined);
}
;

concat_number : number | TILDE number | concat_number COMMA number | concat_number COMMA TILDE number;
//semantic rules prevents inverting unsized numbers and having more than one
//unsized number within a concat_number. See section 6.4.10.
concat_number_list : concat_number | concat_number_list PIPE concat_number ;
pin_id : instance_name DOT pin_id | instance_name DOT port_name  ;

port_name :
  SCALAR_ID
  {
    // port_name : SCALAR_ID
    auto& name = $[SCALAR_ID];
    $$ = make_tuple(std::move(name), ""s, ""s);
  }
| vector_id
  {
    // port_name : vector_id
    $$ = $[vector_id];
  }
;

register_name :
  SCALAR_ID
  {
    // register_name : SCALAR_ID
    auto& name = $[SCALAR_ID];
    $$ = make_tuple(std::move(name), ""s, ""s);
  }
| vector_id
  {
    // register_name : vector_id
    $$ = $[vector_id];
  }
;

reg_port_signal_id :
  SCALAR_ID
  {
    // reg_port_signal_id : SCALAR_ID
    auto& name = $[SCALAR_ID];
    $$ = make_tuple(std::move(name), ""s, ""s);
  }
| vector_id
  {
    // reg_port_signal_id : vector_id
    $$ = $[vector_id];
  }
;

instance_name  : SCALAR_ID { $$ = $[SCALAR_ID]; };
namespace_name : SCALAR_ID { $$ = $[SCALAR_ID]; };
module_name    : SCALAR_ID { $$ = $[SCALAR_ID]; };


signal : number | reg_port_signal_id | pin_id  ;
x_signal : signal | TILDE signal ;
data_signal : signal | TILDE signal ;
scan_signal : signal | TILDE signal ;
//clock_signal : signal | TILDE signal ;
//tck_signal : signal ;
//tms_signal : signal ;
//trst_signal : signal ;
//shiftEn_signal : signal ;
captureEn_signal : concat_data_signal ;
updateEn_signal : concat_data_signal ;
concat_reset_signal : x_signal | concat_reset_signal COMMA x_signal;
concat_data_signal : x_signal | concat_data_signal COMMA x_signal;
concat_scan_signal : x_signal | concat_scan_signal COMMA x_signal;
concat_clock_signal : x_signal | concat_clock_signal COMMA x_signal;
//concat_tck_signal : x_signal | concat_tck_signal COMMA x_signal;
concat_shiftEn_signal : x_signal | concat_shiftEn_signal COMMA x_signal;
//concat_captureEn_signal : x_signal | concat_captureEn_signal COMMA x_signal;
//concat_updateEn_signal : x_signal | concat_updateEn_signal COMMA x_signal;
concat_tms_signal : signal | concat_tms_signal COMMA signal;
concat_trst_signal : x_signal | concat_trst_signal COMMA x_signal;

// 6.4.2
icl_source : iclSource_items | icl_source iclSource_items;
iclSource_items : nameSpace_def | useNameSpace_def | module_def;

// 6.4.3
nameSpace_def :
  NAMESPACE namespace_name SEMICOLON
  {
    $$ = nullptr;
  }
| NAMESPACE SEMICOLON
  {
    $$ = nullptr;
  }
;

// 6.4.4
useNameSpace_def :
  USENAMESPACE namespace_name SEMICOLON
  {
    $$ = nullptr;
  }
| USENAMESPACE SEMICOLON
  {
    $$ = nullptr;
  }
;

// 6.4.5
module_def :
  MODULE module_name LEFT_BRACE module_items RIGHT_BRACE
  {
    // module_def : MODULE module_name LEFT_BRACE module_items RIGHT_BRACE
    auto name = $[module_name];

    $$ = ast.Create_Module(name, std::move($[module_items]));
  }
|
  MODULE module_name LEFT_BRACE              RIGHT_BRACE
  {
    // module_def : MODULE module_name LEFT_BRACE  RIGHT_BRACE
    std::vector<Parsers::AST_Node*> children;

    $$ = ast.Create_Module($[module_name], std::move(children));
  }
;

module_items[result] :
  module_items[previous] module_item
  {
    // module_items[result] : module_items[previous] module_item
    auto node = $[module_item];
    if (node != nullptr)
    {
      $[previous].push_back(node);
    }
    $result = std::move($[previous]);
  }
|
  module_item
  {
    // module_items[result] : module_item
    std::vector<Parsers::AST_Node*> children;

    auto node = $[module_item];
    if (node != nullptr)
    {
      children.push_back(node);
    }
    $result = std::move(children);
  }
;

module_item :
 useNameSpace_def
 {
   $$ = $1;
 }
| attribute_def
  {
    $$ = $1;
  }
| parameter_def
  {
    $$ = $1;
  }
| localParameter_def
  {
    $$ = $1;
  }
| scanInterface_def
  {
    $$ = $1;
  }
| port_def
  {
    $$ = $1;
  }
| instance_def
  {
    $$ = $1;
  }
| logicSignal_def
  {
    $$ = $1;
  }
| scanRegister_def
  {
    $$ = $1;
  }
| dataRegister_def
  {
    $$ = $1;
  }
| scanMux_def
  {
    $$ = $1;
  }
| dataMux_def
  {
    $$ = $1;
  }
| clockMux_def
  {
    $$ = $1;
  }
| oneHotDataGroup_def
  {
    $$ = $1;
  }
| oneHotScanGroup_def
  {
    $$ = $1;
  }
| enum_def
  {
    $$ = $1;
  }
| alias_def
  {
    $$ = $1;
  }
| accessLink_def
  {
    $$ = $1;
  }
;

port_def :
  scanInPort_def
  {
    $$ = $1;
  }
| scanOutPort_def
  {
    $$ = $1;
  }
| shiftEnPort_def
  {
    $$ = $1;
  }
| captureEnPort_def
  {
    $$ = $1;
  }
| updateEnPort_def
  {
    $$ = $1;
  }
| dataInPort_def
  {
    $$ = $1;
  }
| dataOutPort_def
  {
    $$ = $1;
  }
| toShiftEnPort_def
  {
    $$ = $1;
  }
| toUpdateEnPort_def
  {
    $$ = $1;
  }
| toCaptureEnPort_def
  {
    $$ = $1;
  }
| selectPort_def
  {
    $$ = $1;
  }
| toSelectPort_def
  {
    $$ = $1;
  }
| resetPort_def
  {
    $$ = $1;
  }
| toIRSelectPort_def
  {
    $$ = $1;
  }
| tmsPort_def
  {
    $$ = $1;
  }
| toTmsPort_def
  {
    $$ = $1;
  }
| tckPort_def
  {
    $$ = $1;
  }
| toTckPort_def
  {
    $$ = $1;
  }
| clockPort_def
  {
    $$ = $1;
  }
| toClockPort_def
  {
    $$ = $1;
  }
| trstPort_def
  {
    $$ = $1;
  }
| toTrstPort_def
  {
    $$ = $1;
  }
| toResetPort_def
  {
    $$ = $1;
  }
| addressPort_def
  {
    $$ = $1;
  }
| writeEnPort_def
  {
    $$ = $1;
  }
| readEnPort_def
;

// 6.4.6.1
scanInPort_def : SCANINPORT scanInPort_name  scanInPort_tail
{
  $$ = nullptr;
}
;
scanInPort_tail: SEMICOLON | LEFT_BRACE scanInPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
scanInPort_items: scanInPort_items attribute_def | attribute_def;
scanInPort_name : port_name ;

// 6.4.6.2
scanOutPort_def : SCANOUTPORT scanOutPort_name scanOutPort_tail
{
  $$ = nullptr;
}
;
scanOutPort_tail: SEMICOLON | LEFT_BRACE scanOutPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
scanOutPort_items: scanOutPort_items scanOutPort_item | scanOutPort_item;
scanOutPort_name : port_name;
scanOutPort_item : attribute_def |
scanOutPort_source |
scanOutPort_enable |
scanOutPort_launchEdge ;
scanOutPort_source : SOURCE concat_scan_signal SEMICOLON;
scanOutPort_enable : ENABLE data_signal SEMICOLON;
scanOutPort_launchEdge : LAUNCHEDGE rising_or_falling SEMICOLON ;
rising_or_falling : RISING | FALLING;

// 6.4.6.3
shiftEnPort_def : SHIFTENPORT shiftEnPort_name shiftEnPort_tail
{
  $$ = nullptr;
}
;
shiftEnPort_tail: SEMICOLON | LEFT_BRACE shiftEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
shiftEnPort_items: shiftEnPort_items attribute_def | attribute_def;
shiftEnPort_name : port_name ;

// 6.4.6.4
captureEnPort_def : CAPTUREENPORT captureEnPort_name captureEnPort_tail
{
  $$ = nullptr;
}
;
captureEnPort_tail: SEMICOLON | LEFT_BRACE captureEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
captureEnPort_items: captureEnPort_items attribute_def | attribute_def;
captureEnPort_name : port_name ;

// 6.4.6.5
updateEnPort_def : UPDATEENPORT updateEnPort_name updateEnPort_tail
{
  $$ = nullptr;
}
;
updateEnPort_tail: SEMICOLON | LEFT_BRACE updateEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
updateEnPort_items: updateEnPort_items attribute_def | attribute_def;
updateEnPort_name : port_name ;

// 6.4.6.6
dataInPort_def : DATAINPORT dataInPort_name dataInPort_tail
{
  $$ = nullptr;
}
;
dataInPort_tail: SEMICOLON | LEFT_BRACE dataInPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
dataInPort_items: dataInPort_items dataInPort_item | dataInPort_item;
dataInPort_name : port_name ;
dataInPort_item : attribute_def | dataInPort_refEnum ;
dataInPort_refEnum : REFENUM enum_name SEMICOLON ;

// 6.4.6.7
dataOutPort_def : DATAOUTPORT dataOutPort_name dataOutPort_tail
{
  $$ = nullptr;
}
;
dataOutPort_tail: SEMICOLON | LEFT_BRACE dataOutPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
dataOutPort_items: dataOutPort_items dataOutPort_item | dataOutPort_item;
dataOutPort_name : port_name ;
dataOutPort_item : attribute_def |
dataOutPort_source |
dataOutPort_enable |
dataOutPort_refEnum ;
dataOutPort_source : SOURCE concat_data_signal SEMICOLON ;
dataOutPort_enable : ENABLE data_signal SEMICOLON ;
dataOutPort_refEnum : REFENUM enum_name SEMICOLON ;

// 6.4.6.8
toShiftEnPort_def : TOSHIFTENPORT toShiftEnPort_name toShiftEnPort_tail
{
  $$ = nullptr;
}
;
toShiftEnPort_tail: SEMICOLON | LEFT_BRACE toShiftEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toShiftEnPort_items: toShiftEnPort_items toShiftEnPort_item | toShiftEnPort_item;
toShiftEnPort_name : port_name ;
toShiftEnPort_item : attribute_def |
toShiftEnPort_source ;
toShiftEnPort_source : SOURCE concat_shiftEn_signal SEMICOLON ;

// 6.4.6.9
toCaptureEnPort_def : TOCAPTUREENPORT toCaptureEnPort_name toCaptureEnPort_tail
{
  $$ = nullptr;
}
;
toCaptureEnPort_tail: SEMICOLON | LEFT_BRACE toCaptureEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toCaptureEnPort_items: toCaptureEnPort_items toCaptureEnPort_item | toCaptureEnPort_item;
toCaptureEnPort_name : port_name ;
toCaptureEnPort_item : attribute_def |
toCaptureEnPort_source ;
toCaptureEnPort_source : SOURCE captureEn_signal SEMICOLON ;

// 6.4.6.10
toUpdateEnPort_def : TOUPDATEENPORT toUpdateEnPort_name toUpdateEnPort_tail
{
  $$ = nullptr;
}
;
toUpdateEnPort_tail: SEMICOLON | LEFT_BRACE toUpdateEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toUpdateEnPort_items: toUpdateEnPort_items toUpdateEnPort_item | toUpdateEnPort_item;
toUpdateEnPort_name : port_name ;
toUpdateEnPort_item : attribute_def | toUpdateEnPort_source ;
toUpdateEnPort_source : SOURCE updateEn_signal SEMICOLON ;

// 6.4.6.11
selectPort_def : SELECTPORT selectPort_name selectPort_tail
{
  $$ = nullptr;
}
;
selectPort_tail: SEMICOLON | LEFT_BRACE selectPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
selectPort_items: selectPort_items attribute_def | attribute_def;
selectPort_name : port_name ;

// 6.4.6.12
toSelectPort_def : TOSELECTPORT toSelectPort_name toSelectPort_tail
{
  $$ = nullptr;
}
;
toSelectPort_name : port_name ;
toSelectPort_tail: SEMICOLON | LEFT_BRACE toSelectPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toSelectPort_items: toSelectPort_items toSelectPort_item | toSelectPort_item;
toSelectPort_item : attribute_def | toSelectPort_source ;
toSelectPort_source : SOURCE concat_data_signal SEMICOLON ;

// 6.4.6.13
resetPort_def : RESETPORT resetPort_name resetPort_tail
{
  $$ = nullptr;
}
;
resetPort_name : port_name ;
resetPort_tail: SEMICOLON | LEFT_BRACE resetPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
resetPort_items: resetPort_items resetPort_item | resetPort_item;
resetPort_item : attribute_def |
resetPort_polarity ;
resetPort_polarity : ACTIVEPOLARITY zero_or_one SEMICOLON ;
zero_or_one : ZERO | ONE;

// 6.4.6.14
toResetPort_def : TORESETPORT toResetPort_name toResetPort_tail
{
  $$ = nullptr;
}
;
toResetPort_name : port_name ;
toResetPort_tail: SEMICOLON | LEFT_BRACE toResetPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toResetPort_items: toResetPort_items toResetPort_item | toResetPort_item;
toResetPort_item : attribute_def |
toResetPort_source |
toResetPort_polarity;
toResetPort_source : SOURCE concat_reset_signal SEMICOLON ;
toResetPort_polarity : ACTIVEPOLARITY zero_or_one SEMICOLON ;

// 6.4.6.15
tmsPort_def : TMSPORT tmsPort_name tmsPort_tail
{
  $$ = nullptr;
}
;
tmsPort_tail: SEMICOLON | LEFT_BRACE tmsPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
tmsPort_items: tmsPort_items attribute_def | attribute_def;
tmsPort_name : port_name ;

// 6.4.6.16
toTmsPort_def : TOTMSPORT toTmsPort_name toTmsPort_tail
{
  $$ = nullptr;
}
;
toTmsPort_name : port_name ;
toTmsPort_tail: SEMICOLON | LEFT_BRACE toTmsPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toTmsPort_items: toTmsPort_items toTmsPort_item | toTmsPort_item;
toTmsPort_item : attribute_def |
toTmsPort_source ;
toTmsPort_source : SOURCE concat_tms_signal SEMICOLON ;

// 6.4.6.17
toIRSelectPort_def : TOIRSELECTPORT toIRSelectPort_name toIRSelectPort_tail
{
  $$ = nullptr;
}
;
toIRSelectPort_tail: SEMICOLON | LEFT_BRACE toIRSelectPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toIRSelectPort_items: toIRSelectPort_items attribute_def | attribute_def;
toIRSelectPort_name : port_name ;

// 6.4.6.18
tckPort_def : TCKPORT tckPort_name tckPort_tail
{
  $$ = nullptr;
}
;
tckPort_tail: SEMICOLON | LEFT_BRACE tckPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
tckPort_items: tckPort_items attribute_def | attribute_def;
tckPort_name : port_name ;

// 6.4.6.19
toTckPort_def : TOTCKPORT toTckPort_name toTckPort_tail
{
  $$ = nullptr;
}
;
toTckPort_tail: SEMICOLON | LEFT_BRACE toTckPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toTckPort_items: toTckPort_items attribute_def | attribute_def;
toTckPort_name : port_name ;

// 6.4.6.20
clockPort_def : CLOCKPORT clockPort_name clockPort_tail
{
  $$ = nullptr;
}
;
clockPort_name : port_name ;
clockPort_tail: SEMICOLON | LEFT_BRACE clockPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
clockPort_items: clockPort_items clockPort_item | clockPort_item;
clockPort_item : attribute_def |
clockPort_diffPort ;
clockPort_diffPort : DIFFERENTIALINVOF concat_clock_signal SEMICOLON ;

// 6.4.6.21
toClockPort_def : TOCLOCKPORT toClockPort_name toClockPort_tail
{
  $$ = nullptr;
}
;
toClockPort_name : port_name ;
toClockPort_tail: SEMICOLON | LEFT_BRACE toClockPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toClockPort_items: toClockPort_items toClockPort_item | toClockPort_item;
toClockPort_item : attribute_def |
toClockPort_source |
freqMultiplier_def |
freqDivider_def |
differentialInvOf_def |
period_def ;
toClockPort_source : SOURCE concat_clock_signal SEMICOLON ;
freqMultiplier_def : FREQMULTIPLIER pos_int SEMICOLON ;
freqDivider_def : FREQDIVIDER pos_int SEMICOLON ;
differentialInvOf_def : DIFFERENTIALINVOF concat_clock_signal SEMICOLON ;
period_def : PERIOD pos_int tunit SEMICOLON ;
tunit : SEC | MSEC | USEC | NSEC | PSEC ;

// 6.4.6.22
trstPort_def : TRSTPORT trstPort_name trstPort_tail
{
  $$ = nullptr;
}
;
trstPort_tail: SEMICOLON | LEFT_BRACE trstPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
trstPort_items: trstPort_items attribute_def | attribute_def;
trstPort_name : port_name ;

// 6.4.6.23
toTrstPort_def : TOTRSTPORT toTrstPort_name toTrstPort_tail
{
  $$ = nullptr;
}
;
toTrstPort_name : port_name ;
toTrstPort_tail: SEMICOLON | LEFT_BRACE toTrstPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toTrstPort_items: toTrstPort_items toTrstPort_item | toTrstPort_item;
toTrstPort_item : attribute_def |
toTrstPort_source ;
toTrstPort_source : SOURCE concat_trst_signal SEMICOLON ;

// 6.4.6.24
addressPort_def : ADDRESSPORT addressPort_name addressPort_tail
{
  $$ = nullptr;
}
;
addressPort_tail: SEMICOLON | LEFT_BRACE addressPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
addressPort_items: addressPort_items attribute_def | attribute_def;
addressPort_name : port_name ;

// 6.4.6.25
writeEnPort_def : WRITEENPORT writeEnPort_name writeEnPort_tail
{
  $$ = nullptr;
}
;
writeEnPort_tail: SEMICOLON | LEFT_BRACE writeEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
writeEnPort_items: writeEnPort_items attribute_def | attribute_def;
writeEnPort_name : port_name ;

// 6.4.6.26
readEnPort_def : READENPORT readEnPort_name readEnPort_tail
{
  $$ = nullptr;
}
;
readEnPort_tail: SEMICOLON | LEFT_BRACE readEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
readEnPort_items: readEnPort_items attribute_def | attribute_def;
readEnPort_name : port_name ;

// 6.4.7
instance_def : INSTANCE instance_name OF instance_module instance_tail
{
  $$ = nullptr;
}
;
instance_module : module_name | namespace_name DOUBLE_COLON module_name | DOUBLE_COLON module_name;
instance_tail: SEMICOLON | LEFT_BRACE instance_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
instance_items: instance_items instance_item | instance_item;

instance_item : inputPort_connection |
allowBroadcast_def |
attribute_def |
parameter_override |
instance_addressValue ;

inputPort_connection : INPUTPORT inputPort_name EQUAL inputPort_source SEMICOLON ;
allowBroadcast_def : ALLOWBROADCASTONSCANINTERFACE allowBroadcast_items SEMICOLON ;
allowBroadcast_items : allowBroadcast_items COMMA scanInterface_name | scanInterface_name ;
inputPort_name : port_name ;
inputPort_source : concat_data_signal ;
parameter_override : parameter_def;
instance_addressValue : ADDRESSVALUE number SEMICOLON ;

// 6.4.8
scanRegister_def : SCANREGISTER scanRegister_name scanRegister_tail
{
  // scanRegister_def : SCANREGISTER scanRegister_name scanRegister_tail
  auto& name  = std::get<0>($[scanRegister_name]);
  auto& left  = std::get<1>($[scanRegister_name]);
  auto& right = std::get<2>($[scanRegister_name]);

  auto node = ast.Create_ScanRegister(name, left, right);
  $$ = node;
}
;
scanRegister_name : register_name { $$ = $[register_name]; }
scanRegister_tail: SEMICOLON | LEFT_BRACE scanRegister_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
scanRegister_items: scanRegister_items scanRegister_item | scanRegister_item;
scanRegister_item :
  attribute_def
| scanRegister_scanInSource
| scanRegister_defaultLoadValue
| scanRegister_captureSource
| scanRegister_resetValue
| scanRegister_refEnum
;

scanRegister_scanInSource : SCANINSOURCE scan_signal SEMICOLON ;
scanRegister_defaultLoadValue : DEFAULTLOADVALUE number_or_enum SEMICOLON ;
scanRegister_captureSource : CAPTURESOURCE signal_or_enum SEMICOLON;
scanRegister_resetValue : RESETVALUE number_or_enum SEMICOLON ;
scanRegister_refEnum : REFENUM enum_name SEMICOLON ;
number_or_enum : concat_number | enum_symbol;
signal_or_enum : number | SCALAR_ID | pin_id;

// 6.4.9
dataRegister_def : DATAREGISTER dataRegister_name dataRegister_tail
{
  $$ = nullptr;
}
;
dataRegister_name : register_name ;
dataRegister_tail: SEMICOLON | LEFT_BRACE dataRegister_items RIGHT_BRACE ;
dataRegister_items: dataRegister_items dataRegister_item | dataRegister_item;
dataRegister_item : dataRegister_type |
dataRegister_common ;
dataRegister_type : dataRegister_selectable |
dataRegister_addressable |
dataRegister_readCallBack |
dataRegister_writeCallBack ;
// Common to all types:
dataRegister_common : dataRegister_resetValue |
dataRegister_defaultLoadValue |
dataRegister_refEnum |
attribute_def ;
dataRegister_resetValue : RESETVALUE number_or_enum SEMICOLON;
dataRegister_defaultLoadValue : DEFAULTLOADVALUE number_or_enum SEMICOLON ;
dataRegister_refEnum : REFENUM enum_name SEMICOLON ;
//For Selectable Data Register:
dataRegister_selectable : dataRegister_writeEnSource |
dataRegister_writeDataSource;
dataRegister_writeEnSource : WRITEENSOURCE data_signal SEMICOLON ;
dataRegister_writeDataSource : WRITEDATASOURCE concat_data_signal SEMICOLON ;
// Addressable Data Register:
dataRegister_addressable : dataRegister_addressValue;
dataRegister_addressValue : ADDRESSVALUE number SEMICOLON ;
// CallBack Data Register:
dataRegister_readCallBack : dataRegister_readCallBack_proc |
dataRegister_readDataSource ;
dataRegister_readCallBack_proc : READCALLBACK iProc_namespace iProc_name iProc_arglist SEMICOLON
                               | READCALLBACK iProc_namespace iProc_name SEMICOLON;
iProc_arglist : iProc_arglist iProc_args | iProc_args ;
dataRegister_readDataSource : READDATASOURCE concat_data_signal SEMICOLON ;
dataRegister_writeCallBack : WRITECALLBACK iProc_namespace iProc_name iProc_arglist SEMICOLON
                           | WRITECALLBACK iProc_namespace iProc_name SEMICOLON;
iProc_namespace : namespace_name DOUBLE_COLON ref_module_name
 DOUBLE_COLON sub_namespace | namespace_name DOUBLE_COLON ref_module_name | ref_module_name ;
iProc_name : SCALAR_ID | parameter_ref ;
iProc_args : D_SUBST |
R_SUBST |
number |
STRING ;
sub_namespace : SCALAR_ID |
parameter_ref ;
ref_module_name : SCALAR_ID |
parameter_ref ;

// 6.4.10
logicSignal_def : LOGICSIGNAL logicSignal_name LEFT_BRACE logic_expr SEMICOLON RIGHT_BRACE
{
  $$ = nullptr;
}
;
logicSignal_name : reg_port_signal_id;
logic_expr : logic_expr_lvl1 ;
logic_expr_lvl1 : logic_expr_lvl2 | logic_expr_lvl2 logic_and_or_or logic_expr_lvl1  ;
logic_and_or_or : LOGIC_AND | LOGIC_OR;
logic_expr_lvl2 : logic_expr_lvl3 | logic_expr_lvl3 and_or_xor logic_expr_lvl2 |
and_or_xor logic_expr_lvl2 ;
and_or_xor : AND | PIPE | XOR;
logic_expr_lvl3 : logic_expr_lvl4 | logic_expr_lvl4 eq_or_neq logic_expr_num_arg ;
eq_or_neq : LOGIC_EQUAL | NOT_EQUAL;
logic_expr_lvl4 : logic_expr_arg | logic_expr_arg COMMA logic_expr_lvl4 ;
logic_unary_expr : neg_or_not logic_expr_arg;
logic_expr_arg : logic_expr_paren | logic_unary_expr | signal ;
neg_or_not : TILDE | LOGIC_NOT;
logic_expr_paren : LEFT_PAREN logic_expr RIGHT_PAREN;
logic_expr_num_arg : number | enum_name | LEFT_PAREN logic_expr_num_arg RIGHT_PAREN ;

// 6.4.11
scanMux_def : SCANMUX scanMux_name SELECTEDBY scanMux_select LEFT_BRACE scanMux_selections RIGHT_BRACE
{
  $$ = nullptr;
}
;
scanMux_selections : scanMux_selections scanMux_selection | scanMux_selection ;
scanMux_name : reg_port_signal_id ;
scanMux_select : concat_data_signal ;
scanMux_selection : concat_number_list COLON concat_scan_signal SEMICOLON ;

// 6.4.12
dataMux_def : DATAMUX dataMux_name SELECTEDBY dataMux_select LEFT_BRACE dataMux_selections RIGHT_BRACE
{
  $$ = nullptr;
}
;
dataMux_selections : dataMux_selections dataMux_selection | dataMux_selection ;
dataMux_name : reg_port_signal_id ;
dataMux_select : concat_data_signal ;
dataMux_selection : concat_number_list COLON concat_data_signal SEMICOLON ;

// 6.4.13
clockMux_def : CLOCKMUX clockMux_name SELECTEDBY clockMux_select LEFT_BRACE clockMux_selections RIGHT_BRACE
{
  $$ = nullptr;
}
;
clockMux_selections : clockMux_selections clockMux_selection | clockMux_selection ;
clockMux_name : reg_port_signal_id ;
clockMux_select : concat_data_signal ;
clockMux_selection : concat_number_list COLON concat_clock_signal SEMICOLON ;

// 6.4.14
oneHotScanGroup_def : ONEHOTSCANGROUP oneHotScanGroup_name LEFT_BRACE oneHotScanGroup_items RIGHT_BRACE
{
  $$ = nullptr;
}
;
oneHotScanGroup_items : oneHotScanGroup_items oneHotScanGroup_item | oneHotScanGroup_item ;
oneHotScanGroup_name : reg_port_signal_id ;
oneHotScanGroup_item : PORT concat_scan_signal SEMICOLON ;

// 6.4.15
oneHotDataGroup_def : ONEHOTDATAGROUP oneHotDataGroup_name LEFT_BRACE oneHotDataGroup_items RIGHT_BRACE
{
  $$ = nullptr;
}
;
oneHotDataGroup_items : oneHotDataGroup_items oneHotDataGroup_item | oneHotDataGroup_item ;
oneHotDataGroup_name : reg_port_signal_id ;
oneHotDataGroup_item : instance_def |
dataRegister_def |
oneHotDataGroup_portSource ;
oneHotDataGroup_portSource : PORT concat_data_signal SEMICOLON ;

// 6.4.16
accessLink_def : accessLink1149_def | accessLinkGeneric_def;
accessLinkGeneric_def : ACCESSLINK accessLink_name OF accessLink_genericID  LEFT_BRACE RIGHT_BRACE
{
  $$ = nullptr;
}
;

accessLink_genericID : SCALAR_ID;
accessLink1149_def : ACCESSLINK accessLink_name OF accessLink1149_stds LEFT_BRACE BSDLENTITIY bsdlEntity_name SEMICOLON bsdl_instr_refs RIGHT_BRACE
{
  $$ = nullptr;
}
;

accessLink1149_stds : STD_1149_1_2001 | STD_1149_1_2013;
accessLink_name : SCALAR_ID;
bsdlEntity_name : SCALAR_ID ;
bsdl_instr_refs : bsdl_instr_refs bsdl_instr_ref | bsdl_instr_ref ;
bsdl_instr_ref : bsdl_instr_name LEFT_BRACE bsdl_instr_selected_items RIGHT_BRACE ;
bsdl_instr_selected_items : bsdl_instr_selected_items bsdl_instr_selected_item | bsdl_instr_selected_item ;
bsdl_instr_name : SCALAR_ID ;
bsdl_instr_selected_item : SCANINTERFACE
LEFT_BRACE accessLink1149_ScanInterface_names RIGHT_BRACE |
ACTIVESIGNALS
LEFT_BRACE accessLink1149_ActiveSignal_names RIGHT_BRACE  ;
accessLink1149_ScanInterface_names : accessLink1149_ScanInterface_names accessLink1149_ScanInterface_name SEMICOLON | accessLink1149_ScanInterface_name SEMICOLON;
accessLink1149_ActiveSignal_name : reg_port_signal_id ;
accessLink1149_ActiveSignal_names : accessLink1149_ActiveSignal_names accessLink1149_ActiveSignal_name SEMICOLON | accessLink1149_ActiveSignal_name SEMICOLON;
accessLink1149_ScanInterface_name : instance_name | instance_name DOT scanInterface_name ;

// 6.4.17
scanInterface_def : SCANINTERFACE scanInterface_name LEFT_BRACE scanInterface_items RIGHT_BRACE
{
  $$ = nullptr;
}
;
scanInterface_items : scanInterface_items scanInterface_item | scanInterface_item ;
scanInterface_name : SCALAR_ID;
scanInterface_item : attribute_def | scanInterfacePort_def | defaultLoad_def | scanInterfaceChain_def ;
scanInterfacePort_def : PORT reg_port_signal_id SEMICOLON;
scanInterfaceChain_def : CHAIN scanInterfaceChain_name LEFT_BRACE scanInterfaceChain_items RIGHT_BRACE ;
scanInterfaceChain_items : scanInterfaceChain_items scanInterfaceChain_item | scanInterfaceChain_item ;
scanInterfaceChain_name : SCALAR_ID;
scanInterfaceChain_item : attribute_def | scanInterfacePort_def | defaultLoad_def ;
defaultLoad_def : DEFAULTLOADVALUE concat_number SEMICOLON ;

// 6.5.2
alias_def : ALIAS alias_name EQUAL concat_hier_data_signal alias_tail
{
  $$ = nullptr;
}
;
alias_name : reg_port_signal_id;
alias_tail: SEMICOLON | LEFT_BRACE alias_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
alias_items: alias_items alias_item | alias_item;
alias_item : attribute_def |
ACCESSTOGETHER SEMICOLON |
alias_iApplyEndState |
alias_refEnum ;
alias_iApplyEndState : IAPPLYENDSTATE concat_number SEMICOLON ;
alias_refEnum : REFENUM enum_name SEMICOLON ;
concat_hier_data_signal : concat_hier_data_signal COMMA x_hier_data_signal | x_hier_data_signal;
x_hier_data_signal : TILDE hier_data_signal | hier_data_signal ;
hier_data_signal : hier_data_signal_instances reg_port_signal_id | reg_port_signal_id ;
hier_data_signal_instances : hier_data_signal_instances instance_name DOT | instance_name DOT;

// 6.5.3
enum_def : ENUM enum_name LEFT_BRACE enum_items RIGHT_BRACE
{
  $$ = nullptr;
}
;
enum_items : enum_items enum_item | enum_item;
enum_name : SCALAR_ID ;
enum_item : enum_symbol EQUAL enum_value SEMICOLON ;
enum_symbol : SCALAR_ID;
enum_value : concat_number;

// 6.5.4
parameter_def : PARAMETER parameter_name EQUAL parameter_value SEMICOLON
{
  $$ = nullptr;
}
;
localParameter_def : LOCALPARAMETER parameter_name EQUAL parameter_value SEMICOLON
{
  $$ = nullptr;
}
;
parameter_name : SCALAR_ID;
parameter_value : concat_string | concat_number;
concat_string : concat_string COMMA string_or_parm | string_or_parm ;
string_or_parm : STRING | parameter_ref;

// 6.5.5
attribute_def :
  ATTRIBUTE attribute_name EQUAL attribute_value SEMICOLON
  {
    $$ = nullptr;
  }
|
  ATTRIBUTE attribute_name SEMICOLON
  {
    $$ = nullptr;
  }
;
attribute_name : SCALAR_ID;
attribute_value : concat_string | concat_number ;

%%

void ICL::ICL_Parser::error(const location_type& loc, const std::string& errorMessage)
{
  auto isValidLoc = loc.begin != loc.end;
  if (isValidLoc)
  {
    throw mast::ParserException("ICL", "", loc.begin.line, loc.begin.column, loc.end.column, errorMessage);
  }

  throw mast::ParserException("ICL", "", my_location->begin.line, my_location->begin.column, my_location->end.column, errorMessage);
}
