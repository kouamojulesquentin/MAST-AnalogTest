//===========================================================================
//                           StaticTestSpy.hpp
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//
// Project : Multi-Edit extension DLL
//
//! @file StaticTestSpy.hpp
//!
//! Declares class StaticTestSpy
//!
//===========================================================================



#ifndef STATICTESTSPY_H__6C0C2CC4_7AA7_4E43_EEA7_DD8560B6EB75__INCLUDED_
  #define STATICTESTSPY_H__6C0C2CC4_7AA7_4E43_EEA7_DD8560B6EB75__INCLUDED_

#include <string>
#include <sstream>
#include <atomic>
#include <cxxtest/ValueTraits.h>


//! Provides observation of construction, destruction, copy construction, assignment of instances of StaticTestSpy
//!
//! @note   Counters are shared static variables
//! @note   It holds also a value, that can be changed at will
class StaticTestSpy
{
  public:
           StaticTestSpy() noexcept;
  explicit StaticTestSpy(int value) noexcept;
           StaticTestSpy(const StaticTestSpy& other) noexcept;
           StaticTestSpy(StaticTestSpy&&      other) noexcept;
  virtual ~StaticTestSpy() noexcept;

  //! Returns current value
  //!
  size_t GetValue () const noexcept;

  //! Sets current value
  //!
  void SetValue (int newValue) noexcept;

  //! Resets value to default value
  //!
  void Reset() noexcept;

  //! Assigns copying from another StaticTestSpy
  //!
  StaticTestSpy& operator= (const StaticTestSpy& rhs) noexcept;

  //! Assigns moving from another StaticTestSpy
  //!
  StaticTestSpy& operator= (StaticTestSpy&& rhs) noexcept;

  //! Assigns a new value
  //!
  StaticTestSpy& operator=(int newValue) noexcept;

  //! Returns true if lhs and rhs operand have the same value
  //!
  bool operator==(const StaticTestSpy& rhs) const noexcept;

  //! Returns true if lhs and rhs operand have not the same value
  //!
  bool operator!=(const StaticTestSpy& rhs) const noexcept;

  //! Returns true if lhs is strictly less than rhs operand
  //!
  bool operator<(const StaticTestSpy& rhs) const { return m_value < rhs.m_value; }

  //! Returns true if lhs is less or equal than rhs operand
  //!
  bool operator<=(const StaticTestSpy& rhs) const { return m_value <= rhs.m_value; }

  //! Returns true if lhs is strictly greater than rhs operand
  //!
  bool operator>(const StaticTestSpy& rhs) const { return m_value > rhs.m_value; }

  //! Returns true if lhs is greater or equal than rhs operand
  //!
  bool operator>=(const StaticTestSpy& rhs) const { return m_value >= rhs.m_value; }

  //! Pre-increment operator
  //!
  StaticTestSpy& operator++() { ++m_value ; return *this; }

  //! Post-increment operator
  //!
  StaticTestSpy operator++(int) { auto before = *this ; ++m_value ; return before; }

  //! Add operator
  //!
  StaticTestSpy operator+(const StaticTestSpy& rhs) { return StaticTestSpy(m_value + rhs.m_value); }

  //! Compound add operator
  //!
  StaticTestSpy& operator+=(const StaticTestSpy& rhs) { m_value += rhs.m_value ; return *this; }


  //! Creates a textual representation that is easily readable
  //!
  std::string ToDebugString() const;

  //! Returns singleton of StaticTestSpy with default value.
  //!
  static const StaticTestSpy& GetDefault() noexcept;

  //! Returns default value (when using default constructor)
  //!
  static int GetDefaultValue() noexcept;

  //! Returns number of times instances of StaticTestSpy have been constructed (whatever kind of constructors)
  //!
  static size_t GetNbAnyConstructorsCall() noexcept;

  //! Returns number of times instances of StaticTestSpy have been assigned (move, copy or value flavor)
  //!
  static size_t GetNbAnyAssignmentCall() noexcept;


  //! Returns the number of times operator=(StaticTestSpy&) has been called
  //!
  static size_t GetNbCopyAssignmentCall() noexcept;

  //! Returns the number of times operator=(StaticTestSpy&&) has been called
  //!
  static size_t GetNbMoveAssignmentCall () noexcept;


  //! Returns number of times instances of StaticTestSpy have been copy constructed
  //!
  static size_t GetNbCopyConstructorCall() noexcept;

  //! Returns number of times instances of StaticTestSpy have been move constructed
  //!
  static size_t GetNbMoveConstructorCall() noexcept;

  //! Returns number of times instances of StaticTestSpy have been default constructed
  //!
  static size_t GetNbDefaultConstructorCall () noexcept;

  //! Returns number of times instances of StaticTestSpy have been destructed
  //!
  static size_t GetNbDestructorCall() noexcept;

  //! Returns number of times instances of StaticTestSpy have been compared equal to another
  //!
  static size_t GetNbEqualToOperatorCall () noexcept;

  //! Returns number of times instances of StaticTestSpy have been compared not equal to another
  //!
  static size_t GetNbNotEqualToOperatorCall () noexcept;

  //! Returns the number of times getNullPointer has been called
  //!
  static size_t GetNbGetNullPointerCall() noexcept;

  //! Returns the number of times GetValue has been called
  //!
  static size_t GetNbGetValueCall() noexcept;

  //! Returns the number of times Reset has been called
  //!
  static size_t GetNbResetCall() noexcept;

  //! Returns the number of times SetValue has been called
  //!
  static size_t GetNbSetValueCall() noexcept;

  //! Returns the number of times operator=(size_t) has been called
  //!
  static size_t GetNbValueAssignmentCall () noexcept;

  //! Returns number of times instances of StaticTestSpy have been constructed with a (size_t) value
  //!
  static size_t GetNbValueConstructorCall () noexcept;

  //! Returns true if there has been at least one method or constructor call since last ResetAll() and false otherwise
  //!
  static bool HasCountAnyCall() noexcept;

  //! Resets all counters
  //!
  static void ResetAll() noexcept;

  // ---------------- Private  Fields
  //
  private:
  int m_value;  //!< Instance inner value

  static const int sm_defaultValue; //!< Instances default (inner) value

  static std::atomic<size_t> sm_nbAnyConstructorsCall;     //!< Counts the number of call to StaticTestSpy constructorS (any one of them)
  static std::atomic<size_t> sm_nbCopyAssignmentCall;      //!< Counts the number of call to operator=(StaticTestSpy&)
  static std::atomic<size_t> sm_nbMoveAssignmentCall;      //!< Counts the number of call to operator=(StaticTestSpy&&)
  static std::atomic<size_t> sm_nbCopyConstructorCall;     //!< Counts the number of call to StaticTestSpy copy constructor
  static std::atomic<size_t> sm_nbMoveConstructorCall;     //!< Counts the number of call to StaticTestSpy move constructor
  static std::atomic<size_t> sm_nbDefaultConstructorsCall; //!< Counts the number of call to StaticTestSpy default constructor
  static std::atomic<size_t> sm_nbDestructorCall;          //!< Counts the number of call to StaticTestSpy destructor
  static std::atomic<size_t> sm_nbEqualToOperatorCall;     //!< Counts the number of call to operator==(StaticTestSpy)
  static std::atomic<size_t> sm_nbGetValueCall;            //!< Counts the number of call to GetValue
  static std::atomic<size_t> sm_nbNotEqualToOperatorCall;  //!< Counts the number of call to operator!=(StaticTestSpy)
  static std::atomic<size_t> sm_nbResetCall;               //!< Counts the number of call to Reset
  static std::atomic<size_t> sm_nbSetValueCall;            //!< Counts the number of call to SetValue
  static std::atomic<size_t> sm_nbValueAssignmentCall;     //!< Counts the number of call to operator=(int)
  static std::atomic<size_t> sm_nbValueConstructorCall;    //!< Counts the number of call to StaticTestSpy constructor with a value parameter

  static const  StaticTestSpy sm_default;     //!< Singleton that holds the default value
};


namespace CxxTest
{
  //! Gives a way to CxxTest framework to display a human readable content of a StaticTestSpy
  //!
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<StaticTestSpy>
  {
    std::string m_asString;
    public:
    ValueTraits(const StaticTestSpy& val)
    {
      m_asString = val.ToDebugString();
    }
    const char* asString() const { return m_asString.c_str(); }
  };
}


#endif  // not defined STATICTESTSPY_H__6C0C2CC4_7AA7_4E43_EEA7_DD8560B6EB75__INCLUDED_

//--------------------------------END-OF-FILE------------------------------------
