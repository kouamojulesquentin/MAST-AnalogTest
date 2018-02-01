//===========================================================================
//                           AST.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.hpp
//!
//! Declares AST class
//!
//===========================================================================

#ifndef AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_
  #define AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

#include "AST_Node.hpp"
#include "AST_Builder.hpp"
#include "AST_Network.hpp"

#include <vector>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
class AST_AccessLink;
class AST_BsdlInstructionRef;
class AST_ConcatNumber;
class AST_EnumRef;
class AST_FileRef;
class AST_Identifier;
class AST_IntegerLiteral;
class AST_IntegerExprRef;
class AST_Namespace;
class AST_ParameterRef;
class AST_ScanInterface;
class AST_ScanInterfaceRef;
class AST_ScanMuxSelection;
class AST_Signal;
class AST_SimpleNode;
class AST_Source;
class AST_Value;

enum class AccessLinkType;

//! Abstract Syntax Tree resulting in parsing some test network description
//!
//! @note It serves also as an AST node builder () to have a single place to manage memory allocations
//!
class AST final : public AST_Builder
{
  // ---------------- Public Methods
  //
  public:
  ~AST();
  AST();

  AST_AccessLink*         Create_AccessLink         (const AST_ScalarIdentifier*            identifier,
                                                     AccessLinkType                         type,
                                                     AST_FileRef*                           bsdlFile,
                                                     std::vector<AST_BsdlInstructionRef*>&& bsdlInstructionRef);
  AST_AccessLink*         Create_AccessLink         (const AST_ScalarIdentifier* identifier, const AST_ScalarIdentifier* genericId);
  AST_EnumRef*            Create_EnumRef            (std::string&& name);
  AST_FileRef*            Create_FileRef            (Kind kind, std::string&& name);
  AST_BasedNumber*        Create_BasedNumber        (Kind kind, std::string&&    baseAndDigits) override;                              //!< Creates an AST_BasedNumber
  AST_ConcatNumber*       Create_ConcatNumber       ()                          override;                                              //!< Creates an AST_ConcatNumber empty
  AST_ConcatNumber*       Create_ConcatNumber       (std::vector<AST_Number*>&& numbers) override;                                     //!< Creates an AST_ConcatNumber
  AST_IntegerExprRef*     Create_IntegerExprRef     (AST_ParameterRef* parameterRef) override;                                         //!< Creates an AST_IntegerExprRef node
  AST_IntegerLiteral*     Create_IntegerLiteral     (std::string&& integerText) override;                                              //!< Creates an AST_IntegerLiteral node
  AST_IntegerLiteral*     Create_IntegerLiteral     (uint32_t      value)       override;                                              //!< Creates an AST_IntegerLiteral node
  AST_IntegerBinaryExpr*  Create_IntegerBinaryExpr  (Kind kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand) override; //!< Creates an AST_IntegerBinaryExpr node
  AST_IntegerUnaryExpr*   Create_IntegerUnaryExpr   (Kind kind, AST_IntegerExpr* operand) override;                                    //!< Creates an AST_IntegerUnaryExpr node
  AST_Namespace*          Create_Namespace          (std::string&& name);
  AST_Attribute*          Create_Attribute          (std::string&& name);
  AST_Attribute*          Create_Attribute          (std::string&& name, AST_ConcatNumber* concatNumber);
  AST_Attribute*          Create_Attribute          (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_Parameter*          Create_LocalParameter     (std::string&& name, AST_ConcatNumber* concatNumber);
  AST_Parameter*          Create_LocalParameter     (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_Parameter*          Create_Parameter          (std::string&& name, AST_ConcatNumber* concatNumber);
  AST_Parameter*          Create_Parameter          (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_ParameterRef*       Create_ParameterRef       (std::string&& name);
  AST_BsdlInstructionRef* Create_BsdlInstructionRef (std::string&& instructionName, std::vector<AST_Node*>&& children);
  AST_String*             Create_String             (std::string&& content) override;
  AST_Signal*             Create_Signal             (AST_Number* number);
  AST_Signal*             Create_Signal             (AST_Identifier* portName);
  AST_Signal*             Create_Signal             (std::vector<AST_ScalarIdentifier*>&& scope, AST_Identifier* portName);
  AST_ScanMux*            Create_ScanMux            (AST_VectorIdentifier*                identifier,
                                                     std::vector<Parsers::AST_Signal*>&&  selectors,
                                                     std::vector<AST_ScanMuxSelection*>&& scanMuxSelection);
  AST_ScanMuxSelection*   Create_ScanMuxSelection   (std::vector<AST_ConcatNumber*>&& selectionValues, std::vector<Parsers::AST_Signal*>&& selectedSignals);
  AST_Source*             Create_Source             (Kind kind, AST_Signal* signal);
  AST_Source*             Create_Source             (Kind kind, std::vector<AST_Signal*>&& signals);
  AST_Port*               Create_Port               (Kind kind, AST_VectorIdentifier* identifier);
  AST_Port*               Create_Port               (Kind kind, AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_Instance*           Create_Instance           (const AST_ScalarIdentifier* instanceIdentifier,
                                                     const AST_ModuleIdentifier* moduleIdentifier);
  AST_Instance*           Create_Instance           (const AST_ScalarIdentifier* instanceIdentifier,
                                                     const AST_ModuleIdentifier* moduleIdentifier,
                                                     std::vector<AST_Node*>&&    children);
  AST_ModuleIdentifier*   Create_ModuleIdentifier   (const AST_Namespace* namespaceName, const AST_ScalarIdentifier* moduleName);
  AST_Module*             Create_Module             (const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScanInterface*      Create_ScanInterface      (const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScanInterfaceRef*   Create_ScanInterfaceRef   (std::vector<std::tuple<AST_ScalarIdentifier*, std::string>>&& scanInterfaceNames);
  AST_ScanRegister*       Create_ScanRegister       (AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScalarIdentifier*   Create_ScalarIdentifier   (std::string&& name);
  AST_VectorIdentifier*   Create_VectorIdentifier   (std::string&& name, AST_IntegerExpr* leftIndex, AST_IntegerExpr* rightIndex = nullptr);
  AST_Value*              Create_Value              (Kind kind, AST_ConcatNumber* concatNumber);


  void SaveInstanceDefaultNamespace ()           { m_savedInstancesDefaultNamespace = m_instancesDefaultNamespace; }  //!< Saves instance default namespace (before parsing new module body)

  void SetRootNamespace ()                       { SetNamespace(m_rootNamespace); }                    //!< Forces following modules to be created in "root" namespace
  void SetModuleNamespace   (std::string&& name) { SetNamespace(Create_Namespace(std::move(name))); }  //!< Forces following modules to be created in specified namespace
  void SetInstanceNamespace ()                   { m_instancesDefaultNamespace = m_rootNamespace; }    //!< Forces following instances to refer to modules in "root" namespace
  void SetInstanceNamespace (std::string&& name) { m_instancesDefaultNamespace = Create_Namespace(std::move(name)); }  //!< Forces following instances to refer to modules in specified namespace

  AST_Network* Network() { return &m_network; } //!< Returns test network

  //! Returns root namespace
  //!
  const AST_Namespace* RootNamespace() const { return m_rootNamespace; }

  //! Returns current default namespace for module instances
  //!
  const AST_Namespace* InstancesDefaultNamespace() const { return m_instancesDefaultNamespace; }

  //! Uniquifies all modules in network
  //!
  void Uniquify();

  // AST_Builder cloning interface implementation

  AST_Attribute*         Clone_Attribute         (const AST_Attribute*         attribute)         override;  //!< Clones an attribute
  AST_BasedNumber*       Clone_BasedNumber       (const AST_BasedNumber*       basedNumber)       override;  //!< Clones a "based" number
  AST_ConcatNumber*      Clone_ConcatNumber      (const AST_ConcatNumber*      concatNumber)      override;  //!< Clones a concated number
  AST_Port*              Clone_Port              (const AST_Port*              port)              override;  //!< Clones a Port
  AST_Instance*          Clone_Instance          (const AST_Instance*          instance)          override;  //!< Clones an instance
  AST_IntegerExprRef*    Clone_IntegerExprRef    (const AST_IntegerExprRef*    integerExprRef)    override;  //!< Clones an integer expression reference
  AST_IntegerBinaryExpr* Clone_IntegerBinaryExpr (const AST_IntegerBinaryExpr* integerBinaryExpr) override;  //!< Clones an integer binary expression
  AST_IntegerLiteral*    Clone_IntegerLiteral    (const AST_IntegerLiteral*    integerLiteral)    override;  //!< Clones a integer literal
  AST_IntegerUnaryExpr*  Clone_IntegerUnaryExpr  (const AST_IntegerUnaryExpr*  integerUnaryExpr)  override;  //!< Clones an integer unary expression
  AST_Module*            Clone_Module            (const AST_Module*            module)            override;  //!< Clones a module
  AST_Parameter*         Clone_Parameter         (const AST_Parameter*         parameter)         override;  //!< Clones a parameter definition
  AST_ScanMux*           Clone_ScanMux           (const AST_ScanMux*           scanMux)           override;  //!< Clones a scan multiplexer
  AST_ScanRegister*      Clone_ScanRegister      (const AST_ScanRegister*      scanRegister)      override;  //!< Clones a scan register
  AST_VectorIdentifier*  Clone_VectorIdentifier  (const AST_VectorIdentifier*  identifier)        override ; //!< Clones a vector identifier

  AST_ScalarIdentifier* Create_UniquifiedIdentifier       (const AST_ScalarIdentifier* identifier) override; //!< Creates an identifier for a uniquified entity
  AST_ModuleIdentifier* Create_UniquifiedModuleIdentifier (const AST_Module*           module)     override; //!< Creates an module identifier for a uniquified module

  // ---------------- Private Methods
  //
  AST_Namespace* Create_Namespace_Impl (std::string&& name);
  void           SetNamespace          (const AST_Namespace* newNamespace);

  template<typename T, typename... TArgs> T* Create_Node (TArgs&& ... args)
  {
    auto node    = std::make_unique<T>(std::forward<TArgs>(args) ...);
    auto pointer = node.get();

    m_nodes.emplace_back(std::move(node));
    return pointer;
  }

  template<typename T> T* Clone_Node (const T* other)
  {
    auto node    = std::make_unique<T>(*other);
    auto pointer = node.get();

    m_nodes.emplace_back(std::move(node));
    return pointer;
  }

  // ---------------- Private Fields
  //
  private:
  AST_Network                                 m_network;                                  //!< Build test network
  std::vector<std::unique_ptr<AST_Module>>    m_modules;                                  //!< Managed module nodes
  std::vector<std::unique_ptr<AST_Node>>      m_nodes;                                    //!< Managed, not modules nor namespaces, nodes
  std::vector<std::unique_ptr<AST_Namespace>> m_namespaces;                               //!< Managed namespace nodes
  const AST_Namespace*                        m_rootNamespace                  = nullptr; //!< Root namespace
  const AST_Namespace*                        m_modulesNamespace               = nullptr; //!< Namespace used for following modules definitions
  const AST_Namespace*                        m_uniquifiedNamespace            = nullptr; //!< Namespace used for uniquified modules definitions
  const AST_Namespace*                        m_savedInstancesDefaultNamespace = nullptr; //!< Saved default namespace for instance (to be restored at end of module parsing)
  const AST_Namespace*                        m_instancesDefaultNamespace      = nullptr; //!< Default namespace used for following module instantiations
  int32_t                                     m_uniqueIdCounter                = 0;       //!< To differentiate uniquified entities (mostly modules)
};
//
//  End of AST class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

//===========================================================================
// End of AST.hpp
//===========================================================================
