//===========================================================================
//                           GenericAccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file GenericAccessInterfaceProtocol.hpp
//!
//! Declares GenericAccessInterfaceProtocol class
//!
//===========================================================================

#ifndef GENERICACCESSINTERFACEPROTOCOL_H__ED4322E2_314_4FDF_85AC_27E06DBA5D09__INCLUDED_
  #define GENERICACCESSINTERFACEPROTOCOL_H__ED4322E2_314_4FDF_85AC_27E06DBA5D09__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Platform.hpp"

#include <vector>
#include <initializer_list>
#include <functional>


namespace mast
{
//! Manages AccessInterfaceProtocol using actions and protocol tables
//!
class DLL_EXPORT GenericAccessInterfaceProtocol : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~GenericAccessInterfaceProtocol() = default;
  GenericAccessInterfaceProtocol() = delete;

  //! Primitives interface defining the Access Interface prototocol
  //!
  //! @note The argument is a void* so it can be recast to the actual parameters
  //!
  using Primitive = std::function<void(void*)>;

  //! Prototype of the commands used to access a derivation
  //! Parameters:
  //!  - The protocol table of available functions
  //!  - The optional data the AI might requite
  //!  - The total number of the derivations
  //!  - The vector to push into the SUT (obtained from the current System Model)
  //!
  //! Return
  //!  - The vector received from the SUT, which must be used to update the System Model
  //!    after the command finishes
  //!
  using Action  = std::function<BinaryVector(const std::vector<Primitive>&, void*, const BinaryVector&)>;

  GenericAccessInterfaceProtocol(std::initializer_list<Action> actions, std::initializer_list<Primitive> primitives);
  GenericAccessInterfaceProtocol(std::vector<Action>           actions, std::vector<Primitive>           primitives);

  //! Calls action associated with AccessInterface derivation
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Fields
  //
  std::vector<Action>    m_actions;    //!< Provide Actions to access the derivations based on the set of primitives
  std::vector<Primitive> m_primitives; //!< Primitives composing the protocol
};
//
//  End of GenericAccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined GENERICACCESSINTERFACEPROTOCOL_H__ED4322E2_314_4FDF_85AC_27E06DBA5D09__INCLUDED_
//===========================================================================
// End of GenericAccessInterfaceProtocol.hpp
//===========================================================================
