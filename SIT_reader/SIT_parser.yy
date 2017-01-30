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

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <experimental/string_view>

// ---------------- What is needed for parser to build the SystemModel
//
#include "DefaultNHotPathSelector.hpp"
#include "SystemModelBuilder.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "GenericAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "OfflineProtocol.hpp"
#include "I2C_Player.hpp"
#include "I2C_EmulationProtocol.hpp"
#include "SPI_Protocol.hpp"
#include "STIL_EmulationProtocol.hpp"
#include "AppFunctionNameAndNode.hpp"
#include "OpenOCDProtocol.hpp"
#include "Utility.hpp"
#include "UnresolvedPathSelector.hpp"

#ifdef INTEL_EXPERIMENT
#include "Intel_EmulationProtocol.hpp"
#endif
#include "g3log/g3log.hpp"
#include <experimental/string_view>



#include <map>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::pair;
using std::string;
using std::experimental::string_view;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;


#undef yylex
#define yylex scanner.yylex

#define N_AIs 6
#define MAX_AI_NAME 30

#define DEFAULT_I2C_PREFIX "S2R"

extern int nlines;
extern SIT::SIT_Parser::location_type *my_location;

#define OPENOCD_DEFAULT_CONFIG "openocd-ft2232.cfg"
#ifdef _WIN32
 #define DIR_SEPARATOR '\\'
#else
 #define DIR_SEPARATOR '/'
#endif

#define OPENOCD_DEFAULT_PATH "openocd"

std::vector<std::string> AI_protocol_table  =
  {"JTAG_Loopback","JTAG_SVF_Simulation","JTAG_SVF_Emulation",
  "SPI_FTDI", "STIL_Emulation","I2C_Emulation",
  "Offline","Intel_Packet"
  };
enum AI_protocol_t {JTAG_Loopback,JTAG_SVF_Simulation,JTAG_SVF_Emulation, SPI_FTDI,STIL_Emulation,I2C_Emulation,Offline,Intel_Packet};

std::vector<std::string> JTAG_AI_protocol_table  =
  {"Loopback","SVF_Simulation","SVF_openOCD","SVF_Emulation"
  };
enum JTAG_AI_protocol_t {Loopback,SVF_Simulation,SVF_openOCD,SVF_Emulation};

enum class PathSelectorKind {Binary, One_Hot, N_Hot, Binary_noidle, One_Hot_noidle, N_Hot_noidle};
const vector<PathSelectorKind> PathSelectorKinds {PathSelectorKind::Binary,
                                                  PathSelectorKind::One_Hot,
                                                  PathSelectorKind::N_Hot,
                                                  PathSelectorKind::Binary_noidle,
                                                  PathSelectorKind::One_Hot_noidle,
                                                  PathSelectorKind::N_Hot_noidle};
const vector<string> Path_Selector_table  = {"Binary", "One_Hot", "N_Hot", "Binary_noidle", "One_Hot_noidle", "N_Hot_noidle"};

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

inline std::uint32_t extract_number(std::string s)
{
  char *end;
  auto tmp = remove_quotes(s);
  auto res = std::strtoul (tmp.c_str(), &end,0);
  auto result = res;
  return  result;
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

std::shared_ptr<OpenOCDProtocol>  make_openOCD_protocol(const std::string& designName,std::uint32_t IR_size)
{
  std::string path;

   auto path_tmp = std::getenv("MAST_CONFIGURATION_PATH");

   if (path_tmp == NULL)
    {
     LOG(INFO) << "MAST_CONFIGURATION_PATH environment variable not found, using ./" << OPENOCD_DEFAULT_PATH << " instead";
    path = std::string("./");
    path.append(OPENOCD_DEFAULT_PATH);
    }
    else
     path = std::string(path_tmp);


   if (path.back()!=DIR_SEPARATOR)
	   path.push_back(DIR_SEPARATOR);
    path.append(OPENOCD_DEFAULT_CONFIG);

    CHECK_FILE_EXISTS(path);
   std::cerr << "Creating OpenOCD protocol, designName :" <<  designName <<"path "<< path << "\n";

   auto protocol = make_shared<OpenOCDProtocol> (path, designName, IR_size);
   if (protocol->is_initialized()==false)
    return nullptr;
   else
   return protocol;
}

std::pair<shared_ptr<Register>, shared_ptr<PathSelector>> make_PathSelector
           (
           std::shared_ptr<mast::SystemModelBuilder> builder,
           PathSelectorKind                          SelectorType,
           const SelectorProperty&                   sel_properties,
           const std::string&                        selectorRegName,
           std::uint32_t                             max_derivations
           )
  {
     pair<shared_ptr<Register>, shared_ptr<PathSelector>> res; /*cannot use auto inside a switch*/
     switch(SelectorType)
     {
      case PathSelectorKind::Binary :
      case PathSelectorKind::Binary_noidle :
          res =  builder->Create_PathSelector(SelectorKind::Binary, selectorRegName, max_derivations, sel_properties);
         break;
      case PathSelectorKind::One_Hot :
      case PathSelectorKind::One_Hot_noidle :
          res  = builder->Create_PathSelector(SelectorKind::One_Hot, selectorRegName, max_derivations, sel_properties);
          break;
      case PathSelectorKind::N_Hot :
      case PathSelectorKind::N_Hot_noidle :
          res  = builder->Create_PathSelector(SelectorKind::N_Hot, selectorRegName, max_derivations, sel_properties);
          break;
     }
     return res;
 }

std::shared_ptr<PathSelector> make_PathSelector
           (
            std::shared_ptr<mast::SystemModelBuilder> builder,
            PathSelectorKind                          SelectorType,
            const SelectorProperty&                   sel_properties,
            std::shared_ptr<Register>                 selectorReg,
            std::uint32_t                             max_derivations
	   )
  {
     shared_ptr<PathSelector> res; /*cannot use auto inside a switch*/
 	switch(SelectorType)
	 {
    case PathSelectorKind::Binary :
    case PathSelectorKind::Binary_noidle :
	      res =  builder->Create_PathSelector(SelectorKind::Binary, selectorReg, max_derivations,sel_properties);
	     break;
    case PathSelectorKind::One_Hot :
    case PathSelectorKind::One_Hot_noidle :
 	      res  = builder->Create_PathSelector(SelectorKind::One_Hot, selectorReg, max_derivations,sel_properties);
	      break;
    case PathSelectorKind::N_Hot :
    case PathSelectorKind::N_Hot_noidle :
 	      res  = builder->Create_PathSelector(SelectorKind::N_Hot, selectorReg, max_derivations,sel_properties);
	      break;
	 }
  return res;
 }

}
} /*end of %code section*/

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
%type  <std::uint32_t> n_chains
%type  <std::vector<uint32_t>> AI_Coding_list
%type  <std::vector<mast::BinaryVector>> IR_Coding_list
%type  <std::vector<mast::BinaryVector>> IR_TABLE
%type  <std::vector<uint32_t>> AI_TABLE
%type  <std::string> JTAG_protocol
%type  <std::shared_ptr<mast::SystemModelNode>> root_node
%type  <std::shared_ptr<mast::SystemModelNode>> register_node
%type  <std::shared_ptr<mast::SystemModelNode>> leaf_node
%type  <std::shared_ptr<mast::SystemModelNode>> parent_node
%type  <std::shared_ptr<mast::SystemModelNode>> node
%type  <std::shared_ptr<mast::SystemModelNode>> parent_node_with_children
%type  <std::vector<std::string>> function_list
%type  <std::pair <std::vector<std::string>,std::uint32_t>> PDL_declaration
%type  <std::string> AI_identifier
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
%token  t_RightParenthesis
%token  t_LeftParenthesis
%%

root_node:
  node END
  {
    /*resolve evetual Linkers*/
    while (!driver.unresolved_linkers.empty())
    {
      const auto& this_linker = driver.unresolved_linkers.front();

      // remove resolved linker
      auto selector_info= driver.declared_registers.find(this_linker.selector_name);
      if (selector_info==driver.declared_registers.end())
      {
        std::cerr << "Error, Selector register " << this_linker.selector_name << " required by linker " <<  this_linker.linker_node->Name() << " at line " << this_linker.line <<":"<<this_linker.column << " does not exist\n" ;
        YYERROR;
      }

      auto properties   = Path_Selector_prop_t[this_linker.selector_kind_index];
      auto selectorKind = PathSelectorKinds[this_linker.selector_kind_index];
      auto selector = make_PathSelector (driver.builder,
                                         selectorKind,
                                         properties,
                                         dynamic_pointer_cast<Register>(selector_info->second),
                                         this_linker.derivations);

      auto linker_node = dynamic_pointer_cast<Linker>(this_linker.linker_node);
      linker_node->ReplacePathSelector(selector);
      driver.unresolved_linkers.pop();
    }
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
  if (!$2.first.empty())
   {
    for (auto this_function : $2.first)
      driver.namesAndNodes.push_back(AppFunctionNameAndNode(this_function,asParentNode,$2.second));
   }
 }

PDL_declaration:
 t_PDL function_list {
           pair <std::vector<std::string>,std::uint32_t> ret;
	   ret.first = $2;
//	   ret.second =my_location->begin.line;
   ret.second =nlines;
	   $$ = ret;
	   }
 |
  {
   std::pair <std::vector<std::string>,std::uint32_t> ret;
   ret.first = std::vector<std::string>();
//   ret.second =my_location->begin.line;
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
t_CHAIN  node_name  {
		     auto node = driver.main_sm->CreateChain($2.name);
                     if ($2.is_transparent)
		           node->IgnoreForNodePath(true);
 	  	     $$ = node;
  		     }
 |
t_LINKER  node_name path_selector ctrl_node max_derivations{
      int selectorKindIndex = index_in_table(Path_Selector_table, $3);
      if (selectorKindIndex == -1)
      {
   			std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-"
					<< my_location->end.column << ": " ;
      			std::cerr << "node " << $2.name<< " \""<< $3 << "\"" << ": Unknown Linker Path Selector \n";
	  		YYERROR;
      }

      if ($4 == "")
      {
        std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-";
        std::cerr << my_location->end.column << ": " ;
        std::cerr << "Must specify a control node for path selector\n";
        YYERROR;
      }

      std::ostringstream os;
      os << "Node type LINKER, idf " << $2.name << " ";
      if ($2.is_transparent)
        os << "(transparent) ";
      os << $3 << "_PathSelector";
      os << " controlled by node " << $4;
      LOG(DEBUG) << os;

      auto pathSelector = make_shared<UnresolvedPathSelector>();
      auto node = driver.main_sm->CreateLinker ($2.name, pathSelector);

      linker_information linkerInfo;
      linkerInfo.linker_node         = node;
      linkerInfo.column              = my_location->begin.column;
      linkerInfo.line                = my_location->begin.line;
      linkerInfo.selector_kind_index = selectorKindIndex;
      linkerInfo.selector_name       = $4;
      linkerInfo.derivations         = $5;
      driver.unresolved_linkers.push(linkerInfo);

      $$ = node;
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
  int index = index_in_table(Path_Selector_table,$7);
  if (index==-1)
	 {
	 std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
   std::cerr << "node " << $2.name<< " \""<< $7 << "\"" << ": Unknown MIB Path Selector \n";
	 YYERROR;
	 }

   auto selectorRegName = $2.name + MIB_CTRL_EXT;
   SelectorProperty sel_properties = $4 | $5| Path_Selector_prop_t[index];

   auto selectorKind = PathSelectorKinds[index];
   auto res = make_PathSelector(driver.builder,
                                selectorKind,
                                sel_properties,
                                selectorRegName,
                                $6);

    auto selectorReg = res.first;
    auto selector    = res.second;
    auto node        = driver.builder->Create_MIB($2.name, selector, selectorReg, $3);

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
t_ACCESS_INTERFACE  node_name t_WORD AI_identifier AI_TABLE n_chains {

  int l = index_in_table(AI_protocol_table,$3);
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
         {
	 protocol = make_shared<LoopbackAccessInterfaceProtocol > ();
	 break;
	 }
        case JTAG_SVF_Simulation :
         {
	 protocol = make_shared<SVF_SimulationProtocol > ();
	 break;
	 }
        case JTAG_SVF_Emulation :
         {
	 protocol = make_shared<SVF_EmulationProtocol> ();
	 break;
	 }
        case STIL_Emulation :
         {
	 protocol = make_shared<STIL_EmulationProtocol> ($6+1); //one derivation for reset
	 break;
	 }
        case AI_protocol_t::SPI_FTDI :
        {
          if ($5.size()==0)
          {
            std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
            std::cerr << "Error, " << AI_protocol_table[l] <<" needs an address table\n";
            YYERROR;
          }
          if (($5.size()%3)!=0)
          {
            std::cerr << "Error, " << AI_protocol_table[l] <<" requires 3 addresses for each slave\n";
          }

          LOG(DEBUG) << "Generating " << AI_protocol_table[l] << " Access Interface";

          auto chipselectCommands = std::vector<uint32_t>();
          auto readCommands       = std::vector<uint32_t>();
          auto writeCommands      = std::vector<uint32_t>();

          for ( auto i = 0 ; i < $5.size(); i += 3)
          {
            chipselectCommands.push_back($5[i]);
            readCommands.push_back($5[i+1]);
            writeCommands.push_back($5[i+2]);
          }


          auto displayContent = [](string_view name, const std::vector<uint32_t>& container)
          {
            LOG(DEBUG) << name << " (size " << std::noshowbase << container.size() << ") ";
            for (auto item : container)
            {
              LOG(DEBUG) << "0x" << std::hex << item << " ";
            }
          };

          displayContent("chipselectCommands", chipselectCommands);
          displayContent("readCommands",       readCommands);
          displayContent("writeCommands",      writeCommands);

	   if (!$4.empty())
            protocol = make_shared<SPI_Protocol > (std::move(chipselectCommands), std::move(readCommands), std::move(writeCommands));
          else
            {
    	    auto usbDeviceID = extract_number($4);
		protocol = make_shared<SPI_Protocol > (std::move(chipselectCommands), std::move(readCommands), std::move(writeCommands), "", static_cast<std::uint16_t>(usbDeviceID));
		}

          break;
        } // End of: case AI_protocol_t::SPI_FTDI :
        case I2C_Emulation :
         {
          if ($5.size()==0)
          {
            std::cerr << "Line " << my_location->begin.line << ":" << my_location->begin.column << "-" << my_location->end.column << ": " ;
            std::cerr << "Error, " << AI_protocol_table[l] <<" needs an address table\n";
            YYERROR;
          }
          if (($5.size())!=$6)
          {
            std::cerr << "Error, " << AI_protocol_table[l] <<" requires 1 address for each register chain\n";
            YYERROR;
          }
          LOG(DEBUG) << "Generating " << AI_protocol_table[l] << " Access Interface";

          auto I2C_addresses       = std::vector<uint32_t>();

          for ( auto i = 0 ; i < $5.size(); i ++)
          {
            I2C_addresses.push_back($5[i]);
          }
	 protocol = make_shared<I2C_EmulationProtocol> (I2C_addresses);
	 break;
	 }
        case Offline :
         {
	 protocol = make_shared<OfflineProtocol > ();
	 break;
	 }
        case Intel_Packet :
         {
          #ifdef INTEL_EXPERIMENT
            if (($5.size())!=$6)
            {
              std::cerr << "Error, " << AI_protocol_table[l] <<" requires 1 address for each register chain\n";
              YYERROR;
            }
            auto Region_addresses       = std::vector<uint32_t>();
            for ( auto i = 0 ; i < $5.size(); i ++)
            {
              Region_addresses.push_back($5[i]);
            }
            protocol = make_shared<Intel_EmulationProtocol > (Region_addresses);
          #else
           std::cerr << "Error, Intel_EmulationProtocol has not been built";
          #endif
	 break;
	 }
      } // End of: switch(l)

      auto node = driver.main_sm->CreateAccessInterface($2.name, protocol);
      $$ = node;
		}
	  }
 |
  t_JTAG_TAP node_name JTAG_protocol AI_identifier IR_size IR_TABLE n_DR_chains
      {
      {
  	{
    int l = index_in_table(JTAG_AI_protocol_table,$3);
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
	  case SVF_Simulation :
	   {
	   protocol = make_shared<SVF_SimulationProtocol > ();
	  break;
	  }
	  case SVF_Emulation :
	   protocol = make_shared<SVF_EmulationProtocol > ();
	  break;
	  case SVF_openOCD :
	   {
	   if ($4.empty())
	    {
	     std::cerr<<"Error, OpenOCD protocol requires providing a Design Name before IR Size\n";
	     YYERROR;
	    }
	   protocol=make_openOCD_protocol( $4, $5);
	   if (protocol==nullptr)
	    {
	    std::cerr<<"Error while setting up OpenOCD interface\n";
	    YYERROR;
	    }
	  break;
           }
	  }
	  if ($6.size()==0)
	   {
	   auto node = driver.builder->Create_JTAG_TAP($2.name,$5,$7+1,protocol);
  	   $$ = node;
	   }
	  else
	   {
	     if ($6.size()!=($7+1))
	      {
	      std::cerr<<"Error Coding must be provided for bypass register and each chain\n";
	     YYERROR;
	      }
	   auto node = driver.builder->Create_JTAG_TAP($2.name,$5,$7+1,protocol,$6);
  	   $$ = node;

	   }
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


n_chains :
 {$$ = 0;}
 |
 t_DecimalLiteral { $$ = $1;}
;


IR_TABLE:
 t_LeftBracket IR_Coding_list t_RightBracket  {$$=$2;}
 |  {$$ = std::vector<mast::BinaryVector>();}
 ;

AI_TABLE:
t_LeftBracket AI_Coding_list t_RightBracket  {$$=$2;}
 |  {$$ = std::vector<uint32_t>();}
 ;

AI_identifier:
 t_WORD
    {
       $$ = $1;
    }
 |
 { $$="";
 }

AI_Coding_list:
 t_QUOTED_STRING
    {
       $$.push_back(extract_number($1));
    }
 |
 t_QUOTED_STRING t_Comma  AI_Coding_list
  {
   auto tmp = $3;
   tmp.insert(tmp.begin(),extract_number($1));
   $$ = tmp;
   }
;

IR_Coding_list:
 t_QUOTED_STRING
    {
       $$.push_back(BinaryVector::CreateFromString(remove_quotes($1)));
    }
 |
 t_QUOTED_STRING t_Comma  IR_Coding_list
  {
   auto tmp = $3;
   tmp.insert(tmp.begin(),BinaryVector::CreateFromString(remove_quotes($1)));
   $$ = tmp;
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
		     driver.declared_registers.insert ( std::pair<std::string, std::shared_ptr<mast::SystemModelNode>>
		     			($2.name,node) );

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
  std::ostringstream os;

  os << "Line " << my_location->begin.line
     << ":"     << my_location->begin.column
     << "-"     << my_location->end.column
     << ": "    << err_message << "\n";

  std::cerr << os.str();
  driver.parsed_sut = nullptr;

  auto exceptionMessage = "SIT parsing error: "s + os.str();
  THROW_RUNTIME_ERROR(exceptionMessage);
}
