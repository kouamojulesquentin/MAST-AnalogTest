//===========================================================================
//                           SystemModelFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelFactory.hpp
//!
//! Declares SystemModelFactory class
//!
//===========================================================================


#ifndef SYSTEMMODELFACTORY_H__912C33CC_F11E_4374_F094_51C175F67E76__INCLUDED_
  #define SYSTEMMODELFACTORY_H__912C33CC_F11E_4374_F094_51C175F67E76__INCLUDED_

#include "Factory.hpp"

namespace mast
{
class ParentNode;
class SystemModel;

//! Factory for creating MAST sub-model
//!
class MAST_CORE_EXPORT SystemModelFactory final : public Factory<ParentNode,
                                                                std::shared_ptr<ParentNode>,
                                                                std::function<std::shared_ptr<ParentNode>(SystemModel&, const std::string& parameters)>>
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelFactory() = default;

  //! Returns sole instance
  //!
  static SystemModelFactory& Instance();

  //! Creates a MAST sub-model registered creation function
  //!
  //! @param systemModel  System model that manages creation of SystemModelNode
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  std::shared_ptr<ParentNode> Create(const std::string& creatorId, SystemModel& systemModel, const std::string& parameters = "") const;


  //! Creates a MAST sub-model registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  std::shared_ptr<ParentNode> Create(const std::string& creatorId, const std::string& parameters = "") const override;

  //! Fills up with default SystemModel creator
  //!
  //! @note There is no default creator yet
  //!
  virtual void InitializeWithDefaults() override;

  // ---------------- Private  Methods
  //
  private:
  SystemModelFactory() = default;

  // ---------------- Private  Fields
  //
  private:
  static std::unique_ptr<SystemModelFactory> CreateFactory();  //!< Called once to create single instance initialized with default creation function(s)
};
//
//  End of SystemModelFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SYSTEMMODELFACTORY_H__912C33CC_F11E_4374_F094_51C175F67E76__INCLUDED_

//===========================================================================
// End of SystemModelFactory.hpp
//===========================================================================
