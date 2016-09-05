//===========================================================================
//                           ErrorCode_C.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ErrorCode_C.h
//!
//! Declares ErrorCode enum
//!
//===========================================================================


#ifndef ERRORCODE_C_H__48DD9310_1431_487D_1284_81D741B9345F__INCLUDED_
  #define ERRORCODE_C_H__48DD9310_1431_487D_1284_81D741B9345F__INCLUDED_

extern "C"
{
  //! Error codes for 'C' API
  //!
  enum ErrorCode_t
  {
    Ok               =  0,
    Failure          = -1,
    UndefinedFailure = -2,
    StdException     = -3,
    RegexException   = -4,
    RuntimeError     = -5,
    OutOfRange       = -6,
    LogicError       = -7,
    InvalidArgument  = -8,
    InvalidModel     = -9,
  };
  typedef enum ErrorCode_t ErrorCode;
}
#endif  // not defined ERRORCODE_C_H__48DD9310_1431_487D_1284_81D741B9345F__INCLUDED_
//===========================================================================
// End of ErrorCode_C.h
//===========================================================================



