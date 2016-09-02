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

//! Options of Gml graph constructions
//!
enum class GmlPrinterOptions
{
  Default              = 0,
  DisplayIdentifiers   = 0b000001,  //!< To display node identifier
  DisplayRegisterValue = 0b000010,  //!< To show Register values
  DisplayValueAuto     = 0b000100,  //!< To show Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
  ShowProtocol         = 0b001000, //!< To show Linker selector associated register with an edge between the Linker and the Register
  ShowSelectorWithEdge = 0b010000,  //!< To show Linker selector associated register with an edge between the Linker and the Register
  ShowSelectionValues  = 0b100000,  //!< To show Selector Register value along edge between linker and derivations
  Std                  = DisplayIdentifiers | DisplayRegisterValue | DisplayValueAuto | ShowSelectionValues,
  All                  = Std | ShowProtocol | ShowSelectionValues,
};

#include <type_traits>
constexpr GmlPrinterOptions operator | (GmlPrinterOptions X, GmlPrinterOptions Y)
{
  return static_cast<GmlPrinterOptions>(  static_cast<std::underlying_type_t<GmlPrinterOptions>>(X)
                                        | static_cast<std::underlying_type_t<GmlPrinterOptions>>(Y)
                                       );
}

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
    : GmlPrinterVisitor("", GmlPrinterOptions::Default)
  {}

  GmlPrinterVisitor(std::experimental::string_view graphName, bool displayIdentifiers = false, bool displayRegisterValue = false, bool displayValueAuto = false)
    : m_graphName            (graphName)
    , m_displayIdentifier    (displayIdentifiers)
    , m_displayRegisterValue (displayRegisterValue)
    , m_displayRegValueAuto  (displayValueAuto)
  {
    CreateRoot();
  }

  GmlPrinterVisitor(std::experimental::string_view graphName, GmlPrinterOptions options = GmlPrinterOptions::Default);

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
  static std::string Graph(std::shared_ptr<SystemModelNode> topNode, std::experimental::string_view graphName = "", GmlPrinterOptions options = GmlPrinterOptions::Std);

  // ---------------- Private  Methods
  //
  private:

  void CreateRoot();
  void CloseRoot();

  void AppendParentNode (std::experimental::string_view shapeName,
                         std::experimental::string_view backgroundColor,
                         std::experimental::string_view notes,
                         const ParentNode&              parentNode
                        );

  void AppendNode       (std::experimental::string_view shapeName,
                         std::experimental::string_view outlineStyle,
                         std::experimental::string_view backgroundColor,
                         std::experimental::string_view notes,
                         const SystemModelNode&         node
                         );

  void PrintEdge        (const ParentNode& parentNode, const SystemModelNode& childNode, uint32_t childId, std::experimental::string_view style = "", std::experimental::string_view note = "");

  // ---------------- Private  Fields
  //
  private:
  std::string        m_graphName;                      //!< Name associated to the all graph
  uint32_t           m_depth                = 0u;      //!< Current nodes tree depth
  bool               m_visited              = false;   //!< Becomes true when a tree traversal has been completely done
  bool               m_displayIdentifier    = false;   //!< When true, node identifiers are displayed along with their name
  bool               m_displayRegisterValue = false;   //!< When true, register values are displayed (below its name)
  bool               m_displayRegValueAuto  = false;   //!< When true, register values are displayed as hexadecimal string if large enough and not complete nibble as binary
  bool               m_showProtocol         = false;   //!< When true, protocol kind is displayed in AccessInterface box
  bool               m_showSelectorWithEdge = false;   //!< When true an edge is drawn from Linkers and Registers used by the selector
  bool               m_showSelectionValues  = false;   //!< When true the value to select a Linker derivation is displayed along side of derivation id (label of the edge)
  const Linker*      m_linker               = nullptr; //!< When not nullptr, we are visiting a path selector (while visiting a linker)
  std::ostringstream m_osGraph;                        //!< Stream to build up a representation of visited system model nodes
  std::ostringstream m_osEdges;                        //!< Stream to build up links between nodes

  static const std::experimental::string_view m_shape_AccessInterface;
  static const std::experimental::string_view m_shape_Linker;
  static const std::experimental::string_view m_shape_Chain;
  static const std::experimental::string_view m_shape_Register;

  static const std::experimental::string_view m_color_AccessInterface;
  static const std::experimental::string_view m_color_Linker;
  static const std::experimental::string_view m_color_Chain;
  static const std::experimental::string_view m_color_Register;

  static const std::experimental::string_view m_fontName;
};
//
//  End of GmlPrinterVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined GMLPRINTERVISITOR_H__2DA805E1_7CD4_4D2D_2AB8_5A6B17EEEB3D__INCLUDED_

//===========================================================================
// End of GmlPrinterVisitor.hpp
//===========================================================================
