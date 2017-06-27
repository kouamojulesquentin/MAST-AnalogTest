//===========================================================================
//                           SubModelPlaceHolder.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SubModelPlaceHolder.hpp
//!
//! Declares SubModelPlaceHolder class
//!
//===========================================================================


#ifndef SUBMODELPLACEHOLDER_H__B7EE991_866D_4AB5_9B81_2FAE4D71730__INCLUDED_
  #define SUBMODELPLACEHOLDER_H__B7EE991_866D_4AB5_9B81_2FAE4D71730__INCLUDED_

#include "Mast_Core_export.hpp"

#include <string>
#include <memory>

namespace mast
{
class SystemModelNode;
class ParentNode;

//! Tells how to create a sub-model instance
//!
enum class PlaceHolderKind
{
  SIT,      //!< Instance must be build-up from SIT file
  Factory, //!< Instance must be build-up from a model factory (usually registered by a plugin)
};

//! This is a SystemModelNode used during SystemModel creation to stand for a sub-model
//!
//! @note Represented sub-model can be reduced to a single node or macro but generally
//!       this is a model build from a description file or by some factory.
//!
class MAST_CORE_EXPORT SubModelPlaceHolder final
{
  // ---------------- Public Methods
  //
  public:
  virtual ~SubModelPlaceHolder() = default;
  SubModelPlaceHolder()  = default;    // Must be default constructible to be used in vector

  //! Constructs a new SubModelPlaceHolder
  //!
  //! @param kind             Tells what kind of mecanism must be used to build the represented sub-model
  //! @param identifier       Identifies the model to construct, instanciate and splice instead of the SubModelPlaceHolder
  //! @param parent           Parent node in which represented sub-model must be spliced
  //! @param previousSibling  Node after which to splice the represented sub-model (nullptr when first child)
  //!
  SubModelPlaceHolder(PlaceHolderKind                  kind,
                      std::string                      identifier,
                      std::shared_ptr<ParentNode>      parent
//+                      std::shared_ptr<SystemModelNode> previousSibling = nullptr
                 )
    : m_kind            (kind)
    , m_identifier      (identifier)
    , m_parent          (parent)
//+    , m_previousSibling (previousSibling)
  {
  }

  //! Tells what kind of mecanism must be used to build the represented sub-model
  //!
  PlaceHolderKind Kind() const { return m_kind; }

  //! Identifies the model to construct, instanciate and splice instead of the SubModelPlaceHolder
  //!
  std::string Identififer() const { return m_identifier; }

  //! Parent node in which represented sub-model must be spliced
  //!
  std::shared_ptr<ParentNode> Parent() const { return m_parent; }

  //! Node after which to splice the represented sub-model (nullptr when first child)
  //!
//+  std::shared_ptr<SystemModelNode> PreviousSibling() const { return m_previousSibling; }


  // ---------------- Private Fields
  //
  private:
  PlaceHolderKind                  m_kind;            //!< Tells what kind of mecanism must be used to build the represented sub-model
  std::string                      m_identifier;      //!< Identifies the model to construct, instanciate and splice instead of the SubModelPlaceHolder
  std::shared_ptr<ParentNode>      m_parent;          //!< Parent node in which represented sub-model must be spliced
//+  std::shared_ptr<SystemModelNode> m_previousSibling; //!< Node after which to splice the represented sub-model (nullptr when first child)
};
//
//  End of SubModelPlaceHolder class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SUBMODELPLACEHOLDER_H__B7EE991_866D_4AB5_9B81_2FAE4D71730__INCLUDED_

//===========================================================================
// End of SubModelPlaceHolder.hpp
//===========================================================================
