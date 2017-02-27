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

#include "Platform.hpp"
#include "Factory.hpp"



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
class DLL_EXPORT AccessInterfaceProtocolFactories final : public Factory<AccessInterfaceProtocol>
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceProtocolFactories() = default;

  //! Returns sole instance
  //!
  static AccessInterfaceProtocolFactories& Instance();

  using Creator_t = Factory<AccessInterfaceProtocol>::Creator_t;

  //! Creates an AccessInterfaceProtocol using factory indentified by a name and optional parameters
  //!
  //! @param creatorId  A name that identified registered factory (may be name of default)
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::shared_ptr<AccessInterfaceProtocol> Create(const std::string& creatorId, const std::string& parameters = "") const override;

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
  virtual void InitializeWithDefaults() override;


  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceProtocolFactories() = default;

  // ---------------- Private  Methods
  //
  private:
  static std::unique_ptr<AccessInterfaceProtocolFactories> CreateInstanceWithDefaultFactories();  //!< Called once to create single instance initialized with default factories
};
//
//  End of AccessInterfaceProtocolFactories class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined ACCESSINTERFACEPROTOCOLFACTORIES_H__1A3C4FD_59BD_4F04_FA8E_1434A12434AD__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocolFactories.hpp
//===========================================================================
