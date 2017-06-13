//===========================================================================
//                           PDL_AlgorithmsRepository.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_AlgorithmsRepository.hpp
//!
//! Declares PDL_AlgorithmsRepository class
//!
//===========================================================================

#ifndef PDL_ALGORITHMSREPOSITORY_H__658486EF_C341_465A_1B82_478BA5F8B5AD__INCLUDED_
  #define PDL_ALGORITHMSREPOSITORY_H__658486EF_C341_465A_1B82_478BA5F8B5AD__INCLUDED_

#include "Mast_Core_export.hpp"

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Repository for PDL algorithms functions
//!
//! @note PDL algorithms are registered/retrieved by their name (case sensitive)
//! @note PDL algorithms are usually provided by plugins that register
//!       their algorithm with this repository
//!
class MAST_CORE_EXPORT PDL_AlgorithmsRepository
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~PDL_AlgorithmsRepository() = default;

  //! Returns sole instance
  //!
  static PDL_AlgorithmsRepository& Instance();

  using PDL_Algorithm_t = std::function<void()>; //!< A PDL algorithm takes no parameters nor return anything

  //! Returns the number of PDL algorithm functions currently registered (associated with a name)
  //!
  //! @note Mainly intended for test/check purpose
  size_t RegisteredAlgorithmCount() const { return m_algorithms.size(); };

  //! Registers a PDL algorithm function
  //!
  void RegisterAlgorithm(const std::string& algorithmName, PDL_Algorithm_t pdlAlgorithm);

  //! Removes any, registered, PDL algorithms
  //!
  void Clear() { m_algorithms.clear(); }

  //! Creates an PathSelector using registered creation function
  //!
  PDL_Algorithm_t GetAlgorithm(const std::string& algorithmName) const;

  // ---------------- Private  Methods
  //
  private:
  PDL_AlgorithmsRepository();

  static std::unique_ptr<PDL_AlgorithmsRepository> CreateRepository();  //!< Called once to create single instance initialized with default creation function(s)

  // ---------------- Private  Fields
  //
  private:
  std::map<std::string, PDL_Algorithm_t> m_algorithms; //!< PDL algorithms
};
//
//  End of PDL_AlgorithmsRepository class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined PDL_ALGORITHMSREPOSITORY_H__658486EF_C341_465A_1B82_478BA5F8B5AD__INCLUDED_

//===========================================================================
// End of PDL_AlgorithmsRepository.hpp
//===========================================================================
