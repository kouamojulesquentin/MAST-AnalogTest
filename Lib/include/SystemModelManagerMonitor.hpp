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

#include "GmlPrinterVisitor.hpp"
#include "Platform.hpp"
#include <string>

namespace mast
{
class ParentNode;

//! Provides monitoring of SystemModelManager behavior and SystemModel state
//!
class DLL_EXPORT SystemModelManagerMonitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SystemModelManagerMonitor() = default;
  SystemModelManagerMonitor()  = default;

  virtual void Reset();                                         //!< Resets data cyles counter
  virtual void CreateApplication   (const ParentNode& topNode); //!< Monitors creation of application thread
  virtual void StartDataCycles();                               //!< Monitors start of new data cycles
  virtual void StartDataCycle();                                //!< Monitors start of a new data cycle
  virtual void BeforeConfiguration (ParentNode&       root);    //!< Monitors state of SystemModel (from parentNode) before configuration
  virtual void AfterConfiguration  (ParentNode&       root);    //!< Monitors state of SystemModel (from parentNode) after configuration

  std::string GmlBasePath()                const { return m_gmlPrinterBasePath;         }
  bool        MonitorAfterConfiguration()  const { return m_monitorAfterConfiguration;  }
  bool        MonitorBeforeConfiguration() const { return m_monitorBeforeConfiguration; }

  void GmlBasePath                (std::string gmlBasePath)                { m_gmlPrinterBasePath         = gmlBasePath;                }
  void MonitorAfterConfiguration  (bool        monitorAfterConfiguration)  { m_monitorAfterConfiguration  = monitorAfterConfiguration;  }
  void MonitorBeforeConfiguration (bool        monitorBeforeConfiguration) { m_monitorBeforeConfiguration = monitorBeforeConfiguration; }

  // ---------------- Protected Methods
  //
  protected:
  std::string MakeFilePath (std::experimental::string_view basePath, std::experimental::string_view step);
  void        ExportGml    (std::experimental::string_view step, ParentNode& root);

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  uint32_t          m_dataCyclesCount            = 0;     //!< Number of data cycles since startup or last reset
  bool              m_monitorAppThreadCreation   = true;  //!< True when requested to monitor application thread creation
  bool              m_monitorAfterConfiguration  = false; //!< True when requested to monitor SystemModel state after configuration
  bool              m_monitorBeforeConfiguration = false; //!< True when requested to monitor SystemModel state before configuration
  GmlPrinterVisitor m_gmlMonitor;                         //!< Visitor for building graph with SystemModel state
  std::string       m_gmlPrinterBasePath;                 //!< Base path use when exporting graph representing system model
};
//
//  End of SystemModelManagerMonitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELMANAGERMONITOR_H__831655E4_CE44_4DFD_F886_30ECFC1CC3FC__INCLUDED_

//===========================================================================
// End of SystemModelManagerMonitor.hpp
//===========================================================================
