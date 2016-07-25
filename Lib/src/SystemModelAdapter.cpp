//===========================================================================
//                           SystemModelAdapter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter.cpp
//!
//! Implements 'C' language API to system model
//===========================================================================

#include "SystemModelAdapter.h"
#include "SystemModelManager.hpp"
#include "Utility.hpp"

#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

using namespace std::string_literals;
using namespace mast;


namespace
{
  thread_local string thread_error_message;

} // End of unnamed namespace


//! Clears last error message
//!
void ClearErrorMessage()
{
  thread_error_message.clear();
}


//! Sets error message
//!
//! @note This is normally used only by library internals
void SetErrorMessage(const char* message)
{
  thread_error_message = message;
}


//! Returns error message from last command
//!
//! @return Pointer to the message string (only valid till no other command is issued)
const char* ErrorMessage ()
{
  return thread_error_message.c_str();
}


//===========================================================================
// End of SystemModelAdapter.cpp
//===========================================================================
