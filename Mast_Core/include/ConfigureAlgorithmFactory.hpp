//===========================================================================
//                           ConfigureAlgorithmFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithmFactory.hpp
//!
//! Declares ConfigureAlgorithmFactory class
//!
//===========================================================================


#ifndef CONFIGUREALGORITHMFACTORY_H__2393F0E2_4F1D_4DEE_9C9D_1DD746B5644B__INCLUDED_
  #define CONFIGUREALGORITHMFACTORY_H__2393F0E2_4F1D_4DEE_9C9D_1DD746B5644B__INCLUDED_
#include "Factory.hpp"
#include "ConfigurationAlgorithm.hpp"

namespace mast
{
//! Factory for creating ConfigurationAlgorithm derived class instances
//!
class MAST_CORE_EXPORT ConfigureAlgorithmFactory final : public Factory<ConfigurationAlgorithm>
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~ConfigureAlgorithmFactory() = default;

  //! Returns sole instance
  //!
  static ConfigureAlgorithmFactory& Instance();

  //! Creates an ConfigurationAlgorithm using registered creation function
  //!
  //! @param creatorId    A name that identified registered creation function
  //! @param parameters   String of (optional) parameters
  //!
  virtual std::unique_ptr<ConfigurationAlgorithm> Create(const std::string& creatorId, const std::string& parameters = "") const override;

  //! Fills up with default ConfigurationAlgorithm
  //!
  //! @note Default ConfigurationAlgorithm are:
  //!         - LastOrDefault
  //!         - LastOrDefault_Greedy
  //!         - Last_Lazy
  //!
  virtual void InitializeWithDefaults() override;

  // ---------------- Private  Methods
  //
  private:
  ConfigureAlgorithmFactory()  = default;

  // ---------------- Private  Fields
  //
  private:
  static std::unique_ptr<ConfigureAlgorithmFactory> CreateFactoryWithDefaultCreators();  //!< Called once to create single instance initialized with default creation function(s)
};
//
//  End of ConfigureAlgorithmFactory class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined CONFIGUREALGORITHMFACTORY_H__2393F0E2_4F1D_4DEE_9C9D_1DD746B5644B__INCLUDED_

//===========================================================================
// End of ConfigureAlgorithmFactory.hpp
//===========================================================================
