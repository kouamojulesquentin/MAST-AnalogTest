%skeleton "lalr1.cc"
%require  "3.0"
%debug
%locations
%defines
%define api.namespace {SIT}
%define parser_class_name {SIT_Parser}

%code requires{
   namespace SIT {
      class SIT_Reader;
      class SIT_Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "SIT_types.h"
#include "BinaryVector.hpp"
#include "SystemModelBuilder.hpp"
#include "SystemModelNode.hpp"
#include "PathSelector.hpp"
}

%parse-param { SIT_Scanner  &scanner  }
%parse-param { SIT_Reader  &driver  }

%code{
/* include for all driver functions */
#include "SIT_reader.hpp"
#include "SIT_scanner.hpp"
#include "SIT_types.h"

// ---------------- What is needed for parser to build the SystemModel
//
#include "DefaultNHotPathSelector.hpp"
#include "SystemModelBuilder.hpp"
#include "AccessInterfaceProtocolFactory.hpp"
#include "AppFunctionNameAndNode.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"
#include "UnresolvedPathSelector.hpp"
#include "g3log/g3log.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <experimental/string_view>
#include <map>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::pair;
using std::string;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

#define STREAM_MY_LOCATION  "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": "

#undef yylex
#define yylex scanner.yylex

#define THROW_PARSER_ERROR(msg) THROW_IMPL(ParserException, msg)

extern int nlines;
extern SIT::SIT_Parser::location_type *my_location;


const vector<string> Path_Selector_table  = {"Binary", "One_Hot", "N_Hot", "Binary_noidle", "One_Hot_noidle", "N_Hot_noidle"};
enum class           PathSelectorKind       { Binary,   One_Hot,   N_Hot,   Binary_noidle,   One_Hot_noidle,   N_Hot_noidle};

const vector<PathSelectorKind> PathSelectorKinds {PathSelectorKind::Binary,
                                                  PathSelectorKind::One_Hot,
                                                  PathSelectorKind::N_Hot,
                                                  PathSelectorKind::Binary_noidle,
                                                  PathSelectorKind::One_Hot_noidle,
                                                  PathSelectorKind::N_Hot_noidle};

const std::vector<SelectorProperty> Path_Selector_prop_t {SelectorProperty::Binary_Default,
                                                          SelectorProperty::One_Hot_Default,
                                                          SelectorProperty::N_Hot_Default,
                                                          SelectorProperty::CannotSelectNone,
                                                          SelectorProperty::CannotSelectNone,
                                                          SelectorProperty::CannotSelectNone};
inline std::string remove_quotes(std::string s)
{
 // Remove all double-quote characters
  s.erase(
    remove( s.begin(), s.end(), '\"' ),
    s.end()
    );
 return s;
}

namespace
{
int index_in_table(const std::vector<std::string>& table, const std::string& s)
{
  auto pos = std::find(table.cbegin(), table.cend(), s);
  if (pos == table.cend())
  {
    return -1;
  }

  auto index = pos - table.cbegin();
  return static_cast<int>(index);
}


pair<shared_ptr<Register>, shared_ptr<PathSelector>>  make_PathSelector(shared_ptr<SystemModelBuilder> builder,
                                                                        PathSelectorKind               selectorKind,
                                                                        SelectorProperty               selectorProperty,
                                                                        const string&                  selectorRegName,
                                                                        uint32_t                       maxDerivations)
{
  switch (selectorKind)
  {
    case PathSelectorKind::Binary :
    case PathSelectorKind::Binary_noidle :
      return builder->Create_PathSelector(SelectorKind::Binary, selectorRegName, maxDerivations, selectorProperty);
    case PathSelectorKind::One_Hot :
    case PathSelectorKind::One_Hot_noidle :
      return builder->Create_PathSelector(SelectorKind::One_Hot, selectorRegName, maxDerivations, selectorProperty);
    case PathSelectorKind::N_Hot :
    case PathSelectorKind::N_Hot_noidle :
      return builder->Create_PathSelector(SelectorKind::N_Hot, selectorRegName, maxDerivations, selectorProperty);
    default:
      THROW_RUNTIME_ERROR("Unexpected selector type ");
  }
}

shared_ptr<PathSelector> make_PathSelector (shared_ptr<SystemModelBuilder> builder,
                                            PathSelectorKind               selectorKind,
                                            SelectorProperty               selectorProperty,
                                            shared_ptr<Register>           selectorReg,
                                            uint32_t                       maxDerivations)
{
  switch (selectorKind)
  {
    case PathSelectorKind::Binary :
    case PathSelectorKind::Binary_noidle :
      return builder->Create_PathSelector(SelectorKind::Binary, selectorReg, maxDerivations, selectorProperty);
    case PathSelectorKind::One_Hot :
    case PathSelectorKind::One_Hot_noidle :
      return builder->Create_PathSelector(SelectorKind::One_Hot, selectorReg, maxDerivations, selectorProperty);
      break;
    case PathSelectorKind::N_Hot :
    case PathSelectorKind::N_Hot_noidle :
      return builder->Create_PathSelector(SelectorKind::N_Hot, selectorReg, maxDerivations, selectorProperty);
    default:
      THROW_RUNTIME_ERROR("Unexpected selector type ");
  }
}
} // End of: unnamed namespace


} /*end of %code section*/

%define api.value.type variant
%define parse.assert

%type <node_list_type>        node_list
%type <node_list_type>        children_list
%type <std::uint8_t>          is_transparent
%type <name_type>             node_name
%type <std::uint8_t>          hold
%type <std::string>           bypass
%type <mast::MuxRegPlacement> mux_register_position


%type <mast::SelectorProperty>          active
%type <mast::SelectorProperty>          reverse
%type <std::uint32_t>                   max_derivations
%type <std::string>                     path_selector_kind
%type <std::string>                     selector_register_name
%type <std::uint32_t>                   IR_size
%type <std::uint32_t>                   size
%type <std::uint32_t>                   n_DR_chains
%type <std::vector<mast::BinaryVector>> IR_Coding_list
%type <std::vector<mast::BinaryVector>> IR_TABLE
%type <std::string>                     JTAG_protocol
%type <std::string>                     AI_identifier
%type <std::string>                     AI_protocol_parameters

%type <std::shared_ptr<mast::SystemModelNode>> root_node
%type <std::shared_ptr<mast::SystemModelNode>> register_node
%type <std::shared_ptr<mast::SystemModelNode>> leaf_node
%type <std::shared_ptr<mast::SystemModelNode>> parent_node
%type <std::shared_ptr<mast::SystemModelNode>> node
%type <std::shared_ptr<mast::SystemModelNode>> parent_node_with_children
%type <std::vector<std::string>>               function_list
%type <std::pair<std::vector<std::string>, std::uint32_t>>  PDL_declaration

%token END 0 "end of file"
%token UPPER
%token LOWER
%token CHAR
%token t_SEMICOLON
%token t_START_HIERARCHY
%token t_END_HIERARCHY
%token t_RightBracket
%token t_LeftBracket
%token t_Comma
%token t_RightParenthesis
%token t_LeftParenthesis

%token <std::string>   t_WORD
%token <std::string>   t_CHAIN
%token <std::string>   t_REGISTER
%token <std::string>   t_LINKER
%token <std::string>   t_ACCESS_INTERFACE
%token <std::string>   t_SIB
%token <std::string>   t_MIB
%token <std::string>   t_1500_WRAPPER
%token <std::string>   t_JTAG_TAP
%token <std::string>   t_PDL
%token <std::string>   t_BASED_INTEGER
%token <std::string>   t_TRANSPARENT
%token <std::string>   t_HOLD_VALUE
%token <std::string>   t_BYPASS
%token <std::string>   t_BINARY_VECTOR
%token <std::string>   t_QUOTED_STRING
%token <std::string>   t_POST
%token <std::string>   t_PRE
%token <std::string>   t_HIGH
%token <std::string>   t_LOW
%token <std::string>   t_REVERSE
%token <std::uint32_t> t_DecimalLiteral


%%
root_node:
  node END
  {
    /*resolve evetual Linkers*/
    while (!driver.unresolved_linkers.empty())
    {
      const auto& linkerInfo = driver.unresolved_linkers.front();

      // remove resolved linkerInfo
      const auto& registerIter = driver.declared_registers.find(linkerInfo.selector_name);
      if (registerIter == driver.declared_registers.end())
      {
        LOG(ERROR_LVL) << "Error, Selector register \"" << linkerInfo.selector_name << "\" required by linker \"" << linkerInfo.linker_node->Name() << "\""
                       << " at line " << linkerInfo.line << ":" << linkerInfo.column << " does not exist" ;
        YYERROR;
      }

      auto registerNode = registerIter->second;
      auto properties   = Path_Selector_prop_t[linkerInfo.selector_kind_index];
      auto selectorKind = PathSelectorKinds[linkerInfo.selector_kind_index];
      auto selector     = make_PathSelector (driver.builder,
                                             selectorKind,
                                             properties,
                                             registerNode,
                                             linkerInfo.max_derivations);

      auto linkerNode = linkerInfo.linker_node;
      linkerNode->ReplacePathSelector(selector);
      driver.unresolved_linkers.pop();
    }
    driver.parsed_sut = $[node];
  }
  ;

children_list:
  t_START_HIERARCHY node_list  t_END_HIERARCHY{ $$ = $2;}
  ;

node_list:
    node node_list { $$.name = $1->Name() + ' ' + $2.name; $$.n_nodes = $2.n_nodes+1;
                     auto tmp = $2.nodes;
                     tmp.insert(tmp.begin(),$1);
                     $$.nodes = tmp;
        }
  |   node { $$.name = $1->Name();$$.n_nodes = 1;$$.nodes.push_back($1);}
;

node:
   parent_node_with_children { $$ = $1; }
 | leaf_node                 { $$ = $1; }
;

is_transparent:
   t_TRANSPARENT {$$ = 1;}
 |
  { $$ = 0;}
;

node_name :  t_WORD is_transparent
{
  $$.name           = $[t_WORD];
  $$.is_transparent = $[is_transparent];
}
;

parent_node_with_children: parent_node PDL_declaration children_list
{
  auto asParentNode = dynamic_pointer_cast <ParentNode>($1);

  for (auto this_child : $3.nodes)
  {
    asParentNode->AppendChild(this_child);
  }
  $$ = $1;

  if (!$2.first.empty())
  {
    for (auto this_function : $2.first)
      driver.namesAndNodes.push_back(AppFunctionNameAndNode(this_function, asParentNode, $2.second));
  }
}

PDL_declaration:
 t_PDL function_list
 {
   pair <std::vector<std::string>,std::uint32_t> ret;
   ret.first = $2;
   ret.second =nlines;
   $$ = ret;
 }
 |
 {
   std::pair <std::vector<std::string>,std::uint32_t> ret;
   ret.first = std::vector<std::string>();
   ret.second =nlines;
   $$ = ret;
 }
;

function_list:
 t_WORD
    {
    $$.push_back($1);
    }
 |
 t_WORD t_Comma  function_list
  {
   $$ = $3;
   $$.push_back($1);
   }
;

parent_node:
t_CHAIN  node_name
{
  auto chain = driver.main_sm->CreateChain($[node_name].name);
  chain->IgnoreForNodePath($[node_name].is_transparent);
  $$ = chain;
}
|
t_LINKER  node_name path_selector_kind selector_register_name max_derivations
{
  int selectorKindIndex = index_in_table(Path_Selector_table, $[path_selector_kind]);
  if (selectorKindIndex == -1)
  {
    LOG(ERROR_LVL) << STREAM_MY_LOCATION << "node " << $[node_name].name << " \"" << $[path_selector_kind] << "\": Unsupported Linker Path Selector kind";
    YYERROR;
  }

  if ($[selector_register_name] == "")
  {
    LOG(ERROR_LVL) << STREAM_MY_LOCATION << "Must specify a control node (Register) for linker path selector";
    YYERROR;
  }

  std::ostringstream os;
  os << "Node type LINKER, idf " << $[node_name].name << " ";
  if ($[node_name].is_transparent)
    os << "(transparent) ";
  os << $[path_selector_kind] << "_PathSelector controlled by node " << $[selector_register_name];
  LOG(DEBUG) << os.str();

  auto pathSelector = make_shared<UnresolvedPathSelector>();
  auto linker       = driver.main_sm->CreateLinker ($[node_name].name, pathSelector);

  linker_information linkerInfo;
  linkerInfo.linker_node         = linker;
  linkerInfo.column              = my_location->begin.column;
  linkerInfo.line                = my_location->begin.line;
  linkerInfo.selector_kind_index = selectorKindIndex;
  linkerInfo.selector_name       = $[selector_register_name];
  linkerInfo.max_derivations     = $[max_derivations];
  driver.unresolved_linkers.push(linkerInfo);

  $$ = linker;
}
|
t_SIB node_name mux_register_position active
{
  auto node =  driver.builder->Create_SIB($[node_name].name, $[active], $[mux_register_position]);
  $$ = node;
}
|
t_MIB node_name mux_register_position active reverse max_derivations path_selector_kind
{
  int index = index_in_table(Path_Selector_table, $[path_selector_kind]);
  if (index == -1)
  {
    LOG(ERROR_LVL) << STREAM_MY_LOCATION << "node " << $[node_name].name << " \"" << $[path_selector_kind] << "\" Unknown MIB Path Selector kind";
    YYERROR;
  }

  auto             selectorKind     = PathSelectorKinds[index];
  auto             selectorRegName  = $[node_name].name + MIB_CTRL_EXT;
  SelectorProperty selectorProperty = $[active] | $[reverse] | Path_Selector_prop_t[index];

  auto res = make_PathSelector(driver.builder,
                               selectorKind,
                               selectorProperty,
                               selectorRegName,
                               $[max_derivations]);

  auto selectorReg = res.first;
  auto selector    = res.second;
  auto node        = driver.builder->Create_MIB($[node_name].name, selector, selectorReg, $[mux_register_position]);

  $$ = node;
}
|
t_1500_WRAPPER node_name max_derivations
{
  auto node = driver.builder->Create_1500_Wrapper ($[node_name].name,$3);
  $$ = node;
}
|
t_ACCESS_INTERFACE  node_name AI_identifier AI_protocol_parameters
{
    const auto& nodeName           = $[node_name].name;
    const auto& protocolName       = $3;
    const auto& protocolParameters = $4;

    try
    {
      auto& factory  = AccessInterfaceProtocolFactory::Instance();
      auto  protocol = factory.Create(protocolName, protocolParameters);

      if (!protocol)
      {
        LOG(ERROR_LVL) << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": "
                       << "node " << nodeName << " Cannot create protocol: \"" << protocolName << "\"";
        YYERROR;
      }
      else
      {
        auto node = driver.main_sm->CreateAccessInterface(nodeName, shared_ptr<AccessInterfaceProtocol>(std::move(protocol)));
        $$ = node;
      }
    }
    catch(std::invalid_argument exc)  // Catch C++ standard exceptions
    {
      LOG(ERROR_LVL) << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": "
                     << "node " << nodeName << " Cannot create protocol: \"" << protocolName << "\"; " << exc.what();
      YYERROR;
    }
}
|
// $1      $2        $3            $4                     $5      $6       $7
t_JTAG_TAP node_name JTAG_protocol AI_protocol_parameters IR_size IR_TABLE n_DR_chains
{
  const auto& nodeName           = $2.name;
  const auto& protocolName       = $3;
  const auto& protocolParameters = $4;
  const auto  irSize             = $5;
  const auto& irTable            = $6;
  const auto  nbDRChains         = $7;
  const auto  nbDerivations      = nbDRChains + 1u;

  string      creatorId("JTAG_");
  creatorId.append(protocolName);

  try
  {
    auto& factory  = AccessInterfaceProtocolFactory::Instance();
    auto  protocol = factory.Create(creatorId, protocolParameters);

    if (!protocol)
    {
      LOG(ERROR_LVL) << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": "
                     << "node " << nodeName << " Cannot create protocol: \"" << protocolName << "\"";
      YYERROR;
    }
    else
    {
      if (irTable.empty())
      {
        auto node = driver.builder->Create_JTAG_TAP(nodeName,
                                                    irSize,
                                                    nbDerivations,
                                                    shared_ptr<AccessInterfaceProtocol>(std::move(protocol)));
        $$ = node;
      }
      else
      {
        if (irTable.size() != nbDerivations)
        {
          LOG(ERROR_LVL) << "Error Coding must be provided for bypass register and each chain";
          YYERROR;
        }
        auto node = driver.builder->Create_JTAG_TAP(nodeName,
                                                    irSize,
                                                    nbDerivations,
                                                    shared_ptr<AccessInterfaceProtocol>(std::move(protocol)),
                                                    irTable);
        $$ = node;
      }
    }
  }
  catch(std::invalid_argument exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": "
                   << "node " << nodeName << " Cannot create protocol: \"" << protocolName << "\"; " << exc.what();
    YYERROR;
  }
}
;

JTAG_protocol: t_WORD
   {$$ =$1;}
   ;

path_selector_kind: t_WORD
   { $$ =$1;}
   ;

selector_register_name: t_WORD
   { $$ = $1;}
   |
   { $$ = "";}
   ;

IR_size :
 t_DecimalLiteral { $$ = $1;}
;

n_DR_chains :
 t_DecimalLiteral { $$ = $1;}
;


IR_TABLE:
 t_LeftBracket IR_Coding_list t_RightBracket  {$$=$2;}
 |  {$$ = std::vector<mast::BinaryVector>();}
 ;

AI_identifier:
  t_WORD
  {
     $$ = $1;
  }
;

AI_protocol_parameters:
 t_QUOTED_STRING
 {
   $$ = remove_quotes($1);
 }
|
 {
   $$="";
 }
;

IR_Coding_list:
  t_QUOTED_STRING
  {
     $$.push_back(BinaryVector::CreateFromString(remove_quotes($1)));
  }
| t_QUOTED_STRING t_Comma  IR_Coding_list
  {
    auto tmp = $3;
    tmp.insert(tmp.begin(), BinaryVector::CreateFromString(remove_quotes($1)));
    $$ = tmp;
  }
;

max_derivations :
 t_DecimalLiteral { $$ = $1;}
;
mux_register_position :
 t_POST { $$ = MuxRegPlacement::AfterMux;}
 |
 t_PRE  { $$ = MuxRegPlacement::BeforeMux;}
 ;

active :
 t_HIGH { $$ = SelectorProperty::None;}
 |
 t_LOW{ $$ =  SelectorProperty::InvertedBits;}
 ;

reverse :
 t_REVERSE {$$ = SelectorProperty::ReverseOrder;}
 |
 {  $$ = SelectorProperty::None; }
 ;

leaf_node: register_node
{
  $$=  $1;
}
;

register_node:
   t_REGISTER  node_name size hold bypass
   {
     auto bin_value = BinaryVector::CreateFromString(remove_quotes($[bypass]));
     if (bin_value.BitsCount() != $[size])
     {
       LOG(ERROR_LVL) << STREAM_MY_LOCATION
                      << "Node " << $[node_name].name << " size (" << $[size] << ") "
                      << "does not match Bypass value bit count (" << bin_value.BitsCount() << ")";
       YYERROR;
     }

     auto registerNode = driver.main_sm->CreateRegister ($[node_name].name, bin_value, nullptr);

     //! @todo  save return value and check if binaryVector.size == size is correct and report error if not

     registerNode->SetHoldValue($[hold] == 1);
     driver.declared_registers.insert (make_pair($[node_name].name, registerNode));

     $$ = registerNode;
   }

size:
 t_DecimalLiteral { $$ = $1;}
;

hold:
 t_HOLD_VALUE{  $$=1 ; }
 |{  $$=0 ; }
 ;

bypass:
 t_BYPASS t_SEMICOLON t_QUOTED_STRING
  {
   $$=$3 ;
  }
  ;

%%

void SIT::SIT_Parser::error( const location_type &l, const std::string &err_message )
{
  std::ostringstream os;

  os << "Line " << my_location->begin.line
     << ":"     << my_location->begin.column
     << "-"     << my_location->end.column
     << ": "    << err_message;

  LOG(ERROR_LVL) << os.str();
  driver.parsed_sut = nullptr;

  auto exceptionMessage = "SIT parsing error: "s + os.str();
  THROW_PARSER_ERROR(exceptionMessage);
}
