// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "SIT_parser.tab.hh"

// User implementation prologue.

#line 51 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 32 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:413

   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "SIT_driver.hpp"
   #include "SIT_types.h"

#undef yylex
#define yylex scanner.yylex

#define N_AIs 6
#define MAX_AI_NAME 30

std::vector<std::string> AI_protocol_table  =
  {"JTAG_SVF_loopback","JTAG_SVF_simulation","JTAG_SVF_openOCD",
  "I2C_loopback","I2C_simulation","I2C_api"
  };

std::vector<std::string> JTAG_AI_target_table  =
  {"SVF_loopback","SVF_simulation","SVF_openOCD"
  };

std::vector<std::string> Path_Selector_table  =
  {"Default_Binary","Default_Table_based"
  };

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
   

#line 95 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:479
namespace SIT {
#line 181 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:479

  /// Build a parser object.
  SIT_Parser::SIT_Parser (SIT_Scanner  &scanner_yyarg, SIT_Driver  &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  SIT_Parser::~SIT_Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  SIT_Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  SIT_Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  SIT_Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.copy< IR_coding_type > (other.value);
        break;

      case 48: // position
        value.copy< ctrl_position > (other.value);
        break;

      case 49: // active
        value.copy< logic_polarity > (other.value);
        break;

      case 38: // node_name
        value.copy< name_type > (other.value);
        break;

      case 34: // children_list
      case 35: // node_list
        value.copy< node_list_type > (other.value);
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.copy< std::shared_ptr<mast::SystemModelNode> > (other.value);
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.copy< std::string > (other.value);
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.copy< std::uint32_t > (other.value);
        break;

      case 37: // is_transparent
      case 53: // hold
        value.copy< std::uint8_t > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.copy< IR_coding_type > (v);
        break;

      case 48: // position
        value.copy< ctrl_position > (v);
        break;

      case 49: // active
        value.copy< logic_polarity > (v);
        break;

      case 38: // node_name
        value.copy< name_type > (v);
        break;

      case 34: // children_list
      case 35: // node_list
        value.copy< node_list_type > (v);
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.copy< std::shared_ptr<mast::SystemModelNode> > (v);
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.copy< std::string > (v);
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.copy< std::uint32_t > (v);
        break;

      case 37: // is_transparent
      case 53: // hold
        value.copy< std::uint8_t > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const IR_coding_type v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ctrl_position v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const logic_polarity v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const name_type v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const node_list_type v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::shared_ptr<mast::SystemModelNode> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::uint32_t v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  SIT_Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::uint8_t v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  SIT_Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  SIT_Parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.template destroy< IR_coding_type > ();
        break;

      case 48: // position
        value.template destroy< ctrl_position > ();
        break;

      case 49: // active
        value.template destroy< logic_polarity > ();
        break;

      case 38: // node_name
        value.template destroy< name_type > ();
        break;

      case 34: // children_list
      case 35: // node_list
        value.template destroy< node_list_type > ();
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.template destroy< std::shared_ptr<mast::SystemModelNode> > ();
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.template destroy< std::string > ();
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.template destroy< std::uint32_t > ();
        break;

      case 37: // is_transparent
      case 53: // hold
        value.template destroy< std::uint8_t > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  SIT_Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  SIT_Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.move< IR_coding_type > (s.value);
        break;

      case 48: // position
        value.move< ctrl_position > (s.value);
        break;

      case 49: // active
        value.move< logic_polarity > (s.value);
        break;

      case 38: // node_name
        value.move< name_type > (s.value);
        break;

      case 34: // children_list
      case 35: // node_list
        value.move< node_list_type > (s.value);
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.move< std::shared_ptr<mast::SystemModelNode> > (s.value);
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.move< std::string > (s.value);
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.move< std::uint32_t > (s.value);
        break;

      case 37: // is_transparent
      case 53: // hold
        value.move< std::uint8_t > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  SIT_Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  SIT_Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  SIT_Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  SIT_Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  SIT_Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  SIT_Parser::by_type::type_get () const
  {
    return type;
  }
  // Implementation of make_symbol for each symbol type.
  SIT_Parser::symbol_type
  SIT_Parser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_UPPER (const location_type& l)
  {
    return symbol_type (token::UPPER, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_LOWER (const location_type& l)
  {
    return symbol_type (token::LOWER, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_WORD (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_WORD, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_CHAR (const location_type& l)
  {
    return symbol_type (token::CHAR, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::t_SEMICOLON, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_CHAIN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_CHAIN, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_REGISTER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_REGISTER, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_LINKER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_LINKER, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_ACCESS_INTERFACE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_ACCESS_INTERFACE, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_SIB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_SIB, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_MIB (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_MIB, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_1500_WRAPPER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_1500_WRAPPER, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_JTAG_TAP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_JTAG_TAP, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_BASED_INTEGER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_BASED_INTEGER, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_START_HIERARCHY (const location_type& l)
  {
    return symbol_type (token::t_START_HIERARCHY, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_END_HIERARCHY (const location_type& l)
  {
    return symbol_type (token::t_END_HIERARCHY, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_TRANSPARENT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_TRANSPARENT, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_HOLD_VALUE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_HOLD_VALUE, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_BYPASS (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_BYPASS, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_BINARY_VECTOR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_BINARY_VECTOR, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_QUOTED_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_QUOTED_STRING, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_POST (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_POST, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_PRE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_PRE, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_HIGH (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_HIGH, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_LOW (const std::string& v, const location_type& l)
  {
    return symbol_type (token::t_LOW, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_DecimalLiteral (const std::uint32_t& v, const location_type& l)
  {
    return symbol_type (token::t_DecimalLiteral, v, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_RightBracket (const location_type& l)
  {
    return symbol_type (token::t_RightBracket, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_LeftBracket (const location_type& l)
  {
    return symbol_type (token::t_LeftBracket, l);
  }

  SIT_Parser::symbol_type
  SIT_Parser::make_t_Comma (const location_type& l)
  {
    return symbol_type (token::t_Comma, l);
  }



  // by_state.
  inline
  SIT_Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  SIT_Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  SIT_Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  SIT_Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  SIT_Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  SIT_Parser::symbol_number_type
  SIT_Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  SIT_Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  SIT_Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.move< IR_coding_type > (that.value);
        break;

      case 48: // position
        value.move< ctrl_position > (that.value);
        break;

      case 49: // active
        value.move< logic_polarity > (that.value);
        break;

      case 38: // node_name
        value.move< name_type > (that.value);
        break;

      case 34: // children_list
      case 35: // node_list
        value.move< node_list_type > (that.value);
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.move< std::shared_ptr<mast::SystemModelNode> > (that.value);
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.move< std::string > (that.value);
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.move< std::uint32_t > (that.value);
        break;

      case 37: // is_transparent
      case 53: // hold
        value.move< std::uint8_t > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  SIT_Parser::stack_symbol_type&
  SIT_Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        value.copy< IR_coding_type > (that.value);
        break;

      case 48: // position
        value.copy< ctrl_position > (that.value);
        break;

      case 49: // active
        value.copy< logic_polarity > (that.value);
        break;

      case 38: // node_name
        value.copy< name_type > (that.value);
        break;

      case 34: // children_list
      case 35: // node_list
        value.copy< node_list_type > (that.value);
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        value.copy< std::shared_ptr<mast::SystemModelNode> > (that.value);
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        value.copy< std::string > (that.value);
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        value.copy< std::uint32_t > (that.value);
        break;

      case 37: // is_transparent
      case 53: // hold
        value.copy< std::uint8_t > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  SIT_Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  SIT_Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  SIT_Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  SIT_Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  SIT_Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SIT_Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SIT_Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SIT_Parser::debug_level_type
  SIT_Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  SIT_Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline SIT_Parser::state_type
  SIT_Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  SIT_Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  SIT_Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SIT_Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 45: // IR_TABLE
      case 46: // IR_coding_list
        yylhs.value.build< IR_coding_type > ();
        break;

      case 48: // position
        yylhs.value.build< ctrl_position > ();
        break;

      case 49: // active
        yylhs.value.build< logic_polarity > ();
        break;

      case 38: // node_name
        yylhs.value.build< name_type > ();
        break;

      case 34: // children_list
      case 35: // node_list
        yylhs.value.build< node_list_type > ();
        break;

      case 33: // root_node
      case 36: // node
      case 39: // internal_node
      case 50: // leaf_node
      case 51: // register_node
        yylhs.value.build< std::shared_ptr<mast::SystemModelNode> > ();
        break;

      case 5: // t_WORD
      case 8: // t_CHAIN
      case 9: // t_REGISTER
      case 10: // t_LINKER
      case 11: // t_ACCESS_INTERFACE
      case 12: // t_SIB
      case 13: // t_MIB
      case 14: // t_1500_WRAPPER
      case 15: // t_JTAG_TAP
      case 16: // t_BASED_INTEGER
      case 19: // t_TRANSPARENT
      case 20: // t_HOLD_VALUE
      case 21: // t_BYPASS
      case 22: // t_BINARY_VECTOR
      case 23: // t_QUOTED_STRING
      case 24: // t_POST
      case 25: // t_PRE
      case 26: // t_HIGH
      case 27: // t_LOW
      case 40: // JTAG_target
      case 41: // path_selector
      case 42: // ctrl_node
      case 54: // bypass
        yylhs.value.build< std::string > ();
        break;

      case 28: // t_DecimalLiteral
      case 43: // IR_size
      case 44: // n_DR_chains
      case 47: // max_derivations
      case 52: // size
        yylhs.value.build< std::uint32_t > ();
        break;

      case 37: // is_transparent
      case 53: // hold
        yylhs.value.build< std::uint8_t > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 139 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
    std::cout << "Parsing OK, Root node is " <<yystack_[1].value.as< std::shared_ptr<mast::SystemModelNode> > ().get()->Name() << "  \n";
    driver.parsed_sut->ReplaceRoot(std::dynamic_pointer_cast<ParentNode>(yystack_[1].value.as< std::shared_ptr<mast::SystemModelNode> > ()),false);
    }
#line 1395 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 146 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< node_list_type > () = yystack_[1].value.as< node_list_type > ();}
#line 1401 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 150 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< node_list_type > ().name = yystack_[1].value.as< std::shared_ptr<mast::SystemModelNode> > ().get()->Name() + ' ' + yystack_[0].value.as< node_list_type > ().name; yylhs.value.as< node_list_type > ().n_nodes = yystack_[0].value.as< node_list_type > ().n_nodes+1;
                    auto tmp = yystack_[0].value.as< node_list_type > ().nodes;
		    tmp.push_back(yystack_[1].value.as< std::shared_ptr<mast::SystemModelNode> > ());
		    yylhs.value.as< node_list_type > ().nodes = tmp; 
		    }
#line 1411 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 155 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< node_list_type > ().name = yystack_[0].value.as< std::shared_ptr<mast::SystemModelNode> > ().get()->Name();yylhs.value.as< node_list_type > ().n_nodes = 1;yylhs.value.as< node_list_type > ().nodes.push_back(yystack_[0].value.as< std::shared_ptr<mast::SystemModelNode> > ());}
#line 1417 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 159 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = yystack_[0].value.as< std::shared_ptr<mast::SystemModelNode> > ();}
#line 1423 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 161 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = yystack_[0].value.as< std::shared_ptr<mast::SystemModelNode> > ();}
#line 1429 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 165 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {yylhs.value.as< std::uint8_t > () = 1;}
#line 1435 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 167 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::uint8_t > () = 0;}
#line 1441 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 171 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { 
  		     yylhs.value.as< name_type > ().name = yystack_[1].value.as< std::string > ();
		     yylhs.value.as< name_type > ().is_transparent = yystack_[0].value.as< std::uint8_t > ();
		     }
#line 1450 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 179 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { 
                     std::cout << "Node type CHAIN, idf " << yystack_[1].value.as< name_type > ().name ;
                     if (yystack_[1].value.as< name_type > ().is_transparent) 
		           std::cout << "(transparent)";
                     std::cout << ", " << yystack_[0].value.as< node_list_type > ().n_nodes << " children:  " << yystack_[0].value.as< node_list_type > ().name << " \n";
		     
		     auto node = driver.parsed_sut->CreateChain(yystack_[1].value.as< name_type > ().name);
		//     for_each($3.nodes.begin(),$3.nodes.end(),node->AppendChild); 
		     for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
		       node->AppendChild(this_child);
 	  	     yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
  		     }
#line 1467 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 192 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
			int   l;
			l = find_in_table(Path_Selector_table,yystack_[2].value.as< std::string > ());
			if (l==-1) 
			 {
	  		std::cerr << "node " << yystack_[3].value.as< name_type > ().name<< " \""<< yystack_[2].value.as< std::string > () << "\"" << ": Unkown Linker Path Selector \n";
	  		YYERROR; 
	  		}
	  	       else
	  	    	{ 
                     	std::cout << "Node type LINKER, idf " << yystack_[3].value.as< name_type > ().name ;
                     	if (yystack_[3].value.as< name_type > ().is_transparent) 
		           std::cout << "(transparent) ";
			std::cout <<  yystack_[2].value.as< std::string > () <<"_PathSelector";
			 std::cout <<" controlled by node "<<yystack_[1].value.as< std::string > ();
                     	std::cout << ", " << yystack_[0].value.as< node_list_type > ().n_nodes << " children:  " << yystack_[0].value.as< node_list_type > ().name << " \n";
//		     auto node = driver.parsed_sut->CreateLinker ($5.name,   pathSelector, nullptr);
 		     auto node = driver.parsed_sut->CreateChain (yystack_[3].value.as< name_type > ().name);
		     for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
		       node->AppendChild(this_child);
 	  	     yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
  		      }
		}
#line 1495 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 216 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { 

	int   l;
	l = find_in_table(AI_protocol_table,yystack_[1].value.as< std::string > ());
  	if (l==-1) 
	  {
	  std::cerr << "node " << yystack_[2].value.as< name_type > ().name<< " \""<< yystack_[1].value.as< std::string > () << "\"" << ": Unkown AccessInterface Protocol \n";
	  YYERROR; 
	  }
	  else
	  {
          	std::cout << "Node type ACCESS_INTERFACE , idf " << yystack_[2].value.as< name_type > ().name;
          	if (yystack_[2].value.as< name_type > ().is_transparent) std::cout << "(transparent)";
		std::cout  << " Protocol : " << yystack_[1].value.as< std::string > ();
         	std::cout << ", children:  " << yystack_[0].value.as< node_list_type > ().name << " \n";
		
		auto node = driver.parsed_sut->CreateAccessInterface(yystack_[2].value.as< name_type > ().name, nullptr);
		for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
		   node->AppendChild(this_child);

  	  	yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;        
		
		}
	  }
#line 1524 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 242 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
      if (yystack_[0].value.as< node_list_type > ().n_nodes!=1) 
	 {
	  std::cerr << "SIB " << yystack_[3].value.as< name_type > ().name<< " has " << yystack_[0].value.as< node_list_type > ().n_nodes << " derivations instead of 1\n";
	  YYERROR; 
	 }
        else
      {
  	std::cout << "Node type SIB, idf	 " << yystack_[3].value.as< name_type > ().name << " " << yystack_[2].value.as< ctrl_position > () <<" " << yystack_[1].value.as< logic_polarity > () <<"\n";

 	auto node = driver.parsed_sut->CreateChain (yystack_[3].value.as< name_type > ().name,nullptr);
	for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
	    node->AppendChild(this_child);

 	yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
       }
  }
#line 1546 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 261 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
      if (yystack_[0].value.as< node_list_type > ().n_nodes>yystack_[3].value.as< std::uint32_t > ()) 
	 {
	  std::cerr << "MIB " << yystack_[6].value.as< name_type > ().name<< " has " << yystack_[0].value.as< node_list_type > ().n_nodes << " derivations instead of maximum "<< yystack_[3].value.as< std::uint32_t > () <<"\n";
	  YYERROR; 
	 }
        else
      {
  	std::cout << "Node type MIB, idf " << yystack_[6].value.as< name_type > ().name << " " << yystack_[5].value.as< ctrl_position > () <<" " << yystack_[4].value.as< logic_polarity > () << " Max derivations " << yystack_[3].value.as< std::uint32_t > () << " " << yystack_[2].value.as< std::string > () << "_PathSelctor ctrl_node " << yystack_[1].value.as< std::string > () <<"\n";
        auto node = driver.parsed_sut->CreateChain (yystack_[6].value.as< name_type > ().name);
	for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
	    node->AppendChild(this_child);
  	yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
       }
  }
#line 1566 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 278 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
      if (yystack_[0].value.as< node_list_type > ().n_nodes>yystack_[1].value.as< std::uint32_t > ()) 
	 {
	  std::cerr << "1500 Wrapper " << yystack_[2].value.as< name_type > ().name<< " has " << yystack_[0].value.as< node_list_type > ().n_nodes << " derivations instead of maximum "<< yystack_[1].value.as< std::uint32_t > () <<"\n";
	  YYERROR; 
	 }
        else
      {
  	std::cout << "1500_Wrapper Macro, idf " << yystack_[2].value.as< name_type > ().name << " Max derivations " << yystack_[1].value.as< std::uint32_t > () <<"\n";
        auto node = driver.parsed_sut->CreateChain (yystack_[2].value.as< name_type > ().name);
	for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
	    node->AppendChild(this_child);
  	yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
       }
  }
#line 1586 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 295 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { if (yystack_[0].value.as< node_list_type > ().n_nodes>(yystack_[1].value.as< std::uint32_t > ()+1)) 
	 {
	  std::cerr << "JTAG TAP " << yystack_[5].value.as< name_type > ().name<< " has " << yystack_[0].value.as< node_list_type > ().n_nodes-1 << " DR derivations instead of maximum "<< yystack_[1].value.as< std::uint32_t > () <<"\n";
	  YYERROR; 
	 }
        else
      {
       if (yystack_[2].value.as< IR_coding_type > ().n_words <(yystack_[0].value.as< node_list_type > ().n_nodes+1))
         {
	  std::cerr << "JTAG TAP " << yystack_[5].value.as< name_type > ().name<< " has only " << yystack_[2].value.as< IR_coding_type > ().n_words << " IR codings for " << yystack_[0].value.as< node_list_type > ().n_nodes << " DR derivations + BPY\n";
	  YYERROR; 
	 }
       else 
  	{
	int   l;
	l = find_in_table(JTAG_AI_target_table,yystack_[4].value.as< std::string > ());
  	if (l==-1) 
	  {
	  std::cerr << "node " << yystack_[5].value.as< name_type > ().name<< " \""<< yystack_[4].value.as< std::string > () << "\"" << ": Unkown JTAG Target \n";
	  YYERROR; 
	  }
	  else
	{
	 std::cout << "JTAG TAP Macro, idf " << yystack_[5].value.as< name_type > ().name << " IR size " << yystack_[3].value.as< std::uint32_t > () <<" max DR chains " << yystack_[1].value.as< std::uint32_t > () <<"\n";
         auto node = driver.parsed_sut->CreateChain (yystack_[5].value.as< name_type > ().name);
	 for (auto this_child : yystack_[0].value.as< node_list_type > ().nodes)
	    node->AppendChild(this_child);
  	 yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;
	 }
	}
       }
     }
#line 1623 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 330 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () =yystack_[0].value.as< std::string > ();}
#line 1629 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 334 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () =yystack_[0].value.as< std::string > ();}
#line 1635 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 337 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () =yystack_[0].value.as< std::string > ();}
#line 1641 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 341 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::uint32_t > () = yystack_[0].value.as< std::uint32_t > ();}
#line 1647 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 344 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::uint32_t > () = yystack_[0].value.as< std::uint32_t > ();}
#line 1653 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 347 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {yylhs.value.as< IR_coding_type > ()=yystack_[1].value.as< IR_coding_type > ();}
#line 1659 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 351 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< IR_coding_type > ().codeword = yystack_[0].value.as< std::string > ();yylhs.value.as< IR_coding_type > ().n_words = 1; }
#line 1665 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 353 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {yylhs.value.as< IR_coding_type > ().codeword = yystack_[0].value.as< IR_coding_type > ().codeword + ' ' + yystack_[2].value.as< std::string > (); yylhs.value.as< IR_coding_type > ().n_words = (yystack_[0].value.as< IR_coding_type > ().n_words+1); }
#line 1671 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 358 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::uint32_t > () = yystack_[0].value.as< std::uint32_t > ();}
#line 1677 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 361 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ctrl_position > () = AFTER;}
#line 1683 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 363 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ctrl_position > () = BEFORE;}
#line 1689 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 367 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< logic_polarity > () = HIGH;}
#line 1695 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 369 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< logic_polarity > () = LOW;}
#line 1701 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 372 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {     yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > ()=  yystack_[0].value.as< std::shared_ptr<mast::SystemModelNode> > ();
                           }
#line 1708 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 377 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
                      auto node = driver.parsed_sut->CreateRegister (yystack_[3].value.as< name_type > ().name, BinaryVector(12, 0), nullptr); 
  		     yylhs.value.as< std::shared_ptr<mast::SystemModelNode> > () = node;}
#line 1716 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 382 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::uint32_t > () = yystack_[0].value.as< std::uint32_t > ();}
#line 1722 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 386 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {  yylhs.value.as< std::uint8_t > ()=1 ; }
#line 1728 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 387 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {  yylhs.value.as< std::uint8_t > ()=0 ; }
#line 1734 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 392 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:859
    {
   yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > () ;
  }
#line 1742 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
    break;


#line 1746 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  SIT_Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  SIT_Parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char SIT_Parser::yypact_ninf_ = -27;

  const signed char SIT_Parser::yytable_ninf_ = -1;

  const signed char
  SIT_Parser::yypact_[] =
  {
      21,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    14,    15,
      -6,     0,    25,    32,   -23,   -23,   -10,    33,   -27,   -27,
     -27,   -27,    13,   -27,   -27,    36,     0,   -27,   -27,   -22,
     -22,   -27,     0,   -27,    -9,    -2,    26,    13,   -27,   -27,
     -27,   -27,     0,   -27,   -27,   -27,     0,   -10,   -27,   -27,
      16,    17,   -27,   -27,   -27,   -27,    25,    20,    19,   -27,
      28,    36,    18,    22,   -27,     0,   -27,    29,     0,    20,
     -27,   -27,    45,   -27,   -27,   -27,    30,   -27
  };

  const unsigned char
  SIT_Parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     0,     0,     0,     1,     2,
       8,    10,     0,    11,    19,     0,     0,    27,    28,     0,
       0,    26,     0,    18,     0,     0,     0,     5,     6,     7,
      31,    20,     0,    13,    29,    30,     0,     0,    16,    21,
       0,     0,     3,     4,    12,    14,     0,     0,     0,    33,
      35,     0,    24,     0,    22,     0,    34,     0,     0,     0,
      23,    17,     0,    32,    15,    25,     0,    36
  };

  const signed char
  SIT_Parser::yypgoto_[] =
  {
     -27,   -27,   -26,    23,   -27,   -27,     5,    54,   -27,    -1,
      -5,   -27,   -27,   -27,   -12,    11,    44,    31,   -27,   -27,
     -27,   -27,   -27
  };

  const signed char
  SIT_Parser::yydefgoto_[] =
  {
      -1,     8,    23,    36,    37,    21,    11,    38,    34,    25,
      42,    50,    65,    58,    63,    32,    29,    46,    39,    40,
      60,    67,    73
  };

  const unsigned char
  SIT_Parser::yytable_[] =
  {
      43,    27,    28,    10,    44,    45,    48,    12,    13,    14,
      15,    16,    17,    20,    18,    19,    54,    22,    31,    49,
      55,     1,    35,     2,     3,     4,     5,     6,     7,     1,
      24,     2,     3,     4,     5,     6,     7,    26,    33,    71,
      51,    41,    74,    62,    52,    59,    57,    64,    66,    69,
      72,    70,    76,    77,     9,    61,    68,    75,    56,    30,
      53,    47
  };

  const unsigned char
  SIT_Parser::yycheck_[] =
  {
      26,    24,    25,     5,    26,    27,    32,     2,     3,     4,
       5,     6,     7,    19,     0,     0,    42,    17,    28,    28,
      46,     8,     9,    10,    11,    12,    13,    14,    15,     8,
       5,    10,    11,    12,    13,    14,    15,     5,     5,    65,
      35,     5,    68,    23,    18,    28,    30,    28,    20,    31,
      21,    29,     7,    23,     0,    56,    61,    69,    47,    15,
      37,    30
  };

  const unsigned char
  SIT_Parser::yystos_[] =
  {
       0,     8,    10,    11,    12,    13,    14,    15,    33,    39,
       5,    38,    38,    38,    38,    38,    38,    38,     0,     0,
      19,    37,    17,    34,     5,    41,     5,    24,    25,    48,
      48,    28,    47,     5,    40,     9,    35,    36,    39,    50,
      51,     5,    42,    34,    26,    27,    49,    49,    34,    28,
      43,    38,    18,    35,    34,    34,    47,    30,    45,    28,
      52,    41,    23,    46,    28,    44,    20,    53,    42,    31,
      29,    34,    21,    54,    34,    46,     7,    23
  };

  const unsigned char
  SIT_Parser::yyr1_[] =
  {
       0,    32,    33,    34,    35,    35,    36,    36,    37,    37,
      38,    39,    39,    39,    39,    39,    39,    39,    40,    41,
      42,    43,    44,    45,    46,    46,    47,    48,    48,    49,
      49,    50,    51,    52,    53,    53,    54
  };

  const unsigned char
  SIT_Parser::yyr2_[] =
  {
       0,     2,     2,     3,     2,     1,     1,     1,     1,     0,
       2,     3,     5,     4,     5,     8,     4,     7,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     5,     1,     1,     0,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const SIT_Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "UPPER", "LOWER", "t_WORD",
  "CHAR", "t_SEMICOLON", "t_CHAIN", "t_REGISTER", "t_LINKER",
  "t_ACCESS_INTERFACE", "t_SIB", "t_MIB", "t_1500_WRAPPER", "t_JTAG_TAP",
  "t_BASED_INTEGER", "t_START_HIERARCHY", "t_END_HIERARCHY",
  "t_TRANSPARENT", "t_HOLD_VALUE", "t_BYPASS", "t_BINARY_VECTOR",
  "t_QUOTED_STRING", "t_POST", "t_PRE", "t_HIGH", "t_LOW",
  "t_DecimalLiteral", "t_RightBracket", "t_LeftBracket", "t_Comma",
  "$accept", "root_node", "children_list", "node_list", "node",
  "is_transparent", "node_name", "internal_node", "JTAG_target",
  "path_selector", "ctrl_node", "IR_size", "n_DR_chains", "IR_TABLE",
  "IR_coding_list", "max_derivations", "position", "active", "leaf_node",
  "register_node", "size", "hold", "bypass", YY_NULLPTR
  };


  const unsigned short int
  SIT_Parser::yyrline_[] =
  {
       0,   138,   138,   146,   150,   155,   159,   160,   165,   167,
     170,   179,   192,   216,   241,   260,   277,   294,   329,   333,
     336,   341,   344,   347,   351,   353,   358,   361,   363,   367,
     369,   372,   377,   382,   386,   387,   391
  };

  // Print the state stack on the debug stream.
  void
  SIT_Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  SIT_Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  SIT_Parser::token_number_type
  SIT_Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
    };
    const unsigned int user_token_number_max_ = 286;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 5 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:1167
} // SIT
#line 2125 "/home/michele/mast-devel/SIT_parser/SIT_parser.tab.cc" // lalr1.cc:1167
#line 397 "/home/michele/mast-devel/SIT_parser/SIT_parser.yy" // lalr1.cc:1168



void 
SIT::SIT_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
