//===========================================================================
//                           PathSelectorFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PathSelectorFactory.hpp
//!
//! Declares PathSelectorFactory class
//!
//===========================================================================

#ifndef PATHSELECTORFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_
  #define PATHSELECTORFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_

#include "PathSelector.hpp"
#include "Mast_Core_export.hpp"

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <experimental/string_view>

namespace mast
{
class Register;

//! Factory for creating PathSelector derived class instances
//!
class MAST_CORE_EXPORT PathSelectorFactory
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~PathSelectorFactory() = default;

  //! Returns sole instance
  //!
  static PathSelectorFactory& Instance();

  using Creator_Std_t        = std::function<std::unique_ptr<PathSelector>(uint32_t pathsCount, SelectorProperty   properties, std::shared_ptr<Register> associatedRegister)>; //!< Creators to create a "standard" path selector
  using Creator_Custom_t     = std::function<std::unique_ptr<PathSelector>(uint32_t pathsCount, const std::string& parameters, std::shared_ptr<Register> associatedRegister)>; //!< Creators to create "custom" path selector driven by a Register in model
  using Creator_FullCustom_t = std::function<std::unique_ptr<PathSelector>(uint32_t pathsCount, const std::string& parameters)>;                                               //!< Creators to create "custom" path selector NOT DRIVEN by a Register in model

  //! Returns the number of factories currently registered (associated with a name)
  //!
  //! @note Mainly intended for test/check purpose
  size_t RegisteredCreatorsCount() const;

  //! Register an instance creation method for "standard" path selector
  //!
  //! @note If a factory already exists with the same factory name, it is replaced with the new one
  //!
  //! @param creatorId  Name associated with the creation function (typically named after the actual type to create)
  //! @param creator    Function that can create an instance of type associated with the concrete factory
  //!
  void RegisterCreator(const std::string& creatorId, Creator_Std_t creator)
  {
    m_stdCreators[creatorId] = creator;
  }

  //! Register an instance creation method for "custom" path selector
  //!
  //! @note If a factory already exists with the same factory name, it is replaced with the new one
  //!
  //! @param creatorId  Name associated with the creation function (typically named after the actual type to create)
  //! @param creator    Function that can create an instance of type associated with the concrete factory
  //!
  void RegisterCreator(const std::string& creatorId, Creator_Custom_t creator)
  {
    m_customCreators[creatorId] = creator;
  }

  //! Register an instance creation method for "custom" path selector that DO NOT USE Register described by the SystemModel
  //!
  //! @note If a factory already exists with the same factory name, it is replaced with the new one
  //!
  //! @param creatorId  Name associated with the creation function (typically named after the actual type to create)
  //! @param creator    Function that can create an instance of type associated with the concrete factory
  //!
  void RegisterCreator(const std::string& creatorId, Creator_FullCustom_t creator)
  {
    m_fullCustomCreators[creatorId] = creator;
  }

  //! Removes any, registered, factory
  //!
  void Clear();


  //! Creates an PathSelector using registered creation function
  //!
  //! @param creatorId            A name that identified registered creation function
  //! @param pathsCount           Number of managed paths (including, optional, bypass register)
  //! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
  //! @param associatedRegister   Register that is used to drive the path multiplexer
  //!
  virtual std::unique_ptr<PathSelector> Create(const std::string& creatorId, uint32_t pathsCount, SelectorProperty properties, std::shared_ptr<Register> associatedRegister) const;

  //! Creates an PathSelector using registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param pathsCount   Number of managed paths (including, optional, bypass register)
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::unique_ptr<PathSelector> Create(const std::string& creatorId, uint32_t pathsCount, const std::string& parameters) const;

  //! Creates an PathSelector using registered creation function
  //!
  //! @param creatorId           A name that identified registered creation function
  //! @param pathsCount          Number of managed paths (including, optional, bypass register)
  //! @param parameters          String of (optional) parameters
  //! @param associatedRegister  Register that is used to drive the path multiplexer
  //!
  virtual std::unique_ptr<PathSelector> Create(const std::string& creatorId, uint32_t pathsCount, const std::string& parameters, std::shared_ptr<Register> associatedRegister) const;


  //! Fills up with default PathSelector
  //!
  //! @note Default PathSelector are:
  //!         - Binary
  //!         - Binary_noidle
  //!         - One_Hot
  //!         - One_Hot_noidle
  //!         - N_Hot
  //!         - N_Hot_noidle
  //!         - Table_Based  (custom table)
  //!
  virtual void InitializeWithDefaults();

  // ---------------- Private  Methods
  //
  private:
  PathSelectorFactory();

  static std::unique_ptr<PathSelectorFactory> CreateFactoryWithDefaultCreators();  //!< Called once to create single instance initialized with default creation function(s)

  //! Implements creation of actual instance.
  //!
  //! @note This is a default implementation provided for concrete factories to implement Create method
  //!
  //! @param creatorId    Creation method identifier
  //! @param parameters   Parameters to pass to creation method
  //!
  //!
  //! @return Created instance or nullptr when no creation method has been register with given name
  template<typename Factory_t, typename... Ts>
  static std::unique_ptr<PathSelector> CreateImpl(const Factory_t& factory, const std::string& creatorId, uint32_t pathsCount, Ts... args)
  {
    std::unique_ptr<PathSelector> instance;

    auto pos = factory.find(creatorId);
    if (pos != factory.end())
    {
      instance = pos->second(pathsCount, args...);
    }

    return instance;
  }

  // ---------------- Private  Fields
  //
  private:
  std::map<std::string, Creator_Std_t>        m_stdCreators;        //!< Creators function to create "standard" PathSelector
  std::map<std::string, Creator_Custom_t>     m_customCreators;     //!< Creators function to create "Custom" PathSelector
  std::map<std::string, Creator_FullCustom_t> m_fullCustomCreators; //!< Creators function to create "Full Custom" PathSelector
};
//
//  End of PathSelectorFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined PATHSELECTORFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_
//===========================================================================
// End of PathSelectorFactory.hpp
//===========================================================================
