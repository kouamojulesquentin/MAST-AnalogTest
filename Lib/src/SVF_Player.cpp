//===========================================================================
//                           SVF_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_Player.cpp
//!
//! Implements class SVF_Player
//!
//===========================================================================

#include "SVF_Player.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"

#include <experimental/string_view>
#include <sstream>

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


//! Creates an SVF command associated to derivation identifier and BinaryVector to send to SUT
//!
string SVF_Player::CreateSVFCommand (uint32_t derivationId, const BinaryVector& toSutData)
{
  string_view commandType;

  switch (derivationId)
  {
    case 0u:
      THROW_INVALID_ARGUMENT("Reset operation is not yet implemented");
      //! @todo [JFC]-[August/26/2016]: In DoAction(): Do a reset operation for derivationId 0
      //!
      break;
    case 1u:
      commandType = "SIR";
      break;
    case 2u:
      commandType = "SDR";
      break;
    default:
      THROW_INVALID_ARGUMENT("DerivationId must be '0' (for Reset), '1' (for SIR) or '2' (for SDR)");
      break;
  }

  ostringstream os;
  os << commandType << " " << toSutData.BitsCount() << " TDI(" << SVFVector(toSutData).Data() << ");\n";

  auto svfCommand = os.str();

  return svfCommand;
}
//
//  End of: SVF_Player::CreateSVFCommand
//---------------------------------------------------------------------------




//===========================================================================
// End of SVF_Player.cpp
//===========================================================================
