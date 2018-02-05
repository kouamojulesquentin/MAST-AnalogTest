//===========================================================================
//                           Readers_Common_Enums_Traits.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Readers_Common_Enums_Traits.hpp
//!
//! Declares cxxtest traits for Readers common enums. It enables CxxTest to display
//! enums using their names (instead of their integer values)
//!
//===========================================================================


#ifndef READERS_COMMON_ENUMS_TRAITS_H__28D56446_9A9_41CC_C880_9B738BE2BED6__INCLUDED_
  #define READERS_COMMON_ENUMS_TRAITS_H__28D56446_9A9_41CC_C880_9B738BE2BED6__INCLUDED_

#include "AST_Node.hpp"

#include <cxxtest/ValueTraits.h>

CXXTEST_ENUM_TRAITS(Parsers::Kind,
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Undefined)

           CXXTEST_ENUM_MEMBER(Parsers::Kind::AccessLink)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::AccessTogether)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Alias)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ApplyEndState)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Attribute)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::BsdlInstructionRef)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ConcatenatedNumbers)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::DefaultLoadValue)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::DataInPort)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::DataOutPort)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Enum_def)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Enum_item)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::BSDLEntity)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::InputPort)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::InputPortSource)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Instance)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::IntegerExprRef)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::IntegerLiteral)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::LocalParameter)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Module)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ModuleIdentifier)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::NameSpace)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Network)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Number_Binary)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Number_Decimal)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Number_Hexa)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Operator_Add)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Operator_Substract)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Operator_Multiply)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Operator_Divide)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Operator_Modulo)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Parameter)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Parameter_ref)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ParenthesizedExpr)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::PlaceHolder)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Port)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::RefEnum)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ResetValue)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScalarIdentifier)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanInPort)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanInSource)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanInterface)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanInterfaceRef)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanMux)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanMuxSelection)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanOutPort)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::ScanRegister)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Signal)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Source)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::String)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::TranslationUnit)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::Value_Bool)
           CXXTEST_ENUM_MEMBER(Parsers::Kind::VectorIdentifier)
          );


#endif  // not defined READERS_COMMON_ENUMS_TRAITS_H__28D56446_9A9_41CC_C880_9B738BE2BED6__INCLUDED_

//===========================================================================
// End of Readers_Common_Enums_Traits.hpp
//===========================================================================



