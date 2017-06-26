//===========================================================================
//                           StaticTestSpy.cpp
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//
// Project : Multi-Edit extension DLL
//
//! @file StaticTestSpy.cpp
//!
//! Implements class that provides facilities to spy operations done on a object
//!
//===========================================================================


#include "StaticTestSpy.hpp"

const int StaticTestSpy::sm_defaultValue = 0xFF55AA00;

std::atomic<size_t>    StaticTestSpy::sm_nbAnyConstructorsCall     (0);
std::atomic<size_t>    StaticTestSpy::sm_nbCopyAssignmentCall      (0);
std::atomic<size_t>    StaticTestSpy::sm_nbMoveAssignmentCall      (0);
std::atomic<size_t>    StaticTestSpy::sm_nbCopyConstructorCall     (0);
std::atomic<size_t>    StaticTestSpy::sm_nbMoveConstructorCall     (0);
std::atomic<size_t>    StaticTestSpy::sm_nbDefaultConstructorsCall (0);
std::atomic<size_t>    StaticTestSpy::sm_nbValueConstructorCall    (0);
std::atomic<size_t>    StaticTestSpy::sm_nbDestructorCall          (0);
std::atomic<size_t>    StaticTestSpy::sm_nbEqualToOperatorCall     (0);
std::atomic<size_t>    StaticTestSpy::sm_nbGetValueCall            (0);
std::atomic<size_t>    StaticTestSpy::sm_nbNotEqualToOperatorCall  (0);
std::atomic<size_t>    StaticTestSpy::sm_nbResetCall               (0);
std::atomic<size_t>    StaticTestSpy::sm_nbSetValueCall            (0);
std::atomic<size_t>    StaticTestSpy::sm_nbValueAssignmentCall     (0);

const StaticTestSpy StaticTestSpy::sm_default(StaticTestSpy::sm_defaultValue);

StaticTestSpy::StaticTestSpy () noexcept
: m_value (sm_defaultValue)
{
  ++sm_nbAnyConstructorsCall;
  ++sm_nbDefaultConstructorsCall;
}


StaticTestSpy::StaticTestSpy (int value) noexcept
: m_value (value)
{
  ++sm_nbAnyConstructorsCall;
  ++sm_nbValueConstructorCall;
}

StaticTestSpy::StaticTestSpy (const StaticTestSpy& other) noexcept
: m_value (other.m_value)
{
  ++sm_nbAnyConstructorsCall;
  ++sm_nbCopyConstructorCall;
}

StaticTestSpy::StaticTestSpy (StaticTestSpy&& other) noexcept
: m_value (other.m_value)
{
  other.m_value = 0xbad;
  ++sm_nbAnyConstructorsCall;
  ++sm_nbMoveConstructorCall;
}

StaticTestSpy::~StaticTestSpy () noexcept
{
  ++sm_nbDestructorCall;
}


//! Returns singleton of StaticTestSpy with default value.
//!
const StaticTestSpy& StaticTestSpy::GetDefault() noexcept
{
  return sm_default;
}



//! Returns default value (when using default constructor) noexcept
//!
int StaticTestSpy::GetDefaultValue () noexcept
{
  return sm_defaultValue;
}


//! Returns number of times instances of StaticTestSpy have been constructed
//!
size_t StaticTestSpy::GetNbAnyConstructorsCall () noexcept
{
  return sm_nbAnyConstructorsCall;
}

//! Returns number of times instances of StaticTestSpy have been assigned (move, copy or value flavor)
//!
size_t StaticTestSpy::GetNbAnyAssignmentCall () noexcept
{
  return sm_nbValueAssignmentCall + sm_nbCopyAssignmentCall + sm_nbMoveAssignmentCall;
}



//! Returns the number of times operator=(StaticTestSpy&) has been called
//!
size_t StaticTestSpy::GetNbCopyAssignmentCall () noexcept
{
  return sm_nbCopyAssignmentCall;
}

//! Returns the number of times operator=(StaticTestSpy&&) has been called
//!
size_t StaticTestSpy::GetNbMoveAssignmentCall () noexcept
{
  return sm_nbMoveAssignmentCall;
}



//! Returns number of times instances of StaticTestSpy have been copy constructed
//!
size_t StaticTestSpy::GetNbCopyConstructorCall () noexcept
{
  return sm_nbCopyConstructorCall;
}

//! Returns number of times instances of StaticTestSpy have been copy constructed
//!
size_t StaticTestSpy::GetNbMoveConstructorCall () noexcept
{
  return sm_nbMoveConstructorCall;
}

//! Returns number of times instances of StaticTestSpy have been default constructed
//!
size_t StaticTestSpy::GetNbDefaultConstructorCall () noexcept
{
  return sm_nbDefaultConstructorsCall;
}

//! Returns number of times instances of StaticTestSpy have been destructed
//!
size_t StaticTestSpy::GetNbDestructorCall () noexcept
{
  return sm_nbDestructorCall;
}


//! Returns number of times instances of StaticTestSpy have been compared equal to another
//!
size_t StaticTestSpy::GetNbEqualToOperatorCall () noexcept
{
  return sm_nbEqualToOperatorCall;
}


//! Returns the number of times GetValue has been called
//!
size_t StaticTestSpy::GetNbGetValueCall () noexcept
{
  return sm_nbGetValueCall;
}

//! Returns number of times instances of StaticTestSpy have been compared not equal to another
//!
size_t StaticTestSpy::GetNbNotEqualToOperatorCall () noexcept
{
  return sm_nbNotEqualToOperatorCall;
}


//! Returns the number of times reset has been called
//!
size_t StaticTestSpy::GetNbResetCall () noexcept
{
  return sm_nbResetCall;
}


//! Returns the number of times SetValue has been called
//!
size_t StaticTestSpy::GetNbSetValueCall () noexcept
{
  return sm_nbSetValueCall;
}



//! Returns the number of times operator=(size_t) has been called
//!
size_t StaticTestSpy::GetNbValueAssignmentCall () noexcept
{
  return sm_nbValueAssignmentCall;
}

//! Returns number of times instances of StaticTestSpy have been constructed with a (size_t) value
//!
size_t StaticTestSpy::GetNbValueConstructorCall () noexcept
{
  return sm_nbValueConstructorCall;
}



//! Returns current value
//!
size_t StaticTestSpy::GetValue () const noexcept
{
  ++sm_nbGetValueCall;
  return m_value;
}

//! Assigns from another StaticTestSpy
//!
StaticTestSpy& StaticTestSpy::operator= (const StaticTestSpy& rhs) noexcept
{
  ++sm_nbCopyAssignmentCall;
  m_value = rhs.m_value;
  return *this;
}


//! Assigns moving from another StaticTestSpy
//!
StaticTestSpy& StaticTestSpy::operator= (StaticTestSpy&& rhs) noexcept
{
  ++sm_nbMoveAssignmentCall;
  m_value = rhs.m_value;
  rhs.m_value = 0xDeadBad;
  return *this;
}

//! Assigns a new value
//!
StaticTestSpy& StaticTestSpy::operator= (int newValue) noexcept
{
  ++sm_nbValueAssignmentCall;
  m_value = newValue;
  return *this;
}


//! Returns true if lhs and rhs operand have the same value
//!
bool StaticTestSpy::operator== (const StaticTestSpy& rhs) const noexcept
{
  ++sm_nbEqualToOperatorCall;
  return m_value == rhs.m_value;
}

//! Returns true if lhs and rhs operand have not the same value
//!
bool StaticTestSpy::operator!= (const StaticTestSpy& rhs) const noexcept
{
  ++sm_nbNotEqualToOperatorCall;
  return m_value != rhs.m_value;
}

//! Returns true if there has been at least one method or constructor call since last ResetAll() and false otherwise
//!
bool StaticTestSpy::HasCountAnyCall () noexcept
{
  return    (sm_nbAnyConstructorsCall     != 0)
         || (sm_nbCopyAssignmentCall      != 0)
         || (sm_nbMoveAssignmentCall      != 0)
         || (sm_nbDefaultConstructorsCall != 0)
         || (sm_nbValueConstructorCall    != 0)
         || (sm_nbCopyConstructorCall     != 0)
         || (sm_nbMoveConstructorCall     != 0)
         || (sm_nbDestructorCall          != 0)
         || (sm_nbEqualToOperatorCall     != 0)
         || (sm_nbGetValueCall            != 0)
         || (sm_nbNotEqualToOperatorCall  != 0)
         || (sm_nbSetValueCall            != 0)
         || (sm_nbResetCall               != 0)
         || (sm_nbValueAssignmentCall     != 0);
}



//! Resets value to default value
//!
void StaticTestSpy::Reset () noexcept
{
  m_value = sm_defaultValue;
  ++sm_nbResetCall;
}


//! Resets all counters
//!
void StaticTestSpy::ResetAll () noexcept
{
  sm_nbAnyConstructorsCall     = 0;
  sm_nbDefaultConstructorsCall = 0;
  sm_nbValueConstructorCall    = 0;
  sm_nbCopyAssignmentCall      = 0;
  sm_nbMoveAssignmentCall      = 0;
  sm_nbCopyConstructorCall     = 0;
  sm_nbMoveConstructorCall     = 0;
  sm_nbDestructorCall          = 0;
  sm_nbEqualToOperatorCall     = 0;
  sm_nbGetValueCall            = 0;
  sm_nbNotEqualToOperatorCall  = 0;
  sm_nbResetCall               = 0;
  sm_nbSetValueCall            = 0;
  sm_nbValueAssignmentCall     = 0;
}


//! Sets current value
//!
void StaticTestSpy::SetValue (int newValue) noexcept
{
  ++sm_nbSetValueCall;
  m_value = newValue;
}


//! Creates a textual representation that is easily readable
//!
//! @note   An instance is displayed like: "StaticTestSpy(256/0x100)"
//!
std::string StaticTestSpy::ToDebugString () const
{
    std::string debugString;
    std::ostringstream stream;
    stream << "StaticTestSpy(" << m_value << "/0x" << std::hex << std::uppercase << m_value << ")";
    debugString = stream.str();

  return debugString;
}

//--------------------------------END-OF-FILE------------------------------------
