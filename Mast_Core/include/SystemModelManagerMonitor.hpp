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

#include "GmlPrinter.hpp"
#include "Platform.hpp"
#include <string>

namespace mast
{
class ParentNode;

//! Options for SystemModelManagerMonitor
//!
enum class ManagerMonitorOptions
{
  Min                 = 0,
  AppThreadCreation   = 1 << 0, //!< Monitor application thread creation
  BeforeConfiguration = 1 << 1, //!< Monitor SystemModel state before configuration
  AfterConfiguration  = 1 << 2, //!< Monitor SystemModel state after configuration
  ExportGml           = 1 << 3, //!< Tells whether GML graph is exported (before/after  configuration)
  ExportPrettyPrint   = 1 << 4, //!< Tells whether a "Pretty Print" is exported (before/after  configuration)

  Std                 = AppThreadCreation | BeforeConfiguration | AfterConfiguration,
  All                 = Std | ExportGml | ExportPrettyPrint,
};

//! Provides monitoring of SystemModelManager behavior and SystemModel state
//!
class DLL_EXPORT SystemModelManagerMonitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelManagerMonitor() = default;
  SystemModelManagerMonitor()  = default;

  //! Returns current monitoring options
  //!
  ManagerMonitorOptions  Options() const { return m_options; }

  //! Sets monitoring options
  //!
  void  Options (ManagerMonitorOptions options) { m_options = options; }


  using string_view = std::experimental::string_view;

  void LogUncondionally(string_view message);                                                     //!< Always log message
  void LogUncondionally(string_view message, const SystemModelNode& node, string_view debugName); //!< Always log message (in relation with a node)

  virtual void Reset();                                                                //!< Resets data cyles counter
  virtual void CreateApplication   (const ParentNode& topNode, string_view debugName); //!< Monitors creation of application thread
  virtual void StartDataCycles();                                                      //!< Monitors start of new data cycles
  virtual void StartDataCycle();                                                       //!< Monitors start of a new data cycle
  virtual void BeforeConfiguration (ParentNode&       root);                           //!< Monitors state of SystemModel (from parentNode) before configuration
  virtual void AfterConfiguration  (ParentNode&       root);                           //!< Monitors state of SystemModel (from parentNode) after configuration

  std::string ExportBasePath() const { return m_exportBasePath; }

  void ExportBasePath (std::string exportBasePath) { m_exportBasePath = exportBasePath; }


  // ---------------- Protected Methods
  //
  protected:
  void        SaveToFile        (string_view text,     string_view basePath,  string_view extension, string_view step) const;
  std::string MakeFilePath      (string_view basePath, string_view extension, string_view step) const;
  void        ExportGml         (string_view step,     ParentNode& root);
  void        ExportPrettyPrint (string_view step,     ParentNode& root);

  static std::string NodeInfos    (const SystemModelNode& node);

  //! Wraps debug name within simple quotes
  std::string WrapDebugName (string_view debugName) { return debugName.empty() ? "" : "'" + debugName.to_string() + "': "; }

  // ---------------- Private  Fields
  //
  private:
  uint32_t              m_dataCyclesCount = 0; //!< Number of data cycles since startup or last reset
  std::string           m_exportBasePath;      //!< Base path use when exporting graph representing system model
  ManagerMonitorOptions m_options         = ManagerMonitorOptions::Std;
};
//
//  End of SystemModelManagerMonitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGERMONITOR_H__831655E4_CE44_4DFD_F886_30ECFC1CC3FC__INCLUDED_

//===========================================================================
// End of SystemModelManagerMonitor.hpp
//===========================================================================
