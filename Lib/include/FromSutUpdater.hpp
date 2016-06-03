//===========================================================================
//                           FromSutUpdater.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FromSutUpdater.hpp
//!
//! Declares FromSutUpdater class
//!
//===========================================================================

#ifndef FROMSUTUPDATER_H__DB106156_53FA_4185_BBB_EE8E7A3FBE72__INCLUDED_
  #define FROMSUTUPDATER_H__DB106156_53FA_4185_BBB_EE8E7A3FBE72__INCLUDED_

#include "SystemModel.hpp"
#include "Platform.hpp"

#include <vector>

namespace mast
{
class BinaryVector;

//! Helper to update system model from data received from SUT
//!
class DLL_EXPORT FromSutUpdater final
{
  // ---------------- Public  Methods
  //
  public:
  ~FromSutUpdater() = default;
  FromSutUpdater()  = delete;
  FromSutUpdater(SystemModel& systemModel) : m_sm(systemModel) {}

  using NodeIdentifier = SystemModelNode::NodeIdentifier;

  //! Updates system model registers from vector shifted from SUT and ordered list active registers at the time of JTAG shift
  //!
  void UpdateRegisters(const std::vector<NodeIdentifier>& activeRegisters, const BinaryVector& fromSutVector);

  // ---------------- Private  Fields
  //
  SystemModel m_sm; //!< System model (to get nodes from identifier)
};
//
//  End of FromSutUpdater class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined FROMSUTUPDATER_H__DB106156_53FA_4185_BBB_EE8E7A3FBE72__INCLUDED_

//===========================================================================
// End of FromSutUpdater.hpp
//===========================================================================
