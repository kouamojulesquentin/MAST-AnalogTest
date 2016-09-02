//===========================================================================
//                           SystemModelCheckResult.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckResult.hpp
//!
//! Declares SystemModelCheckResult class
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_
  #define SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_

#include "Platform.hpp"

#include <string>
#include <sstream>
#include <cstdint>


namespace mast
{
//! Collects warning and error while SystemModel is being checked
//!
class DLL_EXPORT SystemModelCheckResult final
{
  public:
  ~SystemModelCheckResult() = default;
  SystemModelCheckResult()  = default;

  //! Merges other results into this
  //!
  void Merge(const SystemModelCheckResult& other);

  //! Returns true when it equals to another SystemModelCheckResult
  //!
  bool operator==(const SystemModelCheckResult& rhs) const
  {
    return    (infosCount    == rhs.infosCount)
           && (warningsCount == rhs.warningsCount)
           && (errorsCount   == rhs.errorsCount)
           && (infos         == rhs.infos)
           && (warnings      == rhs.warnings)
           && (errors        == rhs.errors);
  }

  //! Returns true when it differs from another SystemModelCheckResult
  //!
  bool operator!=(const SystemModelCheckResult& rhs) const
  {
    return !operator==(rhs);
  }

  bool HasIssues()   const { return (warningsCount != 0)  || (errorsCount != 0); }
  bool HasWarnings() const { return (warningsCount != 0); }
  bool HasErrors()   const { return (errorsCount   != 0); }

  std::string MakeReport() const; //!< Returns a string representing the check result

  static const SystemModelCheckResult None; //!< Instance to compare for no error (very useful in unit tests along with traits to show the issues)

  uint32_t    infosCount    = 0; //!< Total number of collected info messages
  uint32_t    warningsCount = 0; //!< Total number of collected warnings messages
  uint32_t    errorsCount   = 0; //!< Total number of collected errors messages
  std::string infos;             //!< Collected infos
  std::string warnings;          //!< Collected warnings
  std::string errors;            //!< Collected errors
};
//
//  End of SystemModelCheckResult class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELCHECKRESULT_H__FFBEA6CE_417E_43A0_32B5_EA2A1A0AF395__INCLUDED_

//===========================================================================
// End of SystemModelCheckResult.hpp
//===========================================================================
