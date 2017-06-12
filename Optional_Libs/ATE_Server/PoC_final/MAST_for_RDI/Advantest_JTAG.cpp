//===========================================================================
//                           Advantest_jtag.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_jtag.cpp
//!
//! Implements functions to abstract jtag access on ADVANTEST ATE
//===========================================================================

//+#include "mapi.hpp"
#include "rdi.hpp"
#include "Advantest_JTAG.hpp"

#define RTI_TO_SHIFT_DR 3
#define RTI_TO_SHIFT_IR 4
#define SHIFT_TO_RTI 	3

#define WORD_BITS 32
#define WORD_BYTES WORD_BITS/8

using std::string;

void jtag_access(int ir_data_in, int dr_data_in, const string& id_ir_dr)
{
  rdi.port("jtag").smartVec(id_ir_dr)
          .pin("tck").defaultVec("P")
          .pin("tdi").writeData(ir_data_in, 6, 4).writeData(dr_data_in, 32, 14).defaultVec("0")
          .pin("tdo").readData(6, 4).readData(32, 14).defaultVec("X")
          .pin("tms").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 2, 10).fillVec("1", 1, 45).fillVec("1", 1, 46).defaultVec("0")
          .vecLength(48)
          .comment   ("access_" + rdi.itos_hex(ir_data_in) + "_" +  rdi.itos_hex(dr_data_in) + "_" + id_ir_dr, 0)
          .labelName ("access_" + rdi.itos_hex(ir_data_in) + "_"  + rdi.itos_hex(dr_data_in) + "_" + id_ir_dr)
          .execute();
}

void jtag_access_dr(int dr_data_in, const string& id_dr_data)
{
	 /*Supposes TAP FSM to be at RTI state*/
  rdi.port("jtag").smartVec(id_dr_data)
          /*toggle tck*/
          .pin("tck").defaultVec("P")
          /*TMS: bring TAP to ShiftDR on bit 3, shift data, come back to RTI*/
          .pin("tms").fillVec("1", 1, 0).fillVec("1", 1, 34).fillVec("1", 1, 35).defaultVec("0")
          /*Write 32 bits starting from bit 3*/
          .pin("tdi").writeData(dr_data_in, 32, 3).defaultVec("0")
          /*Data is valid during Shift-DR (32 bits starting at 3)*/
          .pin("tdo").readData(32, 3).defaultVec("X")
          /*Length = 3 (get to ShiftDR) + 32 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
          .vecLength(37)
          .comment   ("access_dr_" + rdi.itos_hex(dr_data_in) + "_" + id_dr_data, 0)
          .labelName ("access_dr_" + rdi.itos_hex(dr_data_in) + "_" + id_dr_data)
          .execute();
}

void jtag_access_dr         (ARRAY_I& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data)
{
	jtag_shift_cycle (dr_data_in,bitsCount,
			RTI_TO_SHIFT_DR,	id_dr_data);
}
void jtag_access_ir         (ARRAY_I& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data)
{
	jtag_shift_cycle (dr_data_in,bitsCount,
			RTI_TO_SHIFT_IR,	id_dr_data);
}
void jtag_shift_cycle (ARRAY_I& dr_data_in, uint32_t  bitsCount,
					   int shift_state,	const std::string& id_dr_data)
{
	/*Define runtime value for vector*/
	//rdi.runTimeVal("dr_data_in", dr_data_in);
	ARRAY_I bit_length(dr_data_in);
	ARRAY_I start_bit(dr_data_in);
	start_bit[0] = shift_state;
	bit_length[0]=32;

	std::cout << "bit_length.size() " << bit_length.size()<< "\n";
	for (int i=0;i<bit_length.size();i++)
		{
		start_bit[i] = shift_state+i*32;
		bit_length[i]=32;
        std::cout<< "start_bit " <<start_bit[i] << " bit_length " <<bit_length[i] <<endl;
		}
	std::cout << "bitsCount " << bitsCount<< "\n";

	int new_data = 0x42;
	int new_bits = 7;
	RDI_BEGIN();
	 /*Supposes TAP FSM to be at RTI state*/
 rdi.port("jtag").smartVec(id_dr_data)
         /*toggle tck*/
         .pin("tck").defaultVec("P")
         /*TMS: bring TAP to ShiftDR on bit 3, shift data, come back to RTI*/
         /*   : for IR, needs two 1 to get to IR branch*/
         .pin("tms").fillVec("1", 1+(shift_state-RTI_TO_SHIFT_DR), 0).fillVec("1", 1, bitsCount+shift_state-1+new_bits).fillVec("1", 1, bitsCount+shift_state+new_bits).defaultVec("0")
         .pin("tdo").readData(32, shift_state).defaultVec("X")
         /*Write bitsCount bits starting from bit 3*/
         .pin("tdi").writeDataBurst(dr_data_in, bit_length, start_bit)
                     .writeData(new_data,new_bits,bitsCount+shift_state)
         	 	 	 .defaultVec("0")
         /*Length = 3 (get to ShiftDR) + 32 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
//         .vecLength(bitsCount+shift_state+SHIFT_TO_RTI-1)
         .comment   ("access_dr_ARRAY_I_"+id_dr_data, 0)
         .labelName ("access_dr_ARRAY_I_"+id_dr_data)
         .execute();

 /*Data is valid during Shift-DR (Bitcount bits starting at 3)*/
//  rdi.port("jtag").smartVec()
//  .pin("tdo").readData(32, 3).defaultVec("X")
//  .execute();

 RDI_END();
}


void jtag_access_dr         (const vector<uint8_t>& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data)
{
}
void jtag_access_ir         (const vector<uint8_t>& dr_data_in, uint32_t  bitsCount, const std::string& id_dr_data)
{
	uint8_t cur_byte;
	uint32_t target_shift=RTI_TO_SHIFT_IR;
    uint32_t cur_bits = 8;


	 /*Supposes TAP FSM to be at RTI state*/
  rdi.port("jtag").smartVec(id_dr_data)
          /*toggle tck*/
          .pin("tck").defaultVec("P")
          /*TMS: bring TAP to ShiftDR.IR on bit 3/4, shift data, come back to RTI*/
          .pin("tms").fillVec("1", 1, 0)
                     .fillVec("1", 1, bitsCount+target_shift-1)
                     .fillVec("1", 1, bitsCount+target_shift)
                     .defaultVec("0")
/*Write bitsCount bits starting from bit RTI_TO_SHIFT_DR/IT*/
/*Data is valid during Shift-DR/IR (bitsCount bits starting at RTI_TO_SHIFT_DR/IR)*/
          .pin("tdi").writeData(cur_byte, cur_bits, target_shift)
	      .pin("tdi").defaultVec("0")
          .pin("tdo").readData(bitsCount, target_shift).defaultVec("X")

  /*Length = 3 (get to ShiftDR) + bitsCount (ShiftDR) + 3 (back to RDI)-1 (vector starts at 0)*/
     //  .vecLength(target_shift+bitsCount+SHIFT_TO_RTI-1)
            .comment   ("access_dr", 0)
             .labelName ("access_dr")
       .execute();
}
	void jtag_access_ir(int ir_data_in, const string& id_ir_data)
{
	 /*Supposes TAP FSM to be at RTI state*/
	rdi.port("jtag").smartVec(id_ir_data)
                  /*toggle tck*/
          .pin("tck").defaultVec("P")
          /*TMS: bring TAP to ShiftIR on bit 4, shift instruction, come back to RTI*/
          .pin("tms").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 1, 10).defaultVec("0")
          /*Write 6 bits starting from bit 3*/
          .pin("tdi").writeData(ir_data_in, 6, 4).defaultVec("0")
          /*Data is valid during Shift-IR (6 bits starting at 4)*/
          .pin("tdo").readData(6, 4).defaultVec("X")
          /*Length = 4 (get to ShiftDR) + 6 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
          .vecLength(12)
          .comment   ("access_ir_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data, 0)
          .labelName ("access_ir_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data)
          .execute();
}

void jtag_access_ir_expData(int ir_data_in, const string& id_ir_data, int ir_dataExp)
{
  rdi.port("jtag").smartVec(id_ir_data)
          .pin("tck").defaultVec("P")
          .pin("tdi").writeData(ir_data_in, 6, 4).defaultVec("0")
          .pin("tdo").expectData(ir_dataExp, 6, 4).defaultVec("X")
          .pin("tms").fillVec("1", 2, 0).fillVec("1", 1, 9).fillVec("1", 1, 10).defaultVec("0")
          .vecLength(12)
          .comment   ("access_ir_expData_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data, 0)
          .labelName ("access_ir_expData_" + rdi.itos_hex(ir_data_in) + "_" + id_ir_data)
          .execute();
}

void jtag_synch_reset()
{
  rdi.port("jtag").smartVec()
          .pin("tck").defaultVec("P")
          .pin("tdi").defaultVec("0")
          .pin("tdo").defaultVec("X")
          .pin("tms").fillVec("1", 32, 0).defaultVec("0")
          .vecLength(33)
          .comment   ("synch_reset" , 0)
          .labelName ("synch_reset")
          .execute();
}

void jtag_synch_reset(int size)
{
  rdi.port("jtag").smartVec()
          .pin("tck").defaultVec("P")
          .pin("tdi").defaultVec("0")
          .pin("tdo").defaultVec("X")
          .pin("tms").fillVec("1", size, 0).defaultVec("0")
          .vecLength(size + 1)
          .comment   ("synch_reset" , 0)
          .labelName ("synch_reset")
          .execute();
}


//---------------------------------------------------------------
// FINAL VERSION

//RDI operation for data in a 32-bit integer variable
void jtag_shift_cycle_32_or_less (uint32_t dr_data_in, uint32_t  bitsCount,
					   int shift_state,	const std::string& id_dr_data)
{

	RDI_BEGIN();
	 /*Supposes TAP FSM to be at RTI state*/
 rdi.port("jtag").smartVec(id_dr_data)
         /*toggle tck*/
         .pin("tck").defaultVec("P")
         /*TMS: bring TAP to ShiftDR on bit 3, shift data, come back to RTI*/
         /*   : for IR, needs two 1 to get to IR branch*/
         .pin("tms").fillVec("1", 1+(shift_state-RTI_TO_SHIFT_DR), 0).fillVec("1", 1, bitsCount+shift_state-1).fillVec("1", 1, bitsCount+shift_state).defaultVec("0")
         .pin("tdo").readData(bitsCount, shift_state).defaultVec("X")
         /*Write bitsCount bits starting from bit 3*/
         .pin("tdi").writeData(dr_data_in,bitsCount,shift_state)
         	 	 	.defaultVec("0")
         /*Length = 3 (get to ShiftDR) + 32 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
//         .vecLength(bitsCount+shift_state+SHIFT_TO_RTI-1)
         .comment   ("access_dr_ARRAY_I_"+id_dr_data, 0)
         .labelName ("access_dr_ARRAY_I_"+id_dr_data)
         .execute();

 /*Data is valid during Shift-DR (Bitcount bits starting at 3)*/
//  rdi.port("jtag").smartVec()
//  .pin("tdo").readData(32, 3).defaultVec("X")
//  .execute();

 RDI_END();
}

//RDI operation for data size as multiples of 32
void jtag_shift_cycle_32_multiple (ARRAY_I& dr_data_in, ARRAY_I& bit_length,	ARRAY_I& start_bit,
		               uint32_t  bitsCount,
					   int shift_state,	const std::string& id_dr_data)
{
	/*Define runtime value for vector*/
	//rdi.runTimeVal("dr_data_in", dr_data_in);

	RDI_BEGIN();
	 /*Supposes TAP FSM to be at RTI state*/
 rdi.port("jtag").smartVec(id_dr_data)
         /*toggle tck*/
         .pin("tck").defaultVec("P")
         /*TMS: bring TAP to ShiftDR on bit 3, shift data, come back to RTI*/
         /*   : for IR, needs two 1 to get to IR branch*/
         .pin("tms").fillVec("1", 1+(shift_state-RTI_TO_SHIFT_DR), 0).fillVec("1", 1, bitsCount+shift_state-1).fillVec("1", 1, bitsCount+shift_state).defaultVec("0")
         .pin("tdo").readData(32, shift_state).defaultVec("X")
         /*Write bitsCount bits starting from bit 3*/
         .pin("tdi").writeDataBurst(dr_data_in, bit_length, start_bit)
         	 	 	 .defaultVec("0")
         /*Length = 3 (get to ShiftDR) + 32 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
//         .vecLength(bitsCount+shift_state+SHIFT_TO_RTI-1)
         .comment   ("access_dr_ARRAY_I_"+id_dr_data, 0)
         .labelName ("access_dr_ARRAY_I_"+id_dr_data)
         .execute();

 /*Data is valid during Shift-DR (Bitcount bits starting at 3)*/
//  rdi.port("jtag").smartVec()
//  .pin("tdo").readData(32, 3).defaultVec("X")
//  .execute();

 RDI_END();
}

//RDI operation for data size as multiples of 32
void jtag_shift_cycle_more_than_32 (ARRAY_I& dr_data_in, ARRAY_I& bit_length,	ARRAY_I& start_bit,
						uint32_t prepend_dr_data_in,uint32_t  wordOffset,
					    uint32_t  bitsCount,
					   int shift_state,	const std::string& id_dr_data)
{

	RDI_BEGIN();
	 /*Supposes TAP FSM to be at RTI state*/
 rdi.port("jtag").smartVec(id_dr_data)
         /*toggle tck*/
         .pin("tck").defaultVec("P")
         /*TMS: bring TAP to ShiftDR on bit 3, shift data, come back to RTI*/
         /*   : for IR, needs two 1 to get to IR branch*/
         .pin("tms").fillVec("1", 1+(shift_state-RTI_TO_SHIFT_DR), 0).fillVec("1", 1, bitsCount+shift_state-1).fillVec("1", 1, bitsCount+shift_state).defaultVec("0")
         .pin("tdo").readData(32, shift_state).defaultVec("X")
         /*Write bitsCount bits starting from bit 3*/
         .pin("tdi").writeDataBurst(dr_data_in, bit_length, start_bit)
         	 	     .writeData(prepend_dr_data_in,wordOffset,shift_state)
                  	 .defaultVec("0")
         /*Length = 3 (get to ShiftDR) + 32 ShiftDR + 3 (back to RDI)-1 (vector starts at 0)*/
//         .vecLength(bitsCount+shift_state+SHIFT_TO_RTI-1)
         .comment   ("access_dr_ARRAY_I_"+id_dr_data, 0)
         .labelName ("access_dr_ARRAY_I_"+id_dr_data)
         .execute();

 /*Data is valid during Shift-DR (Bitcount bits starting at 3)*/
//  rdi.port("jtag").smartVec()
//  .pin("tdo").readData(32, 3).defaultVec("X")
//  .execute();

 RDI_END();
}

static const uint8_t BitReverseTable256[] =
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

inline uint32_t extract_uint32(const vector<uint8_t>& src_vector,int index)
{
	uint32_t result;
	int loops=0;
	while(loops<4)
	{
		result=result << 8;
		result += src_vector[index--];
		loops++;
	}
return result;
}

inline uint32_t reverse_32bits(const int& input)
{
	uint32_t result;
    result = (BitReverseTable256[input & 0xff] ) |
        (BitReverseTable256[(input >> 8) & 0xff] << 8) |
        (BitReverseTable256[(input >> 16) & 0xff] << 16) |
        (BitReverseTable256[(input >> 24) & 0xff]<< 24);

	return result;
}

inline uint32_t extract_and_reverse_32_or_less(const vector<uint8_t>& src_vector,
					int index, int bitsCount)
{
	uint32_t result;

	result = extract_uint32(src_vector,index);
	result = reverse_32bits(result);
	result = result<<((WORD_BITS-bitsCount)%8);
  /*Re-Align word on right border as per RDI requirements*/
	result = result>>(WORD_BITS-bitsCount);
return result;
}

void jtag_shift_cycle (const vector<uint8_t>& toSUTVector, const uint32_t  bitsCount,
					   int shift_state,	const std::string& id_dr_data)
{
    int32_t        nBytes         = bitsCount/8;
    int32_t        wordOffset         = bitsCount%WORD_BITS ;
    int32_t        nWords         = bitsCount/WORD_BITS ;

   /*Format Data and perform transaction: there are 3 possible cases*/
	/*CASE 1: bitsCount <= WORD_SIZE (32)*/
    if (bitsCount <= WORD_BITS)
    {
		std::cout << "CASE 1, Bitscount = "<<bitsCount << " for a ";
		(shift_state==RTI_TO_SHIFT_DR)?(std::cout<<"SHIFT DR"):(std::cout<<"SHIFT IR");
		std::cout << " operation "<<endl;
    	uint32_t toSUT_word=0;
        (bitsCount%8==0)?(nBytes--):(0);
    	toSUT_word = extract_and_reverse_32_or_less(toSUTVector,nBytes,bitsCount);
    	jtag_shift_cycle_32_or_less (toSUT_word,bitsCount,shift_state,	id_dr_data);
    	return;
    }
    int32_t        burst_nBytes = nWords*WORD_BYTES;
    	/*Prepare the ARRAY_I for a writeDataBurst*/
        ARRAY_I data_to_RDI(nWords);
    	ARRAY_I bit_length(data_to_RDI);
    	ARRAY_I start_bit(data_to_RDI);

    	burst_nBytes--;
    	for (int i=0;i<nWords;i++)
    	 {
    		data_to_RDI[i]= extract_and_reverse_32_or_less(toSUTVector,burst_nBytes,WORD_BITS);
    		burst_nBytes-=4;
    	 }
    	for (int i=0;i<bit_length.size();i++)
    		{
    		start_bit[i] = shift_state+wordOffset+i*WORD_BITS;
    		bit_length[i]=WORD_BITS;
    		}

    	/*CASE 2: bitsCount is exact multiple of WORD_SIZE*/
        if (wordOffset==0)
        {
            jtag_shift_cycle_32_multiple(data_to_RDI, bit_length, start_bit
    				,bitsCount,shift_state,	id_dr_data);
    	return;
        }
	/*CASE 3: bitsCount is not an exact multiple of WORD_SIZE*/
//    int32_t        remainingBitsCount = bitsCount;

    	uint32_t prepend_toSUT_word=0;
        int32_t        prepend_nBytes = wordOffset/8+nBytes-1;
        (wordOffset%8==0)?(prepend_nBytes--):(0);
        prepend_toSUT_word = extract_and_reverse_32_or_less(toSUTVector,prepend_nBytes,wordOffset);

        jtag_shift_cycle_more_than_32(data_to_RDI, bit_length, start_bit
        		,prepend_toSUT_word,wordOffset
        		,bitsCount,shift_state,	id_dr_data);

    return;
}

void jtag_shift_dr_cycle (const vector<uint8_t>& toSUTVector, uint32_t  bitsCount,
					  const std::string& id_dr_data)
{
	jtag_shift_cycle (toSUTVector,bitsCount,RTI_TO_SHIFT_DR,id_dr_data);

	return;
}

void jtag_shift_ir_cycle (const vector<uint8_t>& toSUTVector, uint32_t  bitsCount,
					  const std::string& id_dr_data)
{
	jtag_shift_cycle (toSUTVector,bitsCount,RTI_TO_SHIFT_IR,id_dr_data);

	return;
}
//------------------------------------------------------------------

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
