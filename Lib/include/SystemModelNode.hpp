//===========================================================================
//                           SystemModelNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelNode.hpp
//!
//! Declares class SystemModelNode
//!
//===========================================================================

#ifndef SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_
  #define SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_

#include "Platform.hpp"
#include <cstdint>
#include <string>
#include <experimental/string_view>
#include <functional>
#include <memory>

namespace mast
{
class SystemModelVisitor;
class ConditionsChecker;

constexpr char DEFAULT_NODE_NAME[]        = "unnamed";
constexpr char DEFAULT_MIB_NAME[]         = "MIB";
constexpr char MIB_CTRL_EXT[]             = "_ctrl";
constexpr char MIB_MUX_EXT[]              = "_mux";
constexpr char DEFAULT_SIB_NAME[]         = "SIB";
constexpr char DEFAULT_1500_NAME[]        = "1500_wrapper";
constexpr char DEFAULT_TAP_NAME[]         = "1149_1_TAP";
constexpr char DEFAULT_TAP_IR_NAME[]      = "TAP_IR";
constexpr char DEFAULT_TAP_MUX_NAME[]     = "TAP_DR_Mux";
constexpr char DEFAULT_TAP_MUX_BPY_NAME[] = "TAP_BPY";
constexpr char DEFAULT_TAP_IR_EXT[]       = "_IR";
constexpr char DEFAULT_TAP_MUX_EXT[]      = "_DR_Mux";
constexpr char DEFAULT_TAP_MUX_BPY_EXT[]  = "_BPY";

//! Abstract base for common features of system model nodes
//!
class DLL_EXPORT SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:

  using NodeIdentifier   = uint32_t;                              //!< Uniquely identifies a node
//+  using ConditionFunctor = std::function<bool(SystemModelNode*)>; //!< Defines pre- and post- condition functors


  virtual void Accept        (SystemModelVisitor& visitor) = 0;   //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const = 0;          //!< Returns readable type name

  // ---------------- Setters
  //
  void AppendSibling (std::shared_ptr<SystemModelNode> pSibling);      //!< Appends a new sibling node

  //! Sets application specific data
  //! @note System node does not used this data (this is an optional extension point for applications using the system model)
  void SetApplicationData   (void* applicationData)                                { m_applicationData = applicationData; }
  void SetConditionsChecker (std::shared_ptr<ConditionsChecker> conditionsChecker) { m_conditionChecker = conditionsChecker; }  //!< Sets condition checker
  void ResetConditionsChecker ()                                                   { m_conditionChecker.reset(); }              //!< Removes current condition checker
  void SetPriority            (uint32_t priority)                                  { m_priority = priority; }                   //!< Sets new priority (application defined semantic)
  void ForcePending ()                                                             { m_pending = true; }                        //!< Changes state in order that IsPending will return true
  void ResetPending ()                                                             { m_pending = false; }                       //!< Changes state in order that IsPending will return false
  void SetName (std::experimental::string_view name)                               { m_name = MakeNodeName(name); }             //!< Changes node name

  static void ResetNodeIdentifier() { sm_nextIdentifier = 0; } //!< For debug purpose only, reset node identifier (e.g to be able to check construction order, or printers...)

  // ---------------- Getters
  //
  void*                              ApplicationData() const { return m_applicationData;  }                 //!< Retrieve application specific data
  std::shared_ptr<ConditionsChecker> Checker()         const { return m_conditionChecker; }                 //!< Returns current condition checker
  NodeIdentifier                     Identifier()      const { return m_identifier;       }                 //!< Returns node unique identifier
  std::experimental::string_view     Name()            const { return m_name;             }                 //!< Returns current node name
  std::shared_ptr<SystemModelNode>   NextSibling()     const { return m_pNextSibling;     }                 //!< Returns next sibling or nullptr
  uint32_t                           Priority()        const { return m_priority;         }                 //!< Returns currently assigned priority
  bool                               HasConditions()   const { return m_conditionChecker  ? true : false ;} //!< Returns true if there is some condition to check
  bool                               IsPending()       const { return m_pending;          }                 //!< Returns true if at least one node in the hierarchy is pending (need an update cycle)

  // ---------------- Protected Methods
  //
  protected:
  virtual ~SystemModelNode() = default;
  SystemModelNode()  = delete;
  SystemModelNode(std::experimental::string_view name);

  // ---------------- Private  Methods
  //
  private:

  static NodeIdentifier NextIdentifier();
  static std::string    MakeNodeName (std::experimental::string_view name);

  // ---------------- Private  Fields
  //
  private:
  static NodeIdentifier sm_nextIdentifier;   //!< Identifier for the next node to create

  NodeIdentifier                     m_identifier;                //!< Uniquely identifies a node
  std::string                        m_name;                      //!< Node readable name
  bool                               m_pending         = false;   //!< True when at least one node in the hierarchy is pending
  uint32_t                           m_priority        = 0;       //!< Defines application specific priority to the node(e.g for configuration when multiple paths should be selected but only one can be)
  std::shared_ptr<ConditionsChecker> m_conditionChecker;          //!< Optional condition(s) checker
  std::shared_ptr<SystemModelNode>   m_pNextSibling;              //!< Points to next node at same level (forming a singly linked list)
  void*                              m_applicationData = nullptr; //!< Application specific data (semantic managed by the application)
};
//
//  End of SystemModelNode class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELNODE_H__1BEF31DB_FB8A_405C_78B9_75CDBD328A52__INCLUDED_

//===========================================================================
// End of SystemModelNode.hpp
//===========================================================================



















