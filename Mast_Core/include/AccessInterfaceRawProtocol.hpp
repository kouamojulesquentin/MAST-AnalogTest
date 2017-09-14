//===========================================================================
//                           AccessInterfaceRawProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceRawProtocol.hpp
//!
//! Declares class AccessInterfaceRawProtocol. A RAW protocol does not directly generate transaction, but relies on a 
//! Parent Interface of type AccessInterfaceTranslator to execute the Callbcaks and provide the results
//!
//===========================================================================


#ifndef AccessInterfaceRawProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define AccessInterfaceRawProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "AccessInterfaceTranslator.hpp"
#include "BinaryVector.hpp"
#include "CallbackIds.hpp"
#include <experimental/string_view>
#include <memory>

namespace mast
{
//! Constructs SVF commands for SVF related AccessInterfaceProtocol
//!
class MAST_CORE_EXPORT AccessInterfaceRawProtocol : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~AccessInterfaceRawProtocol() = default;
  AccessInterfaceRawProtocol() {m_ParentInterface=nullptr;};
  AccessInterfaceRawProtocol(std::shared_ptr<AccessInterfaceTranslator> ParentInterface) 
  {m_ParentInterface=ParentInterface;};

  std::shared_ptr<AccessInterfaceTranslator> ParentInterface() {return m_ParentInterface;}
  void SetParentInterface(std::shared_ptr<AccessInterfaceTranslator> ParentInterface) {m_ParentInterface=ParentInterface;}
  private:
  std::shared_ptr<AccessInterfaceTranslator> m_ParentInterface;

  // ---------------- Protected Methods
  //
  protected:
};
//
//  End of AccessInterfaceRawProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined AccessInterfaceRawProtocol_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of AccessInterfaceRawProtocol.hpp
//===========================================================================
