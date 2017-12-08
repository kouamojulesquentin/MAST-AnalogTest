//===========================================================================
//                           AccessInterfaceTranslatorProtocolFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceTranslatorProtocolFactory.hpp
//!
//! Declares AccessInterfaceTranslatorProtocolFactory class
//!
//===========================================================================


#ifndef AccessInterfaceTranslatorProtocolFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_
  #define AccessInterfaceTranslatorProtocolFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_

#include "Mast_Core_export.hpp"
#include "Factory.hpp"



namespace mast
{
class AccessInterfaceTranslatorProtocol;

//! Provides facilities to register factories for AccessInterfaceTranslatorProtocol creation.
//!
//! @note Factories are identified by a name and create instance of AccessInterfaceTranslatorProtocol from string parameter
//!       Each factory must parse the string parameter to devise parameters for creating their proper AccessInterfaceTranslatorProtocol
//!
//! @note This is a singleton
//! @note Creation of the singleton initializes it with factories for default AccessInterfaceTranslatorProtocol
//!
class MAST_CORE_EXPORT AccessInterfaceTranslatorProtocolFactory final : public Factory<AccessInterfaceTranslatorProtocol>
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceTranslatorProtocolFactory() = default;

  //! Returns sole instance
  //!
  static AccessInterfaceTranslatorProtocolFactory& Instance();

  //! Creates an AccessInterfaceTranslatorProtocol using registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::unique_ptr<AccessInterfaceTranslatorProtocol> Create(const std::string& creatorId, const std::string& parameters = "") const override;

  //! Fills up with default AccessInterfaceTranslatorProtocol
  //!
  //! @note Default AccessInterfaceTranslatorProtocol are:
  //!         - LoopbackAccessInterfaceTranslatorProtocol
  //!         - SVF_SimulationProtocol
  //!         - SVF_EmulationProtocol
  //!         - STIL_Emulation
  //!         - OfflineProtocol
  //!         - I2C_EmulationProtocol
  //!         - RemoteProxy
  //!
  virtual void InitializeWithDefaults() override;


  // ---------------- Protected Methods
  //
  protected:
  AccessInterfaceTranslatorProtocolFactory() = default;

  // ---------------- Private  Methods
  //
  private:
  static std::unique_ptr<AccessInterfaceTranslatorProtocolFactory> CreateInstanceWithDefaultFactories();  //!< Called once to create single instance initialized with default factories
};
//
//  End of AccessInterfaceTranslatorProtocolFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined AccessInterfaceTranslatorProtocolFACTORY_H__B1F211CA_FF00_49A4_1FA6_E83DEC699EC7__INCLUDED_

//===========================================================================
// End of AccessInterfaceTranslatorProtocolFactory.hpp
//===========================================================================
