//===========================================================================
//                           Factory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Factory.hpp
//!
//! Declares Factory class
//!
//===========================================================================


#ifndef FACTORY_H__D3D0FAFB_AFA1_4AFE_F3AA_3AA69ECD884__INCLUDED_
  #define FACTORY_H__D3D0FAFB_AFA1_4AFE_F3AA_3AA69ECD884__INCLUDED_

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Defines common implementation for factories using register creation methods
//!
//! @note Creation methods must have been registered before calling Create (otherwise a nullptr is return)
//!       Concrete class are requested to implement InitializeWithDefaults (the common way to initialize default
//!       creation methods)
//!
template<typename BuildType>
class Factory
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Factory() = default;
  Factory()  = default;

  using Creator_t = std::function<std::shared_ptr<BuildType>(const std::string& parameters)>;

  //! Fills up with default creation methods
  //! @note Default creation methods a those that must not be registered explicitly
  //!       but provided by the concrete factory
  //!
  virtual void InitializeWithDefaults() = 0;

  //! Returns the number of factories currently registered (associated with a name)
  //!
  //! @note Mainly intended for test/check purpose
  size_t RegisteredCreatorsCount() const { return m_creators.size(); }

  //! Register an instance creation method
  //!
  //! @note If a factory already exists with the same factory name, it is replaced with the new one
  //!
  //! @param creatorId  Name associated with the creation function (typically named after the actual type to create)
  //! @param creator    Function that can create an instance of type associated with the concrete factory
  //!
  void RegisterCreator(const std::string& creatorId, Creator_t creator) { m_creators[creatorId] = creator; }

  //! Removes any, registered, factory
  //!
  void Clear() { m_creators.clear(); }


  //! Creates a BuildType instance using registered creation method and optional parameters
  //!
  //! @param creatorId    A name that identifies registered creation method
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::shared_ptr<BuildType> Create(const std::string& creatorId, const std::string& parameters = "") const = 0;

  // ---------------- Protected Methods
  //
  protected:

  //! Implements creation of actual instance.
  //!
  //! @note This is a default implementation provided for concrete factories to implement Create method
  //!
  //! @param creatorId    Creation method identifier
  //! @param parameters   Parameters to pass to creation method
  //!
  //!
  //! @return Created instance or nullptr when no creation method has been register with given name
  std::shared_ptr<BuildType> CreateImpl(const std::string& creatorId, const std::string& parameters = "") const
  {
    std::shared_ptr<BuildType> instance;

    auto pos = m_creators.find(creatorId);
    if (pos != m_creators.end())
    {
      instance = pos->second(parameters);
    }

    return instance;
  }

  // ---------------- Private  Fields
  //
  private:
  std::map<std::string, Creator_t>  m_creators;
};
//
//  End of Factory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined FACTORY_H__D3D0FAFB_AFA1_4AFE_F3AA_3AA69ECD884__INCLUDED_

//===========================================================================
// End of Factory.hpp
//===========================================================================
