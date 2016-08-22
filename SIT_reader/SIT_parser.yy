%skeleton "lalr1.cc"
%require  "3.0"
%debug 
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
   #include "SIT_types.h"

#undef yylex
#define yylex scanner.yylex

#define N_AIs 6
#define MAX_AI_NAME 30

extern int nlines;

std::vector<std::string> AI_protocol_table  =
  {"JTAG_SVF_loopback","JTAG_SVF_simulation","JTAG_SVF_openOCD",
  "I2C_loopback","I2C_simulation","I2C_api"
  };

std::vector<std::string> JTAG_AI_target_table  =
  {"SVF_loopback","SVF_simulation","SVF_openOCD"
  };

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
%type  <IR_coding_type> IR_coding_list
%type  <IR_coding_type> IR_TABLE
%type  <std::string> JTAG_target

%type  <std::shared_ptr<mast::SystemModelNode>> root_node
%type  <std::shared_ptr<mast::SystemModelNode>> register_node
%type  <std::shared_ptr<mast::SystemModelNode>> leaf_node
%type  <std::shared_ptr<mast::SystemModelNode>> internal_node
%type  <std::shared_ptr<mast::SystemModelNode>> node

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
%locations

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
   internal_node { $$ = $1;}
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

internal_node: 

t_CHAIN  node_name children_list { 
                     if ($2.is_transparent) 
		           std::cout << "(transparent)";
		     
		     auto node = driver.main_sm->CreateChain($2.name);
		     for (auto this_child : $3.nodes)
		       node->AppendChild(this_child);
 	  	     $$ = node;
  		     }
 |
t_LINKER  node_name path_selector ctrl_node children_list {
			int   l;
			l = find_in_table(Path_Selector_table,$3);
			if (l==-1) 
			 {
	  		std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unkown Linker Path Selector \n";
	  		YYERROR; 
	  		}
	  	       else
	  	    	{ 
                     	std::cout << "Node type LINKER, idf " << $2.name ;
                     	if ($2.is_transparent) 
		           std::cout << "(transparent) ";
			std::cout <<  $3 <<"_PathSelector";
			 std::cout <<" controlled by node "<<$4;
                     	std::cout << ", " << $5.n_nodes << " children:  " << $5.name << " \n";
//		     auto node = driver.main_sm->CreateLinker ($5.name,   pathSelector, nullptr);
 		     auto node = driver.main_sm->CreateChain ($2.name);
		     for (auto this_child : $5.nodes)
		       node->AppendChild(this_child);
 	  	     $$ = node;
  		      }
		}
 |
t_ACCESS_INTERFACE  node_name t_WORD children_list { 

	int   l;
	l = find_in_table(AI_protocol_table,$3);
  	if (l==-1) 
	  {
	  std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unkown AccessInterface Protocol \n";
	  YYERROR; 
	  }
	  else
	  {
          	std::cout << "Node type ACCESS_INTERFACE , idf " << $2.name;
          	if ($2.is_transparent) std::cout << "(transparent)";
		std::cout  << " Protocol : " << $3;
         	std::cout << ", children:  " << $4.name << " \n";
		
		auto node = driver.main_sm->CreateAccessInterface($2.name, nullptr);
		for (auto this_child : $4.nodes)
		   node->AppendChild(this_child);

  	  	$$ = node;        
		
		}
	  }
 |  
 t_SIB node_name position active children_list
  {
      if ($5.n_nodes!=1) 
	 {
	  std::cerr << "SIB " << $2.name<< " has " << $5.n_nodes << " derivations instead of 1\n";
	  YYERROR; 
	 }
        else
      {
  	std::cout << "Node type SIB, idf	 " << $2.name << "\n";

 	auto node = driver.main_sm->CreateChain ($2.name,nullptr);
	for (auto this_child : $5.nodes)
	    node->AppendChild(this_child);

 	$$ = node;
       }
  }			
 |  
 t_MIB node_name position active reverse max_derivations path_selector children_list
  {
      if ($8.n_nodes>$6) 
	 {
	  std::cerr << "MIB " << $2.name<< " has " << $8.n_nodes << " derivations instead of maximum "<< $6 <<"\n";
	  YYERROR; 
	 }
        else
      {
      int   l;
	l = find_in_table(Path_Selector_table,$7);
	if (l==-1) 
	 {
	 std::cerr << "node " << $2.name<< " \""<< $7 << "\"" << ": Unkown MIB Path Selector \n";
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

	for (auto this_child : $8.nodes)
	    node->AppendChild(this_child);
  	$$ = node;
       }
  }			
 |  
 t_1500_WRAPPER node_name max_derivations children_list
  {
      if ($4.n_nodes>$3) 
	 {
	  std::cerr << "1500 Wrapper " << $2.name<< " has " << $4.n_nodes << " derivations instead of maximum "<< $3 <<"\n";
	  YYERROR; 
	 }
        else
      {
  	std::cout << "1500_Wrapper Macro, idf " << $2.name << " Max derivations " << $3 <<"\n";
        auto node = driver.main_sm->CreateChain ($2.name);
	for (auto this_child : $4.nodes)
	    node->AppendChild(this_child);
  	$$ = node;
       }
  }			
 |  
  t_JTAG_TAP node_name JTAG_target IR_size IR_TABLE n_DR_chains children_list
      { if ($7.n_nodes>($6+1)) 
	 {
	  std::cerr << "JTAG TAP " << $2.name<< " has " << $7.n_nodes-1 << " DR derivations instead of maximum "<< $6 <<"\n";
	  YYERROR; 
	 }
        else
      {
       if ($5.n_words <($7.n_nodes+1))
         {
	  std::cerr << "JTAG TAP " << $2.name<< " has only " << $5.n_words << " IR codings for " << $7.n_nodes << " DR derivations + BPY\n";
	  YYERROR; 
	 }
       else 
  	{
	int   l;
	l = find_in_table(JTAG_AI_target_table,$3);
  	if (l==-1) 
	  {
	  std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unkown JTAG Target \n";
	  YYERROR; 
	  }
	  else
	{
	 std::cout << "JTAG TAP Macro, idf " << $2.name << " IR size " << $4 <<" max DR chains " << $6 <<"\n";
         auto node = driver.main_sm->CreateChain ($2.name);
	 for (auto this_child : $7.nodes)
	    node->AppendChild(this_child);
  	 $$ = node;
	 }
	}
       }
     }
  ; 

JTAG_target: t_WORD
   { $$ =$1;}
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
 t_LeftBracket IR_coding_list t_RightBracket  {$$=$2;}
 ;

IR_coding_list:
 t_QUOTED_STRING { $$.codeword = $1;$$.n_words = 1; }
 |
 t_QUOTED_STRING t_Comma  IR_coding_list {$$.codeword = $3.codeword + ' ' + $1; $$.n_words = ($3.n_words+1); }	
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
                      auto node = driver.main_sm->CreateRegister ($2.name,  BinaryVector::CreateFromBinaryString(remove_quotes($5)), nullptr);
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
//   std::cerr << "Error: " << err_message << " at " << l << "\n";
   std::cerr << "Error: " << err_message << " at line " << nlines << "\n";
   driver.parsed_sut=nullptr;
}
