//===========================================================================
//                           ConfigureVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureVisitor.hpp
//!
//! Declares class ConfigureVisitor
//!
//===========================================================================


#ifndef CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_
  #define CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

#include "SystemModelVisitor.hpp"
#include "ConfigureAlgorithm_LastOrDefault.hpp"

#include <memory>

namespace mast
{
//! System model tree visitor for configuration
//!
//!
class MAST_CORE_EXPORT ConfigureVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~ConfigureVisitor() = default;
  ConfigureVisitor(std::shared_ptr<ConfigurationAlgorithm> configurationAlgorithm = std::make_shared<ConfigureAlgorithm_LastOrDefault>())
  : m_configurationAlgorithm (configurationAlgorithm)
  {
  }

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitBlackBox        (BlackBox&        bbox)             override;

  // ---------------- Protected Methods
  //
  protected:
  virtual uint32_t ConfigureChildren (const ParentNode& parentNode);

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<ConfigurationAlgorithm>  m_configurationAlgorithm;
};
//
//  End of ConfigureVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CONFIGUREVISITOR_H__9C86FE0D_78AA_4852_8CA8_25CA6E34AC9__INCLUDED_

//===========================================================================
// End of ConfigureVisitor.hpp
//===========================================================================
