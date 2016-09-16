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
#include "DefaultNHotPathSelector.hpp"
#include "SystemModelBuilder.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "GenericAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "I2C_Player.hpp"

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::pair;

using namespace mast;

}

%parse-param { SIT_Scanner  &scanner  }
%parse-param { SIT_Reader  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "SIT_reader.hpp"
   #include "SIT_scanner.hpp"
   #include "SIT_types.h"

#undef yylex
#define yylex scanner.yylex

#define N_AIs 6
#define MAX_AI_NAME 30

#define DEFAULT_I2C_PREFIX "S2R"

extern int nlines;
extern SIT::SIT_Parser::location_type *my_location;

std::vector<std::string> AI_protocol_table  =
  {"JTAG_Loopback","JTAG_SVF_simulation","JTAG_SVF_openOCD",
  "I2C_Player"
  };
enum AI_protocol_t {JTAG_Loopback,JTAG_SVF_simulation,JTAG_SVF_openOCD, I2C_Player};

std::vector<std::string> JTAG_AI_protocol_table  =
  {"Loopback","SVF_simulation","SVF_openOCD"
  };
enum JTAG_AI_protocol_t {Loopback,SVF_simulation,SVF_openOCD};

std::vector<std::string> Path_Selector_table  =
  {"Binary","One_Hot","N_Hot","Binary_noidle","One_Hot_noidle","N_Hot_noidle"};
enum Path_Selector_t {Binary,One_Hot,N_Hot,Binary_noidle,One_Hot_noidle,N_Hot_noidle};

std::vector<SelectorProperty> Path_Selector_prop_t {
 SelectorProperty::Binary_Default  ,SelectorProperty::One_Hot_Default,SelectorProperty::N_Hot_Default,
 SelectorProperty::CannotSelectNone,SelectorProperty::CannotSelectNone,SelectorProperty::CannotSelectNone,};

static int find_in_table(std::vector<std::string> table, std::string s)
{
	unsigned int   l;
 	for (l=0;l<table.size();l++)
   	if ((table)[l]==s)
     		{
     		break;
     	    }
 if (l>= table.size())	return -1;
 else return l;
}

inline std::string remove_quotes(std::string s)
{
 // Remove all double-quote characters
  s.erase(
    remove( s.begin(), s.end(), '\"' ),
    s.end()
    );
 return s;
}

inline std::uint32_t extract_number(std::string s)
{
  char *end;
  auto tmp = remove_quotes(s);
  auto res = std::strtoul (tmp.c_str(), &end,0);
  auto result = res;
  return  result;
}

}

%define api.value.type variant
%define parse.assert

%type  <node_list_type> node_list
%type  <node_list_type> children_list
%type  <std::uint8_t> is_transparent
%type  <name_type> node_name
%type  <std::uint8_t> hold
%type  <std::string> bypass
%type  <mast::MuxRegPlacement> position

%type  <mast::SelectorProperty> active
%type  <mast::SelectorProperty> reverse
%type  <std::uint32_t> max_derivations
%type  <std::string> path_selector
%type  <std::string> ctrl_node
%type  <std::uint32_t> IR_size
%type  <std::uint32_t> size
%type  <std::uint32_t> n_DR_chains
%type  <Coding_type> Coding_list
%type  <Coding_type> IR_TABLE
%type  <Coding_type> AI_TABLE
%type  <std::string> JTAG_protocol
%type  <std::string> PDL_declaration
%type  <std::shared_ptr<mast::SystemModelNode>> root_node
%type  <std::shared_ptr<mast::SystemModelNode>> register_node
%type  <std::shared_ptr<mast::SystemModelNode>> leaf_node
%type  <std::shared_ptr<mast::SystemModelNode>> parent_node
%type  <std::shared_ptr<mast::SystemModelNode>> node
%type  <std::shared_ptr<mast::SystemModelNode>> parent_node_with_children

%token               END    0     "end of file"
%token               UPPER
%token               LOWER
%token <std::string> t_WORD
%token               CHAR
%token               t_SEMICOLON

%token  <std::string> t_CHAIN
%token  <std::string> t_REGISTER
%token  <std::string> t_LINKER
%token  <std::string> t_ACCESS_INTERFACE
%token  <std::string> t_SIB
%token  <std::string> t_MIB
%token  <std::string> t_1500_WRAPPER
%token  <std::string> t_JTAG_TAP
%token  <std::string> t_PDL

%token  <std::string> t_BASED_INTEGER
%token		t_START_HIERARCHY
%token		t_END_HIERARCHY
%token 	<std::string>	t_TRANSPARENT
%token 	<std::string>	t_HOLD_VALUE
%token 	<std::string>	t_BYPASS
%token 	<std::string>	t_BINARY_VECTOR
%token 	<std::string>	t_QUOTED_STRING
%token  <std::string> t_POST
%token  <std::string> t_PRE
%token  <std::string> t_HIGH
%token  <std::string> t_LOW
%token  <std::string> t_REVERSE
%token  <std::uint32_t>  t_DecimalLiteral
%token  t_RightBracket
%token  t_LeftBracket
%token  t_Comma

%%

root_node:
   node END
    {
    driver.parsed_sut=$1;
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
   parent_node_with_children { $$ = $1;}
  | leaf_node
  		    { $$ = $1;}
  ;

is_transparent:
 t_TRANSPARENT {$$ = 1;}
 |
  { $$ = 0;}
;

node_name :  t_WORD is_transparent
     			{
  		     $$.name = $1;
		     $$.is_transparent = $2;
		     }
     ;

parent_node_with_children: parent_node PDL_declaration children_list
 {
  auto asParentNode = dynamic_pointer_cast<ParentNode>($1);
   for (auto this_child : $3.nodes)
       asParentNode->AppendChild(this_child);
     $$ = $1;
  if (!$2.empty())
   {
    std::cout<< "Function " << $2 <<" is associated with node " << $1->Name() <<"\n";
   }
 }

PDL_declaration:
 t_PDL t_WORD { $$ = $2;}
 |
  {  $$ = "";}
;

parent_node:

t_CHAIN  node_name  {
		     auto node = driver.main_sm->CreateChain($2.name);
                     if ($2.is_transparent)
		           node->IgnoreForNodePath(true);
 	  	     $$ = node;
  		     }
 |
t_LINKER  node_name path_selector ctrl_node {
			int   l;
			l = find_in_table(Path_Selector_table,$3);
			if (l==-1)
			 {
   			std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
      std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unknown Linker Path Selector \n";
	  		YYERROR;
	  		}
	  	       else
	  	    	{
                     	std::cout << "Node type LINKER, idf " << $2.name ;
                     	if ($2.is_transparent)
		           std::cout << "(transparent) ";
			std::cout <<  $3 <<"_PathSelector";
			 std::cout <<" controlled by node "<<$4;
//		     auto node = driver.main_sm->CreateLinker ($5.name,   pathSelector, nullptr);
 		     auto node = driver.main_sm->CreateChain ($2.name);
 	  	     $$ = node;
  		      }
		}
 |
 t_SIB node_name position active
  {
      {
 	auto node =  driver.builder->Create_SIB($2.name,$4,$3);

 	$$ = node;
       }
  }
 |
 t_MIB node_name position active reverse max_derivations path_selector
  {
      {
      int   l;
	l = find_in_table(Path_Selector_table,$7);
	if (l==-1)
	 {
	 std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
   std::cerr << "node " << $2.name<< " \""<< $7 << "\"" << ": Unknown MIB Path Selector \n";
	 YYERROR;
	 }
        auto selectorRegName = $2.name + MIB_CTRL_EXT;
	SelectorProperty sel_properties = $4 | $5| Path_Selector_prop_t[l];

        pair<shared_ptr<Register>, shared_ptr<PathSelector>> res; /*cannot use auto inside a switch*/
 	switch(l)
	 {
	  case Binary :
	  case Binary_noidle :
	      res =  driver.builder->Create_PathSelector(SelectorKind::Binary, selectorRegName, $6,sel_properties);
	     break;
	  case One_Hot :
	  case One_Hot_noidle :
 	      res  = driver.builder->Create_PathSelector(SelectorKind::One_Hot, selectorRegName, $6,sel_properties);
	      break;
	  case N_Hot :
	  case N_Hot_noidle :
 	      res  = driver.builder->Create_PathSelector(SelectorKind::N_Hot, selectorRegName, $6,sel_properties);
	      break;
	 }
   	auto selectorReg = res.first;
  	auto selector    = res.second;
        auto node = driver.builder->Create_MIB($2.name, selector, selectorReg, $3);

  	$$ = node;
       }
  }
 |
 t_1500_WRAPPER node_name max_derivations
  {
      {
        auto node = driver.builder->Create_1500_Wrapper ($2.name,$3);
  	$$ = node;
       }
  }
 |
t_ACCESS_INTERFACE  node_name t_WORD AI_TABLE  {

	int   l;
	l = find_in_table(AI_protocol_table,$3);
  	if (l==-1)
	  {
	  std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
    std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unknown AccessInterface Protocol \n";
	  YYERROR;
	  }
	  else
	  {

          std::shared_ptr<AccessInterfaceProtocol> protocol;
	  switch(l)
	  {
	  case JTAG_Loopback :
	   protocol = make_shared<LoopbackAccessInterfaceProtocol > ();
	  case JTAG_SVF_simulation :
	   protocol = make_shared<SVF_SimulationProtocol > ();
	  case JTAG_SVF_openOCD :
	   protocol = make_shared<SVF_SimulationProtocol> ();
	  break;
	  case AI_protocol_t::I2C_Player :
	   if ($4.n_words==0)
	    {
	    std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
	    std::cerr << "Error, " << AI_protocol_table[l] <<" needs an address table\n";
	    YYERROR;
	    }
//	    protocol = make_shared<I2C_Player> ($4.codevalue, DEFAULT_I2C_PREFIX);
	   protocol = make_shared<LoopbackAccessInterfaceProtocol > ();

	  break;

	  }

		auto node = driver.main_sm->CreateAccessInterface($2.name, protocol);

  	  	$$ = node;

		}
	  }
 |
  t_JTAG_TAP node_name JTAG_protocol IR_size IR_TABLE n_DR_chains
      {
      {
  	{
	int   l;
	l = find_in_table(JTAG_AI_protocol_table,$3);
  	if (l==-1)
	  {
	  std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
    std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unknown JTAG protocol \n";
	  YYERROR;
	  }
	  else
	{
          std::shared_ptr<AccessInterfaceProtocol> protocol;
	  switch(l)
	  {
	  case Loopback :
	   protocol = make_shared<LoopbackAccessInterfaceProtocol > ();
	  break;
	  case SVF_simulation :
	   protocol = make_shared<SVF_SimulationProtocol > ();
	  break;
	  case SVF_openOCD :
	   protocol = make_shared<SVF_SimulationProtocol> ();
	  break;

	  }
	 auto node = driver.builder->Create_JTAG_TAP($2.name,$4,$6+1,protocol);
  	 $$ = node;
	 }
	}
       }
     }
  ;

JTAG_protocol: t_WORD
   {$$ =$1;}
   ;

path_selector: t_WORD
   { $$ =$1;}
   ;
ctrl_node: t_WORD
   { $$ =$1;}
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
 t_LeftBracket Coding_list t_RightBracket  {$$=$2;}
 |  {$$.n_words = 0;}
 ;

AI_TABLE:
t_LeftBracket Coding_list t_RightBracket  {$$=$2;}
 |  {$$.n_words = 0;}
 ;

Coding_list:
 t_QUOTED_STRING
    {
       $$.codevalue.push_back(extract_number($1));
    $$.n_words = 1;
    }
 |
 t_QUOTED_STRING t_Comma  Coding_list
  {
   $$.n_words = ($3.n_words+1);
   auto tmp = $3.codevalue;
   tmp.insert(tmp.begin(),extract_number($1));
   $$.codevalue = tmp;
   }
;

max_derivations :
 t_DecimalLiteral { $$ = $1;}
;
position :
 t_POST { $$ = MuxRegPlacement::AfterMux;}
 |
 t_PRE{ $$ = MuxRegPlacement::BeforeMux;}
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

leaf_node: register_node {     $$=  $1;
                           }
  ;

register_node:
   t_REGISTER  node_name size hold bypass {
                      auto bin_value = BinaryVector::CreateFromString(remove_quotes($5));
		      if (bin_value.BitsCount() != $3)
		        {
	  		std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
			std::cerr << "Node " << $2.name<< " size "<< $3 << " does not match Bypass value bit count " << bin_value.BitsCount() << "\n";
	  		YYERROR;
			}
		      auto node = driver.main_sm->CreateRegister ($2.name, bin_value , nullptr);
		      /*TODO: save return value and check if binaryVector.size == size is correct
		       and raise error if not*/
		      if ($4==1) node->SetHoldValue(true);
  		     $$ = node;}

size :
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

void
SIT::SIT_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
   std::cerr << err_message << "\n";
   driver.parsed_sut=nullptr;
}
