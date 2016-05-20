//===========================================================================
//                           GmlPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GmlPrinterVisitor.hpp
//!
//! Declares class GmlPrinterVisitor
//!
//===========================================================================


#ifndef GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_
  #define GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_

#include "SystemModelVisitor.hpp"
#include "SystemModelNode.hpp"
#include <string>
#include <sstream>
#include <experimental/string_view>

namespace mast
{
//! System model visitors for creation of a GML formated representation of the
//! system mode tree
//!
class DLL_EXPORT GmlPrinterVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~GmlPrinterVisitor() = default;
  GmlPrinterVisitor()
    : GmlPrinterVisitor("")
  {}

  GmlPrinterVisitor(std::experimental::string_view graphName)
    : m_graphName (graphName)
  {
    CreateRoot();
  }

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  std::string  GetGraph();  //!< Returns currently visited nodes graph representation

  // ---------------- Private  Methods
  //
  private:

  void CreateRoot();
  void CloseRoot();

  void AppendNode       (std::experimental::string_view typeName, const SystemModelNode& node);
  void AppendParentNode (std::experimental::string_view shapeName,
                         std::experimental::string_view backgroundColor,
                         std::experimental::string_view typeName,
                         const ParentNode&              parentNode
                        );

  void AppendNode       (std::experimental::string_view shapeName,
                         std::experimental::string_view backgroundColor,
                         std::experimental::string_view nodeTypeName,
                         const SystemModelNode&         node
                         );

  void PrintEdge        (const ParentNode& parentNode, const SystemModelNode& childNode, uint32_t childId, std::experimental::string_view style = "");

  // ---------------- Private  Fields
  //
  private:
  std::string        m_graphName;             //!< Name associated to the all graph
  uint32_t           m_depth   = 0u;          //!< Current nodes tree depth
  bool               m_visited = false;       //!< Becomes true when a tree traversal has been completely done
  const Linker*      m_linker  = nullptr;     //!< When not nullptr, we are visiting a path selector (while visiting a linker)
  std::ostringstream m_osGraph;               //!< Stream to build up a representation of visited system model nodes
  std::ostringstream m_osEdges;               //!< Stream to build up links between nodes

  static const std::experimental::string_view m_shape_AccessInterface;
  static const std::experimental::string_view m_shape_Linker;
  static const std::experimental::string_view m_shape_Chain;
  static const std::experimental::string_view m_shape_Register;

  static const std::experimental::string_view m_color_AccessInterface;
  static const std::experimental::string_view m_color_Linker;
  static const std::experimental::string_view m_color_Chain;
  static const std::experimental::string_view m_color_Register;
};
//
//  End of GmlPrinterVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_

//===========================================================================
// End of GmlPrinterVisitor.hpp
//===========================================================================
