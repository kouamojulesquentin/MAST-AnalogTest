//===========================================================================
//                           Pusher.h
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//               2017 Michele Portolan, TIMA             
// 
// Project : C++11 blocking queues
// based on : Multi-Edit extension DLL
//
//! @file Pusher.hpp
//!
//! Declares class Pusher
//!
// Some Tags are inserted for Doxygen (JavaDoc like) doc generator.
//===========================================================================
// Created by JFC on May/04/2015
//===========================================================================

#ifndef PUSHER_H__AAFDCE17_E3CE_4F24_3F92_ABAD61C82BD__INCLUDED_
  #define PUSHER_H__AAFDCE17_E3CE_4F24_3F92_ABAD61C82BD__INCLUDED_


#include "TestStrategies.hpp"
#include "StaticTestSpy.hpp"
#include <utility>


namespace
{

//! Template to define loops for pushing into a container items
//!
template<template <typename> class TContainer, typename TPayload, DataTransferStrategy TDataTransferStrategy>
class Pusher final {};


//! Loops for pushing items using copy constructor
//!
template<template <typename> class TContainer, typename TPayload>
class Pusher<TContainer, TPayload, DataTransferStrategy::Copy> final
{
  public: static void Push(TContainer<TPayload>& container, TPayload& start, const TPayload& end, const TPayload& increment)
  {
    for (TPayload value = start ; value < end; value += increment)
    {
      container.Push(value);
    }
  }
};


//! Loops for pushing items using move constructor
//!
template<template <typename> class TContainer, typename TPayload>
class Pusher<TContainer, TPayload, DataTransferStrategy::Move> final
{
  public: static void Push(TContainer<TPayload>& container, TPayload& start, const TPayload& end, const TPayload& increment)
  {
    StaticTestSpy item(0);
    for (TPayload value = start ; value < end; value += increment)
    {
      item.SetValue(value.GetValue());    // Minimize usage of construction (to simplify tally of them)
      container.Push(std::move(item));      // Moving have no detrimental effect on source StaticTestSpy (so it can be reused)
    }
  }
};


//! Loops for pushing items using emplace construction
//!
template<template <typename> class TContainer, typename TPayload>
class Pusher<TContainer, TPayload, DataTransferStrategy::Emplace> final
{
  public: static void Push(TContainer<TPayload>& container, TPayload& start, const TPayload& end, const TPayload& increment)
  {
    for (auto value = start.GetValue(); value < end.GetValue(); value += increment.GetValue())
    {
      container.Emplace(value);
    }
  }
};

} // End of unnamed namespace



#endif  // not defined PUSHER_H__AAFDCE17_E3CE_4F24_3F92_ABAD61C82BD__INCLUDED_

//===========================================================================
// End of Pusher.h
//===========================================================================
