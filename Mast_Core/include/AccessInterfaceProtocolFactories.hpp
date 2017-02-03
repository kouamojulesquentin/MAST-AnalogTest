//===========================================================================
//                           AccessInterfaceProtocolFactories.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocolFactories.hpp
//!
//! Declares AccessInterfaceProtocolFactories class
//!
//===========================================================================


#ifndef ACCESSINTERFACEPROTOCOLFACTORIES_H__1A3C4FD_59BD_4F04_FA8E_1434A12434AD__INCLUDED_
  #define ACCESSINTERFACEPROTOCOLFACTORIES_H__1A3C4FD_59BD_4F04_FA8E_1434A12434AD__INCLUDED_

#include <string>
#include <experimental/string_view>
#include <functional>
#include <memory>
#include <map>

#include "Platform.hpp"

namespace mast
{
class AccessInterfaceProtocol;

//! Provides facilities to register factories for AccessInterfaceProtocol creation.
//!
//! @note Factories are identified by a name and create instance of AccessInterfaceProtocol from string parameter
//!       Each factory must parse the string parameter to devise parameters for creating their proper AccessInterfaceProtocol
//!
//! @note This is a singleton
//! @note Creation of the singleton initializes it with factories for default AccessInterfaceProtocol
//!
class DLL_EXPORT AccessInterfaceProtocolFactories final
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceProtocolFactories() = default;

  //! Returns sole instance
  //!
  static AccessInterfaceProtocolFactories& Instance();

  using Factory_t = std::function<std::shared_ptr<AccessInterfaceProtocol>(const std::string& parameters)>;

  //! Returns the number of factories currently registered (associated with a name)
  //!
  //! @note Mainly intended for test/check purpose
  size_t RegisteredFactoriesCount() const { return m_factories.size(); }


  //! Appends a factories
  //!
  //! @note If a factory already exists with the same factory name, it is replaced with the new one
  //!
  //! @param factoryName  Name associated with the factory (typically named after the AccessInterfaceProtocol to create)
  //! @param factory      An AccessInterfaceProtocol factory
  //!
  void AddFactory(const std::string& factoryName, Factory_t factory) { m_factories[factoryName] = factory; }


  //! Removes any factories
  //!
  void Clear() { m_factories.clear(); }

  //! Creates an AccessInterfaceProtocol using factory indentified by a name and optional parameters
  //!
  //! @param factoryName  A name that identified registered factory (may be name of default)
  //! @param parameters   String of (optional) parameters
  //!
  std::shared_ptr<AccessInterfaceProtocol> CreateProtocol(const std::string& factoryName, const std::string& parameters = "") const;

  //! Fills up with default AccessInterfaceProtocol
  //!
  //! @note Default AccessInterfaceProtocol are:
  //!         - LoopbackAccessInterfaceProtocol
  //!         - SVF_SimulationProtocol
  //!         - SVF_EmulationProtocol
  //!         - STIL_Emulation
  //!         - OfflineProtocol
  //!         - I2C_EmulationProtocol
  //!
  void InitializeWithDefaults();


  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceProtocolFactories() = default;

  // ---------------- Private  Methods
  //
  private:
  static std::unique_ptr<AccessInterfaceProtocolFactories> CreateInstanceWithDefaultFactories();  //!< Called once to create single instance initialized with default factories

  // ---------------- Private  Fields
  //
  private:
  std::map<std::string, Factory_t>  m_factories;
};
//
//  End of AccessInterfaceProtocolFactories class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined ACCESSINTERFACEPROTOCOLFACTORIES_H__1A3C4FD_59BD_4F04_FA8E_1434A12434AD__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocolFactories.hpp
//===========================================================================
