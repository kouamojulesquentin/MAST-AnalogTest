//===========================================================================
//                           RemoteProtocolFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RemoteProtocolFactory.hpp
//!
//! Declares RemoteProtocolFactory class
//!
//===========================================================================


#ifndef REMOTEPROTOCOLFACTORY_H__A8FAF65F_1DFE_412B_18B5_E6D364E1E0ED__INCLUDED_
  #define REMOTEPROTOCOLFACTORY_H__A8FAF65F_1DFE_412B_18B5_E6D364E1E0ED__INCLUDED_


#include "Factory.hpp"
#include "Remote_Protocol_Client.hpp"

namespace mast
{
//! Factory for creating Remote_Protocol_Client derived instance
//!
//!
class DLL_EXPORT RemoteProtocolFactory final : public Factory<Remote_Protocol_Client>
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~RemoteProtocolFactory() = default;

  //! Returns sole instance
  //!
  static RemoteProtocolFactory& Instance();

  //! Creates an Remote_Protocol_Client using registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::shared_ptr<Remote_Protocol_Client> Create(const std::string& creatorId, const std::string& parameters = "") const override;

  //! Fills up with default Remote_Protocol_Client
  //!
  //! @note Default Remote_Protocol_Clientare:
  //!         -
  //!
  virtual void InitializeWithDefaults() override;

  // ---------------- Private  Methods
  //
  private:
  RemoteProtocolFactory()  = default;

  // ---------------- Private  Fields
  //
  private:
  static std::unique_ptr<RemoteProtocolFactory> CreateFactoryWithDefaultCreators();  //!< Called once to create single instance initialized with default creation function(s)
};
//
//  End of RemoteProtocolFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined REMOTEPROTOCOLFACTORY_H__A8FAF65F_1DFE_412B_18B5_E6D364E1E0ED__INCLUDED_

//===========================================================================
// End of RemoteProtocolFactory.hpp
//===========================================================================
