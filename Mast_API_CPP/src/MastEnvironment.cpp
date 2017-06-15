//===========================================================================
//                           MastEnvironment.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment.cpp
//!
//! Implements class MastEnvironment
//!
//===========================================================================

#include "MastEnvironment.hpp"
#include "MastEnvironment_impl.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

using namespace mast;

//! Releases implementation instance
//! @note Cannot be declared as default because of usage of unique_ptr
MastEnvironment::~MastEnvironment ()
{
}
//
//  End of: MastEnvironment::~MastEnvironment
//---------------------------------------------------------------------------


//! Initializes default logger if not in unit tests context
//!
//! @note In unit tests context, the logger is already initialized (and managed by unit tests main)
//!
//! @param unitTestContext  True when in unit tests context
//!
MastEnvironment::MastEnvironment (bool unitTestContext)
  : m_impl(make_unique<MastEnvironment_impl>(unitTestContext))
{
}
//
//  End of: MastEnvironment::MastEnvironment
//---------------------------------------------------------------------------




//! Creates system model using parsed options and loaded plugins
//!
void MastEnvironment::CreateSystemModel ()
{
  m_impl->CreateSystemModel();
}
//
//  End of: MastEnvironment::CreateSystemModel
//---------------------------------------------------------------------------




//! Loads plugins defined by options
//!
//! @note Plugins are loaded in following order:
//!   - Plugins given at Command-line
//!   - Plugins contained in directories given at Command-line
//!   - Plugins given in Configuration file
//!   - Plugins contained in directory given in Configuration file
//!   - Plugins in current directory
//!   - Plugins in MAST root "Plugins" directory
//!
void MastEnvironment::LoadPlugins ()
{
  m_impl->LoadPlugins();
}
//
//  End of: MastEnvironment::LoadPlugins
//---------------------------------------------------------------------------



//! Parses options - from C-Style command line arguments
//!
//! @param argc   Number of arguments (first one is application name)
//! @param argv   Array of arguments
//!
void MastEnvironment::ParseOptions (int argc, const char* argv[])
{
  vector<string> arguments;

  for (int ii = 0 ; ii < argc ; ++ii)
  {
    arguments.emplace_back(argv[ii]);
  }
  ParseOptions(arguments);
}
//
//  End of: MastEnvironment::ParseOptions
//---------------------------------------------------------------------------


//! Parses options - from list of command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
void MastEnvironment::ParseOptions (vector<string> arguments)
{
  m_impl->ParseOptions(arguments);
}
//
//  End of: MastEnvironment::ParseOptions
//---------------------------------------------------------------------------





//===========================================================================
// End of MastEnvironment.cpp
//===========================================================================
