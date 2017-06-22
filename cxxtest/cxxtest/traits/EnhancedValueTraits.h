//===========================================================================
//                           EnhancedValueTraits.h
//===========================================================================
//
//! @file EnhancedValueTraits.h
//!
//! Declares EnhancedValueTraits class
//!
//===========================================================================

#ifndef ENHANCEDVALUETRAITS_H__86D27D99_AE8E_4834_8BAB_E5209F70C035__INCLUDED_
  #define ENHANCEDVALUETRAITS_H__86D27D99_AE8E_4834_8BAB_E5209F70C035__INCLUDED_


//! Base class to declare ValueTraits that implement "enhanced" feature like
//! indent level and embedding aware formatting
//!
//! @note It is expected to be used in templatized ValueTraits in context of meta template programming
//! @note Derived ValueTraits classes are expected to provide a constructor with the following parameters:
//!       (const ItemType& instance, const std::string& indent = "", bool embeddedInContainer = false)
//!       Default parameters are required to be compatible with Cxxtest macros that are unaware of those
//!       "enhanced" ValueTraits
//! @note Embedded ValueTraits are expected to:
//!         - Not insert new lines before and after their "stringified" value.
//!         - Ignore the indent level for their first line but use it for the following ones
//!
class EnhancedValueTraits
{
};
//
//  End of EnhancedValueTraits class declaration
//---------------------------------------------------------------------------

#endif  // not defined ENHANCEDVALUETRAITS_H__86D27D99_AE8E_4834_8BAB_E5209F70C035__INCLUDED_

//===========================================================================
// End of EnhancedValueTraits.h
//===========================================================================
