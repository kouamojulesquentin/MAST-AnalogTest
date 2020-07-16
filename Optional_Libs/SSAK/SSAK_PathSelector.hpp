//===========================================================================
//                           SSAK_PathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SSAK_PathSelector.hpp
//!
//! Declares class SSAK_PathSelector
//!
//===========================================================================

#ifndef SSAK_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_
  #define SSAK_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

#include "PathSelector.hpp"
#include "BinaryVector.hpp"
#include "VirtualRegister.hpp"
#include "Mast_Core_export.hpp"
#include "EnumsUtility.hpp"
#include "SSAKplugin.h"

#include <vector>
#include <memory>
#include <experimental/string_view>
#include <ostream>

namespace mast
{
class Register;

//! Base class for implementation of path selector that are based upon a LUTs
//! for selection and deselection values
//!
//! @note Do not support independent selection
//!       Only one path can be selected at any given time (unless there are voluntarily coupled)
//! @note Do not support independent deselection
//!
//! @note Path identifier are defined in range [1..nb_path]
//!
class MAST_CORE_EXPORT SSAK_PathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~SSAK_PathSelector() = default;
  SSAK_PathSelector()  = delete;

  using TablesType = std::vector<BinaryVector> ; //!< Selection/deselection LUT types

/*  SSAK_PathSelector(std::shared_ptr<Register> associatedRegister,
                                uint32_t               pathsCount,
				const std::string& parameters);
*/
  //! Constructor Compatible with Hybrid factory creator (multiple registers)
  //!
  SSAK_PathSelector(std::vector<std::shared_ptr<Register>> associatedRegisters,
                                uint32_t               pathsCount,
				const std::string& parameters);

  virtual bool IsActive            (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathIdentifier) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path

  virtual const VirtualRegister* AssociatedRegisters() const override { return &m_muxRegisters; }  //!< Returns associated Registers

  virtual uint32_t SelectablePaths() const override { return m_pathsCount; };   //!< Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)

  virtual uint32_t ActiveCount() const override;    //!< Returns the number of paths that are currently active

  virtual void Accept   (SystemModelVisitor& visitor) override;   //!< Forwards call to any embedded Register

  //! Returns some value associated with specified path selection
  //!
  //! @note When cannot provide a value, an empty BinaryVector is returned
  //!
  //! @param pathIdentifier   Path identifier in range [1..nb_path]
  //!
  virtual const BinaryVector& SelectionValue (uint32_t pathIdentifier) const override;

  //! Returns readable type of selector
  //!
  virtual std::experimental::string_view KindName() const override { return "SSAK"; };

  //! Returns debug information about selector
  //!
  virtual std::string DebugSelectorInfo(bool onlyProperties) const override;

  enum SSAK_SelectorState{CLOSED,
                          CONFIG_SENT,
			  AUTHENTICATION_CHECK,
			  OPEN
			  };
		  
  // ---------------- Protected Methods
  //
  protected:

  virtual VirtualRegister* AssociatedRegisters() override { return &m_muxRegisters; }  //!< Returns associated Registers

  void CheckPathIdentifier (uint32_t pathIdentifier) const;

  // ---------------- Protected Fields
  //
  protected:
  BinaryVector m_SSAKSelectorInformation;
  
  const BinaryVector m_SSAKAuthenticationSuccess = BinaryVector::CreateFromHexString("0x00000000000000000000000000000001");
  const BinaryVector m_SSAKAuthenticationFailure = BinaryVector::CreateFromHexString("0x00000000000000000000000000000002");

  // ---------------- Private Fields
  //
  uint32_t        m_pathsCount;                 //!< Number of managed paths
  VirtualRegister m_muxRegisters; 	        //!< SSAK control Register
  std::shared_ptr<Register> m_S2IB_Register;   //!< S2B control Register

  std::shared_ptr<cryptoProc>     m_CryptoProcesseur ;
  std::shared_ptr<SSAKplugin>      m_SSAKdriver;    //!< Instantiation of the SSAK authentication processor
  uint32_t        m_interfaceSize;    //!< Maximal Number of managed paths
  
const u8 m_SSAK_bits[16]={0x72 ,0xc4 ,0x35 ,0x8f ,0x5a ,0x8a ,0x07 ,0xaf ,0x3d ,0x0f ,0x7d ,0x56 ,0x0a ,0x87 ,0x2a ,0x2b};
//!<SSAK private key : to be loaded from SIT

  SSAK_SelectorState m_SelectorState;
    uint32_t        m_attempts;//!< Authentication Timeout Watchdog
};
//
//  End of SSAK_PathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined SSAK_PathSelector_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of SSAK_PathSelector.hpp
//===========================================================================
