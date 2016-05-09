//===========================================================================
//                           PrettyPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PrettyPrinterVisitor.hpp
//!
//! Declares PrettyPrinterVisitor class
//!
//===========================================================================


#ifndef PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_
  #define PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_

#include "SystemModelVisitor.hpp"
#include <string>
#include <sstream>
#include <experimental/string_view>

namespace mast
{
class BinaryVector;

//! System model visitors for creation of a text, readable, and hierarchical
//! representation of the system mode tree
//!
class DLL_EXPORT PrettyPrinterVisitor : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~PrettyPrinterVisitor() = default;
  PrettyPrinterVisitor()  = default;

  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;
  virtual void VisitTap             (Tap&             tap)             override;

  std::string GetPrettyPrint() const { return m_os.str(); }       //!< Returns currently visited nodes representation
  bool        GetVerbose()     const { return m_verbose;  }       //!< Returns whether verbose mode is active

  void        SetVerbose (bool verbose) { m_verbose = verbose; }  //!< Changes verbosity mode


  // ---------------- Private  Methods
  //
  private:

  void PrintChildren (const ParentNode& parentNode);

  void AlignOnNewLine  (std::fpos<int> targetPos);
  void AlignRelativeTo (std::fpos<int> refPos, std::fpos<int> targetPos);

  void StreamDepth()
  {
    m_os << std::string(m_depth, ' ');
  }

  void StreamBinaryVector (std::experimental::string_view name, const BinaryVector&    bits);
  void StreamNodeCommon   (const SystemModelNode&         node);
  void StreamNodeHeader   (std::experimental::string_view type, const SystemModelNode& node);
  void StreamParentNode   (std::experimental::string_view type, const ParentNode&      parentNode);

  // ---------------- Private  Fields
  //
  private:
  uint32_t           m_depth              = 0u;    //!< Current nodes tree depth
  std::ostringstream m_os;                         //!< Stream to build up a representation of visited system model nodes
  bool               m_processingSelector = false; //!< When true, we are visiting a path selector (while visiting a linker)
  std::fpos<int>     m_startPos           = 0;     //!< Position, in stream, of first character of current line
  bool               m_verbose            = false; //!< When true, more information are printed
  bool               m_first              = true;  //!< True when nothing as been streamed yet (useful to add first new line)
};
//
//  End of PrettyPrinterVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PRETTYPRINTERVISITOR_H__DF84532_49ED_4EAF_9A8D_7D71C57AA2E6__INCLUDED_

//===========================================================================
// End of PrettyPrinterVisitor.hpp
//===========================================================================
