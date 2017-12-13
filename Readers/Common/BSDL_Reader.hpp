//===========================================================================
//                           BSDL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BSDL_Reader.hpp
//!
//! Declares BSDL_Reader class
//!
//===========================================================================

#ifndef BSDL_READER_H__C7B0CDAF_5D85_4B03_BCA4_3891FE66E42F__INCLUDED_
  #define BSDL_READER_H__C7B0CDAF_5D85_4B03_BCA4_3891FE66E42F__INCLUDED_

#include <vector>
#include <experimental/string_view>

namespace Parsers
{
//! Reader for BSDL files
//!
//! @note It only reads what is necessary for creation of JTAG TAP in ICL AccessLink context
//!
class BSDL_Reader final
{
  // ---------------- Public Methods
  //
  public:
  ~BSDL_Reader() = default;
  BSDL_Reader()  = default;

  //! Parses BSDL content with special care of specified instructions
  //!
  void Parse(std::experimental::string_view                     bsdlContent,
             const std::vector<std::experimental::string_view>& instructionsNames);

  //! Returns JTAG TAP IR register bits count
  //!
  size_t IrBitsCount() const { return m_irBitsCount; }

  //! Returns values associated with instruction name (given as Parse parameter)
  //!
  //! @note First one is always for bypass
  const std::vector<std::experimental::string_view>& SelectTable() const { return m_selectTable; }

  // ---------------- Private Fields
  //
  private:
  size_t                                      m_irBitsCount = 0u; //!< JTAG TAP IR register bits count
  std::vector<std::experimental::string_view> m_selectTable;      //!< Values associated with given instruction name (first one is always for bypass)
};
//
//  End of BSDL_Reader class declaration
//---------------------------------------------------------------------------
} // End of namespace ICL




#endif  // not defined BSDL_READER_H__C7B0CDAF_5D85_4B03_BCA4_3891FE66E42F__INCLUDED_

//===========================================================================
// End of BSDL_Reader.hpp
//===========================================================================
