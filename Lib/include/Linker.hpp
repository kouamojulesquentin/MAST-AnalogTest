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
#include <memory>
#include <experimental/string_view>

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

class PathSelector;
class BinaryVector;

//! Represents dynamically selectable chains of registers
//! It models dynamic topologies, like for instance the ScanMux/SIB of 1687.
//! Like the Chain node it has some derivations stored as child, but they are not always available (i.e. "active") for
//! data access.
//!
//! Based on its status, the LINKER can determine if each of its derivations is "active", and is therefore part of the
//! active scan path.
//!
class DLL_EXPORT Linker : public ParentNode
{
  public:

  // ---------------- Public  Methods
  //
  public:
  ~Linker() = default;
  Linker()  = delete;
  Linker(std::experimental::string_view name, std::shared_ptr<PathSelector> pathSelector);

  bool IsActive (uint32_t pathIdentifier) const ; //!< Returns true when the specified path is already selected
  void Deselect (uint32_t pathIdentifier);        //!< Requests deactivation of the specified path
  void Select   (uint32_t pathIdentifier);        //!< Requests activation of the specified path


  //+ (JFC April/19/2016): Consider how to manage multiple kind of selectors:
  //+ 1 - A selector instance that wrapped the knowledge of how to select/deselect and check of isActive
  //+ 2 - A specialization of a Register with the knowlege of -1-
  //+ 3 - An aggregate of a Register and knowlege of -1-
  //+ Take note that the selection/deselection depends on encoding and possilbly support for multiple selections
  //+ The bit size of the MIBS must be defined (a byte seems to be not much at all): I propose unsigned long but solution 2 and 3
  //+ above allow for better tuning
  //+ Probably most selector/deselector LUT can be shared

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "Linker"; } //!< Returns readable type name

  std::shared_ptr<PathSelector> Selector() const { return m_pathSelector; }

     //+ (JFC April/20/2016): Move to linkerInfo
//+  virtual BinaryVector GetLastSequence() const;  //!< Returns last sequence shifted from sut
//+  virtual BinaryVector GetNextSequence() const;  //!< Returns sequence to shift into sut

  // ---------------- Protected Methods
  //
  protected:
  void CheckPathIdentifier(uint32_t pathIdentifier) const;

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  uint32_t                      m_pathsCount = 0; //!< Maximum number of derivations that can be appended to the node
  std::shared_ptr<PathSelector> m_pathSelector;   //!< Provides genericity of how to manage path(s) selection
};
//
//  End of Linker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined LINKER_H__4E199FE8_B1BC_4778_4A89_2A5817AEECE__INCLUDED_

//===========================================================================
// End of Linker.hpp
//===========================================================================
