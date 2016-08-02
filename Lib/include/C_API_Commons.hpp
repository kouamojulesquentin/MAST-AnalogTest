//===========================================================================
//                           C_API_Commons.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file C_API_Commons.hpp
//!
//! Declares commonalities to implement 'C' API
//!
//===========================================================================


#ifndef C_API_COMMONS_H__E0B3788E_4855_41D3_C1BA_D6E49F3B5EE__INCLUDED_
  #define C_API_COMMONS_H__E0B3788E_4855_41D3_C1BA_D6E49F3B5EE__INCLUDED_



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


#endif  // not defined C_API_COMMONS_H__E0B3788E_4855_41D3_C1BA_D6E49F3B5EE__INCLUDED_
//===========================================================================
// End of C_API_Commons.hpp
//===========================================================================



