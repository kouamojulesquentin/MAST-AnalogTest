//===========================================================================
//                           AST.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.cpp
//!
//! Implements class AST
//!
//===========================================================================

#include "AST.hpp"
#include "AST_Attribute.hpp"
#include "AST_Module.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

#include "Utility.hpp"

using std::vector;
using std::string;
using std::experimental::string_view;
using std::make_unique;

using namespace Parsers;

//! Releases constructed AST nodes
//!
AST::~AST ()
{
}


//! Initializes AST
//!
AST::AST ()
{
}


//! Creates an AST_Attribute node
//!
//! @param name   Attribute name
//!
AST_Attribute* AST::Create_Attribute (string&& name)
{
  return Create_Node<AST_Attribute>(std::move(name));
}
//
//  End of: AST::Create_Attribute
//---------------------------------------------------------------------------


//! Creates an AST_Parameter node for local parameter
//!
//! @param name           Local parameter name
//! @param numbersValue   Numbers that define parameter value
//!
AST_Parameter* AST::Create_LocalParameter (std::string&& name, std::string&& numbersValue)
{
  auto kind = Kind::LocalParameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(numbersValue));
}
//
//  End of: AST::Create_LocalParameter
//---------------------------------------------------------------------------


//! Creates an AST_Parameter node for local parameter
//!
//! @param name           Parameter name
//! @param numbersValue   Numbers that define parameter value
//!
AST_Parameter* AST::Create_Parameter (std::string&& name, std::string&& numbersValue)
{
  auto kind = Kind::Parameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(numbersValue));
}
//
//  End of: AST::Create_Parameter
//---------------------------------------------------------------------------


//! Creates a AST_Module node
//!
//! @param identifier   Module name
//! @param children     Module children nodes
//!
AST_Module* AST::Create_Module (AST_ScalarIdentifier* identifier, vector<AST_Node*>&& children)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "identifier must not be nullptr");

  auto node    = make_unique<AST_Module>(identifier, std::move(children));
  auto pointer = node.get();

  m_modulesNodes.emplace_back(std::move(node));
  return pointer;
}
//
//  End of: AST::Create_Module
//---------------------------------------------------------------------------


//! Creates an AST_Port node
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
//!
//! @return Created AST_Port
//!
AST_Port* AST::Create_Port (Kind kind, AST_VectorIdentifier* identifier)
{
  return Create_Node<AST_Port>(kind, identifier);
}
//
//  End of: AST::Create_Port
//---------------------------------------------------------------------------


//! Creates an AST_Port node
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
//! @param children     Port children nodes
//!
//! @return Created AST_Port
//!
AST_Port* AST::Create_Port (Kind kind, AST_VectorIdentifier* identifier, vector<AST_Node*>&& children)
{
  return Create_Node<AST_Port>(kind, identifier, std::move(children));
}
//
//  End of: AST::Create_Port
//---------------------------------------------------------------------------


//! Creates an AST_ScalarIdentifier node
//!
//! @param name Identifier
//!
AST_ScalarIdentifier* AST::Create_ScalarIdentifier (string_view name)
{
  return Create_Node<AST_ScalarIdentifier>(name);
}
//
//  End of: AST::Create_ScalarIdentifier
//---------------------------------------------------------------------------





//! Creates an AST_ScanRegister node
//!
//! @param identifier   ScanRegister identifier
//! @param children     ScanRegister children nodes
//!
//! @return Created AST_ScanRegister
//!
AST_ScanRegister* AST::Create_ScanRegister (AST_VectorIdentifier* identifier, vector<AST_Node*>&& children)
{
  return Create_Node<AST_ScanRegister>(identifier, std::move(children));
}
//
//  End of: AST::Create_ScanRegister
//---------------------------------------------------------------------------

//! Creates an AST_Signal node
//!
//! @param number Signal value
//!
AST_Signal* AST::Create_Signal (string_view number)
{
  return Create_Node<AST_Signal>(number);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Signal node
//!
//! @param portName   Port name
//!
AST_Signal* AST::Create_Signal (AST_Identifier* portName)
{
  return Create_Node<AST_Signal>(portName);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Signal node
//!
//! @param scope      Port scope (dot separated instances names)
//! @param portName   Port name
//!
AST_Signal* AST::Create_Signal (vector<AST_ScalarIdentifier*>&& scope, AST_Identifier* portName)
{
  return Create_Node<AST_Signal>(std::move(scope), portName);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Source node
//!
//! @param kind     Kind of source
//! @param signal   Source signal
//!
//! @return Created AST_Source
//!
AST_Source* AST::Create_Source (Kind kind, AST_Signal* signal)
{
  return Create_Node<AST_Source>(kind, signal);
}
//
//  End of: AST::Create_Source
//---------------------------------------------------------------------------


//! Creates an AST_Source node
//!
//! @param kind     Kind of source
//! @param signals  Source signals
//!
//! @return Created AST_Source
//!
AST_Source* AST::Create_Source (Kind kind, vector<AST_Signal*>&& signals)
{
  return Create_Node<AST_Source>(kind, std::move(signals));
}
//
//  End of: AST::Create_Source
//---------------------------------------------------------------------------


//! Creates an AST_Identifier node
//!
//! @param name         Identifier
//! @param leftIndex    Left index
//! @param rightIndex   Right index (can be empty for single bit identifier)
//!
//! @return Created AST_VectorIdentifier
//!
AST_VectorIdentifier* AST::Create_VectorIdentifier (string_view name,
                                                    string_view leftIndex,
                                                    string_view rightIndex)
{
  return Create_Node<AST_VectorIdentifier>(name, leftIndex, rightIndex);
}
//
//  End of: AST::Create_VectorIdentifier
//---------------------------------------------------------------------------






//! Creates an AST_Value node
//!
//! @param kind             Kind of value
//! @param valueExpression  Value expression
//!
//! @return Created AST_Value
//!
AST_Value* AST::Create_Value (Kind kind, string_view valueExpression)
{
  return Create_Node<AST_Value>(kind, valueExpression);
}
//
//  End of: AST::Create_Value
//---------------------------------------------------------------------------



//! Returns "top" parsed module
//!
AST_Module* AST::TopModule ()
{
  CHECK_VALUE_NOT_EMPTY(m_modulesNodes, "AST has no module yet");

  return m_modulesNodes.front().get();
}
//
//  End of: AST::TopModule
//---------------------------------------------------------------------------




//===========================================================================
// End of AST.cpp
//===========================================================================
