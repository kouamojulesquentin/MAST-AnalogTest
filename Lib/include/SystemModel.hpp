//===========================================================================
//                           SystemModel.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModel.hpp
//!
//! Declares class SystemModel
//!
//===========================================================================

#ifndef SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_
  #define SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_

#include "SystemModelNodes.hpp"

namespace mast
{
//! Manages the system model tree
//!
//!
class SystemModel
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModel() = default;
  SystemModel()  = default;


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  uint32_t         m_totalRegister = 0;       //!< Total number of register in the model
  AccessInterface* m_root          = nullptr; //!< First (top) node of system model tree
};
//
//  End of SystemModel class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_

//===========================================================================
// End of SystemModel.hpp
//===========================================================================
