//===========================================================================
//                           Advantest_JTAG.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_JTAG.hpp
//!
//! @note Derived from Author: ossolano  on: Jan 16, 2015
//!
//! Declares  functions to abstract JTAG access on ADVANTEST ATE
//!
//===========================================================================



#ifndef ADVANTEST_JTAG_HPP__9928A8FD_BE37_47A7_3EA5_A5789A7F7472__INCLUDED_
  #define ADVANTEST_JTAG_HPP__9928A8FD_BE37_47A7_3EA5_A5789A7F7472__INCLUDED_

  #include "rdi.hpp"
  #include <string>

 using std::vector;

  void jtag_access            (int ir_data_in, int    dr_data_in, const std::string& id_ir_dr_data);
  void jtag_access_dr         (int dr_data_in,                    const std::string& id_dr_data);
  void jtag_access_dr         (const vector<uint8_t>& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data);
  void jtag_access_ir         (int ir_data_in,                    const std::string& id_ir_data);
  void jtag_access_ir_expData (int ir_data_in,                    const std::string& id_ir_data, int ir_dataExp);
  void jtag_access_ir         (const vector<uint8_t>& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data);
  void jtag_synch_reset       ();
  void jtag_synch_reset       (int size);

  void jtag_access_dr         (ARRAY_I& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data);
  void jtag_access_ir         (ARRAY_I& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data);
  void jtag_shift_cycle (ARRAY_I& dr_data_in, uint32_t  bitsCount,
  					   int shift_state,	const std::string& id_dr_data);

  void jtag_shift_dr_cycle (const vector<uint8_t>& toSUTVector, uint32_t  bitsCount,
  					  const std::string& id_dr_data);
  void jtag_shift_ir_cycle (const vector<uint8_t>& toSUTVector, uint32_t  bitsCount,
  					  const std::string& id_dr_data);

#endif  // not defined ADVANTEST_JTAG_HPP__9928A8FD_BE37_47A7_3EA5_A5789A7F7472__INCLUDED_
