//===========================================================================
//                           DefaultBinaryPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultBinaryPathSelector.hpp
//!
//! Declares class DefaultBinaryPathSelector
//!
//===========================================================================


#ifndef DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_
  #define DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

#include "BinaryVector.hpp"
#include "PathSelector.hpp"
#include <vector>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using binary encoding
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
class DefaultBinaryPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultBinaryPathSelector() = default;
  DefaultBinaryPathSelector()  = delete;
  DefaultBinaryPathSelector(SystemModelNode* associatedNode, uint32_t pathCount, bool isInverted = false, bool canSelectNone = false);

  virtual bool IsActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already selected
  virtual void Deselect (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path
  virtual void Select   (uint32_t pathIdentifier) override;       //!< Request activation of the specified path


  // ---------------- Protected Methods
  //
  protected:
  void CheckPathIdentifier (uint32_t pathIdentifier) const;

  // ---------------- Private  Methods
  //
  private:
  using TablesType = std::vector<BinaryVector > ; //!< Selection/deselection LUT types

  static TablesType CreateSelectTable   (uint32_t pathCount, bool isInverted, bool noIdle);
  static TablesType CreateDeselectTable (uint32_t pathCount, bool isInverted);
  static void       InvertTable         (TablesType& table);

  // ---------------- Private  Fields
  //
  private:

  const TablesType m_select;                   //!< Selection LUT
  const TablesType m_deselect;                 //!< Deselection LUT
  Register*        m_muxRegister = nullptr;    //!< Register that drives the paths multiplexer
};
//
//  End of DefaultBinaryPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

//===========================================================================
// End of DefaultBinaryPathSelector.hpp
//===========================================================================
