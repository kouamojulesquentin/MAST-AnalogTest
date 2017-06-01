//===========================================================================
//                           GmlPrinter.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GmlPrinter.hpp
//!
//! Declares class GmlPrinter
//!
//===========================================================================


#ifndef GMLPRINTER_H__C3248668_6D34_4480_B5A2_38F17955B4CE__INCLUDED_
  #define GMLPRINTER_H__C3248668_6D34_4480_B5A2_38F17955B4CE__INCLUDED_

#include "SystemModelVisitor.hpp"
#include "SystemModelNode.hpp"

#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <experimental/string_view>

namespace mast
{
class PathSelector;
class Linker;

//! Options of Gml graph constructions
//!
enum class GmlPrinterOptions
{
  Default                = 0,
  DisplayIdentifiers     = 1 << 0, //!< To display node identifier
  DisplayRegisterValue   = 1 << 1, //!< To show Register values
  DisplayValueAuto       = 1 << 2, //!< To show Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
  ShowProtocol           = 1 << 3, //!< To show Protocol name of an access interface
  ShowSelectorWithEdge   = 1 << 4, //!< To show Linker selector associated register with an edge between the Linker and the Register
  ShowSelectorProperties = 1 << 5, //!< To show selector properties
  ShowSelectorTables     = 1 << 6, //!< To show selector tables for selection/deselection
  ShowSelectionValues    = 1 << 7, //!< To show Selector Register value along edge between linker and derivations

  Std                  = DisplayIdentifiers | DisplayRegisterValue | DisplayValueAuto | ShowSelectionValues,
  All                  = Std | ShowProtocol | ShowSelectionValues,
};

//! System model visitors for creation of a GML formated representation of the
//! system mode tree
//!
class MAST_CORE_EXPORT GmlPrinter : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~GmlPrinter() = default;
  GmlPrinter()
    : GmlPrinter("", GmlPrinterOptions::Default)
  {}

  using string_view = std::experimental::string_view;

  GmlPrinter(string_view graphName, GmlPrinterOptions options = GmlPrinterOptions::Default);

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  std::string  Graph();  //!< Returns currently visited nodes graph representation


  bool DisplayIdentifier()    const { return m_displayIdentifier;    } //!< Returns whether node identifier are displayed or not
  bool DisplayRegisterValue() const { return m_displayRegisterValue; } //!< Returns whether registers value are displayed (below the name)
  bool DisplayValueAuto()     const { return m_displayRegValueAuto;  } //!< Returns whether registers value are displayed as hexadecimal string (otherwise they are displayed as binary)

  void DisplayIdentifier    (bool displayIdentifier)    { m_displayIdentifier    = displayIdentifier;    } //!< Sets whether node identifier are displayed or not
  void DisplayRegisterValue (bool displayRegisterValue) { m_displayRegisterValue = displayRegisterValue; } //!< Sets whether registers value are displayed (below the name)
  void DisplayValueAuto     (bool displayValueAuto)     { m_displayRegValueAuto  = displayValueAuto;     } //!< Sets whether registers values are displayed as hexadecimal string if large enough and not complete nibble as binary

  //! Returns graph from a node (usually a ParentNode)
  //!
  static std::string Graph(SystemModelNode& topNode, std::experimental::string_view graphName = "", GmlPrinterOptions options = GmlPrinterOptions::Std);

  //! Returns graph from a node (usually a ParentNode)
  //!
  static std::string Graph(std::shared_ptr<SystemModelNode> topNode, std::experimental::string_view graphName = "", GmlPrinterOptions options = GmlPrinterOptions::Std);

  // ---------------- Private  Methods
  //
  private:

  void CreateRoot();
  void CloseRoot();

  void AppendParentNode (string_view       shapeName,
                         string_view       backgroundColor,
                         string_view       notes,
                         const ParentNode& parentNode
                        );

  void AppendNode       (uint32_t    nodeIdentifier,
                         string_view nodeName,
                         string_view shapeName,
                         string_view outlineStyle,
                         string_view backgroundColor,
                         string_view notes);

  void AppendNode       (string_view            shapeName,
                         string_view            outlineStyle,
                         string_view            backgroundColor,
                         string_view            notes,
                         const SystemModelNode& node
                         );

  void AppendSelector (const Linker& linker, const PathSelector& selector);

  void PrintEdge (uint32_t    sourceId,
                  uint32_t    targetId,
                  uint32_t    edgeId,
                  string_view style = "",
                  string_view note  = "");

  void PrintEdge (const ParentNode&      parentNode,
                  const SystemModelNode& childNode,
                  uint32_t               edgeId,
                  string_view            style = "",
                  string_view            note  = "");

  std::string FormattedAssociateRegisterId (const Linker& linker);

  // ---------------- Private  Fields
  //
  private:
  using SelectorMapper_t = std::map<const PathSelector*, uint32_t>; //!< To remenber already processed PathSelector (when shared between several muxes)

  std::string        m_graphName;                          //!< Name associated to the all graph
  uint32_t           m_depth                  = 0u;        //!< Current nodes tree depth
  bool               m_visited                = false;     //!< Becomes true when a tree traversal has been completely done
  bool               m_displayIdentifier      = false;     //!< When true, node identifiers are displayed along with their name
  bool               m_displayRegisterValue   = false;     //!< When true, register values are displayed (below its name)
  bool               m_displayRegValueAuto    = false;     //!< When true, register values are displayed as hexadecimal string if large enough and not complete nibble as binary
  bool               m_showProtocol           = false;     //!< When true, protocol kind is displayed in AccessInterface box
  bool               m_showSelectorWithEdge   = false;     //!< When true an edge is drawn from Linkers and Registers used by the selector
  bool               m_showSelectorProperties = false;     //!< When true, selector properties are displayed
  bool               m_showSelectorTables     = false;     //!< When true, selector properties AND tables are displayed
  bool               m_showSelectionValues    = false;     //!< When true the value to select a Linker derivation is displayed along side of derivation id (label of the edge)
  const Linker*      m_linker                 = nullptr;   //!< When not nullptr, we are visiting a path selector (while visiting a linker)
  std::ostringstream m_osGraph;                            //!< Stream to build up a representation of visited system model nodes
  std::ostringstream m_osEdges;                            //!< Stream to build up links between nodes
  uint32_t           m_selectorNodeId         = INT32_MAX; //!< Node identifier when displaying selector properties (UINT32_MAX is not supported by yEd)
  SelectorMapper_t   m_processedSelectors;                 //!< To "recycle" graph node representing already processed PathSelector


  static const string_view m_shape_AccessInterface;
  static const string_view m_shape_Linker;
  static const string_view m_shape_Chain;
  static const string_view m_shape_Register;
  static const string_view m_shape_Selector;

  static const string_view m_color_AccessInterface;
  static const string_view m_color_Linker;
  static const string_view m_color_Chain;
  static const string_view m_color_Register;
  static const string_view m_color_Selector;

  static const string_view m_fontName;
};
//
//  End of GmlPrinter class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined GMLPRINTER_H__C3248668_6D34_4480_B5A2_38F17955B4CE__INCLUDED_
//===========================================================================
// End of GmlPrinter.hpp
//===========================================================================
