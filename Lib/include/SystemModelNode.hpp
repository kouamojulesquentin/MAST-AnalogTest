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

constexpr char DEFAULT_NODE_NAME[] = "unnamed";
constexpr char DEFAULT_MIB_NAME[]  = "MIB";
constexpr char MIB_CTRL_EXT[]      = "_ctrl";
constexpr char MIB_MUX_EXT[]       = "_mux";
constexpr char DEFAULT_SIB_NAME[]  = "SIB";
constexpr char DEFAULT_1500_NAME[] = "1500_wrapper";
constexpr char DEFAULT_TAP_NAME[]  = "1149_1_TAP";

//! Abstract base for common features for system model nodes
//!
class DLL_EXPORT SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:

  using NodeIdentifier   = uint32_t;                              //!< Uniquely identifies a node
//+  using ConditionFunctor = std::function<bool(SystemModelNode*)>; //!< Defines pre- and post- condition functors


  virtual void Accept        (SystemModelVisitor& visitor) = 0;   //!< Visited part of the Visitor pattern

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

  // ---------------- Getters
  //
  void*                              GetApplicationData()   const { return m_applicationData;  }                 //!< Retrieve application specific data
  std::shared_ptr<ConditionsChecker> GetConditionsChecker() const { return m_conditionChecker; }                 //!< Returns current condition checker
  NodeIdentifier                     GetIdentifier()        const { return m_identifier;       }                 //!< Returns node unique identifier
  std::experimental::string_view     GetName()              const { return m_name;             }                 //!< Returns current node name
  std::shared_ptr<SystemModelNode>   GetNextSibling()       const { return m_pNextSibling;     }                 //!< Returns next sibling or nullptr
  uint32_t                           GetPriority()          const { return m_priority;         }                 //!< Returns currently assigned priority
  bool                               HasConditions()        const { return m_conditionChecker  ? true : false ;} //!< Returns true if there is some condition to check
  bool                               IsPending()            const { return m_pending;          }                 //!< Returns true if at least one node in the hierarchy is pending (need an update cycle)

  // ---------------- Protected Methods
  //
  protected:
  virtual ~SystemModelNode() = default;
  SystemModelNode()  = delete;
  SystemModelNode(std::experimental::string_view name);

  // ---------------- Private  Methods
  //
  private:

  static NodeIdentifier GetNextIdentifier();

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



















