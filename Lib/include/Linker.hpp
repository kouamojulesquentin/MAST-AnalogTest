//===========================================================================
//                           Linker.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Linker.hpp
//!
//! Declares class Linker
//!
//===========================================================================

#ifndef LINKER_H__4E199FE8_B1BC_4778_4A89_2A5817AEECE__INCLUDED_
  #define LINKER_H__4E199FE8_B1BC_4778_4A89_2A5817AEECE__INCLUDED_

#include "Chain.hpp"
namespace mast
{

//! Defines encoding to use to select chains managed by a linker node
//!
enum class MibEncoding
{
  One_Hot,
  N_Hot,
  Binary,
  Binary_No_Idle,   //!< Binary encoding ensuring that at least one chain is selected
};

//! Represents dynamically selectable chains of registers
//!
class Linker : Chain
{
  // ---------------- Public  Methods
  //
  public:
  ~Linker() = default;
  Linker()  = default;

  bool IsActive (uint32_t pathIdentifier);  //!< Returns true when the specified path is already selected
  void Deselect (uint32_t pathIdentifier);  //!< Request desactivation of the specified path
  void Select   (uint32_t pathIdentifier);  //!< Request activation of the specified path


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
};
//
//  End of Linker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined LINKER_H__4E199FE8_B1BC_4778_4A89_2A5817AEECE__INCLUDED_

//===========================================================================
// End of Linker.hpp
//===========================================================================
