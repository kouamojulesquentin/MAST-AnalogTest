//===========================================================================
//                           MSPG_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MSPG_TranslatorProtocol.hpp
//!
//! Declares MSPG_TranslatorProtocol class
//!
//===========================================================================


#ifndef MSPG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define MSPG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT MSPG_TranslatorProtocol final : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~MSPG_TranslatorProtocol()
  {
   if (m_ofs.is_open())
    {
      m_ofs.close();
    }
  } 

  
  //! Constructor with no parameters
  //!
   MSPG_TranslatorProtocol()
   {
    m_ofs.open(m_EmulationLog, std::ios_base::trunc);

   if (!m_ofs.is_open())
   {
     THROW_RUNTIME_ERROR("Cannot open output file: " + m_EmulationLog);
   }
  }
 
  // Constructor with parameters defined by a string
  //
  //MSPG_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "MSPG"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Space for internal utility functions
  //!
  std::string               m_EmulationLog   = "Emulation.log";      //!< File used to log Emulated commands
  std::ofstream             m_ofs;
  

  // ---------------- Private  Fields for internal uses
  //
};
//
//  End of MSPG_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined MSPG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of MSPG_TranslatorProtocol.hpp
//===========================================================================
