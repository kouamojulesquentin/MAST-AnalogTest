//===========================================================================
//                           Trivium_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2019 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Trivium_TranslatorProtocol.hpp
//!
//! Declares Trivium_TranslatorProtocol class
//!
//===========================================================================


#ifndef Trivium_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define Trivium_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

#include <Python.h>
/*pyhelper.hpp : helper class that handles deferencing inside the destructor to help clean-up*/
#include "pyhelper.hpp"

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT Trivium_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
   ~Trivium_TranslatorProtocol();
  Trivium_TranslatorProtocol() = delete;

   //! Constructor with parameters defined by a string
  //!
  Trivium_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Trivium"; }


  // ---------------- Protected Methods
  //


  protected:

   BinaryVector  TRIVIUM_XOR_MASK(long int xor_bits);
   BinaryVector  TRIVIUM_ECRYPT(BinaryVector Plaintext);
   BinaryVector  TRIVIUM_ECRYPT(BinaryVector Plaintext, uint32_t plaintext_bits);

  //! Space for internal utility functions
  //!
  //!Pyhton Objects
  const char TRIVIUM_PYTHON[100] = "trivium";
  CPyInstance hInstance;
  CPyObject pDict, pPython_class, pObject;

   //!Mask used for most recent coding
   BinaryVector m_Trinvium_Xor_Mask;
   //!Unused Mask bits from most recent coding
   BinaryVector m_Residual_Xor_Mask;
    

  // ---------------- Private  Fields for internal uses
  //
};
//
//  End of Trivium_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined Trivium_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of Trivium_TranslatorProtocol.hpp
//===========================================================================
