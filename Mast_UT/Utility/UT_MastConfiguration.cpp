//===========================================================================
//                           UT_MastConfiguration.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastConfiguration.cpp
//!
//! Implements test fixture for testing MastConfiguration
//!
//===========================================================================

#include "UT_MastConfiguration.hpp"
#include "MastConfiguration.hpp"
#include "MastConfig.hpp"

#include <cxxtest/ValueTraits.h>

using namespace mast;

//! Checks MastConfiguration::constructor()
//!
void UT_MastConfiguration::test_constructor ()
{
  // ---------------- Exercise
  //
  MastConfiguration sut;

  // ---------------- Verify
  //
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "DUT.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "LastOrDefault");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "MastModel.gml");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "DUT");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "Mast.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "DUT");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "MastModel.txt");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::Std);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Info);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins.front(), PLUGINS_DIRECTORY_NAME);
}






//===========================================================================
// End of UT_MastConfiguration.cpp
//===========================================================================
