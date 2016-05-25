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
#include "Utility.hpp"
#include <vector>
#include <memory>

namespace mast
{

class SystemModelNode;
class Register;

//! Defines a default path selector using binary encoding
//!
//! @note Path identifier are one based (in range [1..path_count])
//! @note Internal table contains a reserved slot at index zero
class DLL_EXPORT DefaultBinaryPathSelector : public PathSelector
{
  // ---------------- Public  Methods
  //
  public:
  ~DefaultBinaryPathSelector() = default;
  DefaultBinaryPathSelector()  = delete;
  DefaultBinaryPathSelector(std::shared_ptr<SystemModelNode> associatedNode, uint32_t pathsCount, bool isInverted = false, bool canSelectNone = false);

  virtual bool IsActive (uint32_t pathIdentifier) const override; //!< Returns true when the specified path is already selected
  virtual void Deselect (uint32_t pathIdentifier) override;       //!< Request deactivation of the specified path
  virtual void Select   (uint32_t pathIdentifier) override;       //!< Request activation of the specified path

  virtual uint32_t SelectablePaths() const override { return m_pathsCount; }; //!< Returns the maximum number of selectable paths (max value for IsActive, Select and Deselect)

  uint32_t ActiveCount() const;    //!< Returns the number of paths that are currently active

  virtual void Accept   (SystemModelVisitor& visitor) override;   //!< Forwards call to any embedded Register

  //! Returns minimal bits count a register should have to drive a mux for number of path
  //!
  static uint32_t RegWidthForPathCount(uint32_t pathCount, bool canSelectNone);

  // ---------------- Protected Methods
  //
  protected:
  void CheckPathIdentifier (uint32_t pathIdentifier) const;

  // ---------------- Private  Methods
  //
  private:
  using TablesType = std::vector<BinaryVector> ; //!< Selection/deselection LUT types

  static TablesType CreateSelectTable   (uint32_t pathCount, bool isInverted, bool canSelectNone);
  static TablesType CreateDeselectTable (uint32_t pathCount, bool isInverted);
  static void       InvertTable         (TablesType& table);

  // ---------------- Private  Fields
  //
  private:

  uint32_t                  m_pathsCount;                    //!< Number of managed paths
  const TablesType          m_select;                        //!< Selection LUT
  const TablesType          m_deselect;                      //!< Deselection LUT
  std::shared_ptr<Register> m_muxRegister;                   //!< Register that drives the paths multiplexer
  const bool                m_canSelectNone = false;         //!< When true zero is reserved to select 'no path' otherwise 0 is used to select first path
};
//
//  End of DefaultBinaryPathSelector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DEFAULTBINARYPATHSELECTOR_H__8EA048FA_454_4D96_79A2_13E04D156F73__INCLUDED_

//===========================================================================
// End of DefaultBinaryPathSelector.hpp
//===========================================================================
