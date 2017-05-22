//===========================================================================
//                           SystemModelManagerMonitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelManagerMonitor.hpp
//!
//! Declares SystemModelManagerMonitor class
//!
//===========================================================================


#ifndef SYSTEMMODELMANAGERMONITOR_H__831655E4_CE44_4DFD_F886_30ECFC1CC3FC__INCLUDED_
  #define SYSTEMMODELMANAGERMONITOR_H__831655E4_CE44_4DFD_F886_30ECFC1CC3FC__INCLUDED_

#include "Mast_Core_export.hpp"
#include <string>
#include <experimental/string_view>

namespace mast
{
class SystemModelNode;
class ParentNode;
class BinaryVector;

//! Options for SystemModelManagerMonitor
//!
enum class ManagerMonitorOptions
{
  Min                 = 0,
  Verbose             = 1 << 0, //!< When active, more information may be reported
  AppThreadCreation   = 1 << 1, //!< Monitor application thread creation
  PDLCommands         = 1 << 2, //!< Monitor PDL commands issued by applications
  ExportGml           = 1 << 3, //!< Tells whether GML graph is exported (before/after  configuration)
  ExportPrettyPrint   = 1 << 4, //!< Tells whether a "Pretty Print" is exported (before/after  configuration)
  DataCycles          = 1 << 5, //!< Monitor data cycle main events
  BeforeConfiguration = 1 << 6, //!< Monitor SystemModel state before configuration
  AfterConfiguration  = 1 << 7, //!< Monitor SystemModel state after configuration
  InternalDebug       = 1 << 8, //!< This are message for internal manager debug (it is normally of no use for applications)

  Std                 = AppThreadCreation | PDLCommands,
  Std_Debug           = Std | InternalDebug,
  All                 = Std | BeforeConfiguration | AfterConfiguration | ExportGml | ExportPrettyPrint,
  All_Debug           = All | InternalDebug,
};

//! Provides monitoring of SystemModelManager behavior and SystemModel state
//!
class MAST_CORE_EXPORT SystemModelManagerMonitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelManagerMonitor() = default;
  SystemModelManagerMonitor()  = default;
  SystemModelManagerMonitor(ManagerMonitorOptions options) : m_options(options) {}

  //! Returns current monitoring options
  //!
  ManagerMonitorOptions  Options() const { return m_options; }

  //! Sets monitoring options
  //!
  void  Options (ManagerMonitorOptions options) { m_options = options; }


  using string_view = std::experimental::string_view;

  void LogDebug(string_view message);                                                     //!< Log internal, debug, message
  void LogDebug(string_view message, const SystemModelNode& node, string_view debugName); //!< Log internal, debug, message (in relation with a node)

  virtual void Reset();                                                                //!< Resets data cyles counter
  virtual void CreateApplication   (const ParentNode& topNode, string_view debugName); //!< Monitors creation of application thread

  //! Monitors PDL commands issued by applications
  //!
  virtual void PDLCommand (string_view message, string_view nodePath, const ParentNode& topNode, string_view debugName);

  //! Logs a PDL command with associated BinaryVector value
  //!
  virtual void PDLCommand (string_view message, string_view nodePath, const BinaryVector& value, const ParentNode& topNode, string_view debugName);

  virtual void StartBackgroundThread();                                                                     //!< Monitors start of SystemModelManager background thread
  virtual void StartDataCycles();                                                                           //!< Monitors start of new data cycles
  virtual void StartDataCycle();                                                                            //!< Monitors start of a new data cycle
  virtual void BeforeConfiguration (ParentNode&       root);                                                //!< Monitors state of SystemModel (from parentNode) before configuration
  virtual void AfterConfiguration  (ParentNode&       root);                                                //!< Monitors state of SystemModel (from parentNode) after configuration


  std::string ExportBasePath() const { return m_exportBasePath; } //!< Returns the file path (without extension) for export files (extension depends on export type)

  void ExportBasePath (std::string exportBasePath) { m_exportBasePath = exportBasePath; } //!< Set file path (without extension) for export files (extension depends on export type)


  // ---------------- Protected Methods
  //
  protected:
  void        SaveToFile        (string_view text,     string_view basePath,  string_view extension, string_view step) const;
  std::string MakeFilePath      (string_view basePath, string_view extension, string_view step) const;
  void        ExportGml         (string_view step,     ParentNode& root);
  void        ExportPrettyPrint (string_view step,     ParentNode& root);

  std::string NodeInfos (const SystemModelNode& node, bool ignoreVerboseMode = false) const;

  std::string NodePath (string_view nodePath) const;

  //! Wraps debug name within simple quotes
  std::string WrapDebugName (string_view debugName) { return debugName.empty() ? "" : "\"" + debugName.to_string() + "\" "; }

  // ---------------- Private  Fields
  //
  private:
  uint32_t              m_dataCyclesCount = 0; //!< Number of data cycles since startup or last reset
  std::string           m_exportBasePath;      //!< Base path use when exporting graph representing system model
  ManagerMonitorOptions m_options         = ManagerMonitorOptions::Std;
//+  ManagerMonitorOptions m_options         = ManagerMonitorOptions::Std_Debug;
};
//
//  End of SystemModelManagerMonitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGERMONITOR_H__831655E4_CE44_4DFD_F886_30ECFC1CC3FC__INCLUDED_

//===========================================================================
// End of SystemModelManagerMonitor.hpp
//===========================================================================
