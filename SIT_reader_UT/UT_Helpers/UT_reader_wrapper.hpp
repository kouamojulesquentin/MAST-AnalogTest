//===========================================================================
//                           UT_reader_wrapper.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_reader_wrapper.hpp
//!
//! Declares a common wrapping to call SIT_reader for UTs
//!
//===========================================================================


#ifndef UT_reader_wrapper_H_INCLUDED_
  #define UT_reader_wrapper_H__INCLUDED_

#include <string>
#include <utility>

namespace test
{
//! Helpers to test SIT_reader
//!
class UT_reader_wrapper final
{
  public:
   UT_reader_wrapper() = default;
  ~UT_reader_wrapper() = default;

  static std::pair<std::string, std::shared_ptr<mast::SystemModelNode>> run_parser_for_UT(const std::string& input_SIT, std::shared_ptr<mast::SystemModel> sm);
};
//
//  End of UT_reader_wrapper class declaration
//---------------------------------------------------------------------------

}
#endif  // not defined UT_reader_wrapper_H_INCLUDED_
//===========================================================================
// End of UT_reader_wrapper.hpp
//===========================================================================
