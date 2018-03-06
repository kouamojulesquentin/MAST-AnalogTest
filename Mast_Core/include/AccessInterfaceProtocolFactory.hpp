//===========================================================================
//                           AccessInterfaceProtocolFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocolFactory.hpp
//!
//! Declares AccessInterfaceProtocolFactory class
//!
//===========================================================================


#ifndef ACCESSINTERFACEPROTOCOLFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_
  #define ACCESSINTERFACEPROTOCOLFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_

#include "Mast_Core_export.hpp"
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
class MAST_CORE_EXPORT AccessInterfaceProtocolFactory final : public Factory<AccessInterfaceProtocol>
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceProtocolFactory() = default;

  //! Returns sole instance
  //!
  static AccessInterfaceProtocolFactory& Instance();

  //! Creates an AccessInterfaceProtocol using registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::unique_ptr<AccessInterfaceProtocol> Create(const std::string& creatorId, const std::string& parameters = "") const override;

  //! Fills up with default AccessInterfaceProtocol
  //!
  //! @note Default AccessInterfaceProtocol are:
  //!         - Emulation
  //!         - JTAG_to_I2C
  //!         - Spy
  //!
  virtual void InitializeWithDefaults() override;



  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceProtocolFactory() = default;

  // ---------------- Private  Methods
  //
  private:
  static std::unique_ptr<AccessInterfaceProtocolFactory> CreateInstanceWithDefaultFactories();  //!< Called once to create single instance initialized with default factories
};
//
//  End of AccessInterfaceProtocolFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACEPROTOCOLFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocolFactory.hpp
//===========================================================================
