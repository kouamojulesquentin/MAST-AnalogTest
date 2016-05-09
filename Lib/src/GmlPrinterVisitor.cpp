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
const std::experimental::string_view GmlPrinterVisitor::m_shape_Tap             = "ellipse";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Linker          = "trapezoid";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Chain           = "ellipse";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Register        = "rectangle";

const std::experimental::string_view GmlPrinterVisitor::m_color_AccessInterface = "#10FFFF";
const std::experimental::string_view GmlPrinterVisitor::m_color_Tap             = "#AA55AA";
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
  m_os << "   node [ id " << node.GetIdentifier();


  m_os << " graphics [";
  m_os << " type \"" << shapeName       << "\"";
  m_os << " fill \"" << backgroundColor << "\"";
  m_os << " w "  << std::max(50u, 11u * node.GetName().length());
  m_os << " h "  << std::max(35u, 3u  * node.GetName().length());
  m_os << " ] ";

  if (!node.GetName().empty())
  {
    m_os << "LabelGraphics [ text \"" << node.GetName() << "\" fontSize 13 fontStyle \"bold\" ]";
  }

  m_os << " ]" << std::endl;
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
  m_os << "   edge ["
       << " source "  << parentNode.GetIdentifier()
       << " target "  << childNode.GetIdentifier()
       << " label \"" << childId << "\"";

  if (!style.empty())
  {
    m_os << " graphics ["
         << " width 1"
         << " style \"" << style << "\""
         << " targetArrow \"standard\""
         << " ]";
  }

  m_os << " ]" << std::endl;
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

  m_os << "]";
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

  m_os << "graph" << std::endl;
  m_os << "["     << std::endl;
  m_os << "   hierarchic 1 directed 1";

  if (!m_graphName.empty())
  {
    m_os << " label \"" << m_graphName << "\"";
  }
  m_os << std::endl;
}
//
//  End of: GmlPrinterVisitor::CloseRoot
//---------------------------------------------------------------------------



//! Returns currently visited nodes representation
//!
string GmlPrinterVisitor::GetGraph ()
{
  string graph;

  if ( m_visited)
  {
    graph = m_os.str();
  }
  else
  {
    CloseRoot();
    graph = m_os.str();
    m_os.seekp(0);
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
  m_linker = &linker;
  selector->Accept(*this);
  m_linker = nullptr;
}

 //! Appends Register node to GML graph
//!
void GmlPrinterVisitor::VisitRegister (Register& reg)
{
  AppendNode(m_shape_Register, m_color_Register, "Register", reg);

  // ---------------- Deal with path selector associated with Linker nodes
  //
  if (m_linker)
  {
    PrintEdge(*m_linker, reg, 0, "dashed");
    m_linker = nullptr;   // Only first SystemModelNode is connected to the linker
  }
}

//! Appends content of tap node in text representation and visits
//! sub-nodes
//!
void GmlPrinterVisitor::VisitTap (Tap& tap)
{
  AppendParentNode(m_shape_Tap, m_color_Tap, "Tap", tap);
}
//===========================================================================
// End of GmlPrinterVisitor.cpp
//===========================================================================
