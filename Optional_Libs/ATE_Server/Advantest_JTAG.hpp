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

  #include <string>

  void jtag_access            (int ir_data_in, int    dr_data_in, const std::string& id_ir_dr_data);
  void jtag_access_dr         (int dr_data_in,                    const std::string& id_dr_data);
  void jtag_access_ir         (int ir_data_in,                    const std::string& id_ir_data);
  void jtag_access_ir_expData (int ir_data_in,                    const std::string& id_ir_data, int ir_dataExp);
  void jtag_synch_reset       ();
  void jtag_synch_reset       (int size);

#endif  // not defined ADVANTEST_JTAG_HPP__9928A8FD_BE37_47A7_3EA5_A5789A7F7472__INCLUDED_
