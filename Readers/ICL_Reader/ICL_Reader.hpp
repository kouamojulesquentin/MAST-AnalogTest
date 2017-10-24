//===========================================================================
//                           ICL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Reader.hpp
//!
//! Declares  class for managing parsing of ICL files
//!
//===========================================================================

#ifndef ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
  #define ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_

#include "Reader.hpp"
#include <istream>
#include <memory>

namespace mast
{
  class SystemModel;
}

namespace ICL
{
class ICL_Reader final : public Parsers::Reader
{
  public:
  ~ICL_Reader() = default;
  ICL_Reader()  = delete;
  ICL_Reader(std::shared_ptr<mast::SystemModel> sm);

  protected:
  virtual void Parse_Impl(std::istream& stream) override;
};
} // End of: namespace ICL

#endif  // not defined ICL_READER_H__9F5136A9_6376_4208_9286_B2DFD0D4D57A__INCLUDED_
