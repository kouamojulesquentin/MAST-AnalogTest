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
using std::ostringstream;
using namespace std::string_literals;
using namespace mast;


const std::experimental::string_view GmlPrinterVisitor::m_shape_AccessInterface = "octagon";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Linker          = "trapezoid";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Chain           = "ellipse";
const std::experimental::string_view GmlPrinterVisitor::m_shape_Register        = "rectangle";

const std::experimental::string_view GmlPrinterVisitor::m_color_AccessInterface = "#10FFFF";
const std::experimental::string_view GmlPrinterVisitor::m_color_Linker          = "#FF3060";
const std::experimental::string_view GmlPrinterVisitor::m_color_Chain           = "#FFCC20";
const std::experimental::string_view GmlPrinterVisitor::m_color_Register        = "#59FF20";

const std::experimental::string_view GmlPrinterVisitor::m_fontName              = "Lucida Console";


inline GmlPrinterOptions& operator |= (GmlPrinterOptions& X, GmlPrinterOptions Y)
{
  X = X | Y;
  return X;
}

inline bool IsEnumFlagSet(GmlPrinterOptions options, GmlPrinterOptions flag)
{
  auto rawOption = static_cast<std::underlying_type_t<GmlPrinterOptions>>(options);
  auto rawFlag   = static_cast<std::underlying_type_t<GmlPrinterOptions>>(flag);

  return (rawOption & rawFlag) == rawFlag;
}

//! Initializes with given options
//!
GmlPrinterVisitor::GmlPrinterVisitor(std::experimental::string_view graphName, GmlPrinterOptions options)
  : m_graphName (graphName)
{
  CreateRoot();
  m_displayIdentifier    = IsEnumFlagSet(options, GmlPrinterOptions::DisplayIdentifiers);
  m_displayRegisterValue = IsEnumFlagSet(options, GmlPrinterOptions::DisplayRegisterValue);
  m_displayRegValueAuto  = IsEnumFlagSet(options, GmlPrinterOptions::DisplayValueAuto);
  m_showSelectorWithEdge = IsEnumFlagSet(options, GmlPrinterOptions::ShowSelectorWithEdge);
  m_showSelectionValues  = IsEnumFlagSet(options, GmlPrinterOptions::ShowSelectionValues);
}
//
//  End of: GmlPrinterVisitor::GmlPrinterVisitor
//---------------------------------------------------------------------------





//! Appends a parent node and its children to the GML graph
//!
//! @param shapeName        The shape to use
//! @param backgroundColor  Background color
//! @param notes            Optional node to add below the node name
//! @param parentNode       Parent node to process
//!
void GmlPrinterVisitor::AppendParentNode (string_view       shapeName,
                                          string_view       backgroundColor,
                                          string_view       notes,
                                          const ParentNode& parentNode)
{
  auto outlineStyle = parentNode.IgnoreForNodePath() ? "dashed" : "";
  AppendNode(shapeName, outlineStyle, backgroundColor, notes, parentNode);

  auto linker   = dynamic_cast<const Linker*>(&parentNode);
  auto selector = m_showSelectionValues && linker ? linker->Selector() : nullptr;

  // ---------------- Print children
  //
  {
    // ---------------- Do support exceptions
    //
    auto restoreDepth = [this, initialValue = m_depth]() { this->m_depth = initialValue; };
    AT_SCOPE_EXIT(restoreDepth);

    ++m_depth;

    auto childId = 1u;
    auto child   = parentNode.FirstChild();
    while (child)
    {
      child->Accept(*this);

      if (selector)
      {
        ostringstream os;
        os << "/[" << selector->SelectionValue(childId).DataAsMixString(8, "", ":");

        string_view edgeStyle = "dotted";

        if (selector->IsSelected(childId))
        {
          os << ":S";
          edgeStyle = "dashed_dotted";
        }

        if (selector->IsActive(childId))
        {
          os << ":A";
          edgeStyle = ""; // Use plain line for active derivation
        }

        os << "]";
        auto note = os.str();
        PrintEdge(parentNode, *child, childId, edgeStyle, note);
      }
      else
      {
        PrintEdge(parentNode, *child, childId);
      }

      ++childId;
      child = child->NextSibling();
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
//! @param shapeName        The shape to use
//! @param outlineStyle     Optional line type for shape (default when empty)
//! @param backgroundColor  Background color
//! @param notes            Optional node to add below the node name
//! @param node             Node to process
//!
void GmlPrinterVisitor::AppendNode (string_view            shapeName,
                                    string_view            outlineStyle,
                                    string_view            backgroundColor,
                                    string_view            notes,
                                    const SystemModelNode& node)
{
  m_osGraph << "   node [ id " << node.Identifier();

  auto displayName  = !node.Name().empty();
  auto displayNotes = !notes.empty();
  auto displayLabel = displayName || m_displayIdentifier || displayNotes;

  // ---------------- Compute height and width for the graphic
  //
  size_t labelCharWidth = node.Name().length();
  size_t linesCount     = 1;

  if (displayNotes)
  {
    ++linesCount;
    size_t startPos = 0;
    size_t foundPos = 0;
    while ((foundPos = notes.find('\n', startPos)) != string_view::npos)
    {
      ++linesCount;
      auto noteWidth = foundPos - startPos;
      labelCharWidth = std::max(labelCharWidth, noteWidth);

      startPos = ++foundPos;
    }
  }
  auto nodeWidth  = std::max(static_cast<size_t>(50u), (95u * labelCharWidth) / 10u);
  auto nodeHeight = std::max(static_cast<size_t>(35u), 18u  * linesCount);

  if (shapeName != "rectangle"s)
  {
    nodeHeight += 8u;
    nodeWidth  += 40u;
  }

  m_osGraph << " graphics [";
  m_osGraph << " type \"" << shapeName       << "\"";
  m_osGraph << " fill \"" << backgroundColor << "\"";
  if (!outlineStyle.empty())
  {
    m_osGraph << " outlineStyle \"" << outlineStyle << "\"";
  }
  m_osGraph << " w "  << nodeWidth;
  m_osGraph << " h "  << nodeHeight;
  m_osGraph << " ] ";

  if (displayLabel)
  {
    m_osGraph << "LabelGraphics [ text \"";

    if (m_displayIdentifier)
    {
      m_osGraph << "(" << node.Identifier() << ")\n";
    }

    if (displayName)
    {
      m_osGraph << node.Name();
    }

    if (displayNotes)
    {
      m_osGraph << "\n" << notes;
    }

    m_osGraph << "\" fontSize 13 fontStyle \"bold\" fontName \"" << m_fontName << "\"]";
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
void GmlPrinterVisitor::PrintEdge (const ParentNode&      parentNode,
                                   const SystemModelNode& childNode,
                                   uint32_t               childId,
                                   string_view            style,
                                   string_view            note)
{
  m_osEdges << "   edge ["
            << " source "  << parentNode.Identifier()
            << " target "  << childNode.Identifier();

  if (childId != 0)
  {
    m_osEdges << " label \"" << childId;
    if (!note.empty())
    {
      m_osEdges << note;
    }
    m_osEdges << "\"";
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
string GmlPrinterVisitor::Graph ()
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
//  End of: GmlPrinterVisitor::Graph
//---------------------------------------------------------------------------



//! Appends AccessInterface node to GML graph
//!
void GmlPrinterVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  AppendParentNode(m_shape_AccessInterface, m_color_AccessInterface, "", accessInterface);
}

//! Appends Chain node to GML graph
//!
void GmlPrinterVisitor::VisitChain (Chain& chain)
{
  AppendParentNode(m_shape_Chain, m_color_Chain, "", chain);
}

//! Appends Linker node to GML graph
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void GmlPrinterVisitor::VisitLinker (Linker& linker)
{
  string notes;

  if (!m_showSelectorWithEdge)
  {
    auto selector           = linker.Selector();
    auto associatedRegister = selector->AssociatedRegister();
    if (associatedRegister)
    {
      ostringstream os;
      os << ":" << associatedRegister->Identifier() << ":";
      notes = os.str();
    }
  }

  AppendParentNode(m_shape_Linker, m_color_Linker, notes, linker);

  // ---------------- Deal with path selector
  //
  auto selector = linker.Selector();
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
    if (m_showSelectorWithEdge)
    {
      PrintEdge(*m_linker, reg, 0, "dashed");
    }
    m_linker = nullptr;   // Only first SystemModelNode is connected to the linker
  }
  else
  {
    if (m_displayRegisterValue)
    {
      ostringstream os;

      auto regValue = [this](auto& regValue) { return m_displayRegValueAuto ? regValue.DataAsMixString() : regValue.DataAsBinaryString(); };

      os << "Width: "     << reg.BypassSequence().BitsCount() << std::endl;

      if (reg.HoldValue())
      {
        os << "Hold value: true" << std::endl;
      }

      os << "Bypass:    " << regValue(reg.BypassSequence())  << std::endl;
      os << "Next to:   " << regValue(reg.NextToSut())       << std::endl;
      os << "Last to:   " << regValue(reg.LastToSut())       << std::endl;
      os << "Last from: " << regValue(reg.LastFromSut())     << std::endl;
      os << "Expected:  " << regValue(reg.ExpectedFromSut());

      AppendNode(m_shape_Register, "", m_color_Register, os.str(), reg);
    }
    else
    {
      AppendNode(m_shape_Register, "", m_color_Register, "", reg);
    }
  }
}

//===========================================================================
// End of GmlPrinterVisitor.cpp
//===========================================================================
