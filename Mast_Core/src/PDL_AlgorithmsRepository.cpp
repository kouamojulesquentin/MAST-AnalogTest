//===========================================================================
//                           PDL_AlgorithmsRepository.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_AlgorithmsRepository.cpp
//!
//! Implements class PDL_AlgorithmsRepository
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <memory>

using std::string;
using std::unique_ptr;

using namespace mast;
using PDL_Algorithm_t = PDL_AlgorithmsRepository::PDL_Algorithm_t;


//! Initializes instance
PDL_AlgorithmsRepository::PDL_AlgorithmsRepository ()
{
}
//
//  End of: PDL_AlgorithmsRepository::PDL_AlgorithmsRepository
//---------------------------------------------------------------------------



//! Creates an PathSelector using registered creation function
//!
//! @param algorithmName  A name that identified registered PDL algorithm function
//!
PDL_Algorithm_t PDL_AlgorithmsRepository::GetAlgorithm (const string& algorithmName) const
{
  CHECK_PARAMETER_NOT_EMPTY(algorithmName, "Cannot get an PDL algorithm with no name");

  auto pos = m_algorithms.find(algorithmName);
  if (pos == m_algorithms.cend())
  {
    auto message = "There is no algorithm registered with name: "s;
    LOG(ERROR_LVL) << message << algorithmName;
    CHECK_FAILED(message.append(algorithmName));
  }

  return pos->second;
}
//
//  End of: PDL_AlgorithmsRepository::GetAlgorithm
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
PDL_AlgorithmsRepository& PDL_AlgorithmsRepository::Instance ()
{
  static auto instance = unique_ptr<PDL_AlgorithmsRepository>(new PDL_AlgorithmsRepository());

  return *instance.get();
}
//
//  End of: PDL_AlgorithmsRepository::Instance
//---------------------------------------------------------------------------


//! Registers a PDL algorithm function
//!
//! @note If an algorithm  already exists with the same name, it is replaced with the new one
//!
//! @param algorithmName  Name associated with the PDL algorithm function (case sensitive)
//! @param pdlAlgorithm   PDL algorithm function
void PDL_AlgorithmsRepository::RegisterAlgorithm (const string& algorithmName, PDL_Algorithm_t pdlAlgorithm)
{
  CHECK_PARAMETER_NOT_EMPTY(algorithmName, "Cannot register a PDL algorithm with no name");

  if (m_algorithms.find(algorithmName) != m_algorithms.cend())
  {
    LOG(WARNING) << "A PDL algorithm is already registered with the name: " << algorithmName;
  }

  LOG(INFO) << "Registering PDL algorithm with the name: " << algorithmName;
  m_algorithms[algorithmName] = pdlAlgorithm;
}
//
//  End of: PDL_AlgorithmsRepository::RegisterAlgorithm
//---------------------------------------------------------------------------


//===========================================================================
// End of PDL_AlgorithmsRepository.cpp
//===========================================================================
