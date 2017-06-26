//===========================================================================
//                           UT_MTQueue.cpp
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//               2017 Michele Portolan, TIMA
//
// Project : C++11 blocking queues
// based on : Multi-Edit extension DLL
//
//! @file UT_MTQueue.cpp
//!
//! Implements test fixture for testing MTQueue
//!
//===========================================================================
// Created by JFC on January/28/2015
//===========================================================================

#include "TestStrategies.hpp"
#include "Pusher.hpp"
#include "MTQueue.hpp"
#include "UT_MTQueue.hpp"
#include "StaticTestSpy.hpp"

#include <cxxtest/traits/STL11_Traits.h>

#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <memory>
#include <sstream>
#include <ostream>
#include <functional>
#include <algorithm>

using mast::MTQueue;
using std::vector;
using std::string;
using std::cv_status;
using std::condition_variable;
using std::future_status;
using std::ostringstream;

using namespace std::chrono;
using namespace std::chrono_literals;


namespace
{

//! Checks there is a "ramp" inside the vector
//!
template<typename TPayload>
void Check_Content (int line, const vector<TPayload>& data)
{
  TPayload rampValue(0);

  for (const auto& item : data)
  {
    std::ostringstream message;

    message << "data[" << CxxTest::ValueTraits<TPayload>(rampValue).asString() << "]";

    _TSM_ASSERT_EQUALS(__FILE__, line, message.str(), rampValue, item);
    ++rampValue;
  }
}
//
//  End of fct_Check_Content
//---------------------------------------------------------------------------


//! "Consumer" functor
//!
template<typename TPayload = int>
class Consumer final
{
  public:
  ~Consumer() = default;
  Consumer(MTQueue<TPayload>&  sut, std::atomic<bool>& moreToProduce, const std::shared_future<void>& startSynchronizer, size_t reserveCount = 10000)
  : m_sut               (sut)
  , m_startSynchronizer (startSynchronizer)
  , m_moreToProduce     (moreToProduce)
  , m_poppedItems       ()
  , m_reserveCount      (reserveCount)
  {
  }


  void operator()()
  {
    constexpr auto timeout = 15ms;
    TPayload  poppedItem;                     // Outside the loop to avoid to tally this construction many times

    m_poppedItems.reserve(m_reserveCount);    // To avoid many reallocation (put there because Consumers may be copied around before actual usage)

    // ---------------- Synchronization point (reduce starting time difference between threads)
    //
    m_startSynchronizer.wait();


    while (m_moreToProduce || !m_sut.IsEmpty())
    {
      auto status = m_sut.Pop(poppedItem, timeout);
      if (status == cv_status::no_timeout)
      {
        m_poppedItems.emplace_back(poppedItem);
      }
      else
      {
        ++m_timeouts;
      }
    }
  }

  const vector<TPayload>& PoppedItems() const { return m_poppedItems; };
  int                     Timeouts()    const { return m_timeouts;    };

  // ---------------- Private  Fields
  //
  private:
  MTQueue<TPayload>&              m_sut;
  const std::shared_future<void>& m_startSynchronizer;
  std::atomic<bool>&              m_moreToProduce;
  vector<TPayload>                m_poppedItems;
  int                             m_timeouts     = 0;
  size_t                          m_reserveCount = 1000u; // To avoid re-allocation when tally of copied items must be precise
};
//
//  End of Consumer class declaration
//---------------------------------------------------------------------------


//! "Producer" functor
//!
template<typename TPayload = int, DataTransferStrategy TDataTransferStrategy = DataTransferStrategy::Copy>
class Producer final
{
  public:
  Producer(MTQueue<TPayload>& sut, const std::shared_future<void>& startSynchronizer)
  : m_sut               (sut)
  , m_startSynchronizer (startSynchronizer)
  { }

  Producer            (const Producer<TPayload, TDataTransferStrategy>&) = default;
  Producer& operator= (const Producer<TPayload, TDataTransferStrategy>&) = default;

  ~Producer() = default;

  void operator()(TPayload start, const TPayload end, const TPayload increment)
  {
    // ---------------- Synchronization point (reduce starting time difference between threads)
    //
    m_startSynchronizer.wait();

    Pusher<MTQueue, TPayload, TDataTransferStrategy>::Push(m_sut, start, end, increment);
  }

  // ---------------- Private  Fields
  //
  private:
  MTQueue<TPayload>&              m_sut;
  const std::shared_future<void>& m_startSynchronizer;
};
//
//  End of Producer class declaration
//---------------------------------------------------------------------------

//! Parameterized tester using StaticTestSpy to check types of constructions and assignments
//!
template<DataTransferStrategy TDataTransferStrategy = DataTransferStrategy::Copy>
class StaticTestSpyBased_MultiThread_Tester
{
  public:
  //! Checks Push/Pop with given number of producers, consumers and produced items count
  //!
  //! @note It is difficult to check that all consumers take part of the share
  //!       So test is tried multiple time until all consumer get some data or loop count is exhausted
  //!
  void test (size_t producersCount, size_t consumersCount, size_t itemsCount)
  {
    ostringstream os;
    os << "producers:"    << producersCount
       << ", consumers: " << consumersCount
       << ", items: "     << itemsCount;

    auto context = os.str();

    using TPayload = StaticTestSpy;
    using TProducer = Producer<TPayload, TDataTransferStrategy>;
    using TConsumer = Consumer<TPayload>;

    uint32_t tryCount         = 10u;
    auto     consumerFamished = false;

    do
    {
      // ---------------- Setup
      //
      consumerFamished = false;

      MTQueue<TPayload>  sut;
      std::atomic<bool>  moreToProduce (true);
      std::promise<void> promise;           // This is for threads synchronization
      auto               threadSynchronizer  = promise.get_future().share();
      vector<TProducer>  producers(producersCount, TProducer(sut, threadSynchronizer));
      vector<TConsumer>  consumers(consumersCount, TConsumer(sut, moreToProduce, threadSynchronizer, itemsCount));

      TPayload start     (0);
      TPayload end       (itemsCount);
      TPayload increment (producers.size());

      StaticTestSpy::ResetAll();

      // ---------------- Exercise
      //
      // Start consumers
      using TConsumerFuture = decltype(std::async(std::launch::async, TConsumer(sut, moreToProduce)));
      std::vector<TConsumerFuture> consumerFutures;
      for (auto& consumer : consumers)
      {
        consumerFutures.emplace_back(std::async(std::launch::async, std::ref(consumer)));
      }

      // Start producers
      using TProducerFuture = decltype(std::async(std::launch::async, TProducer(sut, threadSynchronizer), start, end, increment));
      std::vector<TProducerFuture> producerFutures;
      for (auto& producer : producers)
      {
        producerFutures.emplace_back(std::async(std::launch::async, std::ref(producer), start, end, increment));
        ++start;
      }
      promise.set_value();  // Synchronize all threads


      // ---------------- Verify
      //
      const auto timeout = 100ms;

      // Wait for producer threads to terminate their job
      for (auto& producerFuture: producerFutures)
      {
        auto status = producerFuture.wait_for(timeout);
        TS_ASSERT_EQUALS (status, future_status::ready);
      }

      moreToProduce = false;

      // Wait for consumer threads to terminate their job
      for (auto& consumerFuture: consumerFutures)
      {
        auto status = consumerFuture.wait_for(timeout);
        TS_ASSERT_EQUALS (status, future_status::ready);
      }


      CxxTest::setAbortTestOnFail(false);

      size_t pushCopyConstructed                        = 0;
      size_t pushMoveConstructed                        = 0;
      size_t pushEmplaceConstructed                     = 0;
      size_t popMoveAssigned                            = itemsCount;                      // Each popped items is moved from the queue
      size_t popDefaultConstructed                      = consumersCount;                  // Consumers create each one instance to pop items from the queue;
      size_t parameterPassingMoveConstructed            = producersCount * 6U;             // This comes from passing start, end and increment values
      size_t savedPoppedItemsCopyConstructed            = itemsCount;
      size_t rangeAndIncrementManagementCopyConstructed = 0;
      size_t totalValueConstructed                      = 0;
      size_t totalCopyConstructed                       = 0;
      size_t totalMoveConstructed                       = 0;

      if (strategy == DataTransferStrategy::Copy)
      {
        pushCopyConstructed                        = itemsCount;
        rangeAndIncrementManagementCopyConstructed = producersCount * 4U; // For producer there are 3 for range + 1 that is incremented
      }
      if (strategy == DataTransferStrategy::Move)
      {
        pushMoveConstructed                        = itemsCount;
        totalValueConstructed                      = producersCount;                  // This is the value that is first initialized, then incremented and moved
        rangeAndIncrementManagementCopyConstructed = producersCount * 4U;             // For producer there are 3 for range + 1 that is incremented
      }
      if (strategy == DataTransferStrategy::Emplace)
      {
        pushEmplaceConstructed                     = itemsCount;
        rangeAndIncrementManagementCopyConstructed = producersCount * 3U; // For producer there are 3 for range
      }

      totalValueConstructed += pushEmplaceConstructed;

      totalCopyConstructed =  pushCopyConstructed
                            + savedPoppedItemsCopyConstructed
                            + rangeAndIncrementManagementCopyConstructed;

      totalMoveConstructed =   pushMoveConstructed
                             + parameterPassingMoveConstructed;

      size_t totalAnyConstructed  =   totalCopyConstructed
                                    + totalMoveConstructed
                                    + totalValueConstructed
                                    + popDefaultConstructed;

      TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(),    totalCopyConstructed);
      TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveConstructorCall(),    totalMoveConstructed);
      TS_ASSERT_EQUALS (StaticTestSpy::GetNbValueConstructorCall(),   totalValueConstructed);
      TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveAssignmentCall(),     popMoveAssigned);
      TS_ASSERT_EQUALS (StaticTestSpy::GetNbDefaultConstructorCall(), popDefaultConstructed);
      TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(),    totalAnyConstructed);

      // Merge and sort popped items
      vector<TPayload>  poppedItems;
      for (const auto& consumer : consumers)
      {
        if (consumer.PoppedItems().size() == 0)
        {
          consumerFamished = true;
          std::this_thread::sleep_for(27ms); // Wait a little while to try next time in a "different threading context"
        }
        poppedItems.insert(poppedItems.end(), consumer.PoppedItems().cbegin(), consumer.PoppedItems().cend());
      }
      std::sort(poppedItems.begin(), poppedItems.end());

      TSM_ASSERT        (context.c_str(), !poppedItems.empty());
      TSM_ASSERT_EQUALS (context.c_str(), poppedItems.size(), itemsCount);
      Check_Content     (__LINE__, poppedItems);
    } while (consumerFamished && (--tryCount != 0));

    TSM_ASSERT_FALSE("Workload should be shared by all threads (at least minimally)", consumerFamished);
  }

  private:
  static constexpr DataTransferStrategy strategy = TDataTransferStrategy;
};


} // End of unnamed namespace

//! Constructor...
//!
UT_MTQueue::UT_MTQueue ()
{
}



//! Destructor...
//!
UT_MTQueue::~UT_MTQueue ()
{
}


//! Initializes test (called for each test)
//!
void UT_MTQueue::setUp ()
{
}


//! Cleanups test (called for each test)
//!
void UT_MTQueue::tearDown ()
{
}

//! Checks default constructor
//!
void UT_MTQueue::test_DefaultConstructor ()
{
  // ---------------- Exercise
  //
  MTQueue<string> sut;

  // ---------------- Verify
  //
  TS_ASSERT(sut.IsEmpty());
}


//! Checks push when the queue is empty and there is no concurrent pop
//!
void UT_MTQueue::test_Push_When_Empty_No_Pop()
{
  // ---------------- Setup
  //
  string          item = "This is 1st item";
  MTQueue<string> sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING(sut.Push(item));

  // ---------------- Verify
  //
  TS_ASSERT(!sut.IsEmpty());
}


//! Checks push of spy instance to check copy of it
//!
void UT_MTQueue::test_Push_With_SpyItem_Copy_1 ()
{
  // ---------------- Setup
  //
  StaticTestSpy item(9);

  { // This scope is intended to count spy destructors calls
    MTQueue<StaticTestSpy> sut;

    StaticTestSpy::ResetAll();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING(sut.Push(item));

    // ---------------- Verify
    //
    TS_ASSERT        (!sut.IsEmpty());
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(), 1);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(), 1);
  }

  // ---------------- Verify destructors call
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(), 1);
}


//! Checks push of N spy instances to check copy of them
//!
void UT_MTQueue::test_Push_With_SpyItem_Copy_N ()
{
  // ---------------- Setup
  //
  StaticTestSpy item_1(1);
  StaticTestSpy item_2(2);
  StaticTestSpy item_3(3);
  StaticTestSpy item_4(4);
  StaticTestSpy item_5(5);
  StaticTestSpy item_6(6);
  StaticTestSpy item_7(7);
  StaticTestSpy item_8(8);
  StaticTestSpy item_9(9);

  { // This scope is intended to count spy destructors calls
    MTQueue<StaticTestSpy> sut;

    StaticTestSpy::ResetAll();

    // ---------------- Exercise
    //
    sut.Push(item_1);
    sut.Push(item_2);
    sut.Push(item_3);
    sut.Push(item_4);
    sut.Push(item_5);
    sut.Push(item_6);
    sut.Push(item_7);
    sut.Push(item_8);
    sut.Push(item_9);

    // ---------------- Verify
    //
    TS_ASSERT        (!sut.IsEmpty());
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(), 9);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(), 9);
  }

  // ---------------- Verify destructors call
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(), 9);   // Only count those in the queue
}



//! Checks push of spy instance to check move of it
//!
void UT_MTQueue::test_Push_With_SpyItem_Move_1 ()
{
  // ---------------- Setup
  //
  { // This scope is intended to count spy destructors calls
    MTQueue<StaticTestSpy> sut;

    StaticTestSpy::ResetAll();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(9)));

    // ---------------- Verify
    //
    TS_ASSERT        (!sut.IsEmpty());

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(),  2);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbValueConstructorCall(), 1);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveConstructorCall(),  1);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(),  0);

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyAssignmentCall(), 0);
  }

  // ---------------- Verify destructors call
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(), 2);
}


//! Checks push of N spy instances to check move of them
//!
void UT_MTQueue::test_Push_With_SpyItem_Move_N ()
{
  // ---------------- Setup
  //
  { // This scope is intended to count spy destructors calls
    MTQueue<StaticTestSpy> sut;

    StaticTestSpy::ResetAll();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(1)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(2)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(3)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(4)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(5)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(6)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(7)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(8)));
    TS_ASSERT_THROWS_NOTHING(sut.Push(StaticTestSpy(9)));

    // ---------------- Verify
    //
    TS_ASSERT        (!sut.IsEmpty());

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(),  18);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbValueConstructorCall(), 9);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveConstructorCall(),  9);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(),  0);

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyAssignmentCall(), 0);
  }

  // ---------------- Verify destructors call
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(), 18);    // 9 for temporaries and 9 for those in the queue
}



//! Checks Emplace "push" of a spy instance to check "in place" construction
//!
void UT_MTQueue::test_Push_With_SpyItem_Emplace ()
{
  // ---------------- Setup
  //
  { // This scope is intended to count spy destructors calls
    MTQueue<StaticTestSpy> sut;

    StaticTestSpy::ResetAll();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING(sut.Emplace(12));

    // ---------------- Verify
    //
    TS_ASSERT        (!sut.IsEmpty());

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(),  1);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbValueConstructorCall(), 1);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveConstructorCall(),  0);
    TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyConstructorCall(),  0);

    TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyAssignmentCall(), 0);
  }

  // ---------------- Verify destructors call
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(), 1);
}





//! Checks push when the queue is not empty and there is no concurrent pop
//!
void UT_MTQueue::test_Push_When_NotEmpty_No_Pop()
{
  // ---------------- Setup
  //
  string item_1 = "This is 1st item";
  string item_2 = "This is 2nd item";

  MTQueue<string> sut;
  sut.Push(item_1); // Make it not empty


  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING(sut.Push(item_2));

  // ---------------- Verify
  //
  TS_ASSERT(!sut.IsEmpty());
}



//! Checks pop with timeout when is empty and there is no concurent push
//!
void UT_MTQueue::test_Pop_Timeout_When_Empty()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;
  string          item;
  auto            timeout = 5ms;

  // ---------------- Exercise
  //
  auto status = sut.Pop(item, timeout);

  // ---------------- Verify
  //
  TS_ASSERT        (sut.IsEmpty());
  TS_ASSERT_EMPTY  (item);
  TS_ASSERT_EQUALS (status, std::cv_status::timeout);
}


//! Checks pop with timeout when there is 1 item and there is no concurent push
//!
void UT_MTQueue::test_Pop_Timeout_When_1_Item ()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;
  string          pushedItem = "Pushed item";
  string          poppedItem;
  auto            timeout = 1ms;

  sut.Push(pushedItem);

  // ---------------- Exercise
  //
  auto status = sut.Pop(poppedItem, timeout);

  // ---------------- Verify
  //
  TS_ASSERT        (sut.IsEmpty());
  TS_ASSERT_EQUALS (poppedItem, pushedItem);
  TS_ASSERT_EQUALS (status,     std::cv_status::no_timeout);
}


//! Checks pop with timeout when there is 2 items and there is no concurent push
//!
void UT_MTQueue::test_Pop_Timeout_When_2_Items ()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;
  string          pushedItem_1 = "Pushed item 1";
  string          pushedItem_2 = "Pushed item 2";
  string          poppedItem;
  auto            timeout = 1ms;

  sut.Push(pushedItem_1);
  sut.Push(pushedItem_2);

  // ---------------- Exercise
  //
  auto status = sut.Pop(poppedItem, timeout);

  // ---------------- Verify
  //
  TS_ASSERT(!sut.IsEmpty());
  TS_ASSERT_EQUALS(poppedItem, pushedItem_1);
  TS_ASSERT_EQUALS(status,     std::cv_status::no_timeout);
}

//! Checks pop with timeout when there is 2 'Spy' items and there is no concurent push
//!
void UT_MTQueue::test_Pop_Timeout_When_2_SpyItems ()
{
  // ---------------- Setup
  //
  MTQueue<StaticTestSpy> sut;
  sut.Emplace(45);
  sut.Emplace(23);

  StaticTestSpy poppedItem;
  auto          timeout = 1ms;
  StaticTestSpy::ResetAll();

  // ---------------- Exercise
  //
  auto status = sut.Pop(poppedItem, timeout);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbAnyConstructorsCall(), 0);
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbCopyAssignmentCall(),  0);
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbMoveAssignmentCall(),  1);
  TS_ASSERT_EQUALS (StaticTestSpy::GetNbDestructorCall(),      1);

  TS_ASSERT        (!sut.IsEmpty());
  TS_ASSERT_EQUALS (poppedItem, StaticTestSpy(45));
  TS_ASSERT_EQUALS (status,     std::cv_status::no_timeout);
}


//! Checks pop with timeout when there was 2 items, popping the 2nd one and there is no concurent push
//!
void UT_MTQueue::test_Pop_Timeout_When_Becomes_Empty ()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;
  string          pushedItem_1 = "Pushed item 1";
  string          pushedItem_2 = "Pushed item 2";
  string          poppedItem;
  auto             timeout = 1ms;
  sut.Push(pushedItem_1);
  sut.Push(pushedItem_2);
  sut.Pop(poppedItem);      // Remove first pushed item

  // ---------------- Exercise
  //
  auto status = sut.Pop(poppedItem, timeout);

  // ---------------- Verify
  //
  TS_ASSERT(sut.IsEmpty());
  TS_ASSERT_EQUALS(poppedItem, pushedItem_2);
  TS_ASSERT_EQUALS(status,     std::cv_status::no_timeout);
}

//! Checks push from another thread when the queue is empty
//!
void UT_MTQueue::test_Push_When_DualThreads_Empty_No_Pop()
{
  // ---------------- Setup
  //
  string          item = "This is 1st item";
  MTQueue<string> sut;

  // ---------------- Exercise
  //
  auto otherThread = std::thread([&sut, item]() { sut.Push(item); });

  // ---------------- Verify
  //
  otherThread.join();
  TS_ASSERT(!sut.IsEmpty());
}


//! Checks push from another thread when the queue is empty
//!
void UT_MTQueue::test_Push_When_DualThreads_NotEmpty_No_Pop()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;

  // ---------------- Exercise
  //
  auto otherThread = std::thread([&sut]()
                     {
                       string pushedItem_1 = "This is 1st item";
                       string pushedItem_2 = "This is 2nd item";

                       sut.Push(pushedItem_1);
                       sut.Push(pushedItem_2);
                     });

  // ---------------- Verify
  //
  otherThread.join();
  TS_ASSERT(!sut.IsEmpty());
}


//! Checks pop from another thread when the queue is empty
//!
void UT_MTQueue::test_Pop_When_DualThreads_And_Empty()
{
  // ---------------- Setup
  //
  MTQueue<string> sut;
  std::cv_status   status;
  auto consumerLamba = [&sut, &status]()
                       {
                         string poppedItem;
                         auto    timeout = std::chrono::milliseconds(5);

                         status = sut.Pop(poppedItem, timeout);
                       };

  // ---------------- Exercise
  //
  auto consumerThread = std::thread(consumerLamba);

  // ---------------- Verify
  //
  consumerThread.join();
  TS_ASSERT        (sut.IsEmpty());
  TS_ASSERT_EQUALS (status, std::cv_status::timeout);
}


//! Checks pop from another thread when there is 1 item in the queue before pop is called
//!
void UT_MTQueue::test_Pop_When_DualThreads_1_Item_Before()
{
  // ---------------- Setup
  //
  MTQueue<int>   sut;
  int            pushedItem = 69;

  sut.Push(pushedItem);

  int            poppedItem;
  std::cv_status status;
  auto           consumerLamba = [&sut, &status, &poppedItem]()
                                 {
                                   auto    timeout = std::chrono::milliseconds(5);

                                   status = sut.Pop(poppedItem, timeout);
                                 };

  // ---------------- Exercise
  //
  auto consumerThread = std::thread(consumerLamba);

  // ---------------- Verify
  //
  consumerThread.join();
  TS_ASSERT        (sut.IsEmpty());
  TS_ASSERT_EQUALS (status, std::cv_status::no_timeout);
  TS_ASSERT_EQUALS (pushedItem, poppedItem);
}


//! Checks pop from another thread when there is 1 item in the queue only after pop is called
//!
void UT_MTQueue::test_Pop_When_DualThreads_1_Item_After()
{
  // ---------------- Setup
  //
  MTQueue<int>   sut;
  int            pushedItem = 69;
  int            poppedItem;
  std::cv_status status;
  auto           consumerLamba = [&sut, &status, &poppedItem]()
                                 {
                                   auto    timeout = std::chrono::milliseconds(5);

                                   status = sut.Pop(poppedItem, timeout);
                                 };

  // ---------------- Exercise
  //
  auto consumerFuture = std::async(std::launch::async, consumerLamba);
  sut.Push(pushedItem);

  // ---------------- Verify
  //
  TS_ASSERT (consumerFuture.valid());
  consumerFuture.wait();

  TS_ASSERT        (sut.IsEmpty());
  TS_ASSERT_EQUALS (status, std::cv_status::no_timeout);
  TS_ASSERT_EQUALS (pushedItem, poppedItem);
}


//! Checks in context of high contentions with 1 producer and 1 consumer
//!
void UT_MTQueue::test_When_1_Producers_1_Consumers()
{
  // ---------------- Setup
  //
  using              TPayload      = int;
  MTQueue<TPayload>  sut;
  std::atomic<bool>  moreToProduce (true);
  int                itemsCount    = 200;
  std::promise<void> promise;           // This is for threads synchronization
  auto               threadSynchronizer  = promise.get_future().share();
  Consumer<TPayload> consumer(sut, moreToProduce, threadSynchronizer);
  Producer<TPayload> producer(sut, threadSynchronizer);

  // ---------------- Exercise
  //
  auto consumerThread = std::thread(std::ref(consumer));
  auto producerThread = std::thread(std::ref(producer), 0, itemsCount, 1);
  promise.set_value();  // Synchronize all threads

  // ---------------- Verify
  //
  producerThread.join();
  moreToProduce = false;
  consumerThread.join();

  TS_ASSERT        (sut.IsEmpty());

  auto poppedItems = consumer.PoppedItems();

  TS_ASSERT_LESS_THAN (consumer.Timeouts(), 5);
  TS_ASSERT           (!poppedItems.empty());
  TS_ASSERT_EQUALS    (poppedItems.size(), itemsCount);
  Check_Content       (__LINE__, poppedItems);
}


//! Checks in context of high contentions with 1 producer and N consumers
//!
//! @note It is difficult to check that all consumers take part of the share
//!       So test is tried multiple time until all consumer get some data or loop count is exhausted
//!
void UT_MTQueue::test_When_1_Producers_N_Consumers()
{
  uint32_t tryCount         = 10u;
  auto     consumerFamished = false;

  do
  {
    consumerFamished = false;

    // ---------------- Setup
    //
    using                      TPayload      = int;
    MTQueue<TPayload>          sut;
    std::atomic<bool>          moreToProduce (true);
    int                        itemsCount    = 200;
    std::promise<void>         promise;           // This is for threads synchronization
    auto                       threadSynchronizer  = promise.get_future().share();
    Producer<TPayload>         producer(sut, threadSynchronizer);
    vector<Consumer<TPayload>> consumers(3, Consumer<TPayload>(sut, moreToProduce, threadSynchronizer));

    // ---------------- Exercise
    //
    vector<std::thread> consumerThreads;
    for (auto& consumer : consumers)
    {
      consumerThreads.emplace_back(std::ref(consumer));
    }

    auto producerThread = std::thread(std::ref(producer), 0, itemsCount, 1);

    promise.set_value();  // Synchronize all threads

    // ---------------- Release threads
    //
    producerThread.join();
    moreToProduce = false;
    for (auto& consumerThread : consumerThreads)
    {
      consumerThread.join();
    }

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(false);

    TS_ASSERT        (sut.IsEmpty());

    vector<TPayload> poppedItems;
    auto maximalTimeouts =  itemsCount / consumers.size();
    for (const auto& consumer : consumers)
    {
      if (consumer.PoppedItems().size() == 0)
      {
        consumerFamished = true;
        std::this_thread::sleep_for(27ms); // Wait a little while to try next time in a "different threading context"
      }
      TS_ASSERT_LESS_THAN_EQUALS (consumer.Timeouts(), maximalTimeouts);

      poppedItems.insert(poppedItems.end(), consumer.PoppedItems().cbegin(), consumer.PoppedItems().cend());
    }
    std::sort(poppedItems.begin(), poppedItems.end());

    TS_ASSERT           (!poppedItems.empty());
    TS_ASSERT_EQUALS    (poppedItems.size(), itemsCount);
    Check_Content       (__LINE__, poppedItems);
  } while (consumerFamished && (--tryCount != 0));

  TSM_ASSERT_FALSE("Workload should be shared by all threads (at least minimally)", consumerFamished);
}


//! Checks in context of high contentions with N producers and 1 consumer
//!
void UT_MTQueue::test_When_N_Producers_1_Consumers()
{
  // ---------------- Setup
  //
  using                      TPayload      = int;
  MTQueue<TPayload>          sut;
  std::atomic<bool>          moreToProduce (true);
  std::promise<void>         promise;           // This is for threads synchronization
  auto                       threadSynchronizer  = promise.get_future().share();
  Consumer<TPayload>         consumer(sut, moreToProduce, threadSynchronizer);
  vector<Producer<TPayload>> producers(3, Producer<TPayload>(sut, threadSynchronizer));
  auto                       itemsCount    = 500;
  auto                       start         = 0;
  auto                       end           = itemsCount;
  auto                       increment     = producers.size();

  // ---------------- Exercise
  //
  auto consumerThread = std::thread(std::ref(consumer));

  vector<std::thread> producerThreads;
  for (auto& producer : producers)
  {
    producerThreads.emplace_back(std::ref(producer), start, end, increment);
    ++start;
  }
  promise.set_value();  // Synchronize all threads

  // ---------------- Verify
  //
  for (auto& producerThread : producerThreads)
  {
    producerThread.join();
  }
  moreToProduce = false;
  consumerThread.join();

  TS_ASSERT        (sut.IsEmpty());

  auto poppedItems = consumer.PoppedItems();
  auto maxTimeouts = itemsCount / 100;  // The number is somewhat arbitrary (but it statically increase with itemsCount)

  std::sort(poppedItems.begin(), poppedItems.end());

  TS_ASSERT_LESS_THAN (consumer.Timeouts(), maxTimeouts);
  TS_ASSERT           (!poppedItems.empty());
  TS_ASSERT_EQUALS    (poppedItems.size(), itemsCount);
  Check_Content       (__LINE__, poppedItems);
}


//! Checks in context of high contentions with N producers and N consumers
//!
//! @note It is difficult to check that all consumers take part of the share
//!       So test is tried multiple time until all consumer get some data or loop count is exhausted
//!
void UT_MTQueue::test_When_N_Producers_N_Consumers()
{
  using TPayload  = int;
  using TProducer = Producer<TPayload>;
  using TConsumer = Consumer<TPayload>;

  uint32_t tryCount         = 10u;
  auto     consumerFamished = false;
  do
  {
    consumerFamished = false;

    // ---------------- Setup
    //
    MTQueue<TPayload>  sut;
    std::atomic<bool>  moreToProduce (true);
    std::promise<void> promise;           // This is for threads synchronization
    auto               threadSynchronizer  = promise.get_future().share();
    vector<TConsumer>  consumers(3, TConsumer(sut, moreToProduce, threadSynchronizer));
    vector<TProducer>  producers(3, TProducer(sut, threadSynchronizer));
    auto               itemsCount = 500U;
    auto               start      = 0;
    auto               end        = itemsCount;
    auto               increment  = producers.size();

    // ---------------- Exercise
    //
    vector<std::thread> consumerThreads;
    for (auto& consumer : consumers)
    {
      consumerThreads.emplace_back(std::ref(consumer));
    }

    vector<std::thread> producerThreads;
    for (auto& producer : producers)
    {
      producerThreads.emplace_back(std::ref(producer), start, end, increment);
      ++start;
    }
    promise.set_value();  // Synchronize all threads

    // ---------------- Verify
    //
    for (auto& producerThread : producerThreads)
    {
      producerThread.join();
    }
    moreToProduce = false;

    for (auto& consumerThread : consumerThreads)
    {
      consumerThread.join();
    }

    TS_ASSERT (sut.IsEmpty());

    vector<TPayload>  poppedItems;
    auto maxTimeouts  = itemsCount/consumers.size();
    for (const auto& consumer : consumers)
    {
      if (consumer.PoppedItems().size() == 0)
      {
        consumerFamished = true;
        std::this_thread::sleep_for(27ms); // Wait a little while to try next time in a "different threading context"
      }
      TS_ASSERT_LESS_THAN (consumer.Timeouts(), maxTimeouts);

      poppedItems.insert(poppedItems.end(), consumer.PoppedItems().cbegin(), consumer.PoppedItems().cend());
    }
    std::sort(poppedItems.begin(), poppedItems.end());

    TS_ASSERT_EQUALS    (poppedItems.size(), itemsCount);
    Check_Content       (__LINE__, poppedItems);
  } while (consumerFamished && (--tryCount != 0));

  TSM_ASSERT_FALSE("Workload should be shared by all threads (at least minimally)", consumerFamished);
}




//! Checks in context of high contentions with N producers and N consumers
//! using spy items and copy behavior for push operations
//!
void UT_MTQueue::test_When_N_Producers_N_Consumers_SpyItems_Copy ()
{
  StaticTestSpyBased_MultiThread_Tester<DataTransferStrategy::Copy>().test(2, 2, 456);
}


//! Checks in context of high contentions with N producers and N consumers
//! using spy items and move behavior for push operations
//!
void UT_MTQueue::test_When_N_Producers_N_Consumers_SpyItems_Move ()
{
  StaticTestSpyBased_MultiThread_Tester<DataTransferStrategy::Move>().test(2, 2, 345);
}

//! Checks in context of high contentions with N producers and N consumers
//! using spy items and emplace behavior for push operations
//!
void UT_MTQueue::test_When_N_Producers_N_Consumers_SpyItems_Emplace ()
{
  StaticTestSpyBased_MultiThread_Tester<DataTransferStrategy::Emplace>().test(2, 2, 351);
}

//===========================================================================
// End of UT_MTQueue.cpp
//===========================================================================
