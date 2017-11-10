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
  class ICL_Scanner;
}
namespace Parsers
{
  class AST;
  class AST_AccessLink;
  class AST_Attribute;
  class AST_Identifier;
  class AST_Instance;
  class AST_ModuleIdentifier;
  class AST_Module;
  class AST_Namespace;
  class AST_Node;
  class AST_Parameter;
  class AST_ParameterRef;
  class AST_Port;
  class AST_ScalarIdentifier;
  class AST_ScanInterface;
  class AST_ScanMux;
  class AST_ScanMuxSelection;
  class AST_ScanRegister;
  class AST_Signal;
  class AST_SimpleNode;
  class AST_Source;
  class AST_String;
  class AST_Value;
  class AST_VectorIdentifier;
  enum class AccessLinkType;
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

%parse-param { ICL_Scanner&  scanner }
%parse-param { Parsers::AST& ast}

%code
{
// ---------------- Includes for all driver functions
//
#include "ICL_Scanner.hpp"
#include "ParserException.hpp"

#include "AST.hpp"
#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

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
  //! Removes leading and trailing double quotes and transform escaped
  //! characters with their plain value (normally for double quotes and backslashes)
  //!
  string CleanString(const string& str)
  {
    auto strView = string_view(str);

    if (strView.front() == '"')
    {
      strView.remove_prefix(1u);
    }

    if (strView.back() == '"')
    {
      strView.remove_suffix(1u);
    }

    string result;
    auto acceptBackslash = false;
    for (const auto& character : strView)
    {
      if (character != '\\')
      {
        result.push_back(character);
        acceptBackslash = false;
      }
      else if (acceptBackslash)
      {
        result.push_back(character);
        acceptBackslash = false;
      }
      else
      {
        acceptBackslash = true;
      }
    }

    return result;
  }
  //
  //  End of RemoveBoundaryQuotes
  //---------------------------------------------------------------------------


} // End of unnamed namespace

} /*end of %code section*/

%define api.value.type variant
%define parse.assert

%type <std::string> SCALAR_ID
%type <std::string> STRING
%type <std::string> scanInterfaceChain_name
%type <std::string> scanInterface_name
%type <std::string> parameter_name
%type <std::string> attribute_name
%type <std::string> accessLink_name
%type <std::string> accessLink_genericID

%type <Parsers::AccessLinkType>        accessLink1149_stds

%type <Parsers::AST_ScalarIdentifier*> instance_name
%type <Parsers::AST_ScalarIdentifier*> module_name
%type <std::string>                    namespace_name



%type <Parsers::AST_ModuleIdentifier*>                                                    module_identifier
%type <std::vector<Parsers::AST_ScalarIdentifier*>>                                       scoped_instance_name
%type <std::tuple<std::vector<Parsers::AST_ScalarIdentifier*>, Parsers::AST_Identifier*>> scoped_port_name

%type <std::string> index
%type <Parsers::AST_ParameterRef*> parameter_ref

%type <Parsers::AST_Node*>                     parameter_override
%type <Parsers::AST_Node*>                     instance_item
%type <std::vector<Parsers::AST_Node*>>        instance_items
%type <std::vector<Parsers::AST_SimpleNode*>>  concat_string

%type <std::string> UNSIZED_DEC_NUM
%type <std::string> UNSIZED_BIN_NUM
%type <std::string> UNSIZED_HEX_NUM

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

%type <std::string> number
%type <std::string> number_or_enum
%type <std::string> concat_number
%type <std::string> enum_symbol
%type <std::string> enum_name
%type <std::string> enum_value


%type <std::vector<std::string>> concat_number_list

%type <std::tuple<string, string>> range          // Left & Right indexes
%type <std::tuple<string, string>> index_or_range // Left & Right indexes (Right can be empty)

%type <Parsers::AST_Signal*>              signal
%type <Parsers::AST_Signal*>              signal_or_inverted_signal
%type <Parsers::AST_Signal*>              data_signal
%type <Parsers::AST_Signal*>              scan_signal
%type <Parsers::AST_ScanMuxSelection*>    scanMux_selection

%type <std::vector<Parsers::AST_ScanMuxSelection*>> scanMux_selections


%type <std::vector<Parsers::AST_Signal*>> concat_signal_or_inverted_signal
%type <std::vector<Parsers::AST_Signal*>> concat_scan_signal
%type <std::vector<Parsers::AST_Signal*>> concat_reset_signal
%type <std::vector<Parsers::AST_Signal*>> scanMux_select
%type <std::vector<Parsers::AST_Signal*>> concat_data_signal
%type <std::vector<Parsers::AST_Signal*>> concat_clock_signal
%type <std::vector<Parsers::AST_Signal*>> concat_shiftEn_signal
%type <std::vector<Parsers::AST_Signal*>> concat_trst_signal
%type <std::vector<Parsers::AST_Signal*>> inputPort_source

%type <Parsers::AST_VectorIdentifier*> alias_name           // Name, Left & Right indexes (Right can be empty)
%type <Parsers::AST_VectorIdentifier*> oneHotScanGroup_name // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> inputPort_name       // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> port_name            // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> reg_port_signal_id   // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> register_name        // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scalar_or_vector_id  // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanInPort_name      // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanMux_name         // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanOutPort_name     // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanRegister_name    // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> vector_id            // Name, Left & Right indexes (Right can be empty)

%type <Parsers::AST_Value*>            scanRegister_resetValue  // Value expression


%type <Parsers::AST_Module*>            MODULE  // Pseudo type for mid-rule support (always set as nullptr)
%type <Parsers::AST_Module*>            module_def
%type <Parsers::AST_Module*>            module_body
%type <Parsers::AST_Instance*>          instance_def
%type <Parsers::AST_Attribute*>         attribute_def
%type <Parsers::AST_Parameter*>         localParameter_def
%type <Parsers::AST_Parameter*>         parameter_def
%type <Parsers::AST_ScanRegister*>      scanRegister_def
%type <Parsers::AST_ScanMux*>           scanMux_def
%type <Parsers::AST_ScanInterface*>     scanInterface_def
%type <Parsers::AST_AccessLink*>        accessLink_def
%type <Parsers::AST_AccessLink*>        accessLink1149_def
%type <Parsers::AST_AccessLink*>        accessLinkGeneric_def
%type <Parsers::AST_Node*>              alias_def
%type <Parsers::AST_Node*>              clockMux_def
%type <Parsers::AST_Node*>              dataMux_def
%type <Parsers::AST_Node*>              dataRegister_def
%type <Parsers::AST_Node*>              enum_def
%type <Parsers::AST_Node*>              logicSignal_def
%type <Parsers::AST_Node*>              module_item
%type <Parsers::AST_Node*>              nameSpace_def
%type <Parsers::AST_Node*>              oneHotDataGroup_def
%type <Parsers::AST_Node*>              oneHotScanGroup_def
%type <Parsers::AST_Node*>              useNameSpace_def

%type <Parsers::AST_Port*>              addressPort_def
%type <Parsers::AST_Port*>              captureEnPort_def
%type <Parsers::AST_Port*>              clockPort_def
%type <Parsers::AST_Port*>              dataInPort_def
%type <Parsers::AST_Port*>              dataOutPort_def
%type <Parsers::AST_Port*>              port_def
%type <Parsers::AST_Port*>              readEnPort_def
%type <Parsers::AST_Port*>              resetPort_def
%type <Parsers::AST_Port*>              scanInPort_def
%type <Parsers::AST_Port*>              scanInterfacePort_def
%type <Parsers::AST_Port*>              scanOutPort_def
%type <Parsers::AST_Port*>              selectPort_def
%type <Parsers::AST_Port*>              shiftEnPort_def
%type <Parsers::AST_Port*>              tckPort_def
%type <Parsers::AST_Port*>              tmsPort_def
%type <Parsers::AST_Port*>              toCaptureEnPort_def
%type <Parsers::AST_Port*>              toClockPort_def
%type <Parsers::AST_Port*>              toIRSelectPort_def
%type <Parsers::AST_Port*>              toResetPort_def
%type <Parsers::AST_Port*>              toSelectPort_def
%type <Parsers::AST_Port*>              toShiftEnPort_def
%type <Parsers::AST_Port*>              toTckPort_def
%type <Parsers::AST_Port*>              toTmsPort_def
%type <Parsers::AST_Port*>              toTrstPort_def
%type <Parsers::AST_Port*>              toUpdateEnPort_def
%type <Parsers::AST_Port*>              trstPort_def
%type <Parsers::AST_Port*>              updateEnPort_def
%type <Parsers::AST_Port*>              writeEnPort_def
%type <Parsers::AST_Port*>              inputPort_connection


%type <Parsers::AST_Node*>              scanRegister_captureSource
%type <Parsers::AST_Node*>              scanRegister_defaultLoadValue
%type <Parsers::AST_Node*>              scanRegister_refEnum
%type <Parsers::AST_Source*>            scanRegister_scanInSource
%type <Parsers::AST_Source*>            port_source
%type <Parsers::AST_Source*>            scanOutPort_source


%type <Parsers::AST_Node*>              scanInterface_item
%type <Parsers::AST_Node*>              scanOutPort_item
%type <Parsers::AST_Node*>              scanRegister_item

%type <std::vector<Parsers::AST_Node*>> module_items
%type <std::vector<Parsers::AST_Node*>> scanInterface_items
%type <std::vector<Parsers::AST_Node*>> scanRegister_tail
%type <std::vector<Parsers::AST_Node*>> scanRegister_items
%type <std::vector<Parsers::AST_Node*>> scanInPort_items
%type <std::vector<Parsers::AST_Node*>> scanOutPort_items



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
  POS_INT
  {
    // pos_int : POS_INT
    auto& posInt = $[POS_INT];
    $$ = std::move(posInt);
  }
| ONE  { $$ = "1"; /* pos_int : ONE */ }
| ZERO { $$ = "0"; /* pos_int : ZERO*/ }
;

number :
  UNSIZED_DEC_NUM   { $$ = std::move($1); /* number : UNSIZED_DEC_NUM */ }
| UNSIZED_BIN_NUM   { $$ = std::move($1); /* number : UNSIZED_BIN_NUM */ }
| UNSIZED_HEX_NUM   { $$ = std::move($1); /* number : UNSIZED_HEX_NUM */ }
| integer_expr UNSIZED_DEC_NUM
  {
    // number : integer_expr UNSIZED_DEC_NUM
    auto expr = std::move($[integer_expr]);
    expr.append(" ").append($[UNSIZED_DEC_NUM]);

    $$ = std::move(expr);
  }
| integer_expr UNSIZED_BIN_NUM
  {
    // number : integer_expr UNSIZED_BIN_NUM
    auto expr = std::move($[integer_expr]);
    expr.append($[UNSIZED_BIN_NUM]);

    $$ = std::move(expr);
  }
| integer_expr UNSIZED_HEX_NUM
  {
    // number : integer_expr UNSIZED_HEX_NUM
    auto expr = std::move($[integer_expr]);
    expr.append($[UNSIZED_HEX_NUM]);

    $$ = expr;
  }
| integer_expr
  {
    // number : integer_expr
    auto& expr = $[integer_expr];
    $$ = std::move(expr);
  }
;

// 6.3.12
vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
{
  // vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
  auto& baseName   = $[SCALAR_ID];
  auto& left       = std::get<0>($[index_or_range]);
  auto& right      = std::get<1>($[index_or_range]);
  auto  identifier = ast.Create_VectorIdentifier(std::move(baseName), std::move(left), std::move(right));

  $$ = identifier;
}
;

index_or_range :
  index { $$ = make_tuple(std::move($[index]), ""s); /* index_or_range : index */ }
| range { $$ = std::move($1);                        /* index_or_range : range */ }
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


integer_expr : integer_expr_lvl1 { $$ = std::move($1); /* integer_expr : integer_expr_lvl1 */};

integer_expr_lvl1 :
  integer_expr_lvl2
  {
    // integer_expr_lvl1 : integer_expr_lvl2
    $$ = std::move($1);
  }
| integer_expr_lvl2 integer_expr_lvl1_b
  {
    // integer_expr_lvl1 : integer_expr_lvl2 integer_expr_lvl1_b
    auto combined = $[integer_expr_lvl2].append($[integer_expr_lvl1]);
    $$ = std::move(combined);
  }
;
integer_expr_lvl1_b : plus_or_minus integer_expr_lvl1
{
  // integer_expr_lvl1_b : plus_or_minus integer_expr_lvl1
  auto combined = $[plus_or_minus].append($[integer_expr_lvl1]);
  $$ = std::move(combined);
}
;

plus_or_minus :
  PLUS  { $$ = " + "; /* plus_or_minus: PLUS */}
| MINUS { $$ = " - "; /* plus_or_minus: MINUS*/}
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
  auto op   = $[star_or_slash_or_percent];
  auto expr = $[integer_expr_lvl2];
  $$ = op.append(expr);
}
;

star_or_slash_or_percent :
  STAR    { $$ = " * ";  /* star_or_slash_or_percent : STAR */}
| SLASH   { $$ = " / "; /* star_or_slash_or_percent : SLASH*/}
| PERCENT { $$ = " % ";  /* star_or_slash_or_percent : PERCENT*/}
;

integer_expr_paren : LEFT_PAREN integer_expr RIGHT_PAREN
{
  // integer_expr_paren : LEFT_PAREN integer_expr RIGHT_PAREN
  auto combined = "("s.append($[integer_expr]).append(")");
  $$ = std::move(combined);
}
;

integer_expr_arg :
  integer_expr_paren { $$ = std::move($1); /* integer_expr_arg : integer_expr_paren */ }
| pos_int            { $$ = std::move($1); /* integer_expr_arg : pos_int*/ }
| parameter_ref
  {
    // integer_expr_arg : parameter_ref
    auto parameterRef = $[parameter_ref];
    auto asString     = "$"s.append(parameterRef->Name());

    LOG(DEBUG) << "parameter_ref has been downgraded as mere string !!!";

    $$ = std::move(asString);
  }
;

parameter_ref : DOLLAR SCALAR_ID
{
  // parameter_ref : DOLLAR SCALAR_ID
  auto& parameterName = $[SCALAR_ID];
  auto  node          = ast.Create_ParameterRef(std::move(parameterName));

  $$ = node;
}
;

//semantic rules prevents inverting unsized numbers and having more than one
//unsized number within a concat_number. See section 6.4.10.
concat_number :
  number
  {
    // concat_number : number
    auto& numberValue = $[number];
    $$ = std::move(numberValue);
  }
| TILDE number
  {
    // concat_number : TILDE number
    auto expr = "~"s.append($[number]);

    $$ = std::move(expr);
  }
| concat_number[lhs_concat_number] COMMA number
  {
    // concat_number : concat_number COMMA number
    auto expr = std::move($[lhs_concat_number]);
    expr.append(", ").append($[number]);

    $$ = std::move(expr);
  }
| concat_number[lhs_concat_number] COMMA TILDE number
  {
    // concat_number : concat_number COMMA TILDE number
    auto expr = std::move($[lhs_concat_number]);
    expr.append(", ~").append($[number]);

    $$ = std::move(expr);
  }
;

concat_number_list :
  concat_number
  {
    // concat_number
    vector<string> concatList;
    auto&          number = $[concat_number];

    if (!number.empty())
    {
      concatList.emplace_back(std::move(number));
    }

    $$ = std::move(concatList);
  }
| concat_number_list[lhs] PIPE concat_number
  {
    // concat_number_list PIPE concat_number
    auto& concatList = $[lhs];
    auto& number     = $[concat_number];

    if (!number.empty())
    {
      concatList.emplace_back(std::move(number));
    }

    $$ = std::move(concatList);
  }
;

scalar_or_vector_id:
  SCALAR_ID
  {
    // scalar_or_vector_id : SCALAR_ID
    auto& name       = $[SCALAR_ID];
    auto  identifier = ast.Create_VectorIdentifier(std::move(name), "", "");

    $$ = identifier;
  }
| vector_id
  {
    // scalar_or_vector_id : vector_id
    $$ = $[vector_id];
  }
;

port_name          : scalar_or_vector_id { $$ = $1; /* port_name : scalar_or_vector_id */}
register_name      : scalar_or_vector_id { $$ = $1; /* register_name : scalar_or_vector_id */}
reg_port_signal_id : scalar_or_vector_id { $$ = $1; /* reg_port_signal_id : scalar_or_vector_id */}

instance_name  : SCALAR_ID { $$ = ast.Create_ScalarIdentifier($[SCALAR_ID]); /* instance_name: SCALAR_ID */ };
module_name    : SCALAR_ID { $$ = ast.Create_ScalarIdentifier($[SCALAR_ID]); /* module_name: SCALAR_ID */ };
namespace_name : SCALAR_ID { $$ = $[SCALAR_ID]; /* namespace_name: SCALAR_ID */ };

scoped_instance_name :
  scoped_instance_name[lhs] DOT instance_name
  {
    // scoped_instance_name : scoped_instance_name[lhs] DOT instance_name
    auto& identifiers = $[lhs];

    auto instanceName = $[instance_name];
    if (instanceName != nullptr)
    {
      identifiers.push_back(instanceName);
    }
    $$ = std::move(identifiers);
  }
| instance_name
  {
    // scoped_instance_name : instance_name
    std::vector<Parsers::AST_ScalarIdentifier*> identifiers;

    auto instanceName = $[instance_name];
    if (instanceName != nullptr)
    {
      identifiers.push_back(instanceName);
    }
    $$ = std::move(identifiers);
  }
;

scoped_port_name : scoped_instance_name DOT port_name
{
  // scoped_port_name : scoped_instance_name DOT port_name
  auto& identifiers = $[scoped_instance_name];
  auto  portName    = $[port_name];

  $$ = make_tuple(std::move(identifiers), portName);
}
;


signal : // alias_name is already reg_port_signal_id
  number
  {
    // signal : number
    auto node = ast.Create_Signal($[number]);

    $$ = node;
  }
| reg_port_signal_id
  {
    // signal : reg_port_signal_id
    auto node = ast.Create_Signal($[reg_port_signal_id]);

    $$ = node;
  }
| scoped_port_name
  {
    // signal : scoped_port_name
    auto& path     = std::get<0>($[scoped_port_name]);
    auto  portName = std::get<1>($[scoped_port_name]);
    auto  node     = ast.Create_Signal(std::move(path), portName);

    $$ = node;
  }
;


signal_or_inverted_signal :
  signal
  {
    // scan_signal : signal
    $$ = $1;
  }
| TILDE signal
  {
    // scan_signal : TILDE signal
    auto signalNode = $[signal];
    if (signalNode == nullptr)
    {
      $$ = nullptr;
    }
    else
    {
      signalNode->IsInverted(true);
      $$ = signalNode;
    }
  }
;

data_signal : signal_or_inverted_signal { $$ = $1; /* data_signal : signal_or_inverted_signal */ };
scan_signal : signal_or_inverted_signal { $$ = $1; /* scan_signal : signal_or_inverted_signal */ };

concat_signal_or_inverted_signal :
  signal_or_inverted_signal
  {
    // concat_signal_or_inverted_signal: signal_or_inverted_signal
    std::vector<Parsers::AST_Signal*> signals;

    auto signal = $[signal_or_inverted_signal];
    if (signal != nullptr)
    {
      signals.push_back(signal);
    }
    $$ = std::move(signals);
  }
| concat_signal_or_inverted_signal[lhs] COMMA signal_or_inverted_signal
  {
    // concat_signal_or_inverted_signal: concat_signal_or_inverted_signal[lhs] signal_or_inverted_signal
    auto& signals = $[lhs];
    auto  signal  = $[signal_or_inverted_signal];

    if (signal != nullptr)
    {
      signals.push_back(signal);
    }
    $$ = std::move(signals);
  }
;


//clock_signal : signal | TILDE signal ;
//tck_signal : signal ;
//tms_signal : signal ;
//trst_signal : signal ;
//shiftEn_signal : signal ;
captureEn_signal : concat_data_signal ;
updateEn_signal : concat_data_signal ;

concat_reset_signal   : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_reset_signal   : concat_signal_or_inverted_signal */};
concat_data_signal    : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_data_signal    : concat_signal_or_inverted_signal */};
concat_scan_signal    : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_scan_signal    : concat_signal_or_inverted_signal */};
concat_clock_signal   : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_clock_signal   : concat_signal_or_inverted_signal */};
concat_shiftEn_signal : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_shiftEn_signal : concat_signal_or_inverted_signal */};
concat_trst_signal    : concat_signal_or_inverted_signal { $$ = std::move($1); /* concat_trst_signal    : concat_signal_or_inverted_signal */};

concat_tms_signal : signal | concat_tms_signal COMMA signal;

// 6.4.2
icl_source :
  iclSource_items
  {
    // icl_source : iclSource_items
    // ==> Reset namespace once ALL iclSource_items have been parsed
    ast.SetRootNamespace();
  }
| icl_source iclSource_items
  {
    // icl_source : icl_source iclSource_items
    // ==> Do not reset namespace each time a a iclSource_items is parsed
  }
;

iclSource_items : nameSpace_def | useNameSpace_def | module_def;

// 6.4.3
nameSpace_def :
  NAMESPACE namespace_name SEMICOLON
  {
    // nameSpace_def : namespace_name SEMICOLON
    auto& name = $[namespace_name];
    ast.SetModuleNamespace(std::move(name));
    $$ = nullptr;
  }
| NAMESPACE SEMICOLON
  {
    // nameSpace_def : NAMESPACE SEMICOLON
    ast.SetRootNamespace();
    $$ = nullptr;
  }
;

// 6.4.4
useNameSpace_def :
  USENAMESPACE namespace_name SEMICOLON
  {
    // useNameSpace_def : USENAMESPACE namespace_name SEMICOLON
    auto& name = $[namespace_name];
    ast.SetInstanceNamespace(std::move(name));
    $$ = nullptr;
  }
| USENAMESPACE SEMICOLON
  {
    // useNameSpace_def : USENAMESPACE SEMICOLON
    ast.SetInstanceNamespace();
    $$ = nullptr;
  }
;

// 6.4.5
module_def :
  MODULE
  {
    // module_def : MODULE {mid_rule} module_body
    ast.SaveInstanceDefaultNamespace();
  }
  module_body
  {
    // module_def : MODULE module_body
    auto module = $[module_body];
    $$ = module;
  }
;

module_body :
  module_name LEFT_BRACE module_items RIGHT_BRACE
  {
    // module_def : MODULE module_name LEFT_BRACE module_items RIGHT_BRACE
    auto name   = $[module_name];
    auto module = ast.Create_Module(name, std::move($[module_items]));

    $$ = module;
  }
| module_name LEFT_BRACE RIGHT_BRACE
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
| module_item
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
    // module_item : useNameSpace_def
    $$ = $1;
  }
| attribute_def
  {
    // module_item : attribute_def
    $$ = $1;
  }
| parameter_def
  {
    // module_item : parameter_def
    $$ = $1;
  }
| localParameter_def
  {
    // module_item : localParameter_def
    $$ = $1;
  }
| scanInterface_def
  {
    // module_item : scanInterface_def
    $$ = $1;
  }
| port_def
  {
    // module_item : port_def
    $$ = $1;
  }
| instance_def
  {
    // module_item : instance_def
    $$ = $1;
  }
| logicSignal_def
  {
    // module_item : logicSignal_def
    $$ = $1;
  }
| scanRegister_def
  {
    // module_item : scanRegister_def
    $$ = $1;
  }
| dataRegister_def
  {
    // module_item : dataRegister_def
    $$ = $1;
  }
| scanMux_def
  {
    // module_item : scanMux_def
    $$ = $1;
  }
| dataMux_def
  {
    // module_item : dataMux_def
    $$ = $1;
  }
| clockMux_def
  {
    // module_item : clockMux_def
    $$ = $1;
  }
| oneHotDataGroup_def
  {
    // module_item : oneHotDataGroup_def
    $$ = $1;
  }
| oneHotScanGroup_def
  {
    // module_item : oneHotScanGroup_def
    $$ = $1;
  }
| enum_def
  {
    // module_item : enum_def
    $$ = $1;
  }
| alias_def
  {
    // module_item : alias_def
    $$ = $1;
  }
| accessLink_def
  {
    // module_item : accessLink_def
    $$ = $1;
  }
;

port_def :
  scanInPort_def
  {
    // port_def : scanInPort_def
    $$ = $1;
  }
| scanOutPort_def
  {
    // port_def : scanOutPort_def
    $$ = $1;
  }
| shiftEnPort_def
  {
    // port_def : shiftEnPort_def
    $$ = $1;
  }
| captureEnPort_def
  {
    // port_def : captureEnPort_def
    $$ = $1;
  }
| updateEnPort_def
  {
    // port_def : updateEnPort_def
    $$ = $1;
  }
| dataInPort_def
  {
    // port_def : dataInPort_def
    $$ = $1;
  }
| dataOutPort_def
  {
    // port_def : dataOutPort_def
    $$ = $1;
  }
| toShiftEnPort_def
  {
    // port_def : toShiftEnPort_def
    $$ = $1;
  }
| toUpdateEnPort_def
  {
    // port_def : toUpdateEnPort_def
    $$ = $1;
  }
| toCaptureEnPort_def
  {
    // port_def : toCaptureEnPort_def
    $$ = $1;
  }
| selectPort_def
  {
    // port_def : selectPort_def
    $$ = $1;
  }
| toSelectPort_def
  {
    // port_def : toSelectPort_def
    $$ = $1;
  }
| resetPort_def
  {
    // port_def : resetPort_def
    $$ = $1;
  }
| toIRSelectPort_def
  {
    // port_def : toIRSelectPort_def
    $$ = $1;
  }
| tmsPort_def
  {
    // port_def : tmsPort_def
    $$ = $1;
  }
| toTmsPort_def
  {
    // port_def : toTmsPort_def
    $$ = $1;
  }
| tckPort_def
  {
    // port_def : tckPort_def
    $$ = $1;
  }
| toTckPort_def
  {
    // port_def : toTckPort_def
    $$ = $1;
  }
| clockPort_def
  {
    // port_def : clockPort_def
    $$ = $1;
  }
| toClockPort_def
  {
    // port_def : toClockPort_def
    $$ = $1;
  }
| trstPort_def
  {
    // port_def : trstPort_def
    $$ = $1;
  }
| toTrstPort_def
  {
    // port_def : toTrstPort_def
    $$ = $1;
  }
| toResetPort_def
  {
    // port_def : toResetPort_def
    $$ = $1;
  }
| addressPort_def
  {
    // port_def : addressPort_def
    $$ = $1;
  }
| writeEnPort_def
  {
    // port_def : writeEnPort_def
    $$ = $1;
  }
| readEnPort_def
  {
    // port_def : readEnPort_def
    $$ = $1;
  }
;

// 6.4.6.1
scanInPort_def :
  SCANINPORT scanInPort_name  SEMICOLON
  {
    // scanInPort_def : SCANINPORT scanInPort_name SEMICOLON
    auto& name     = $[scanInPort_name];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanInPort, name);

    $$ = node;
  }
| SCANINPORT scanInPort_name  LEFT_BRACE RIGHT_BRACE
  {
    // scanInPort_def : SCANINPORT scanInPort_name  LEFT_BRACE RIGHT_BRACE
    auto& name     = $[scanInPort_name];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanInPort, name);

    $$ = node;
  }
| SCANINPORT scanInPort_name LEFT_BRACE scanInPort_items RIGHT_BRACE
  {
    // scanInPort_def : SCANINPORT scanInPort_name  LEFT_BRACE scanInPort_items RIGHT_BRACE
    auto& name     = $[scanInPort_name];
    auto& children = $[scanInPort_items];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanInPort, name, std::move(children));

    $$ = node;
  }
;

scanInPort_items:
  scanInPort_items[lhs] attribute_def
  {
    // scanInPort_items: scanInPort_items[lhs] attribute_def
    auto& children = $[lhs];
    auto  item     = $[attribute_def];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| attribute_def
  {
    // scanInPort_items: attribute_def
    std::vector<Parsers::AST_Node*> children;

    auto item = $[attribute_def];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;


scanInPort_name : port_name
{
  // scanInPort_name : port_name
  $$ = std::move($1);
}
;

// 6.4.6.2
scanOutPort_def :
  SCANOUTPORT scanOutPort_name  SEMICOLON
  {
    // scanOutPort_def : SCANOUTPORT scanOutPort_name SEMICOLON
    auto& name     = $[scanOutPort_name];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanOutPort, name);

    $$ = node;
  }
| SCANOUTPORT scanOutPort_name  LEFT_BRACE RIGHT_BRACE
  {
    // scanOutPort_def : SCANOUTPORT scanOutPort_name  LEFT_BRACE RIGHT_BRACE
    auto& name     = $[scanOutPort_name];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanOutPort, name);

    $$ = node;
  }
| SCANOUTPORT scanOutPort_name LEFT_BRACE scanOutPort_items RIGHT_BRACE
  {
    // scanOutPort_def : SCANOUTPORT scanOutPort_name  LEFT_BRACE scanOutPort_items RIGHT_BRACE
    auto& name     = $[scanOutPort_name];
    auto& children = $[scanOutPort_items];
    auto  node     = ast.Create_Port(Parsers::Kind::ScanOutPort, name, std::move(children));

    $$ = node;
  }
;

scanOutPort_name : port_name
{
  // scanOutPort_name : port_name
  $$ = std::move($1);
}
;

scanOutPort_items:
  scanOutPort_items[lhs] scanOutPort_item
  {
    // scanOutPort_items: scanOutPort_items[lhs] scanOutPort_item
    auto& children = $[lhs];
    auto  item     = $[scanOutPort_item];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| scanOutPort_item
  {
    // scanOutPort_items: scanOutPort_item
    std::vector<Parsers::AST_Node*> children;

    auto item = $[scanOutPort_item];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;

scanOutPort_item :
  attribute_def
  {
    // scanOutPort_item : attribute_def
    $$ = $1;
  }
| scanOutPort_source
  {
    // scanOutPort_item : scanOutPort_source
    $$ = $1;
  }
| scanOutPort_enable
  {
    // scanOutPort_item : scanOutPort_enable
    $$ = nullptr;
  }
| scanOutPort_launchEdge
  {
    // scanOutPort_item : scanOutPort_launchEdge
    $$ = nullptr;
  }
;


port_source : SOURCE concat_scan_signal SEMICOLON
{
  // port_source : SOURCE concat_scan_signal SEMICOLON
  auto& signals = $[concat_scan_signal];
  auto  source  = ast.Create_Source(Parsers::Kind::Source, std::move(signals));

  $$ = source;
}

scanOutPort_source : port_source
{
  // scanOutPort_source : port_source
  auto source = $[port_source];
  $$ = source;
}
;
scanOutPort_enable : ENABLE data_signal SEMICOLON;
scanOutPort_launchEdge : LAUNCHEDGE rising_or_falling SEMICOLON ;
rising_or_falling : RISING | FALLING;

// 6.4.6.3
shiftEnPort_def : SHIFTENPORT shiftEnPort_name shiftEnPort_tail
{
  // shiftEnPort_def : SHIFTENPORT shiftEnPort_name shiftEnPort_tail
  $$ = nullptr;
}
;

shiftEnPort_tail: SEMICOLON | LEFT_BRACE shiftEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
shiftEnPort_items: shiftEnPort_items attribute_def | attribute_def;
shiftEnPort_name : port_name ;

// 6.4.6.4
captureEnPort_def : CAPTUREENPORT captureEnPort_name captureEnPort_tail
{
  // captureEnPort_def : CAPTUREENPORT captureEnPort_name captureEnPort_tail
  $$ = nullptr;
}
;

captureEnPort_tail: SEMICOLON | LEFT_BRACE captureEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
captureEnPort_items: captureEnPort_items attribute_def | attribute_def;
captureEnPort_name : port_name ;

// 6.4.6.5
updateEnPort_def : UPDATEENPORT updateEnPort_name updateEnPort_tail
{
  // updateEnPort_def : UPDATEENPORT updateEnPort_name updateEnPort_tail
  $$ = nullptr;
}
;

updateEnPort_tail: SEMICOLON | LEFT_BRACE updateEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
updateEnPort_items: updateEnPort_items attribute_def | attribute_def;
updateEnPort_name : port_name ;

// 6.4.6.6
dataInPort_def : DATAINPORT dataInPort_name dataInPort_tail
{
  // dataInPort_def : DATAINPORT dataInPort_name dataInPort_tail
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
  // dataOutPort_def : DATAOUTPORT dataOutPort_name dataOutPort_tail
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
  // toShiftEnPort_def : TOSHIFTENPORT toShiftEnPort_name toShiftEnPort_tail
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
  // toCaptureEnPort_def : TOCAPTUREENPORT toCaptureEnPort_name toCaptureEnPort_tail
  $$ = nullptr;
}
;

toCaptureEnPort_tail: SEMICOLON | LEFT_BRACE toCaptureEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toCaptureEnPort_items: toCaptureEnPort_items toCaptureEnPort_item | toCaptureEnPort_item;
toCaptureEnPort_name : port_name ;
toCaptureEnPort_item :
  attribute_def
| toCaptureEnPort_source
;

toCaptureEnPort_source : SOURCE captureEn_signal SEMICOLON ;

// 6.4.6.10
toUpdateEnPort_def : TOUPDATEENPORT toUpdateEnPort_name toUpdateEnPort_tail
{
  // toUpdateEnPort_def : TOUPDATEENPORT toUpdateEnPort_name toUpdateEnPort_tail
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
  // selectPort_def : SELECTPORT selectPort_name selectPort_tail
  $$ = nullptr;
}
;

selectPort_tail: SEMICOLON | LEFT_BRACE selectPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
selectPort_items: selectPort_items attribute_def | attribute_def;
selectPort_name : port_name ;

// 6.4.6.12
toSelectPort_def : TOSELECTPORT toSelectPort_name toSelectPort_tail
{
  // toSelectPort_def : TOSELECTPORT toSelectPort_name toSelectPort_tail
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
  // resetPort_def : RESETPORT resetPort_name resetPort_tail
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
  // toResetPort_def : TORESETPORT toResetPort_name toResetPort_tail
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
  // tmsPort_def : TMSPORT tmsPort_name tmsPort_tail
  $$ = nullptr;
}
;

tmsPort_tail: SEMICOLON | LEFT_BRACE tmsPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
tmsPort_items: tmsPort_items attribute_def | attribute_def;
tmsPort_name : port_name ;

// 6.4.6.16
toTmsPort_def : TOTMSPORT toTmsPort_name toTmsPort_tail
{
  // toTmsPort_def : TOTMSPORT toTmsPort_name toTmsPort_tail
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
  // toIRSelectPort_def : TOIRSELECTPORT toIRSelectPort_name toIRSelectPort_tail
  $$ = nullptr;
}
;

toIRSelectPort_tail: SEMICOLON | LEFT_BRACE toIRSelectPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toIRSelectPort_items: toIRSelectPort_items attribute_def | attribute_def;
toIRSelectPort_name : port_name ;

// 6.4.6.18
tckPort_def : TCKPORT tckPort_name tckPort_tail
{
  // tckPort_def : TCKPORT tckPort_name tckPort_tail
  $$ = nullptr;
}
;

tckPort_tail: SEMICOLON | LEFT_BRACE tckPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
tckPort_items: tckPort_items attribute_def | attribute_def;
tckPort_name : port_name ;

// 6.4.6.19
toTckPort_def : TOTCKPORT toTckPort_name toTckPort_tail
{
  // toTckPort_def : TOTCKPORT toTckPort_name toTckPort_tail
  $$ = nullptr;
}
;

toTckPort_tail: SEMICOLON | LEFT_BRACE toTckPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
toTckPort_items: toTckPort_items attribute_def | attribute_def;
toTckPort_name : port_name ;

// 6.4.6.20
clockPort_def : CLOCKPORT clockPort_name clockPort_tail
{
  // clockPort_def : CLOCKPORT clockPort_name clockPort_tail
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
  // toClockPort_def : TOCLOCKPORT toClockPort_name toClockPort_tail
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
  // trstPort_def : TRSTPORT trstPort_name trstPort_tail
  $$ = nullptr;
}
;
trstPort_tail: SEMICOLON | LEFT_BRACE trstPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
trstPort_items: trstPort_items attribute_def | attribute_def;
trstPort_name : port_name ;

// 6.4.6.23
toTrstPort_def : TOTRSTPORT toTrstPort_name toTrstPort_tail
{
  // toTrstPort_def : TOTRSTPORT toTrstPort_name toTrstPort_tail
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
  // addressPort_def : ADDRESSPORT addressPort_name addressPort_tail
  $$ = nullptr;
}
;

addressPort_tail: SEMICOLON | LEFT_BRACE addressPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
addressPort_items: addressPort_items attribute_def | attribute_def;
addressPort_name : port_name ;

// 6.4.6.25
writeEnPort_def : WRITEENPORT writeEnPort_name writeEnPort_tail
{
  // writeEnPort_def : WRITEENPORT writeEnPort_name writeEnPort_tail
  $$ = nullptr;
}
;

writeEnPort_tail: SEMICOLON | LEFT_BRACE writeEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
writeEnPort_items: writeEnPort_items attribute_def | attribute_def;
writeEnPort_name : port_name ;

// 6.4.6.26
readEnPort_def : READENPORT readEnPort_name readEnPort_tail
{
  // readEnPort_def : READENPORT readEnPort_name readEnPort_tail
  $$ = nullptr;
}
;

readEnPort_tail: SEMICOLON | LEFT_BRACE readEnPort_items RIGHT_BRACE | LEFT_BRACE RIGHT_BRACE;
readEnPort_items: readEnPort_items attribute_def | attribute_def;
readEnPort_name : port_name ;

// 6.4.7
instance_def :
  INSTANCE instance_name OF module_identifier SEMICOLON
  {
    // instance_def : INSTANCE instance_name OF module_identifier SEMICOLON
    auto instanceName     = $[instance_name];
    auto moduleIdentifier = $[module_identifier];
    auto node             = ast.Create_Instance(instanceName, moduleIdentifier);

    $$ = node;
  }
| INSTANCE instance_name OF module_identifier LEFT_BRACE RIGHT_BRACE
  {
    // instance_def : INSTANCE instance_name OF module_identifier LEFT_BRACE RIGHT_BRACE
    auto instanceName     = $[instance_name];
    auto moduleIdentifier = $[module_identifier];
    auto node             = ast.Create_Instance(instanceName, moduleIdentifier);

    $$ = node;
  }
| INSTANCE instance_name OF module_identifier LEFT_BRACE instance_items RIGHT_BRACE
  {
    // instance_def : INSTANCE instance_name OF module_identifier LEFT_BRACE instance_items RIGHT_BRACE
    auto  instanceName     = $[instance_name];
    auto  moduleIdentifier = $[module_identifier];
    auto& instanceItems    = $[instance_items];
    auto  node             = ast.Create_Instance(instanceName, moduleIdentifier, std::move(instanceItems));

    $$ = node;
  }
;

module_identifier :
  module_name
  {
    // module_identifier : module_name
    auto moduleNamespace = ast.InstancesDefaultNamespace();
    auto moduleName      = $[module_name];
    auto node            = ast.Create_ModuleIdentifier(moduleNamespace, moduleName);

    $$ = node;
  }
| namespace_name DOUBLE_COLON module_name
  {
    // module_identifier : namespace_name DOUBLE_COLON module_name
    auto namespaceName   = $[namespace_name];
    auto moduleNamespace = ast.Create_Namespace(std::move(namespaceName));
    auto moduleName      = $[module_name];
    auto node            = ast.Create_ModuleIdentifier(moduleNamespace, moduleName);

    $$ = node;
  }
| DOUBLE_COLON module_name
  {
    // module_identifier : DOUBLE_COLON module_name
    auto moduleNamespace = ast.RootNamespace();
    auto moduleName      = $[module_name];
    auto node            = ast.Create_ModuleIdentifier(moduleNamespace, moduleName);

    $$ = node;
  }
;


instance_items:
  instance_item
  {
    // instance_items : instance_item
    auto items = vector<Parsers::AST_Node*>();
    auto item  = $[instance_item];

    if (item != nullptr)
    {
      items.push_back(item);
    }

    $$ = std::move(items);
  }
| instance_items[lhs] instance_item
  {
    // instance_items : instance_items[lhs] instance_item
    auto& items = $[lhs];
    auto  item  = $[instance_item];

    if (item != nullptr)
    {
      items.push_back(item);
    }

    $$ = std::move(items);
  }
;

instance_item :
  inputPort_connection
  {
    // instance_item : inputPort_connection
    auto inputPortConnection = $[inputPort_connection];
    $$ = inputPortConnection;
  }
| allowBroadcast_def
  {
    // instance_item : allowBroadcast_def
    $$ = nullptr;
  }
| attribute_def
  {
    // instance_item : attribute_def
    auto attributeDef = $[attribute_def];
    $$ = attributeDef;
  }
| parameter_override
  {
    // instance_item : parameter_override
    auto parameterOverride = $[parameter_override];
    $$ = parameterOverride;
  }
| instance_addressValue
  {
      // instance_item : instance_addressValue
    $$ = nullptr;
  }
;

inputPort_connection : INPUTPORT inputPort_name EQUAL inputPort_source SEMICOLON
{
  // inputPort_connection : INPUTPORT inputPort_name EQUAL inputPort_source SEMICOLON
  auto  inputPortName   = $[inputPort_name];
  auto& inputPortSource = $[inputPort_source];
  auto  source          = ast.Create_Source(Parsers::Kind::InputPortSource, std::move(inputPortSource));
  auto  children        = vector<Parsers::AST_Node*>({ source });
  auto  node            = ast.Create_Port(Parsers::Kind::InputPort, inputPortName, std::move(children));

  $$ = node;
}
;

allowBroadcast_def : ALLOWBROADCASTONSCANINTERFACE allowBroadcast_items SEMICOLON ;
allowBroadcast_items : allowBroadcast_items COMMA scanInterface_name | scanInterface_name ;

inputPort_name   : port_name          { $$ = $1; /* inputPort_name : port_name */ };
inputPort_source : concat_data_signal { $$ = std::move($1); /* inputPort_source : concat_data_signal */};

parameter_override : parameter_def
{
  // parameter_override : parameter_def
  auto parameter = $[parameter_def];
  $$ = parameter;
}
;

instance_addressValue : ADDRESSVALUE number SEMICOLON ;

// 6.4.8
scanRegister_def : SCANREGISTER scanRegister_name scanRegister_tail
{
  // scanRegister_def : SCANREGISTER scanRegister_name scanRegister_tail
  auto& name     = $[scanRegister_name];
  auto& children = $[scanRegister_tail];
  auto  node     = ast.Create_ScanRegister(name, std::move(children));

  $$ = node;
}
;

scanRegister_name : register_name { $$ = $[register_name]; }

scanRegister_tail:
  SEMICOLON
  {
    // scanRegister_tail: SEMICOLON
    std::vector<Parsers::AST_Node*> children;
    $$ = children;
  }
| LEFT_BRACE scanRegister_items RIGHT_BRACE
  {
    // scanRegister_tail: LEFT_BRACE scanRegister_items RIGHT_BRACE
    auto& children = $[scanRegister_items];
    $$ = std::move(children);
  }
| LEFT_BRACE RIGHT_BRACE
  {
    // scanRegister_tail: LEFT_BRACE RIGHT_BRACE
    std::vector<Parsers::AST_Node*> children;
    $$ = children;
  }
;

scanRegister_items:
  scanRegister_items[lhs] scanRegister_item
  {
    // scanRegister_items: scanRegister_items[lhs] scanRegister_item
    auto& children = $[lhs];
    auto  item     = $[scanRegister_item];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| scanRegister_item
  {
    // scanRegister_items: scanRegister_item
    std::vector<Parsers::AST_Node*> children;

    auto item = $[scanRegister_item];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;

scanRegister_item :
  attribute_def                   { $$ = $1; /* scanRegister_item : attribute_def */ }
| scanRegister_scanInSource       { $$ = $1; /* scanRegister_item : scanRegister_scanInSource      */ }
| scanRegister_defaultLoadValue   { $$ = $1; /* scanRegister_item : scanRegister_defaultLoadValue  */ }
| scanRegister_captureSource      { $$ = $1; /* scanRegister_item : scanRegister_captureSource     */ }
| scanRegister_resetValue         { $$ = $1; /* scanRegister_item : scanRegister_resetValue        */ }
| scanRegister_refEnum            { $$ = $1; /* scanRegister_item : scanRegister_refEnum           */ }
;

scanRegister_scanInSource : SCANINSOURCE scan_signal SEMICOLON
{
  // scanRegister_scanInSource : SCANINSOURCE scan_signal SEMICOLON
  auto signal = $[scan_signal];
  auto source = ast.Create_Source(Parsers::Kind::ScanInSource, signal);

  $$ = source;
};

pin_id : instance_name DOT pin_id | instance_name DOT port_name  ;
signal_or_enum : number | SCALAR_ID | pin_id;

scanRegister_captureSource    : CAPTURESOURCE    signal_or_enum SEMICOLON { $$ = nullptr; /* scanRegister_captureSource : CAPTURESOURCE signal_or_enum SEMICOLON */};


scanRegister_defaultLoadValue : DEFAULTLOADVALUE number_or_enum SEMICOLON
{
  // scanRegister_defaultLoadValue : DEFAULTLOADVALUE number_or_enum SEMICOLON
  auto& valueExpression = $[number_or_enum];
  auto  node            = ast.Create_Value(Parsers::Kind::DefaultLoadValue, valueExpression);

  $$ = node;
};

scanRegister_resetValue : RESETVALUE number_or_enum SEMICOLON
{
  // scanRegister_resetValue : RESETVALUE number_or_enum SEMICOLON
  auto& valueExpression = $[number_or_enum];
  auto  node            = ast.Create_Value(Parsers::Kind::ResetValue, valueExpression);

  $$ = node;
}
;

scanRegister_refEnum : REFENUM enum_name SEMICOLON { $$ = nullptr; /* scanRegister_refEnum : REFENUM enum_name SEMICOLON */};

number_or_enum :
  concat_number
  {
    // number_or_enum : concat_number
    auto& numbers = $[concat_number];
    $$ = std::move(numbers);
  }
| enum_symbol
  {
    // number_or_enum : enum_symbol
    auto& enumSymbol = $[enum_symbol];
    $$ = std::move(enumSymbol);
  }
;

// 6.4.9
dataRegister_def : DATAREGISTER dataRegister_name dataRegister_tail
{
  // dataRegister_def : DATAREGISTER dataRegister_name dataRegister_tail
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

iProc_args :
  D_SUBST
| R_SUBST
| number
| STRING ;

sub_namespace : SCALAR_ID |
parameter_ref ;
ref_module_name : SCALAR_ID |
parameter_ref ;

// 6.4.10
logicSignal_def : LOGICSIGNAL logicSignal_name LEFT_BRACE logic_expr SEMICOLON RIGHT_BRACE
{
  // logicSignal_def : LOGICSIGNAL logicSignal_name LEFT_BRACE logic_expr SEMICOLON RIGHT_BRACE
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
  // scanMux_def : SCANMUX scanMux_name SELECTEDBY scanMux_select LEFT_BRACE scanMux_selections RIGHT_BRACE
  auto& name           = $[scanMux_name];
  auto& selectors      = $[scanMux_select];
  auto& selectionTable = $[scanMux_selections];

  auto  node = ast.Create_ScanMux(name, std::move(selectors), std::move(selectionTable));

  $$ = node;
}
;

scanMux_selections :
  scanMux_selection
  {
    // scanMux_selections : scanMux_selection
    auto  selections = std::vector<Parsers::AST_ScanMuxSelection*>();
    auto  selection  = $[scanMux_selection];

    selections.push_back(selection);

    $$ = std::move(selections);
  }
| scanMux_selections[lhs] scanMux_selection
  {
    // scanMux_selections : scanMux_selections[lhs] scanMux_selection
    auto& selections = $[lhs];
    auto  selection  = $[scanMux_selection];

    selections.push_back(selection);

    $$ = std::move(selections);
  }
;

scanMux_name : reg_port_signal_id ;

scanMux_select : concat_data_signal
{
  // scanMux_select : concat_data_signal
  auto& concatSignals = $[concat_data_signal];

  $$ = std::move(concatSignals);
}
;

scanMux_selection : concat_number_list COLON concat_scan_signal SEMICOLON
{
  // scanMux_selection : concat_number_list COLON concat_scan_signal SEMICOLON
  auto& selectionValues = $[concat_number_list];
  auto& selectedSignals = $[concat_scan_signal];

  auto node = ast.Create_ScanMuxSelection(std::move(selectionValues), std::move(selectedSignals));

  $$ = node;
}
;
// 6.4.12
dataMux_def : DATAMUX dataMux_name SELECTEDBY dataMux_select LEFT_BRACE dataMux_selections RIGHT_BRACE
{
  // dataMux_def : DATAMUX dataMux_name SELECTEDBY dataMux_select LEFT_BRACE dataMux_selections RIGHT_BRACE
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
  // clockMux_def : CLOCKMUX clockMux_name SELECTEDBY clockMux_select LEFT_BRACE clockMux_selections RIGHT_BRACE
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
  // oneHotScanGroup_def : ONEHOTSCANGROUP oneHotScanGroup_name LEFT_BRACE oneHotScanGroup_items RIGHT_BRACE
  $$ = nullptr;
}
;
oneHotScanGroup_items : oneHotScanGroup_items oneHotScanGroup_item | oneHotScanGroup_item ;
oneHotScanGroup_name : reg_port_signal_id ;
oneHotScanGroup_item : PORT concat_scan_signal SEMICOLON ;

// 6.4.15
oneHotDataGroup_def : ONEHOTDATAGROUP oneHotDataGroup_name LEFT_BRACE oneHotDataGroup_items RIGHT_BRACE
{
  // oneHotDataGroup_def : ONEHOTDATAGROUP oneHotDataGroup_name LEFT_BRACE oneHotDataGroup_items RIGHT_BRACE
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
accessLink_def :
  accessLink1149_def
  {
    // accessLink_def : accessLink1149_def
    $$ = $1;
  }
| accessLinkGeneric_def
  {
    // accessLink_def : accessLinkGeneric_def
    $$ = $1;
  }
;

accessLinkGeneric_def : ACCESSLINK accessLink_name OF accessLink_genericID  LEFT_BRACE RIGHT_BRACE
{
  // accessLinkGeneric_def : ACCESSLINK accessLink_name OF accessLink_genericID  LEFT_BRACE RIGHT_BRACE
  auto& name          = $[accessLink_name];
  auto  genericId     = $[accessLink_genericID];
  auto  nameNode      = ast.Create_ScalarIdentifier(name);
  auto  genericIdNode = ast.Create_ScalarIdentifier(genericId);
  auto  node          = ast.Create_AccessLink(nameNode, genericIdNode);

  $$ = node;
}
;

accessLink_genericID : SCALAR_ID
{
  // accessLink_genericID : SCALAR_ID
  auto& id = $[SCALAR_ID];
  $$ = std::move(id);
};

accessLink1149_def : ACCESSLINK accessLink_name OF accessLink1149_stds LEFT_BRACE BSDLENTITIY bsdlEntity_name SEMICOLON bsdl_instr_refs RIGHT_BRACE
{
  // accessLink1149_def : ACCESSLINK accessLink_name OF accessLink1149_stds LEFT_BRACE BSDLENTITIY bsdlEntity_name SEMICOLON bsdl_instr_refs RIGHT_BRACE
  auto& name      = $[accessLink_name];
  auto  nameId    = ast.Create_ScalarIdentifier(name);
  auto  type      = $[accessLink1149_stds];
  auto  children  = vector<Parsers::AST_Node*>();
  auto  node      = ast.Create_AccessLink(nameId, type, std::move(children));

  // bsdlEntity_name and optional bsdl_instr_refs are ignored!

  $$ = node;
}
;

accessLink1149_stds :
  STD_1149_1_2001
  {
    // accessLink1149_stds : STD_1149_1_2001
    $$ = Parsers::AccessLinkType::STD_1149_1_2001;
  }
| STD_1149_1_2013
  {
    // accessLink1149_stds : STD_1149_1_2013
    $$ = Parsers::AccessLinkType::STD_1149_1_2013;
  }
;

accessLink_name : SCALAR_ID
{
  // accessLink_name : SCALAR_ID
  auto& name = $[SCALAR_ID];
  $$ = std::move(name);
};

bsdlEntity_name : SCALAR_ID ;
bsdl_instr_refs : bsdl_instr_refs bsdl_instr_ref | bsdl_instr_ref ;
bsdl_instr_ref : bsdl_instr_name LEFT_BRACE bsdl_instr_selected_items RIGHT_BRACE ;
bsdl_instr_selected_items : bsdl_instr_selected_items bsdl_instr_selected_item | bsdl_instr_selected_item ;
bsdl_instr_name : SCALAR_ID ;

bsdl_instr_selected_item :
  SCANINTERFACE LEFT_BRACE accessLink1149_ScanInterface_names RIGHT_BRACE
| ACTIVESIGNALS LEFT_BRACE accessLink1149_ActiveSignal_names RIGHT_BRACE ;

accessLink1149_ScanInterface_names : accessLink1149_ScanInterface_names accessLink1149_ScanInterface_name SEMICOLON | accessLink1149_ScanInterface_name SEMICOLON;
accessLink1149_ActiveSignal_name : reg_port_signal_id ;
accessLink1149_ActiveSignal_names : accessLink1149_ActiveSignal_names accessLink1149_ActiveSignal_name SEMICOLON | accessLink1149_ActiveSignal_name SEMICOLON;
accessLink1149_ScanInterface_name : instance_name | instance_name DOT scanInterface_name ;

// 6.4.17
scanInterface_def : SCANINTERFACE scanInterface_name LEFT_BRACE scanInterface_items RIGHT_BRACE
{
  // scanInterface_def : SCANINTERFACE scanInterface_name LEFT_BRACE scanInterface_items RIGHT_BRACE
  auto& name       = $[scanInterface_name];
  auto& children   = $[scanInterface_items];
  auto  identifier = ast.Create_ScalarIdentifier(std::move(name));

  auto  node = ast.Create_ScanInterface(identifier, std::move(children));

  $$ = node;
}
;

scanInterface_items :
  scanInterface_item
  {
    // scanInterface_items : scanInterface_item
    auto items = vector<Parsers::AST_Node*>();
    auto item  = $[scanInterface_item];

    if (item != nullptr)
    {
      items.push_back(item);
    }

    $$ = std::move(items);
  }
| scanInterface_items[lhs] scanInterface_item
  {
    // scanInterface_items : scanInterface_items[lhs] scanInterface_item
    auto& items = $[lhs];
    auto  item  = $[scanInterface_item];

    if (item != nullptr)
    {
      items.push_back(item);
    }

    $$ = std::move(items);
  }
;

scanInterface_name : SCALAR_ID
{
  // scanInterface_name : SCALAR_ID
  auto& name = $[SCALAR_ID];

  $$ = std::move(name);
};

scanInterface_item :
  attribute_def
  {
    // scanInterface_item : attribute_def
    $$ = $1;
  }
| scanInterfacePort_def
  {
    // scanInterface_item : scanInterfacePort_def
    $$ = $1;
  }
| defaultLoad_def
  {
    // scanInterface_item : defaultLoad_def
    $$ = nullptr;
  }
| scanInterfaceChain_def
  {
    // scanInterface_item : scanInterfaceChain_def
    $$ = nullptr;
  }
;

scanInterfacePort_def : PORT reg_port_signal_id SEMICOLON
{
  // scanInterfacePort_def : PORT reg_port_signal_id SEMICOLON
  auto identifier = $[reg_port_signal_id];
  auto node       = ast.Create_Port(Parsers::Kind::Port, identifier);

  $$ = node;
};

scanInterfaceChain_def : CHAIN scanInterfaceChain_name LEFT_BRACE scanInterfaceChain_items RIGHT_BRACE ;
scanInterfaceChain_items : scanInterfaceChain_items scanInterfaceChain_item | scanInterfaceChain_item ;
scanInterfaceChain_name : SCALAR_ID;
scanInterfaceChain_item : attribute_def | scanInterfacePort_def | defaultLoad_def ;
defaultLoad_def : DEFAULTLOADVALUE concat_number SEMICOLON ;

// 6.5.2
alias_def : ALIAS alias_name EQUAL concat_hier_data_signal alias_tail
{
  // alias_def : ALIAS alias_name EQUAL concat_hier_data_signal alias_tail
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
  // enum_def : ENUM enum_name LEFT_BRACE enum_items RIGHT_BRACE
  $$ = nullptr;
}
;
enum_items : enum_items enum_item | enum_item;
enum_name : SCALAR_ID
{
  // enum_name : SCALAR_ID
  auto& enumName = $[SCALAR_ID];
  $$ = std::move(enumName);
}
;
enum_item : enum_symbol EQUAL enum_value SEMICOLON ;
enum_symbol : SCALAR_ID
{
  // enum_symbol : SCALAR_ID
  auto& enumSymbol = $[SCALAR_ID];
  $$ = std::move(enumSymbol);
}
;

enum_value : concat_number
{
  // enum_value : concat_number
  auto& concatNumber = $[concat_number];
  $$ = std::move(concatNumber);
};


concat_string :
  STRING
  {
    // concat_string : STRING
    vector<Parsers::AST_SimpleNode*> concatenatedString;
    auto stringContent = CleanString($[STRING]);

    if (!stringContent.empty())
    {
      auto node = ast.Create_String(std::move(stringContent));
      concatenatedString.emplace_back(node);
    }

    $$ = std::move(concatenatedString);
  }
| parameter_ref
  {
    // concat_string : parameter_ref
    vector<Parsers::AST_SimpleNode*> concatenatedString;

    auto  paramRef = $[parameter_ref];

    concatenatedString.emplace_back(paramRef);

    $$ = std::move(concatenatedString);
  }
| concat_string[lhs] COMMA STRING
  {
    // concat_string : concat_string[lhs] COMMA STRING
    auto& concatenatedString = $[lhs];
    auto  stringContent      = CleanString($[STRING]);

    if (!stringContent.empty())
    {
      auto node = ast.Create_String(std::move(stringContent));
      concatenatedString.emplace_back(node);
    }

    $$ = std::move(concatenatedString);
  }
| concat_string[lhs] COMMA parameter_ref
  {
    // concat_string : concat_string[lhs] COMMA parameter_ref
    auto& concatenatedString = $[lhs];;
    auto& paramRef           = $[parameter_ref];

    concatenatedString.emplace_back(paramRef);

    $$ = std::move(concatenatedString);
  }
;

// 6.5.4
parameter_def :
  PARAMETER parameter_name EQUAL concat_number SEMICOLON
  {
    // parameter_def : PARAMETER parameter_name EQUAL concat_number SEMICOLON
    auto& name  = $[parameter_name];
    auto  value = $[concat_number];
    auto  node  = ast.Create_Parameter(std::move(name), std::move(value));

    $$ = node;
  }
| PARAMETER parameter_name EQUAL concat_string SEMICOLON
  {
    // parameter_def : PARAMETER parameter_name EQUAL concat_string SEMICOLON
    auto& name  = $[parameter_name];
    auto  value = $[concat_string];
    auto  node  = ast.Create_Parameter(std::move(name), std::move(value));

    $$ = node;
  }
;

localParameter_def :
  LOCALPARAMETER parameter_name EQUAL concat_number SEMICOLON
  {
    // localParameter_def : LOCALPARAMETER parameter_name EQUAL concat_number SEMICOLON
    auto& name  = $[parameter_name];
    auto  value = $[concat_number];
    auto  node  = ast.Create_LocalParameter(std::move(name), std::move(value));

    $$ = node;
  }
| LOCALPARAMETER parameter_name EQUAL concat_string SEMICOLON
  {
    // localParameter_def : LOCALPARAMETER parameter_name EQUAL concat_string SEMICOLON
    auto& name  = $[parameter_name];
    auto  value = $[concat_string];
    auto  node  = ast.Create_LocalParameter(std::move(name), std::move(value));

    $$ = node;
  }
;

parameter_name : SCALAR_ID
{
  // parameter_name : SCALAR_ID
  auto& name = $[SCALAR_ID];
  $$ = std::move(name);
}
;

// 6.5.5
attribute_def :
  ATTRIBUTE attribute_name SEMICOLON
  {
    // attribute_def : ATTRIBUTE attribute_name SEMICOLON
    auto& name = $[attribute_name];
    auto  node = ast.Create_Attribute(std::move(name));

    $$ = node;
  }
| ATTRIBUTE attribute_name EQUAL concat_number SEMICOLON
  {
    // attribute_def : ATTRIBUTE attribute_name EQUAL concat_number SEMICOLON
    auto& name  = $[attribute_name];
    auto  value = $[concat_number];
    auto  node  = ast.Create_Attribute(std::move(name), std::move(value));

    $$ = node;
  }
| ATTRIBUTE attribute_name EQUAL concat_string SEMICOLON
  {
    // attribute_def : ATTRIBUTE attribute_name EQUAL concat_string SEMICOLON
    auto& name  = $[attribute_name];
    auto  value = $[concat_string];
    auto  node  = ast.Create_Attribute(std::move(name), std::move(value));

    $$ = node;
  }
;

attribute_name : SCALAR_ID
{
  // attribute_name : SCALAR_ID
  auto& name = $[SCALAR_ID];
  $$ = std::move(name);
}
;

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
