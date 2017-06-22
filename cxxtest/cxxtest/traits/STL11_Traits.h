//===========================================================================
//                           STL11_Traits.h
//===========================================================================
//
//! @file STL11_Traits.h
//!
//! Declares traits for C++11 STL classes and enums
//!
//===========================================================================


#ifndef STL11_TRAITS_H__576E9F15_C7F2_4BD4_4799_1E4CDD745360__INCLUDED_
  #define STL11_TRAITS_H__576E9F15_C7F2_4BD4_4799_1E4CDD745360__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <condition_variable>
#include <future>
#include <utility>
#include <functional>
#include <chrono>


CXXTEST_ENUM_TRAITS(std::cv_status,
                    CXXTEST_ENUM_MEMBER(std::cv_status::no_timeout)
                    CXXTEST_ENUM_MEMBER(std::cv_status::timeout)
                   );


CXXTEST_ENUM_TRAITS(std::future_status,
                   CXXTEST_ENUM_MEMBER(std::future_status::deferred)
                   CXXTEST_ENUM_MEMBER(std::future_status::ready)
                   CXXTEST_ENUM_MEMBER(std::future_status::timeout)
                  );


namespace CxxTest
{

// ---------------- tuple<>
//
template<class... Types>
class ValueTraits<std::tuple<Types...>>
{
  public:
  ValueTraits(const std::tuple<Types...>& instance)
  {
    m_asString = ToStdString(instance);
  }
  const char* asString() const { return m_asString.c_str(); }

  private:

  template<class Tuple, std::size_t... Is>
  static void PrintTupleFields(std::ostringstream& os, const Tuple& instance, std::index_sequence<Is...>)
  {
      using swallow = int[]; // guaranties left to right order
      (void)swallow {0, (void(os << (Is == 0? "" : ", ") << traits(std::get<Is>(instance)).asString()), 0)...};
  }

  static std::string ToStdString(const std::tuple<Types...>& instance)
  {
    using std::string;
    std::ostringstream os;

    os << "tuple(";
    PrintTupleFields(os, instance, std::index_sequence_for<Types...>{});
    os << ")";
    return os.str();
  }
  std::string m_asString;
};


//! Provides traits from wrapped reference type
//!
template<typename T>
class ValueTraits<std::reference_wrapper<T>>
{
  public:
  ValueTraits(const std::reference_wrapper<T>& instance)
  {
    m_asString = ToStdString(instance);
  }
  const char* asString() const { return m_asString.c_str(); }

  private:
  static std::string ToStdStringImpl(const T& instance)
  {
    return std::string(traits(instance).asString());
  }

  static std::string ToStdString(const std::reference_wrapper<T>& instance)
  {
    return ToStdStringImpl(instance);
  }

  std::string m_asString;
};


//! Provides traits for std::chrono::milliseconds
//!
template<>
class ValueTraits<std::chrono::milliseconds>
{
  public:
  ValueTraits(const std::chrono::milliseconds& instance)
  {
    m_asString.append(std::to_string(instance.count())).append("ms");
  }

  const char* asString() const { return m_asString.c_str(); }

  private:
  std::string m_asString;
};

} // End of namespace CxxTest


#endif  // not defined STL11_TRAITS_H__576E9F15_C7F2_4BD4_4799_1E4CDD745360__INCLUDED_
//===========================================================================
// End of STL11_Traits.h
//===========================================================================


