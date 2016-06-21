//===========================================================================
//                           SystemModelManager.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManager.hpp
//!
//! Declares SystemModelManager class
//!
//===========================================================================


#ifndef SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_
  #define SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

#include "Platform.hpp"
#include "ConfigureVisitor.hpp"
#include "PropagatePendingVisitor.hpp"
#include "ToSutVisitor.hpp"
#include "FromSutUpdater.hpp"

#include <memory>

namespace mast
{

class AccessInterface;
class SystemModel;

//! Coordinates the dynamic aspects of the system model
//!
//! @note The main task is to trigger configuration of the system model,
//!       collect the bitstream to sent to SUT, retrieve the bitstream
//!       from SUT and updating the system model with it
//!
class DLL_EXPORT SystemModelManager final
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelManager() = default;
  SystemModelManager()  = delete;

  //! Associates a SystemModel to fresh SystemModelManager
  //!
  SystemModelManager(SystemModel& sm,
                     std::shared_ptr<ConfigurationAlgorithm> configurationAlgorithm = std::make_shared<ConfigureAlgorithm_LastOrDefault>()
                    )
    : m_sm                     (sm)
    , m_firstAccessInterface   (GetFirstAccessInterface(sm))
    , m_configurator           (configurationAlgorithm)
    , m_propagator             ()
    , m_toSutVisitor           ()
    , m_fromSutUpdater         (sm)
  {  }

  //! Does a complete data cycles for SystemModel as long as there are pending nodes
  //!
  //! @note It encompasses configuration, bitstream for SUT collection, bitstream fro SUT
  //!       retrieval and SystemModel updating
  //!
  void DoDataCycles();

  // ---------------- Protected Methods
  //
  protected:

  static std::shared_ptr<AccessInterface> GetFirstAccessInterface(const SystemModel& sm);

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  SystemModel&                     m_sm;                   //!< The system model to manage
  std::shared_ptr<AccessInterface> m_firstAccessInterface; //!< The first AccessInterface of the system
  ConfigureVisitor                 m_configurator;         //!< In charge of configuration
  PropagatePendingVisitor          m_propagator;           //!< In charge of propagating pending status bottom up
  ToSutVisitor                     m_toSutVisitor;         //!< In charge of collecting bitstream to SUT
  FromSutUpdater                   m_fromSutUpdater;       //!< In charge of updating SystemModel from bitstream from SUT
};
//
//  End of SystemModelManager class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGER_H__F0470D3B_5E63_4DAF_B2A8_60A2E4F4A868__INCLUDED_

//===========================================================================
// End of SystemModelManager.hpp
//===========================================================================
