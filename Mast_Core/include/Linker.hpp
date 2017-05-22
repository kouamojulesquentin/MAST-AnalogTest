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
#include "PathSelector.hpp"
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

class BinaryVector;

//! Represents dynamically selectable chains of registers
//! It models dynamic topologies, like for instance the ScanMux/SIB of 1687.
//! Like the Chain node it has some derivations stored as child, but they are not always available (i.e. "active") for
//! data access.
//!
//! Based on its status, the LINKER can determine if each of its derivations is "active", and is therefore part of the
//! active scan path.
//!
class MAST_CORE_EXPORT Linker : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Linker() = default;
  Linker()  = delete;
  Linker(std::experimental::string_view name, std::shared_ptr<PathSelector> pathSelector);

  bool IsActive            (uint32_t pathIdentifier) const ; //!< Returns true when the specified path is already selected
  bool IsSelected          (uint32_t pathIdentifier) const ; //!< Returns true when the specified path is currently pending to be selected
  bool IsSelectedAndActive (uint32_t pathIdentifier) const;  //!< Returns true when the specified path is selected and active
  void Deselect            (uint32_t pathIdentifier);        //!< Requests deactivation of the specified path
  void Select              (uint32_t pathIdentifier);        //!< Requests activation of the specified path

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "Linker"; } //!< Returns readable type name

  uint32_t SelectablePaths() const { return m_pathSelector->SelectablePaths(); };   //!< Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)

  std::shared_ptr<PathSelector> Selector() const { return m_pathSelector; }

  void ReplacePathSelector  (const std::shared_ptr<PathSelector>& new_pathSelector) {m_pathSelector=new_pathSelector;};   //!< Replaces current path selector

  // ---------------- Protected Methods
  //
  protected:
  void CheckPathIdentifier(uint32_t pathIdentifier) const;

  // ---------------- Private  Fields
  //
  private:
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
