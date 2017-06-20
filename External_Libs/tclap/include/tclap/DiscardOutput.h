//===========================================================================
//                           DiscardOutput.h
//===========================================================================
//
//! @file DiscardOutput.h
//!
//! Implements class DiscardOutput
//!
//===========================================================================

#ifndef STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_
  #define STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_

#include <tclap/CmdLineInterface.h>
#include <tclap/CmdLineOutput.h>
//+#include <tclap/XorHandler.h>
#include <tclap/Arg.h>

namespace TCLAP
{
//! Discards output for the CmdLine
//!
//! @note This is useful for unit tests context (avoiding to pollute the output)
//!
class DiscardOutput final : public CmdLineOutput
{
  public:
    virtual ~DiscardOutput() = default;
    DiscardOutput() = default;

    virtual void usage   (CmdLineInterface&) {};
    virtual void version (CmdLineInterface&) {};
    virtual void failure (CmdLineInterface&, ArgException&) {};
};
} //namespace TCLAP

#endif  // not defined STREAMOUTPUT_H__E7FB818A_2EDA_4D0A_6C99_F3009B882544__INCLUDED_

//===========================================================================
// End of DiscardOutput.h
//===========================================================================
