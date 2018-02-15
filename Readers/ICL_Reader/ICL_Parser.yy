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
  class AST_Alias;
  class AST_Attribute;
  class AST_BasedNumber;
  class AST_BsdlInstructionRef;
  class AST_ConcatNumber;
  class AST_EnumRef;
  class AST_FileRef;
  class AST_Identifier;
  class AST_Instance;
  class AST_IntegerExpr;
  class AST_IntegerLiteral;
  class AST_IntegerBinaryExpr;
  class AST_IntegerUnaryExpr;
  class AST_ModuleIdentifier;
  class AST_Module;
  class AST_Namespace;
  class AST_Node;
  class AST_Number;
  class AST_Parameter;
  class AST_ParameterRef;
  class AST_Port;
  class AST_ScalarIdentifier;
  class AST_ScanInterface;
  class AST_ScanInterfaceRef;
  class AST_ScanMux;
  class AST_ScanMuxSelection;
  class AST_ScanRegister;
  class AST_Signal;
  class AST_SimpleNode;
  class AST_Source;
  class AST_String;
  class AST_VectorIdentifier;

  template <typename> class AST_PlaceHolder;
  template <typename> class AST_Value;

  enum class AccessLinkType;
  enum class Kind : uint8_t;
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
#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_BasedNumber.hpp"
#include "AST_BsdlInstructionRef.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_EnumRef.hpp"
#include "AST_FileRef.hpp"
#include "AST_Instance.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_IntegerBinaryExpr.hpp"
#include "AST_IntegerExpr.hpp"
#include "AST_IntegerExprRef.hpp"
#include "AST_IntegerLiteral.hpp"
#include "AST_IntegerUnaryExpr.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_PlaceHolder.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanInterfaceRef.hpp"
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

#define THROW_PARSER_ERROR(msg) THROW_IMPL(Parsers::ParserException, msg)

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
%type <std::string> bsdlEntity_name
%type <std::string> bsdl_instr_name

%type <Parsers::AccessLinkType>        accessLink1149_stds

%type <Parsers::AST_ScalarIdentifier*> instance_name
%type <Parsers::AST_ScalarIdentifier*> module_name
%type <std::string>                    namespace_name



%type <Parsers::AST_ModuleIdentifier*>                                                          module_identifier
%type <std::vector<Parsers::AST_BsdlInstructionRef*>>                                           bsdl_instr_refs
%type <std::vector<Parsers::AST_ScalarIdentifier*>>                                             scoped_instance_name
%type <std::tuple<std::vector<Parsers::AST_ScalarIdentifier*>, Parsers::AST_VectorIdentifier*>> scoped_port_name

%type <std::tuple<Parsers::AST_ScalarIdentifier*,              std::string>>              accessLink1149_ScanInterface_name
%type <std::vector<std::tuple<Parsers::AST_ScalarIdentifier*,  std::string>>>             accessLink1149_ScanInterface_names

%type <Parsers::AST_ParameterRef*> parameter_ref

%type <Parsers::AST_Node*>                     parameter_override
%type <Parsers::AST_Node*>                     instance_item
%type <std::vector<Parsers::AST_Node*>>        instance_items
%type <std::vector<Parsers::AST_SimpleNode*>>  concat_string

%type <std::string> UNSIZED_DEC_NUM
%type <std::string> UNSIZED_BIN_NUM
%type <std::string> UNSIZED_HEX_NUM

%type <Parsers::AST_BasedNumber*> unsized_dec_num
%type <Parsers::AST_BasedNumber*> unsized_bin_num
%type <Parsers::AST_BasedNumber*> unsized_hex_num

%type <Parsers::AST_BasedNumber*> sized_dec_num
%type <Parsers::AST_BasedNumber*> sized_bin_num
%type <Parsers::AST_BasedNumber*> sized_hex_num

%type <std::string>                    POS_INT
%type <Parsers::Kind>                  add_operator
%type <Parsers::Kind>                  mult_operator
%type <Parsers::AST_IntegerLiteral*>   pos_int
%type <Parsers::AST_IntegerExpr*>      integer_arg_expr
%type <Parsers::AST_IntegerExpr*>      integer_expr
%type <Parsers::AST_IntegerExpr*>      integer_term_expr
%type <Parsers::AST_IntegerUnaryExpr*> integer_factor_expr
%type <Parsers::AST_Number*>           number
%type <Parsers::AST_ConcatNumber*>     concat_number

%type <std::vector<Parsers::AST_ConcatNumber*>> concat_number_list

%type <std::string> enum_symbol
%type <std::string> enum_name



%type <Parsers::AST_IntegerExpr*>                                        index
%type <std::tuple<Parsers::AST_IntegerExpr*, Parsers::AST_IntegerExpr*>> range          // Left & Right indexes
%type <std::tuple<Parsers::AST_IntegerExpr*, Parsers::AST_IntegerExpr*>> index_or_range // Left & Right indexes (Right can be empty)

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

%type <Parsers::AST_VectorIdentifier*> alias_name                       // Name, Left & Right indexes (Right can be empty)
%type <Parsers::AST_VectorIdentifier*> oneHotScanGroup_name             // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> inputPort_name                   // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> port_name                        // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> reg_port_signal_id               // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> register_name                    // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scalar_or_vector_id              // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanInPort_name                  // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanMux_name                     // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanOutPort_name                 // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> scanRegister_name                // Name, Left & Right indexes (Left & Right can be empty)
%type <Parsers::AST_VectorIdentifier*> vector_id                        // Name, Left & Right indexes (Right can be empty)
%type <Parsers::AST_VectorIdentifier*> accessLink1149_ActiveSignal_name // Name, Left & Right indexes (Left & Right can be empty)

%type <std::vector<Parsers::AST_VectorIdentifier*>> accessLink1149_ActiveSignal_names

%type <Parsers::AST_PlaceHolder<Parsers::AST_ConcatNumber>*> scanRegister_resetValue
%type <Parsers::AST_PlaceHolder<Parsers::AST_ConcatNumber>*> alias_iApplyEndState


%type <Parsers::AST_Module*>             MODULE // Pseudo type for mid-rule support (always set as nullptr)
%type <Parsers::AST_Module*>             module_def
%type <Parsers::AST_Module*>             module_body
%type <Parsers::AST_Instance*>           instance_def
%type <Parsers::AST_Attribute*>          attribute_def
%type <Parsers::AST_BsdlInstructionRef*> bsdl_instr_ref
%type <Parsers::AST_Parameter*>          localParameter_def
%type <Parsers::AST_Parameter*>          parameter_def
%type <Parsers::AST_ScanRegister*>       scanRegister_def
%type <Parsers::AST_ScanMux*>            scanMux_def
%type <Parsers::AST_ScanInterface*>      scanInterface_def
%type <Parsers::AST_AccessLink*>         accessLink_def
%type <Parsers::AST_AccessLink*>         accessLink1149_def
%type <Parsers::AST_AccessLink*>         accessLinkGeneric_def
%type <Parsers::AST_EnumRef*>            enumRef
%type <Parsers::AST_Node*>               alias_def
%type <Parsers::AST_Node*>               clockMux_def
%type <Parsers::AST_Node*>               dataMux_def
%type <Parsers::AST_Node*>               dataRegister_def
%type <Parsers::AST_Node*>               enum_def
%type <Parsers::AST_Node*>               logicSignal_def
%type <Parsers::AST_Node*>               module_item
%type <Parsers::AST_Node*>               nameSpace_def
%type <Parsers::AST_Node*>               oneHotDataGroup_def
%type <Parsers::AST_Node*>               oneHotScanGroup_def
%type <Parsers::AST_Node*>               useNameSpace_def
%type <Parsers::AST_Node*>               dataOutPort_enable

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
%type <Parsers::AST_Source*>            scanRegister_scanInSource
%type <Parsers::AST_Source*>            port_source
%type <Parsers::AST_Source*>            scanOutPort_source


%type <Parsers::AST_Node*>              scanInterface_item
%type <Parsers::AST_Node*>              dataInPort_item
%type <Parsers::AST_Node*>              dataOutPort_item
%type <Parsers::AST_Node*>              scanOutPort_item
%type <Parsers::AST_Node*>              scanRegister_item
%type <Parsers::AST_Node*>              bsdl_instr_selected_item
%type <Parsers::AST_Node*>              alias_item

%type <std::vector<Parsers::AST_Node*>> module_items
%type <std::vector<Parsers::AST_Node*>> dataInPort_items
%type <std::vector<Parsers::AST_Node*>> dataOutPort_items
%type <std::vector<Parsers::AST_Node*>> scanInterface_items
%type <std::vector<Parsers::AST_Node*>> scanRegister_tail
%type <std::vector<Parsers::AST_Node*>> scanRegister_items
%type <std::vector<Parsers::AST_Node*>> scanInPort_items
%type <std::vector<Parsers::AST_Node*>> scanOutPort_items
%type <std::vector<Parsers::AST_Node*>> bsdl_instr_selected_items
%type <std::vector<Parsers::AST_Node*>> alias_items
%type <std::vector<Parsers::AST_Node*>> alias_tail




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

pos_int :
  POS_INT
  {
    // pos_int : POS_INT
    auto& posInt = $[POS_INT];
    auto integerLiteral = ast.Create_IntegerLiteral(std::move(posInt));
    $$ = integerLiteral;
  }
| ONE
  {
    // pos_int : ONE
    auto integerLiteral = ast.Create_IntegerLiteral(1u);
    $$ = integerLiteral;
  }
| ZERO
  {
    // pos_int : ZERO
    auto integerLiteral = ast.Create_IntegerLiteral(0u);
    $$ = integerLiteral;
  }
;

unsized_dec_num:
  UNSIZED_DEC_NUM
  {
    // unsized_dec_num : UNSIZED_DEC_NUM
    auto digits = $[UNSIZED_DEC_NUM];
    auto node   = ast.Create_BasedNumber(Parsers::Kind::Number_Decimal, std::move(digits));

    $$ = node;
  }
;

unsized_bin_num:
  UNSIZED_BIN_NUM
  {
    // unsized_bin_num : UNSIZED_BIN_NUM
    auto digits = $[UNSIZED_BIN_NUM];
    auto node   = ast.Create_BasedNumber(Parsers::Kind::Number_Binary, std::move(digits));

    $$ = node;
  }
;

unsized_hex_num:
  UNSIZED_HEX_NUM
  {
    // unsized_hex_num : UNSIZED_HEX_NUM
    auto digits = $[UNSIZED_HEX_NUM];
    auto node   = ast.Create_BasedNumber(Parsers::Kind::Number_Hexa, std::move(digits));

    $$ = node;
  }
;

sized_dec_num:
  integer_expr unsized_dec_num
  {
    // sized_dec_num : integer_expr unsized_dec_num
    auto sizeExpr    = $[integer_expr];
    auto basedNumber = $[unsized_dec_num];
    basedNumber->SizeExpr(sizeExpr);

    $$ = basedNumber;
  }
;

sized_bin_num:
  integer_expr unsized_bin_num
  {
    // sized_bin_num : integer_expr unsized_bin_num
    auto sizeExpr    = $[integer_expr];
    auto basedNumber = $[unsized_bin_num];
    basedNumber->SizeExpr(sizeExpr);

    $$ = basedNumber;
  }
;

sized_hex_num:
  integer_expr unsized_hex_num
  {
    // sized_hex_num : integer_expr unsized_hex_num
    //
    auto sizeExpr    = $[integer_expr];
    auto basedNumber = $[unsized_hex_num];
    basedNumber->SizeExpr(sizeExpr);

    $$ = basedNumber;
  }
;


number :
  unsized_dec_num { $$ = $1; /* number : unsized_dec_num */ }
| unsized_bin_num { $$ = $1; /* number : unsized_bin_num */ }
| unsized_hex_num { $$ = $1; /* number : unsized_hex_num */ }
| sized_dec_num   { $$ = $1; /* number : sized_dec_num */   }
| sized_bin_num   { $$ = $1; /* number : sized_bin_num */   }
| sized_hex_num   { $$ = $1; /* number : sized_hex_num */   }
| integer_expr    { $$ = $1; /* number : integer_expr */    }
;

// 6.3.12
vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
{
  // vector_id : SCALAR_ID LEFT_BRACKET index_or_range RIGHT_BRACKET
  auto& baseName   = $[SCALAR_ID];
  auto  left       = std::get<0>($[index_or_range]);
  auto  right      = std::get<1>($[index_or_range]);
  auto  identifier = ast.Create_VectorIdentifier(std::move(baseName), left, right);

  $$ = identifier;
}
;

index_or_range :
  index { $$ = make_tuple($[index], nullptr); /* index_or_range : index */ }
| range { $$ = $1;                            /* index_or_range : range */ }
;

index : integer_expr
{
  // index : integer_expr
  $$ = $1;
};

range : index[LEFT] COLON index[RIGHT]
{
  // range : index[LEFT] COLON index[RIGHT]
  auto left  = $[LEFT];
  auto right = $[RIGHT];

  $$ = make_tuple(left, right);
}
;


integer_expr :
  integer_term_expr
  {
    // integer_expr : integer_term_expr
    $$ = $1;
  }
| integer_term_expr add_operator integer_expr[rhs]
  {
    // integer_expr : integer_term_expr add_operator integer_expr[rhs]
    auto lhsExpr    = $[integer_term_expr];
    auto op         = $[add_operator];
    auto rhsExpr    = $[rhs];
    auto binaryExpr = ast.Create_IntegerBinaryExpr(op, lhsExpr, rhsExpr);

    $$ = binaryExpr;
  }
;

integer_term_expr :
  integer_arg_expr
  {
    // integer_term_expr : integer_arg_expr
    $$ = $1;
  }
| integer_arg_expr mult_operator integer_term_expr[rhs]
  {
    // integer_term_expr : integer_arg_expr mult_operator integer_term_expr[rhs]
    auto lhsExpr    = $[integer_arg_expr];
    auto op         = $[mult_operator];
    auto rhsExpr    = $[rhs];
    auto binaryExpr = ast.Create_IntegerBinaryExpr(op, lhsExpr, rhsExpr);

    $$ = binaryExpr;
  }
;

add_operator :
  PLUS  { $$ = Parsers::Kind::Operator_Add;        /* add_operator: PLUS */}
| MINUS { $$ = Parsers::Kind::Operator_Substract;  /* add_operator: MINUS*/}
;

mult_operator :
  STAR    { $$ = Parsers::Kind::Operator_Multiply;  /* mult_operator : STAR */}
| SLASH   { $$ = Parsers::Kind::Operator_Divide;    /* mult_operator : SLASH*/}
| PERCENT { $$ = Parsers::Kind::Operator_Modulo;    /* mult_operator : PERCENT*/}
;

integer_factor_expr : LEFT_PAREN integer_expr RIGHT_PAREN
{
  // integer_factor_expr : LEFT_PAREN integer_expr RIGHT_PAREN
  auto unaryExpr = ast.Create_IntegerUnaryExpr(Parsers::Kind::ParenthesizedExpr, $[integer_expr]);

  $$ = unaryExpr;
}
;

integer_arg_expr :
  integer_factor_expr { $$ = $1;  /* integer_arg_expr : integer_factor_expr */ }
| pos_int             { $$ = $1;  /* integer_arg_expr : pos_int*/ }
| parameter_ref
  {
    // integer_arg_expr : parameter_ref
    auto parameterRef   = $[parameter_ref];
    auto integerExprRef = ast.Create_IntegerExprRef(parameterRef);

    $$ = integerExprRef;
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
    auto numberNode   = $[number];
    auto concatNumber = ast.Create_ConcatNumber();

    concatNumber->Append(numberNode);

    $$ = concatNumber;
  }
| TILDE number
  {
    // concat_number : TILDE number
    auto numberNode   = $[number];
    auto concatNumber = ast.Create_ConcatNumber();

    numberNode->IsInverted(true);
    concatNumber->Append(numberNode);

    $$ = concatNumber;
  }
| concat_number[lhs_concat_number] COMMA number
  {
    // concat_number : concat_number COMMA number
    auto numberNode   = $[number];
    auto concatNumber = $[lhs_concat_number];

    concatNumber->Append(numberNode);

    $$ = concatNumber;
  }
| concat_number[lhs_concat_number] COMMA TILDE number
  {
    // concat_number : concat_number COMMA TILDE number
    auto numberNode   = $[number];
    auto concatNumber = $[lhs_concat_number];

    numberNode->IsInverted(true);
    concatNumber->Append(numberNode);

    $$ = concatNumber;
  }
;

concat_number_list :
  concat_number
  {
    // concat_number
    vector<Parsers::AST_ConcatNumber*> concatList;

    auto concatNumber = $[concat_number];

    if (concatNumber != nullptr)
    {
      concatList.emplace_back(concatNumber);
    }

    $$ = std::move(concatList);
  }
| concat_number_list[lhs] PIPE concat_number
  {
    // concat_number_list PIPE concat_number
    auto& concatList   = $[lhs];
    auto& concatNumber = $[concat_number];

    if (concatNumber != nullptr)
    {
      concatList.emplace_back(concatNumber);
    }

    $$ = std::move(concatList);
  }
;

scalar_or_vector_id:
  SCALAR_ID
  {
    // scalar_or_vector_id : SCALAR_ID
    auto& name       = $[SCALAR_ID];
    auto  identifier = ast.Create_VectorIdentifier(std::move(name), nullptr, nullptr);

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

instance_name : SCALAR_ID
{
  // instance_name : SCALAR_ID
  auto& name = $[SCALAR_ID];

  $$ = ast.Create_ScalarIdentifier(std::move(name));
};

module_name    : SCALAR_ID
{
  // module_name : SCALAR_ID
  auto& name = $[SCALAR_ID];

  $$ = ast.Create_ScalarIdentifier(std::move(name));
};
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

enumRef : REFENUM enum_name SEMICOLON
{
  // enumRef : REFENUM enum_name SEMICOLON
  auto& enumName    = $[enum_name];
  auto  enumRefNode = ast.Create_EnumRef(std::move(enumName));

  $$ = enumRefNode;
}
;


// 6.4.6.6
dataInPort_def :
  DATAINPORT port_name  SEMICOLON
  {
    // dataInPort_def : DATAINPORT port_name SEMICOLON
    auto& name     = $[port_name];
    auto  node     = ast.Create_Port(Parsers::Kind::DataInPort, name);

    $$ = node;
  }
| DATAINPORT port_name  LEFT_BRACE RIGHT_BRACE
  {
    // dataInPort_def : DATAINPORT port_name  LEFT_BRACE RIGHT_BRACE
    auto& name     = $[port_name];
    auto  node     = ast.Create_Port(Parsers::Kind::DataInPort, name);

    $$ = node;
  }
| DATAINPORT port_name LEFT_BRACE dataInPort_items RIGHT_BRACE
  {
    // dataInPort_def : DATAINPORT port_name  LEFT_BRACE dataInPort_items RIGHT_BRACE
    auto& name     = $[port_name];
    auto& children = $[dataInPort_items];
    auto  node     = ast.Create_Port(Parsers::Kind::DataInPort, name, std::move(children));

    $$ = node;
  }
;

dataInPort_items:
  dataInPort_items[lhs] dataInPort_item
  {
    // dataInPort_items: dataInPort_items[lhs] dataInPort_item
    auto& children = $[lhs];
    auto  item     = $[dataInPort_item];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| dataInPort_item
  {
    // dataInPort_items: dataInPort_item
    std::vector<Parsers::AST_Node*> children;

    auto item = $[dataInPort_item];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;

dataInPort_item :
  attribute_def
  {
    // dataInPort_item : attribute_def
    $$ = $[attribute_def];
  }
| enumRef
  {
    // dataInPort_item : enumRef
    $$ = $[enumRef];
  }
;

// 6.4.6.7
dataOutPort_def :
  DATAOUTPORT port_name  SEMICOLON
  {
    // dataOutPort_def : DATAOUTPORT port_name SEMICOLON
    auto& name     = $[port_name];
    auto  node     = ast.Create_Port(Parsers::Kind::DataOutPort, name);

    $$ = node;
  }
| DATAOUTPORT port_name  LEFT_BRACE RIGHT_BRACE
  {
    // dataOutPort_def : DATAOUTPORT port_name  LEFT_BRACE RIGHT_BRACE
    auto& name     = $[port_name];
    auto  node     = ast.Create_Port(Parsers::Kind::DataOutPort, name);

    $$ = node;
  }
| DATAOUTPORT port_name LEFT_BRACE dataOutPort_items RIGHT_BRACE
  {
    // dataOutPort_def : DATAOUTPORT port_name  LEFT_BRACE dataOutPort_items RIGHT_BRACE
    auto& name     = $[port_name];
    auto& children = $[dataOutPort_items];
    auto  node     = ast.Create_Port(Parsers::Kind::DataOutPort, name, std::move(children));

    $$ = node;
  }
;

dataOutPort_items:
  dataOutPort_items[lhs] dataOutPort_item
  {
    // dataOutPort_items: dataOutPort_items[lhs] dataOutPort_item
    auto& children = $[lhs];
    auto  item     = $[dataOutPort_item];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| dataOutPort_item
  {
    // dataOutPort_items: dataOutPort_item
    std::vector<Parsers::AST_Node*> children;

    auto item = $[dataOutPort_item];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;

dataOutPort_item :
  attribute_def
  {
    // dataOutPort_item : attribute_def
    $$ = $[attribute_def];
  }
| port_source
  {
    // dataOutPort_item : port_source
    $$ = $[port_source];
  }
| dataOutPort_enable
  {
    // dataOutPort_item : dataOutPort_enable
    $$ = $[dataOutPort_enable];
  }
| enumRef
  {
    // dataOutPort_item : enumRef
    $$ = $[enumRef];
  }
;

dataOutPort_enable : ENABLE data_signal SEMICOLON
{
  // dataOutPort_enable : ENABLE data_signal SEMICOLON
  $$ = nullptr;
}
;

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
| enumRef                         { $$ = $1; /* scanRegister_item : enumRef */ }
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


scanRegister_defaultLoadValue :
  DEFAULTLOADVALUE concat_number SEMICOLON
  {
    // scanRegister_defaultLoadValue : DEFAULTLOADVALUE concat_number SEMICOLON
    auto concatNumber = $[concat_number];
    auto node         = ast.Create_PlaceHolder(Parsers::Kind::DefaultLoadValue, concatNumber);

    $$ = node;
  }
| DEFAULTLOADVALUE enum_symbol SEMICOLON
  {
    // scanRegister_defaultLoadValue : DEFAULTLOADVALUE enum_symbol SEMICOLON
    CHECK_FAILED("Enum symbol is not yet supported for ScanRegister default load value");
  }
;

scanRegister_resetValue :
  RESETVALUE concat_number SEMICOLON
  {
    // scanRegister_resetValue : RESETVALUE concat_number SEMICOLON
    auto& concatNumber = $[concat_number];
    auto  node         = ast.Create_PlaceHolder(Parsers::Kind::ResetValue, concatNumber);

    $$ = node;
  }
| RESETVALUE enum_symbol SEMICOLON
  {
    // scanRegister_resetValue : RESETVALUE enum_symbol SEMICOLON
    CHECK_FAILED("Enum symbol is not yet supported for ScanRegister reset value");
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
dataRegister_common :
  dataRegister_resetValue
| dataRegister_defaultLoadValue
| enumRef
| attribute_def
;

dataRegister_resetValue :
  RESETVALUE concat_number SEMICOLON
| RESETVALUE enum_symbol SEMICOLON
;
dataRegister_defaultLoadValue :
  DEFAULTLOADVALUE concat_number SEMICOLON
| DEFAULTLOADVALUE enum_symbol SEMICOLON
;
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

iProc_namespace :
  namespace_name DOUBLE_COLON ref_module_name DOUBLE_COLON sub_namespace
| namespace_name DOUBLE_COLON ref_module_name
| ref_module_name ;

iProc_name : SCALAR_ID | parameter_ref ;

iProc_args :
  D_SUBST
| R_SUBST
| number
| STRING ;

sub_namespace :
  SCALAR_ID
| parameter_ref ;

ref_module_name :
  SCALAR_ID
| parameter_ref ;

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
  auto  nameNode      = ast.Create_ScalarIdentifier(std::move(name));
  auto  genericIdNode = ast.Create_ScalarIdentifier(std::move(genericId));
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
  auto& name          = $[accessLink_name];
  auto& bsdlFileName  = $[bsdlEntity_name];
  auto  nameId        = ast.Create_ScalarIdentifier(std::move(name));
  auto  bsdlFile      = ast.Create_FileRef(Parsers::Kind::BSDLEntity, std::move(bsdlFileName));
  auto  type          = $[accessLink1149_stds];
  auto& bsdlInstrRefs = $[bsdl_instr_refs];

  auto  node          = ast.Create_AccessLink(nameId, type, bsdlFile, std::move(bsdlInstrRefs));

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

bsdlEntity_name : SCALAR_ID
{
  // bsdlEntity_name: SCALAR_ID
  auto& name = $[SCALAR_ID];
  $$ = std::move(name);
};

bsdl_instr_refs :
  bsdl_instr_ref
  {
    // bsdl_instr_refs : bsdl_instr_ref
    std::vector<Parsers::AST_BsdlInstructionRef*> bsdlInstrRefs;

    auto bsdlInstrRef = $[bsdl_instr_ref];

    if (bsdlInstrRef != nullptr)
    {
      bsdlInstrRefs.push_back(bsdlInstrRef);
    }

    $$ = std::move(bsdlInstrRefs);
  }
| bsdl_instr_refs[lhs] bsdl_instr_ref
  {
    // bsdl_instr_refs : bsdl_instr_refs[lhs] bsdl_instr_ref
    auto& bsdlInstrRefs = $[lhs];
    auto  bsdlInstrRef  = $[bsdl_instr_ref];

    if (bsdlInstrRef != nullptr)
    {
      bsdlInstrRefs.push_back(bsdlInstrRef);
    }

    $$ = std::move(bsdlInstrRefs);
  }
;

bsdl_instr_ref : bsdl_instr_name LEFT_BRACE bsdl_instr_selected_items RIGHT_BRACE
{
  // bsdl_instr_ref : bsdl_instr_name LEFT_BRACE bsdl_instr_selected_items RIGHT_BRACE
  auto& instructionName = $[bsdl_instr_name];
  auto& selectedItems   = $[bsdl_instr_selected_items];

  auto node = ast.Create_BsdlInstructionRef(std::move(instructionName), std::move(selectedItems));

  $$ = node;
}
;

bsdl_instr_name : SCALAR_ID
{
  // bsdl_instr_name: SCALAR_ID
  auto& name = $[SCALAR_ID];
  $$ = std::move(name);
}
;

bsdl_instr_selected_items :
  bsdl_instr_selected_item
  {
    // bsdl_instr_selected_items : bsdl_instr_selected_item
    std::vector<Parsers::AST_Node*> children;

    auto item = $[bsdl_instr_selected_item];
    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
| bsdl_instr_selected_items[lhs] bsdl_instr_selected_item
  {
    // bsdl_instr_selected_items : bsdl_instr_selected_items bsdl_instr_selected_item
    auto& children = $[lhs];
    auto  item     = $[bsdl_instr_selected_item];

    if (item != nullptr)
    {
      children.push_back(item);
    }
    $$ = std::move(children);
  }
;

bsdl_instr_selected_item :
  SCANINTERFACE LEFT_BRACE accessLink1149_ScanInterface_names RIGHT_BRACE
  {
    // bsdl_instr_selected_item : SCANINTERFACE LEFT_BRACE accessLink1149_ScanInterface_names RIGHT_BRACE
    auto& scanInterfaceNames = $[accessLink1149_ScanInterface_names];
    auto  node               = ast.Create_ScanInterfaceRef(std::move(scanInterfaceNames));

    $$ = node;
  }
| ACTIVESIGNALS LEFT_BRACE accessLink1149_ActiveSignal_names RIGHT_BRACE
  {
    // bsdl_instr_selected_item : ACTIVESIGNALS LEFT_BRACE accessLink1149_ActiveSignal_names RIGHT_BRACE
    LOG(WARNING) << "Active signal for AccessLink BSDL instruction is not yet supported";
    $$ = nullptr;
  }
;

accessLink1149_ScanInterface_names :
  accessLink1149_ScanInterface_name SEMICOLON
  {
    // accessLink1149_ScanInterface_names : accessLink1149_ScanInterface_name SEMICOLON
    std::vector<std::tuple<Parsers::AST_ScalarIdentifier*, std::string>> scanInterfaceNames;

    auto& scanInterfaceName = $[accessLink1149_ScanInterface_name];

    scanInterfaceNames.emplace_back(std::move(scanInterfaceName));

    $$ = std::move(scanInterfaceNames);
  }
| accessLink1149_ScanInterface_names[lhs] accessLink1149_ScanInterface_name SEMICOLON
  {
    // accessLink1149_ScanInterface_names : accessLink1149_ScanInterface_names[lhs]  accessLink1149_ScanInterface_name SEMICOLON
    auto& scanInterfaceNames = $[lhs];
    auto& scanInterfaceName  = $[accessLink1149_ScanInterface_name];

    scanInterfaceNames.emplace_back(std::move(scanInterfaceName));

    $$ = std::move(scanInterfaceNames);
  }
;

accessLink1149_ScanInterface_name :
  instance_name
  {
    // accessLink1149_ScanInterface_name : instance_name
    auto instanceName      = $[instance_name];
    auto scanInterfaceName = ""s;

    $$ = make_tuple(instanceName, scanInterfaceName);
  }
| instance_name DOT scanInterface_name
  {
    // accessLink1149_ScanInterface_name : instance_name DOT scanInterface_name
    auto instanceName      = $[instance_name];
    auto scanInterfaceName = $[scanInterface_name];

    $$ = make_tuple(instanceName, scanInterfaceName);
  }
;

accessLink1149_ActiveSignal_names :
  accessLink1149_ActiveSignal_name SEMICOLON
  {
    // accessLink1149_ActiveSignal_names : accessLink1149_ActiveSignal_name SEMICOLON
    std::vector<Parsers::AST_VectorIdentifier*> signalNames;

    auto signalName = $[accessLink1149_ActiveSignal_name];

    if (signalName != nullptr)
    {
      signalNames.push_back(signalName);
    }
    $$ = std::move(signalNames);
  }
| accessLink1149_ActiveSignal_names[lhs] accessLink1149_ActiveSignal_name SEMICOLON
  {
    // accessLink1149_ActiveSignal_names : accessLink1149_ActiveSignal_names[lhs] accessLink1149_ActiveSignal_name SEMICOLON
    auto& signalNames = $[lhs];
    auto  signalName  = $[accessLink1149_ActiveSignal_name];

    if (signalName != nullptr)
    {
      signalNames.push_back(signalName);
    }
    $$ = std::move(signalNames);
  }
;

accessLink1149_ActiveSignal_name : reg_port_signal_id
{
  // accessLink1149_ActiveSignal_name : reg_port_signal_id
  auto signalName = $[reg_port_signal_id];
  $$ = signalName;
};


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
alias_def : ALIAS alias_name EQUAL concat_signal_or_inverted_signal alias_tail
{
  // alias_def : ALIAS alias_name EQUAL concat_signal_or_inverted_signal alias_tail
  auto  aliasName        = $[alias_name];
  auto& signals          = $[concat_signal_or_inverted_signal];
  auto& optionalChildren = $[alias_tail];

  auto alias = ast.Create_Alias(aliasName, std::move(signals), std::move(optionalChildren));

  $$ = alias;
}
;

alias_name : scalar_or_vector_id
{
  // alias_name : scalar_or_vector_id
  $$ = $1;
}
;

alias_tail:
  SEMICOLON
  {
    // alias_tail : SEMICOLON
    $$ = vector<Parsers::AST_Node*>();
  }
| LEFT_BRACE RIGHT_BRACE
  {
    // alias_tail : LEFT_BRACE RIGHT_BRACE
    $$ = vector<Parsers::AST_Node*>();
  }
| LEFT_BRACE alias_items RIGHT_BRACE
  {
    // alias_tail : LEFT_BRACE alias_items RIGHT_BRACE
    auto& aliasItems = $[alias_items];

    $$ = std::move(aliasItems);
  }
;

alias_items:
  alias_items[lhs] alias_item
  {
    // alias_items: alias_items alias_item
    auto& aliasItems = $[lhs];
    auto  aliasItem  = $[alias_item];

    if (aliasItem != nullptr)
    {
      aliasItems.push_back(aliasItem);
    }

    $$ = std::move(aliasItems);
  }
| alias_item
  {
    // alias_items : alias_item
    vector<Parsers::AST_Node*> aliasItems;
    auto aliasItem = $[alias_item];

    if (aliasItem != nullptr)
    {
      aliasItems.push_back(aliasItem);
    }

    $$ = std::move(aliasItems);
  }
;

alias_item :
  attribute_def
  {
    // alias_item : attribute_def
    $$ = $1;
  }
| ACCESSTOGETHER SEMICOLON
  {
    // alias_item : ACCESSTOGETHER SEMICOLON
    auto node        = ast.Create_Value(true);
    auto placeHolder = ast.Create_PlaceHolder(Parsers::Kind::AccessTogether, node);

    LOG(ERROR_LVL) << "Alias AccessTogether is not yet supported";
    $$ = placeHolder;
  }
| alias_iApplyEndState
  {
    // alias_item : alias_iApplyEndState
    $$ = $1;
  }
| enumRef
  {
    // alias_item : enumRef
    $$ = $1;
  }
;


alias_iApplyEndState : IAPPLYENDSTATE concat_number SEMICOLON
{
  // alias_iApplyEndState : IAPPLYENDSTATE concat_number SEMICOLON
  auto concatNumber = $[concat_number];
  auto node         = ast.Create_PlaceHolder(Parsers::Kind::ApplyEndState, concatNumber);

  LOG(ERROR_LVL) << "Alias iApplyEndState is not yet supported";
  $$ = node;
}
;

// 6.5.3
enum_def : ENUM enum_name LEFT_BRACE enum_items RIGHT_BRACE
{
  // enum_def : ENUM enum_name LEFT_BRACE enum_items RIGHT_BRACE
  LOG(WARNING) << "Enum definition is not yet supported ==> \"" << $[enum_name] << "\" will be ignored";
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
enum_item : enum_symbol EQUAL concat_number SEMICOLON ;
enum_symbol : SCALAR_ID
{
  // enum_symbol : SCALAR_ID
  auto& enumSymbol = $[SCALAR_ID];
  $$ = std::move(enumSymbol);
}
;

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

    auto paramRef = $[parameter_ref];

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
    auto& name        = $[attribute_name];
    auto  concatNumber= $[concat_number];
    auto  node        = ast.Create_Attribute(std::move(name), concatNumber);

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
    throw Parsers::ParserException("ICL", "", loc.begin.line, loc.begin.column, loc.end.column, errorMessage);
  }

  throw Parsers::ParserException("ICL", "", my_location->begin.line, my_location->begin.column, my_location->end.column, errorMessage);
}
