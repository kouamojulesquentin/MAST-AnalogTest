//===========================================================================
//                           MismatchesCollector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MismatchesCollector.cpp
//!
//! Implements class MismatchesCollector
//!
//===========================================================================

#include "MismatchesCollector.hpp"
#include "AccessInterface.hpp"
#include "AccessInterfaceTranslator.hpp"
#include "Chain.hpp"
#include "Linker.hpp"
#include "Register.hpp"
#include "BlackBox.hpp"

using namespace mast;



//! Collects mismatches of a AccessInterface sub-nodes
//!
void MismatchesCollector::VisitAccessInterface (AccessInterface& accessInterface)
{
  VisitChildren(accessInterface);
}

//! Collects mismatches of a AccessInterfaceTranslator sub-nodes
//!
void MismatchesCollector::VisitAccessInterfaceTranslator (AccessInterfaceTranslator &accessInterfaceTranslator)
{
  VisitChildren(accessInterfaceTranslator);
}

//! Collects mismatches of a Chain sub-nodes
//!
void MismatchesCollector::VisitChain (Chain& chain)
{
  VisitChildren(chain);
}

//! Collects mismatches of a Linker sub-nodes
//!
void MismatchesCollector::VisitLinker (Linker& linker)
{
  VisitChildren(linker);
}

//! Collects mismatches of a Register
//!
void MismatchesCollector::VisitRegister (Register& reg)
{
  m_mismatchesCount += reg.Mismatches();
  if (m_clearCounter)
  {
    reg.ResetMismatches();
  }
}

//! Collects mismatches of a BlackBox
//!
void MismatchesCollector::VisitBlackBox (BlackBox& bbox)
{
  m_mismatchesCount += bbox.Mismatches();
  if (m_clearCounter)
  {
    bbox.ResetMismatches();
  }
}



//===========================================================================
// End of MismatchesCollector.cpp
//===========================================================================
