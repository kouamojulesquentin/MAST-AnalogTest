//===========================================================================
//                           AST_Parameter.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Parameter.hpp
//!
//! Declares AST_Parameter class
//!
//===========================================================================

#ifndef AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_
  #define AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <experimental/string_view>
#include <string>
#include <vector>

namespace Parsers
{
class AST_Builder;

//! Represents a parameter (for generic modules) or local parameter (for convenience)
//!
class AST_Parameter final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Parameter() = default;
  AST_Parameter()  = delete;

  //! Parameter name
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of parameter
  //!
  std::string AsText() const override;

  //! Tells whether parameter represents a number
  //!
  //! @note Parameter represents either a number or a string (never both)
  //!
  //! @return true when it represents a number, false when it represents a string
  bool IsNumber() const { return !m_numbersValue.empty(); }

  //! Tells whether parameter represents a number
  //!
  //! @note Parameter represents either a number or a string (never both)
  //!
  //! @return true when it represents a string, false when it represents a number
  bool IsString() const { return !m_stringsValue.empty(); }

  //! Returns true when Parameter is defined using Parameter reference(s)
  //!
  bool HasParameterRef () const override;

  //! Returns true when some string has Parameter reference(s)
  //!
  static bool HasParameterRef(const std::string& value);

  //! Returns true when, in a collection of nodes, at least one is a Parameter reference
  //!
  template<typename T>
  static bool HasParameterRef(const T& nodes)
  {
    for (auto node: nodes)
    {
      if (node->HasParameterRef())
      {
        return true;
      }
    }
    return false;
  }

  //! Replaces parameter references with their actual value, then resolves value expressions
  //!
  template<typename T>
  static void ResolveItems(T& nodes, const std::vector<AST_Parameter*>& parameters)
  {
    for (auto node: nodes)
    {
      node->Resolve(parameters);
    }
  }

  //! Replaces parameter references with their actual value, then resolves value expressions
  //!
  template<typename T>
  static void ResolveItems(T& nodes, std::vector<AST_Parameter*>& parameters, AST_Builder& astBuilder)
  {
    for (auto node: nodes)
    {
      node->Resolve(astBuilder, parameters);
    }
  }


  //! Replaces parameter references used by this parameter (solving transitive definitions)
  //!
  void Resolve (AST_Builder& astBuilder, std::vector<AST_Parameter*>& parameters);

  //! Returns uniquified clone of value or string expression
  //!
  AST_Parameter* UniquifiedClone (AST_Builder& astBuilder) const;

  //! Returns a copy of number expression with parameters references replaces with the actual parameter value
  //!
  static std::string ReplacedParameters (const std::string& numberValue, const std::vector<AST_Parameter*>& parameters);

  //! Replaces parameter references nodes with parameter string value
  //!
  //!
  static void ReplaceStringParameters (std::vector<AST_SimpleNode*>& nodes, const std::vector<AST_Parameter*>& parameters);

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Parameter)(Parsers::Kind&, std::string&&, std::string&&);
  MAKE_UNIQUE_AS_FRIEND(AST_Parameter)(Parsers::Kind&, std::string&&, std::vector<AST_SimpleNode*>&&);

  //! Initializes AST_Parameter representing a number
  //!
  AST_Parameter(Kind kind, std::string&& name, std::string&& numbersValue);

  //! Initializes AST_Parameter representing a string
  //!
  AST_Parameter(Kind kind, std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);

  static AST_Parameter* LocateParameterDef (std::experimental::string_view name, const std::vector<AST_Parameter*>& parameters);

  static std::string ReducedStringExpr (const std::vector<AST_SimpleNode*>& stringsValue);

  static void Resolve (AST_Builder& astBuilder, std::string&                  numberValue,  std::vector<AST_Parameter*>& parameters);
  static void Resolve (AST_Builder& astBuilder, std::vector<AST_SimpleNode*>& stringsValue, std::vector<AST_Parameter*>& parameters);

  // ---------------- Private Fields
  //
  private:
  const std::string            m_name;                                     //!< Parameter name
  std::string                  m_numbersValue;                             //!< Parameter value when defined as numbers
  std::vector<AST_SimpleNode*> m_stringsValue;                             //!< Parameter value when defined as strings and/or parameter reference
  bool                         m_parameterRefResolutionInProgress = false; //!< This is to detect circular parameter references
};
//
//  End of AST_Parameter class declaration
//---------------------------------------------------------------------------

//! Tells whether two AST_Parameter represents same type
//!
//! @note Parameter represents either a number or a string (never both)
//!
//! @return true when both represents a number or both represents a string, false otherwise
inline bool RepresentsSameTypes(const AST_Parameter* lhs, const AST_Parameter* rhs)
{
  return lhs->IsNumber() == rhs->IsNumber();
}
} // End of namespace Parsers


#endif  // not defined AST_PARAMETER_H__6601FC1C_1BB0_4FE1_3DA7_E138C1507BC2__INCLUDED_
//===========================================================================
// End of AST_Parameter.hpp
//===========================================================================
