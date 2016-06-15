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

  std::string PrettyPrint()  const { return m_os.str();     } //!< Returns currently visited nodes representation
  bool        IsVerbose()    const { return m_verbose;      } //!< Returns whether verbose mode is active
  bool        UseAutoFormat() const { return m_useAutoFormat; } //!< Returns true if pretty printer is configured to print Register values as hexadecimal string

  void SetVerbose    (bool verbose)       { m_verbose       = verbose;       } //!< Changes verbosity mode
  void UseAutoFormat (bool useAutoFormat) { m_useAutoFormat = useAutoFormat; } //!< Sets whether pretty printer is configured to print Register values as hexadecimal string (except not complete nibbles that are displayed using binary format)


  // ---------------- Private  Methods
  //
  private:
  using pos_type = std::ostringstream::pos_type;

  void PrintChildren (const ParentNode& parentNode);

  void AlignOnNewLine  (pos_type targetPos);
  void AlignRelativeTo (pos_type refPos, pos_type targetPos);

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
  pos_type           m_startPos           = 0;     //!< Position, in stream, of first character of current line
  bool               m_useAutoFormat      = false; //!< When true, register values are displayed as hexadecimal string if large enough and not complete nibble as binary
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
