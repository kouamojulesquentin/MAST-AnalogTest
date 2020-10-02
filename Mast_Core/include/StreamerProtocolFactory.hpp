//===========================================================================
//                           StreamerProtocolFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file StreamerProtocolFactory.hpp
//!
//! Declares StreamerProtocolFactory class
//!
//===========================================================================

#ifndef STREAMERPROTOCOLFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_
  #define STREAMERPROTOCOLFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_

#include "StreamerProtocol.hpp"
#include "Mast_Core_export.hpp"
#include "SystemModel.hpp"

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <experimental/string_view>
#include <vector>

namespace mast
{
class Register;

//! Factory for creating StreamerProtocol derived class instances
//!
class MAST_CORE_EXPORT StreamerProtocolFactory
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~StreamerProtocolFactory() = default;

  //! Returns sole instance
  //!
  static StreamerProtocolFactory& Instance();

  using Creator_Std_t        = std::function<std::unique_ptr<StreamerProtocol>(const std::string& parameters)>; //!< Creators to create a "standard" Streamer Protocol 

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

  //! Removes any, registered, factory
  //!
  void Clear();


  //! Creates an StreamerProtocol using registered creation function
  //!
  //! @param creatorId            A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::unique_ptr<StreamerProtocol> Create(const std::string& creatorId,const std::string& parameters ) const;


  //! Fills up with default StreamerProtocol
  //!
  //! @note Default StreamerProtocol are:
  //!         - TransparentStreamer
  //!
  virtual void InitializeWithDefaults();

  // ---------------- Private  Methods
  //
  private:
  StreamerProtocolFactory();

  static std::unique_ptr<StreamerProtocolFactory> CreateFactoryWithDefaultCreators();  //!< Called once to create single instance initialized with default creation function(s)

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
  static std::unique_ptr<StreamerProtocol> CreateImpl(const Factory_t& factory, const std::string& creatorId, const std::string& parameters, Ts... args)
  {
    std::unique_ptr<StreamerProtocol> instance;

    auto pos = factory.find(creatorId);
    if (pos != factory.end())
    {
      instance = pos->second(parameters, args...);
    }

    return instance;
  }

  // ---------------- Private  Fields
  //
  private:
  std::map<std::string, Creator_Std_t>        m_stdCreators;        //!< Creators function to create "standard" StreamerProtocol
};
//
//  End of StreamerProtocolFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined STREAMERPROTOCOLFACTORY_H__58D08908_4AFF_41C6_828A_47267EDB6719__INCLUDED_
//===========================================================================
// End of StreamerProtocolFactory.hpp
//===========================================================================
