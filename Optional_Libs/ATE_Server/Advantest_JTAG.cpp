//===========================================================================
//                           Advantest_JTAG.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_JTAG.cpp
//!
//! Implements functions to abstract JTAG access on ADVANTEST ATE
//===========================================================================

#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
#include "rdi.hpp"
#include "Advantest_JTAG.hpp"

using std::string;

void jtag_access(int ir_data_in, int dr_data_in, const string& id_ir_dr)
{
  rdi.port("pJtag").smartVec(id_ir_dr)
          .pin("TCK").defaultVec("p")
          .pin("TDI").writeData(ir_data_in, 6, 4).writeData(dr_data_in, 32, 14).defaultVec("0")
          .pin("TDO").readData(6, 4).readData(32, 14).defaultVec("X")
          .pin("TMS").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 2, 10).fillVec("1", 1, 45).fillVec("1", 1, 46).defaultVec("0")
          .vecLength(48)
          .comment   ("access_" + rdi.itos_hex(ir_data_in) + "_" +  rdi.itos_hex(dr_data_in) + "_" + id_ir_dr, 0)
          .labelName ("access_" + rdi.itos_hex(ir_data_in) + "_"  + rdi.itos_hex(dr_data_in) + "_" + id_ir_dr)
          .execute();
}

void jtag_access_dr(int dr_data_in, const string& id_dr_data)
{
  rdi.port("pJtag").smartVec(id_dr_data)
          .pin("TCK").defaultVec("p")
          .pin("TDI").writeData(dr_data_in, 32, 3).defaultVec("0")
          .pin("TDO").readData(32, 3).defaultVec("X")
          .pin("TMS").fillVec("1", 1, 0).fillVec("1", 1, 34).fillVec("1", 1, 35).defaultVec("0")
          .vecLength(37)
          .comment   ("access_dr_" + rdi.itos_hex(dr_data_in) + "_" + id_dr_data, 0)
          .labelName ("access_dr_" + rdi.itos_hex(dr_data_in) + "_" + id_dr_data)
          .execute();
}

void jtag_access_ir(int ir_data_in, const string& id_ir_data)
{
  rdi.port("pJtag").smartVec(id_ir_data)
          .pin("TCK").defaultVec("p")
          .pin("TDI").writeData(ir_data_in, 6, 4).defaultVec("0")
          .pin("TDO").readData(6, 4).defaultVec("X")
          .pin("TMS").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 1, 10).defaultVec("0")
          .vecLength(12)
          .comment   ("access_ir_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data, 0)
          .labelName ("access_ir_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data)
          .execute();
}

void jtag_access_ir_expData(int ir_data_in, const string& id_ir_data, int ir_dataExp)
{
  rdi.port("pJtag").smartVec(id_ir_data)
          .pin("TCK").defaultVec("p")
          .pin("TDI").writeData(ir_data_in, 6, 4).defaultVec("0")
          .pin("TDO").expectData(ir_dataExp, 6, 4).defaultVec("X")
          .pin("TMS").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 1, 10).defaultVec("0")
          .vecLength(12)
          .comment   ("access_ir_expData_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data, 0)
          .labelName ("access_ir_expData_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data)
          .execute();
}

void jtag_synch_reset()
{
  rdi.port("pJtag").smartVec()
          .pin("TCK").defaultVec("p")
          .pin("TDI").defaultVec("0")
          .pin("TDO").defaultVec("X")
          .pin("TMS").fillVec("1", 32, 0).defaultVec("0")
          .vecLength(33)
          .comment   ("synch_reset" , 0)
          .labelName ("synch_reset")
          .execute();
}

void jtag_synch_reset(int size)
{
  rdi.port("pJtag").smartVec()
          .pin("TCK").defaultVec("p")
          .pin("TDI").defaultVec("0")
          .pin("TDO").defaultVec("X")
          .pin("TMS").fillVec("1", size, 0).defaultVec("0")
          .vecLength(size + 1)
          .comment   ("synch_reset" , 0)
          .labelName ("synch_reset")
          .execute();
}

//
//
//  void spi_write(int data){
//    rdi.port("poSPI").smartVec().pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).fillVec("X",18,4).fillVec("X",2,22).defaultVec("X")    //MISO
//                  .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).writeData(data,18,4).fillVec("X",2,22).defaultVec("X")   //MOSI
//                  .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("wr_" + rdi.itos_hex(data), 0)
//                      .bitLength(24).labelName("write_" + itos(data)).execute();
//  }
//  void spi_write_perSite(const string& rtv){
//    rdi.port("poSPI").smartVec().pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).fillVec("X",18,4).fillVec("X",2,22).defaultVec("X")    //MISO
//                  .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).writeData(rtv,18,4).fillVec("X",2,22).defaultVec("X")    //MOSI
//                  .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("wr_" + rtv, 0)
//                      .bitLength(24).labelName("write_" + rtv).execute();
//  }
//  void spi_read(const string& id){
//    rdi.port("poSPI").smartVec(id).pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).readData(18,4).fillVec("X",2,22).defaultVec("X")   //MISO
//                    .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).fillVec("0",18,4).fillVec("X",2,22).defaultVec("X")    //MOSI
//                    .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("rd_" + id, 0)
//                      .bitLength(24).labelName("read_" + id).execute();
//  }
//  void spi_expect(int data, const string& id){
//    rdi.port("poSPI").smartVec(id).pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).expectData(data,18,4).fillVec("X",2,22).defaultVec("X")    //MISO
//                    .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).fillVec("0",18,4).fillVec("X",2,22).defaultVec("X")    //MOSI
//                    .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("rd_" + rdi.itos_hex(data), 0)
//                      .bitLength(24).labelName("read_" + itos(data)).execute();
//  }
//  void spi_read_write(int WrData, const string& id){
//    rdi.port("poSPI").smartVec(id).pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).readData(18,4).fillVec("X",2,22).defaultVec("X")   //MISO
//                    .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).writeData(WrData,18,4).fillVec("X",2,22).defaultVec("X")   //MOSI
//                    .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("rd_" + id + "_wr_" + rdi.itos_hex(WrData), 0)
//                      .bitLength(24).labelName("read_write" + id + "_" + itos(WrData)).execute();
//  }
//  void spi_expect_write(int RdData,int WrData, const string& id){
//    rdi.port("poSPI").smartVec(id).pin("loop4a").fillVec("X",1,0).fillVec("X",3,1).expectData(RdData,18,4).fillVec("X",2,22).defaultVec("X")    //MISO
//                    .pin("loop3a").fillVec("X",1,0).fillVec("X",3,1).writeData(WrData,18,4).fillVec("X",2,22).defaultVec("X")   //MOSI
//                    .pin("loop1a").fillVec("0",1,0).fillVec("P",3,1).fillVec("P",18,4).fillVec("0",2,22).defaultBit(0)    //SCLK
//                      .pin("loop2a").fillVec("0",1,0).fillVec("0",3,1).fillVec("0",18,4).fillVec("0",1,22).fillVec("1",1,23).defaultBit(0)  //SSB
//                      .comment("rd_" + rdi.itos_hex(RdData) + "_wr_" + rdi.itos_hex(WrData), 0)
//                      .bitLength(24).labelName("read_write" + itos(RdData)+ "_" + itos(WrData)).execute();
//  }
