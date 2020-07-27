//===========================================================================
//                           CallbackIds.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CallbackIds.hpp
//!
//! Defines symbols for CallbackId strings
//!
//===========================================================================


#ifndef CallbackIds_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define CallbackIds_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

typedef  uint32_t Callback_t;

#define SVF             "SVF"
#define TRST           "TRST"
#define SIR             "SIR"
#define SDR             "SDR"

#define I2C_RESET       "I2C_RESET"
#define I2C_READ        "I2C_READ"
#define I2C_WRITE       "I2C_WRITE"

#define UNDEFINED       "UNDEFINED"

//Fake request used to release DataCycle
#define NO_MORE_PENDING  "AI_FINISHED"
#define HALT_REQUEST     "HALT_REQUEST"

#endif
//===========================================================================
// End of CallbackIds.hpp
//===========================================================================
