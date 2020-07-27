//===========================================================================
//                           UT_SystemModelManager.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelManager.hpp
//!
//! Declares test fixture class for testing SystemModelManager
//!
//===========================================================================


#ifndef UT_SYSTEMMODELMANAGERTRANSLATORS_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_
  #define UT_SYSTEMMODELMANAGERTRANSLATORS_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelManager
//!
class UT_SystemModelManager : public CxxTest::TestSuite
{
  public:
   UT_SystemModelManagerTranslator() = default;
  ~UT_SystemModelManagerTranslator() = default;

  void test_DoDataCycles_AccessInterfaceTranslator ();
  void test_DoDataCycles_I2C_AccessInterfaceTranslator ();
  void test_DoDataCycles_DoubleAccessInterfaceTranslator ();
  void test_DoDataCycles_BrocadeTranslator ();
  void test_DoDataCycles_JTAG_to_I2C_Translator ();
  void test_DoDataCycles_JTAG_BitBang_Translator ();

#endif  // not defined UT_SYSTEMMODELMANAGERTRANSLATORS_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_
//===========================================================================
// End of UT_SystemModelManagerTranslators.hpp
//===========================================================================
