//===========================================================================
//                           SVFVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVFVector.hpp
//!
//! Declares class SVFVector
//!
//===========================================================================


#ifndef SVFVECTOR_H__D5B73705_F730_45B1_3290_491061A3294E__INCLUDED_
  #define SVFVECTOR_H__D5B73705_F730_45B1_3290_491061A3294E__INCLUDED_

#include <string>

namespace mast
{
//! Contains bitstream vector in SVR string format (using ASCII '1' for 1 and '0' for 0)
//!
class SVFVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~SVFVector() = default;
  SVFVector()  = default;


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::string m_data;              //!< ASCII formatted bit stream
  uint32_t    m_usedBits = 0;      //!< Number of effective bits (last byte may be not all used)
};
//
//  End of SVFVector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVFVECTOR_H__D5B73705_F730_45B1_3290_491061A3294E__INCLUDED_

//===========================================================================
// End of SVFVector.hpp
//===========================================================================
