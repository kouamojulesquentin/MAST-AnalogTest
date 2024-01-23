//===========================================================================
//                           DefaultS2IBPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultS2IBPathSelector.hpp
//!
//! Declares class DefaultS2IBPathSelector
//!
//===========================================================================


#ifndef DEFAULTS2IBPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_
  #define DEFAULTS2IBPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

#include "BinaryVector.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "Utility.hpp"
#include <vector>
#include <memory>
#include "SystemModelNodes.hpp"
#include "SSAK_PathSelector.hpp"

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using binary encoding
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
class MAST_CORE_EXPORT DefaultS2IBPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultS2IBPathSelector() = default;
  DefaultS2IBPathSelector()  = delete;
  DefaultS2IBPathSelector (std::vector<std::shared_ptr<SystemModelNode>> associatedNodes,
                                uint32_t               pathsCount,
				const std::string& parameters);



  virtual bool IsActive            (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already active
  virtual bool IsSelected          (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected
  virtual bool IsSelectedAndActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is selected and active
  virtual void Select              (uint32_t pathIdentifier) override;       //!< Request activation of the specified path
  virtual void Deselect            (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path

   void CheckPathIdentifier (uint32_t pathIdentifier) const;
   
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

  //! Returns debug information about selector
  //!
  virtual std::string DebugSelectorInfo(bool onlyProperties) const override;

  virtual std::experimental::string_view KindName() const override { return "S2IB"; }; //!< Returns readable type of selector

  // ---------------- Protected Methods
  //
  protected:
  virtual VirtualRegister* AssociatedRegisters() override { return &m_muxRegisters; }  //!< Returns associated Registers

  // ---------------- Private Fields
  //
  BinaryVector m_SelectValue;
  BinaryVector m_DeSelectValue;
  uint32_t        m_pathsCount;    //!< Number of managed paths
  VirtualRegister  m_muxRegisters;  //!< Register(s) that drive(s) the paths multiplexer
  std::shared_ptr<SSAK_PathSelector> m_S2IB_SSAK_PathSelector;
  uint32_t        m_cardinality;    //!< Position in the S2IB chain, used for configuration

};
//
//  End of DefaultS2IBPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DEFAULTS2IBPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

//===========================================================================
// End of DefaultS2IBPathSelector.hpp
//===========================================================================
