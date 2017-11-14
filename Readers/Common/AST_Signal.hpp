//===========================================================================
//                           AST_Signal.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Signal.hpp
//!
//! Declares AST_Signal class
//!
//===========================================================================

#ifndef AST_SIGNAL_H__C83B6175_C11F_4E05_B4A2_722C4034A2A__INCLUDED_
  #define AST_SIGNAL_H__C83B6175_C11F_4E05_B4A2_722C4034A2A__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <vector>
#include <string>

namespace Parsers
{
class AST_ScalarIdentifier;
class AST_Identifier;

//! Represents a signal, i.e. a scoped port name or a number
//!
class AST_Signal final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Signal() = default;
  AST_Signal()  = delete;

  //! Text representation of signal
  //!
  std::string AsText() const override;

  //! Returns true when signal is a number
  //!
  bool  IsNumber() const { return !m_number.empty(); }

  //! Returns port path (without the name)
  //!
  //! @note Valid only when not a number
  //!
  const std::vector<AST_ScalarIdentifier*>& PortScope() const { return m_path; }

  //! Returns port name (without its path)
  //!
  //! @note Valid only when not a number
  //!
  AST_Identifier* PortName() const { return m_portName; }

  //! Tells whether signal bit(s) is/are complemented
  //!
  bool  IsInverted() const { return m_inverted; }

  //! Sets whether signal bit(s) is/are complemented
  //!
  void  IsInverted (bool isInverted) { m_inverted = isInverted; }

  // ---------------- Private Methods
  //
  private:

  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Signal)(std::experimental::string_view&);
  MAKE_UNIQUE_AS_FRIEND(AST_Signal)(Parsers::AST_Identifier*&);
  MAKE_UNIQUE_AS_FRIEND(AST_Signal)(std::vector<Parsers::AST_ScalarIdentifier*>&&,
                                    Parsers::AST_Identifier*&);

  //! Constructs from sole number
  //!
  AST_Signal(std::experimental::string_view number)
    : AST_SimpleNode (Kind::Signal)
    , m_number       (number)
  {
  }

  //! Constructs from only port name
  //!
  AST_Signal(AST_Identifier* portName)
    : AST_SimpleNode (Kind::Signal)
    , m_portName     (portName)
  {
  }

  //! Constructs from path and port name
  //!
  AST_Signal(std::vector<AST_ScalarIdentifier*>&& path, AST_Identifier* portName)
    : AST_SimpleNode (Kind::Signal)
    , m_path         (std::move(path))
    , m_portName     (portName)
  {
  }


  // ---------------- Private Fields
  //
  private:
  std::string                        m_number;             //!< Non empty when signal is a number
  std::vector<AST_ScalarIdentifier*> m_path;               //!< Port path (dot separated instance names)
  AST_Identifier*                    m_portName = nullptr; //!< Port name (when not a number)
  bool                               m_inverted = false;   //!< When true, each bit in the binary representation of the number or port is complemented.
};
//
//  End of AST_Signal class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SIGNAL_H__C83B6175_C11F_4E05_B4A2_722C4034A2A__INCLUDED_

//===========================================================================
// End of AST_Signal.hpp
//===========================================================================
