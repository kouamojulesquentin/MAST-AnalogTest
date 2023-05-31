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
#include "Mast_Core_export.hpp"

#include <vector>
#include <initializer_list>
#include <functional>


namespace mast
{
//! Manages AccessInterfaceProtocol using callbacks and protocol tables
//!
class MAST_CORE_EXPORT GenericAccessInterfaceProtocol : public AccessInterfaceProtocol
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

  //! Prototype of the commands used to access a Channel
  //! Parameters:
  //!  - The protocol table of available functions
  //!  - The optional data the AI might requite
  //!  - The total number the Channels
  //!  - The vector to push into the SUT (obtained from the current System Model)
  //!
  //! Return
  //!  - The vector received from the SUT, which must be used to update the System Model
  //!    after the command finishes
  //!
  using Action  = std::function<BinaryVector(const std::vector<Primitive>&, void*, const BinaryVector&)>;

  //! Gets the number of Channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two Channels
  //!
  //! @return The number of supported Channels (including pseudo Channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return m_callbacks.size(); }

  GenericAccessInterfaceProtocol(std::initializer_list<Action> callbacks, std::initializer_list<Primitive> primitives);
  GenericAccessInterfaceProtocol(std::vector<Action>           callbacks, std::vector<Primitive>           primitives);

  //! Calls callback associated with AccessInterface Channel
  //!
  //! @param ChannelId   Identifies the Channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoCallback(uint32_t channelId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Generic"; }


  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Private  Fields
  //
  std::vector<Action>    m_callbacks;    //!< Provide Actions to access the Channels based on the set of primitives
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
