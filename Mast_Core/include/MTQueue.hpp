//===========================================================================
//                           MTQueue.hpp
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//               2017 Michele Portolan, TIMA
//
// Project : C++11 blocking queues
// based on : Multi-Edit extension DLL
//
//! @file MTQueue.hpp
//!
//! Declares class MTQueue
//!
// Some Tags are inserted for Doxygen (JavaDoc like) doc generator.
//===========================================================================
// Created by JFC on January/28/2015
//===========================================================================

#ifndef MTQUEUE_H__3ACE24EF_7BE_4DFE_F1BC_98E02E40CECE__INCLUDED_
  #define MTQUEUE_H__3ACE24EF_7BE_4DFE_F1BC_98E02E40CECE__INCLUDED_

#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

namespace mast
{

//! Multi-thread compliant queue
//!
//! @note Mutex based
//!
template<typename T>
class MTQueue final
{
  // ---------------- Private  Fields
  //
  private:
  std::queue<T>           m_queue;             //!< Holds items
  std::mutex              m_mutex;             //!< Mutex for thread safety
  std::condition_variable m_conditionVariable; //!< To avoid wait loops

  // ---------------- Public  Methods
  //
  public:
  MTQueue()  = default;
  ~MTQueue() = default;

  //! Returns item that was pushed before all other items and pop it out of the queue
  //!
  //! @note Blocks until there is at least one item in the queue
  //! @note If there is an exception while copying the item, it will be lost forever
  //!
  T Pop()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty())
    {
      m_conditionVariable.wait(lock);
    }
    auto item = m_queue.front();
    m_queue.pop();
    return item;
  }

  //! Copies, item that was pushed before all other items, then pop it out of the queue
  //!
  //! @param [out]     item   Refers to where front element is copied
  //!
  //! @note Blocks until there is at least one item in the queue
  //! @note If there is an exception while copying the item, it will be lost forever
  //!
  void Pop(T& item)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty())
    {
      m_conditionVariable.wait(lock);
    }
    item = std::move(m_queue.front());
    m_queue.pop();
  }


  //! Copies item that was pushed before all other items, then pop it out of the queue
  //! When the queue is empty, it waits until there is at least one ore timeout is expired
  //!
  //! @param [out]     item     Refers to where front element is copied
  //! @param [in]      timeout  Maximum wait duration
  //!
  //! @note Blocks until there is at least one item in the queue
  //! @note If there is an exception while copying the item, it will be lost forever
  //!
  template<class Rep, class Period>
  std::cv_status Pop(T& item, const std::chrono::duration<Rep, Period>& timeout)
  {
    auto status   = std::cv_status::no_timeout;
    auto deadLine = std::chrono::system_clock::now() + timeout;

    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty())
    {
      status = m_conditionVariable.wait_until(lock, deadLine);
      if (status == std::cv_status::timeout)
      {
        return status;
      }
    }

    item = std::move(m_queue.front());
    m_queue.pop();
    return status;
  }

  //! Pushes a new item into (the back of) the queue.
  //!
  //! @note The queue grows as needed
  //!
  void Push(const T& item)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(item);
    lock.unlock();
    m_conditionVariable.notify_one();
  }

  //! Moves a new item into (the back of) the queue.
  //!
  //! @note The queue grows as needed
  void Push(T&& item)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(std::move(item));
    lock.unlock();
    m_conditionVariable.notify_one();
  }

  //! Pushes new item to the end (back) of the queue. The element is constructed in-place, i.e. no
  //! copy or move operations are performed. The constructor of the element is called with exactly
  //! the same arguments as supplied to the function.
  //!
  template<class ... Args>
  void Emplace(Args&&... args)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.emplace(std::forward<Args>(args)...);
    lock.unlock();
    m_conditionVariable.notify_one();
  }

  //! Returns true when the queue is empty, false otherwise
  //!
  bool IsEmpty() const noexcept(noexcept(m_queue.empty()))
  {
    bool isEmpty = m_queue.empty();
    return isEmpty;
  };
};
//
//  End of MTQueue class declaration
//---------------------------------------------------------------------------

} // End of namespace Mast


#endif  // not defined MTQUEUE_H__3ACE24EF_7BE_4DFE_F1BC_98E02E40CECE__INCLUDED_

//===========================================================================
// End of MTQueue.hpp
//===========================================================================
