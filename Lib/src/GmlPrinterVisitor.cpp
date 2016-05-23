//===========================================================================
//                           GmlPrinterVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GmlPrinterVisitor.cpp
//!
//! Implements class GmlPrinterVisitor
//!
//===========================================================================

#include "GmlPrinterVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "Utility.hpp"
#include "PathSelector.hpp"

#include <algorithm>
using std::string;
using std::experimental::string_view;
using namespace mast;


const std::experimental::string_view GmlPrinterVisitor::m_shape_AccessInterface = "octagon";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Linker          = "trapezoid";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Chain           = "ellipse";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Register        = "rectangle";

const std::experimental::string_view GmlPrinterVisitor::m_color_AccessInterface = "#10FFFF";
const std::experimental::string_view GmlPrinterVisitor::m_color_Linker          = "#FF3060";
const std::experimental::string_view GmlPrinterVisitor::m_color_Chain           = "#FFCC20";
const std::experimental::string_view GmlPrinterVisitor::m_color_Register        = "#59FF20";

//! Appends a parent node and its children to the GML graph
//!
//! @param type   Text representation of the node type
//! @param node   The node for which a gml element is to be added
//!
void GmlPrinterVisitor::AppendParentNode (std::experimental::string_view shapeName,
                                          std::experimental::string_view backgroundColor,
                                          std::experimental::string_view typeName,
                                          const ParentNode&              parentNode)
{
  AppendNode(shapeName, backgroundColor, typeName, parentNode);

  // ---------------- Print children
  //
  {
    // ---------------- Do support exceptions
    //
    auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
    AT_SCOPE_EXIT(restoreDepth);

    ++m_depth;

    auto childId = 1u;
    auto child   = parentNode.GetFirstChild();
    while (child)
    {
      child->Accept(*this);
      PrintEdge(parentNode, *child, childId);

      ++childId;
      child = child->GetNextSibling();
    }
  }

  if (m_depth == 0)
  {
    m_visited = true;
    CloseRoot();
  }
}
//
//  End of: GmlPrinterVisitor::AppendParentNode
//---------------------------------------------------------------------------



//! Appends a node to the GML graph
//!
void GmlPrinterVisitor::AppendNode (string_view            shapeName,
                                    string_view            backgroundColor,
                                    string_view            nodeTypeName,
                                    const SystemModelNode& node)
{
  m_osGraph << "   node [ id " << node.GetIdentifier();


  m_osGraph << " graphics [";
  m_osGraph << " type \"" << shapeName       << "\"";
  m_osGraph << " fill \"" << backgroundColor << "\"";
  m_osGraph << " w "  << std::max(static_cast<size_t>(50u), 11u * node.GetName().length());
  m_osGraph << " h "  << std::max(static_cast<size_t>(35u), 3u  * node.GetName().length());
  m_osGraph << " ] ";

  if (!node.GetName().empty() || m_displayIdentifier)
  {
    m_osGraph << "LabelGraphics [ text \"";
    if (m_displayIdentifier)
    {
      m_osGraph << "(" << node.GetIdentifier() << ")\n";
    }

    if (!node.GetName().empty())
    {
      m_osGraph << node.GetName();
    }
    m_osGraph << "\" fontSize 13 fontStyle \"bold\" ]";
  }

  m_osGraph << " ]" << std::endl;
}
//
//  End of: GmlPrinterVisitor::AppendNode
//---------------------------------------------------------------------------


//! Appends an edge between two nodes to the GML graph
//!
//! @param parentNode   A parent node
//! @param childNode    A child of the parent node
//!
void GmlPrinterVisitor::PrintEdge (const ParentNode&              parentNode,
                                   const SystemModelNode&         childNode,
                                   uint32_t                       childId,
                                   std::experimental::string_view style)
{
  m_osEdges << "   edge ["
            << " source "  << parentNode.GetIdentifier()
            << " target "  << childNode.GetIdentifier();

  if (childId != 0)
  {
    m_osEdges << " label \"" << childId << "\"";
  }

  if (!style.empty())
  {
    m_osEdges << " graphics ["
              << " width 1"
              << " style \"" << style << "\""
              << " targetArrow \"standard\""
              << " ]";
  }

  m_osEdges << " ]" << std::endl;
}
//
//  End of: GmlPrinterVisitor::PrintEdge
//---------------------------------------------------------------------------


//! Closes root node
//!
void GmlPrinterVisitor::CloseRoot ()
{
  if (m_depth != 0)
  {
    THROW_LOGIC_ERROR("Closing GML graph should only occur when terminating visiting System Model tree root node");
  }

  m_osGraph << m_osEdges.str();
  m_osGraph << "]";
}
//
//  End of: GmlPrinterVisitor::CloseRoot
//---------------------------------------------------------------------------


//! Creates root node
//!
void GmlPrinterVisitor::CreateRoot ()
{
  if (m_depth != 0)
  {
    THROW_LOGIC_ERROR("Creating GML graph should only occur when starting visiting System Model tree root node");
  }

  m_osGraph << "graph" << std::endl;
  m_osGraph << "["     << std::endl;
  m_osGraph << "   hierarchic 1 directed 1";

  if (!m_graphName.empty())
  {
    m_osGraph << " label \"" << m_graphName << "\"";
  }
  m_osGraph << std::endl;
}
//
//  End of: GmlPrinterVisitor::CloseRoot
//---------------------------------------------------------------------------



//! Returns currently visited nodes representation and edges
//!
string GmlPrinterVisitor::GetGraph ()
{
  string graph;

  if ( m_visited)
  {
    graph = m_osGraph.str();
  }
  else
  {
    CloseRoot();
    graph = m_osGraph.str();
    m_osGraph.seekp(0);
    m_osEdges.seekp(0);
    CreateRoot();
  }

  return graph;
}
//
//  End of: GmlPrinterVisitor::GetGraph
//---------------------------------------------------------------------------



//! Appends AccessInterface node to GML graph
//!
void GmlPrinterVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  AppendParentNode(m_shape_AccessInterface, m_color_AccessInterface, "Access_I", accessInterface);
}

//! Appends Chain node to GML graph
//!
void GmlPrinterVisitor::VisitChain (Chain& chain)
{
  AppendParentNode(m_shape_Chain, m_color_Chain, "Chain", chain);
}

//! Appends Linker node to GML graph
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void GmlPrinterVisitor::VisitLinker (Linker& linker)
{
  AppendParentNode(m_shape_Linker, m_color_Linker, "Linker", linker);

  // ---------------- Deal with path selector
  //
  auto selector = linker.GetPathSelector();
  if (selector)
  {
    m_linker = &linker;
    selector->Accept(*this);
    m_linker = nullptr;
  }
}

 //! Appends Register node to GML graph
//!
void GmlPrinterVisitor::VisitRegister (Register& reg)
{
  // ---------------- Deal with path selector associated with Linker nodes
  //
  if (m_linker)
  {
    PrintEdge(*m_linker, reg, 0, "dashed");
    m_linker = nullptr;   // Only first SystemModelNode is connected to the linker
  }
  else
  {
    AppendNode(m_shape_Register, m_color_Register, "Register", reg);
  }
}

//===========================================================================
// End of GmlPrinterVisitor.cpp
//===========================================================================
