//===========================================================================
//                           Commons_C.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Commons_C.hpp
//!
//! Declares commonalities to implement 'C' API
//!
//===========================================================================


#ifndef COMMONS_C_H__3F71BC5D_1592_40FA_F9AB_AD1EB9ACEE39__INCLUDED_
  #define COMMONS_C_H__3F71BC5D_1592_40FA_F9AB_AD1EB9ACEE39__INCLUDED_
#define CATCH_ALL(retCode)\
catch(std::invalid_argument& exc) { retCode = ErrorCode::InvalidArgument;  SetErrorMessage(exc.what()); }\
catch(std::out_of_range&     exc) { retCode = ErrorCode::OutOfRange;       SetErrorMessage(exc.what()); }\
catch(std::logic_error&      exc) { retCode = ErrorCode::LogicError;       SetErrorMessage(exc.what()); }\
catch(std::runtime_error&    exc) { retCode = ErrorCode::RuntimeError;     SetErrorMessage(exc.what()); }\
catch(std::exception&        exc) { retCode = ErrorCode::StdException;     SetErrorMessage(exc.what()); }\
catch(...)                        { retCode = ErrorCode::UndefinedFailure; SetErrorMessage("Got non C++ std exception"); }

#define TRY_CATCH_ALL(retCode, block)\
try\
{\
  ClearErrorMessage();\
  block;\
}\
CATCH_ALL(retCode)


#endif  // not defined COMMONS_C_H__3F71BC5D_1592_40FA_F9AB_AD1EB9ACEE39__INCLUDED_
//===========================================================================
// End of Commons_C.hpp
//===========================================================================



