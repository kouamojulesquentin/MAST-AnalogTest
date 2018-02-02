//===========================================================================
//                           AST_PlaceHolder.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_PlaceHolder.hpp
//!
//! Declares AST_PlaceHolder class
//!
//===========================================================================

#ifndef AST_PLACEHOLDER_H__4B66B7F_A88B_4535_C0BF_F94A8D53CBDE__INCLUDED_
  #define AST_PLACEHOLDER_H__4B66B7F_A88B_4535_C0BF_F94A8D53CBDE__INCLUDED_

#include "AST_Node.hpp"

namespace Parsers
{
//! Represents a typed AST_Node to assign to some parent child which kind is different from actual node kind.
//! e.g. For a reset value of a scan register that is AST_ConcatNumber can be wrapped into an AST_PlaceHolder
//!      (that has kind "PlaceHolder") indicating it is intended to be set as scan register reset value by using
//!      target kind "ResetValue"
//!
//! @note Base class is to be able to get target kind without knowing yet wrapped node actual type
class AST_PlaceHolderBase : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_PlaceHolderBase() = default;
  AST_PlaceHolderBase()  = delete;

  //! Returns target kind
  //!
  Kind TargetKind() const { return m_targetKind; }

  // ---------------- Protected Methods
  //
  //! Initializes with target kind
  //!
  explicit AST_PlaceHolderBase(Kind targetKind)
    : AST_Node      (Kind::PlaceHolder)
    , m_targetKind  (targetKind)
  {
  }

  // ---------------- Private Fields
  //
  private:
  Kind m_targetKind = Kind::Undefined; //!< Kind of "target" that will be assigned the wrapped node
};

template<typename T>
class AST_PlaceHolder final : public AST_PlaceHolderBase
{
  // ---------------- Public Methods
  //
  public:
  ~AST_PlaceHolder() = default;
  AST_PlaceHolder()  = delete;

  //! @todo [JFC]-[February/02/2018]: Add support for freeing memory once AST_PlaceHolder is no more used (wrapped node retrieved)
  //!

  //! Returns node to assign to specified target kind
  //!
  T* WrappedNode() const { return m_wrappedNode; }

  void SetTargetKindToWrappedNode() { m_wrappedNode->ChangeKind(TargetKind()); }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_PlaceHolder<T>)(Parsers::Kind&, T*&);

  //! Initializes AST_PlaceHolder with effective node
  //!
  AST_PlaceHolder(Kind targetKind, T* wrappedNode)
    : AST_PlaceHolderBase (targetKind)
    , m_wrappedNode       (wrappedNode)
  {
  }

  // ---------------- Private Fields
  //
  private:
  T* m_wrappedNode = nullptr; //!< Actual (wrapped) node
};
//
//  End of AST_PlaceHolder class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_PLACEHOLDER_H__4B66B7F_A88B_4535_C0BF_F94A8D53CBDE__INCLUDED_
//===========================================================================
// End of AST_PlaceHolder.hpp
//===========================================================================
