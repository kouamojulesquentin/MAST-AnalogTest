//===========================================================================
//                           TestStateGuard.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file TestStateGuard.h
//!
//! Declares TestStateGuard class for saving, tests related, global variables
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/24/2015
//===========================================================================


#ifndef TESTSTATEGUARD_H__DD2CCD92_D582_4C06_518E_5544858D636B__INCLUDED_
  #define TESTSTATEGUARD_H__DD2CCD92_D582_4C06_518E_5544858D636B__INCLUDED_

namespace CxxTest
{
//! Saves, tests related, global variables in constructor and restores them in destructor
//!
//! @note Supports data driven tests global variables even though it is not available by default;
//!       This is necessary to be sure that following data driven test used default settings (whatever where changes have been made)
//!
class TestStateGuard
{
    unsigned _maxDumpSize;
    bool     _abortTestOnFail;
    bool     _abortDataDrivenTestsOnFailure;

public:
    TestStateGuard()
      : _maxDumpSize                   (maxDumpSize())
      , _abortTestOnFail               (abortTestOnFail())
      , _abortDataDrivenTestsOnFailure (abortDataDrivenTestsOnFailure())
    {}

    virtual ~TestStateGuard()
    {
        setMaxDumpSize(_maxDumpSize);
        setAbortTestOnFail               (_abortTestOnFail);
        setAbortDataDrivenTestsOnFailure (_abortDataDrivenTestsOnFailure);
    }
};
} // End of namespace CxxTest

#endif  // not defined TESTSTATEGUARD_H__DD2CCD92_D582_4C06_518E_5544858D636B__INCLUDED_
//===========================================================================
// End of TestStateGuard.h
//===========================================================================


