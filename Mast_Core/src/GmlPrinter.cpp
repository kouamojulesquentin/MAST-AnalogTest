//===========================================================================
//                           GmlPrinter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GmlPrinter.cpp
//!
//! Implements class GmlPrinter
//!
//===========================================================================

#include "GmlPrinter.hpp"
#include "SystemModelNodes.hpp"
#include "PathSelector.hpp"
#include "VirtualRegister.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"

#include <algorithm>
using std::shared_ptr;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using namespace std::string_literals;
using namespace mast;


const std::experimental::string_view GmlPrinter::m_shape_AccessInterface = "octagon";
const std::experimental::string_view GmlPrinter::m_shape_Linker          = "trapezoid";
const std::experimental::string_view GmlPrinter::m_shape_Chain           = "ellipse";
const std::experimental::string_view GmlPrinter::m_shape_Register        = "rectangle";
const std::experimental::string_view GmlPrinter::m_shape_Selector        = "rectangle";

const std::experimental::string_view GmlPrinter::m_color_AccessInterface = "#10FFFF";
const std::experimental::string_view GmlPrinter::m_color_Linker          = "#FF3060";
const std::experimental::string_view GmlPrinter::m_color_Chain           = "#FFCC20";
const std::experimental::string_view GmlPrinter::m_color_Register        = "#59FF20";
const std::experimental::string_view GmlPrinter::m_color_Selector        = "#E8E8E8";

const std::experimental::string_view GmlPrinter::m_fontName              = "Lucida Console";

//! Initializes with given options
//!
GmlPrinter::GmlPrinter(std::experimental::string_view graphName, GmlPrinterOptions options)
  : m_graphName              (graphName)
  , m_displayIdentifier      (IsSet(options, GmlPrinterOptions::DisplayIdentifiers))
  , m_displayRegisterValue   (IsSet(options, GmlPrinterOptions::DisplayRegisterValue))
  , m_displayRegValueAuto    (IsSet(options, GmlPrinterOptions::DisplayValueAuto))
  , m_showProtocol           (IsSet(options, GmlPrinterOptions::ShowProtocol))
  , m_showSelectorWithEdge   (IsSet(options, GmlPrinterOptions::ShowSelectorWithEdge))
  , m_showSelectorProperties (IsSet(options, GmlPrinterOptions::ShowSelectorProperties))
  , m_showSelectorTables     (IsSet(options, GmlPrinterOptions::ShowSelectorTables))
  , m_showSelectionValues    (IsSet(options, GmlPrinterOptions::ShowSelectionValues))
{
  CreateRoot();
}
//
//  End of: GmlPrinter::GmlPrinter
//---------------------------------------------------------------------------





//! Appends a parent node and its children to the GML graph
//!
//! @param shapeName        The shape to use
//! @param backgroundColor  Background color
//! @param notes            Optional node to add below the node name
//! @param parentNode       Parent node to process
//!
void GmlPrinter::AppendParentNode (string_view       shapeName,
                                   string_view       backgroundColor,
                                   string_view       notes,
                                   const ParentNode& parentNode)
{
  auto outlineStyle = parentNode.IgnoreForNodePath() ? "dashed" : "";
  AppendNode(shapeName, outlineStyle, backgroundColor, notes, parentNode);

  auto linker   = dynamic_cast<const Linker*>(&parentNode);
  auto selector = linker ? linker->Selector() : nullptr;

  if (selector && (m_showSelectorProperties || m_showSelectorTables))
  {
    AppendSelector(*linker, *selector);
  }

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

      if (selector && m_showSelectionValues)
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
          edgeStyle = ""; // Use plain line for active endpoint
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
//  End of: GmlPrinter::AppendParentNode
//---------------------------------------------------------------------------


//! Appends a node to the GML graph
//!
//! @param nodeIdentifier   Unique identifier for graph node
//! @param nodeNam          Node name (first text line of graph node)
//! @param shapeName        The shape to use
//! @param outlineStyle     Optional line type for shape (default when empty)
//! @param backgroundColor  Background color
//! @param notes            Optional node to add below the node name
//!
void GmlPrinter::AppendNode (uint32_t    nodeIdentifier,
                             string_view nodeName,
                             string_view shapeName,
                             string_view outlineStyle,
                             string_view backgroundColor,
                             string_view notes)
{
  m_osGraph << "   node [ id " << nodeIdentifier;

  auto displayName  = !nodeName.empty();
  auto displayNotes = !notes.empty();
  auto displayLabel = displayName || m_displayIdentifier || displayNotes;

  // ---------------- Compute height and width for the graphic
  //
  size_t labelCharWidth = nodeName.length();
  size_t linesCount     = 1;

  if (displayNotes)
  {
    ++linesCount;
    size_t startPos  = 0;
    size_t foundPos  = 0;
    size_t noteWidth = 0;

    while ((foundPos = notes.find('\n', startPos)) != string_view::npos)
    {
      ++linesCount;
      noteWidth = std::max(noteWidth, foundPos - startPos);
      startPos = ++foundPos;
    }

    if (noteWidth == 0)
    {
      noteWidth = notes.length();
    }
    labelCharWidth = std::max(labelCharWidth, noteWidth);
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
      m_osGraph << "(" << nodeIdentifier << ")\n";
    }

    if (displayName)
    {
      m_osGraph << nodeName;
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
//  End of: GmlPrinter::AppendNode
//---------------------------------------------------------------------------


//! Appends a node to the GML graph
//!
//! @param shapeName        The shape to use
//! @param outlineStyle     Optional line type for shape (default when empty)
//! @param backgroundColor  Background color
//! @param notes            Optional node to add below the node name
//! @param node             Node to process
//!
void GmlPrinter::AppendNode (string_view            shapeName,
                             string_view            outlineStyle,
                             string_view            backgroundColor,
                             string_view            notes,
                             const SystemModelNode& node)
{
  AppendNode(node.Identifier(), node.Name(), shapeName, outlineStyle, backgroundColor, notes);
}
//
//  End of: GmlPrinter::AppendNode
//---------------------------------------------------------------------------



//! Appends path selector associated with a Linker
//!
//! @param linker     Linker associated with the PathSelector
//! @param selector   PathSelector associated with the Linker
//!
void GmlPrinter::AppendSelector (const Linker& linker, const PathSelector& selector)
{
  if (m_processedSelectors.count(&selector) == 0)
  {
    auto notes    = selector.DebugSelectorInfo(!m_showSelectorTables);
    auto nodeName = "Selector " + FormattedAssociateRegisterId(linker);

    SET_FOR_SCOPE(m_displayIdentifier, false);
    AppendNode(m_selectorNodeId, nodeName, m_shape_Selector, "", m_color_Selector, notes);

    m_processedSelectors[&selector] = m_selectorNodeId--;
  }

  auto selectorNodeId = m_processedSelectors[&selector];
  PrintEdge(selectorNodeId, linker.Identifier(), 0, "dotted", "selector");
}
//
//  End of: GmlPrinter::AppendSelector
//---------------------------------------------------------------------------



//! Appends an edge between two nodes of the GML graph
//!
//! @param sourceId   Identifier of source node
//! @param targetId   Identifier of target node
//! @param edgeId     When != 0, this will be used as edge label
//! @param style      Edge style
//! @param note       Edge optional notes
//!
void GmlPrinter::PrintEdge (uint32_t    sourceId,
                            uint32_t    targetId,
                            uint32_t    edgeId,
                            string_view style,
                            string_view note)
{
  m_osEdges << "   edge ["
            << " source "  << sourceId
            << " target "  << targetId;

  if (edgeId != 0)
  {
    m_osEdges << " label \"" << edgeId;
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
//  End of: GmlPrinter::PrintEdge
//---------------------------------------------------------------------------


//! Appends an edge between two nodes to the GML graph
//!
//! @param parentNode A parent node
//! @param childNode  A child of the parent node
//! @param edgeId     When != 0, this will be used as edge label
//! @param style      Edge style
//! @param note       Edge optional notes
//!
void GmlPrinter::PrintEdge (const ParentNode&      parentNode,
                            const SystemModelNode& childNode,
                            uint32_t               edgeId,
                            string_view            style,
                            string_view            note)
{
  PrintEdge(parentNode.Identifier(), childNode.Identifier(), edgeId, style, note);
}
//
//  End of: GmlPrinter::PrintEdge
//---------------------------------------------------------------------------




//! Closes root node
//!
void GmlPrinter::CloseRoot ()
{
  if (m_depth != 0)
  {
    THROW_LOGIC_ERROR("Closing GML graph should only occur when terminating visiting System Model tree root node");
  }

  m_osGraph << m_osEdges.str();
  m_osGraph << "]";
}
//
//  End of: GmlPrinter::CloseRoot
//---------------------------------------------------------------------------


//! Creates root node
//!
void GmlPrinter::CreateRoot ()
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
//  End of: GmlPrinter::CloseRoot
//---------------------------------------------------------------------------



//! Returns formatted reference to Linker PathSelector associated register
//!
//! @param linker A Linker
//!
//! @return Associated register identifier surrounded with colons
string GmlPrinter::FormattedAssociateRegisterId (const Linker& linker)
{
  auto selector            = linker.Selector();
  auto associatedRegisters = selector->AssociatedRegisters();
  if (!associatedRegisters)
  {
    return "";
  }

  ostringstream os;

  for (const auto& sliceReg : *associatedRegisters)
  {
    os << ":" << sliceReg.reg->Identifier();
  }

  os << ":";
  return os.str();
}
//
//  End of: GmlPrinter::FormattedAssociateRegisterId
//---------------------------------------------------------------------------


//! Returns currently visited nodes representation and edges
//!
string GmlPrinter::Graph ()
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
//  End of: GmlPrinter::Graph
//---------------------------------------------------------------------------




//! Returns graph from a parent node
//!
//! @param topNode    Node from which graph is created
//! @param graphName  Optional graph name
//! @param options    Graph options
//!
string GmlPrinter::Graph (SystemModelNode& topNode, string_view graphName, GmlPrinterOptions options)
{
  GmlPrinter printer(graphName, options);
  topNode.Accept(printer);
  return printer.Graph();
}
//
//  End of: GmlPrinter::Graph
//---------------------------------------------------------------------------


//! Returns graph from a parent node
//!
//! @param topNode    Node from which graph is created
//! @param graphName  Optional graph name
//! @param options    Graph options
//!
string GmlPrinter::Graph (shared_ptr<SystemModelNode> topNode, string_view graphName, GmlPrinterOptions options)
{
  CHECK_PARAMETER_NOT_NULL(topNode, "Cannot create GML graph from nullptr");
  return Graph(*topNode, graphName, options);
}
//
//  End of: GmlPrinter::Graph
//---------------------------------------------------------------------------


//! Appends AccessInterface node to GML graph
//!
void GmlPrinter::VisitAccessInterface (AccessInterface& accessInterface)
{
  auto protocol = accessInterface.Protocol();
  auto note     = string();
  if (m_showProtocol)
  {
    note = "Protocol: ";
    note += protocol ? protocol->KindName() : "Not set";
  }

  AppendParentNode(m_shape_AccessInterface, m_color_AccessInterface, note, accessInterface);
}

//! Appends AccessInterfaceTranslator node to GML graph
//! TODO: configure a specific appearance
//!
void GmlPrinter::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  auto protocol = accessInterfaceTranslator.Protocol();
  auto note     = string();

  if (m_showProtocol)
  {
    note = "Protocol: ";
    note += protocol ? protocol->KindName() : "Not set";
  }

  AppendParentNode(m_shape_AccessInterface, m_color_AccessInterface, note, accessInterfaceTranslator);
}

//! Appends Chain node to GML graph
//!
void GmlPrinter::VisitChain (Chain& chain)
{
  AppendParentNode(m_shape_Chain, m_color_Chain, "", chain);
}

//! Appends Linker node to GML graph
//!
//! @note Supposes that path selector associated with linker will be made of SystemModelNode too
void GmlPrinter::VisitLinker (Linker& linker)
{
  string notes;

  if (!m_showSelectorWithEdge)
  {
    notes = FormattedAssociateRegisterId(linker);
  }

  AppendParentNode(m_shape_Linker, m_color_Linker, notes, linker);

  // ---------------- Deal with path selector
  //
  auto selector = linker.Selector();
  if (selector)
  {
    m_linker   = &linker;
    selector->Accept(*this);
    m_linker   = nullptr;
  }
}

 //! Appends Register node to GML graph
//!
void GmlPrinter::VisitRegister (Register& reg)
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

//! Appends BlackBox node to GML graph
//!
void GmlPrinter::VisitBlackBox (BlackBox& bbox)
{
  // ---------------- Deal with path selector associated with Linker nodes
  //
    if (m_displayRegisterValue)
    {
      ostringstream os;

      auto regValue = [this](auto& regValue) { return m_displayRegValueAuto ? regValue.DataAsMixString() : regValue.DataAsBinaryString(); };

      os << "Width: "     << bbox.BypassSequence().BitsCount() << std::endl;

      if (bbox.HoldValue())
      {
        os << "Hold value: true" << std::endl;
      }

      os << "Bypass:    " << regValue(bbox.BypassSequence())  << std::endl;
      os << "Next to:   " << regValue(bbox.NextToSut())       << std::endl;
      os << "Last to:   " << regValue(bbox.LastToSut())       << std::endl;
      os << "Last from: " << regValue(bbox.LastFromSut())     << std::endl;
      os << "Expected:  " << regValue(bbox.ExpectedFromSut());

      AppendNode(m_shape_Register, "", m_color_Register, os.str(), bbox);
    }
    else
    {
      AppendNode(m_shape_Register, "", m_color_Register, "", bbox);
    }
}

//===========================================================================
// End of GmlPrinter.cpp
//===========================================================================
