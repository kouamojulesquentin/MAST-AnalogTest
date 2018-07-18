#include <stdio.h>
#include <stdlib.h>

#include "Remote_Loopback_Protocol.hpp"
#include "XmlRpc_Protocol_Server.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>

#include <svdpi.h>

/*Exchange variables used by DPI calls*/
unsigned char *DPI_data_to_SUT;
unsigned char *DPI_data_from_SUT;
 int cur_bits;


int start_server ();

/*Set bits of next DPI operations*/
void set_DPI_bitsCount(int bitsCount)
{
 cur_bits = bitsCount;
 
}

/*Assign the pointers to data buffers*/
/*NB: NO ALLOCATION IS DONE*/
void set_DPI_data_to_SUT(unsigned char *data_to_SUT)
{
 DPI_data_to_SUT = data_to_SUT;
}

void set_DPI_data_from_SUT(unsigned char *data_from_SUT)
{
 DPI_data_from_SUT = data_from_SUT;
}

static int runtest_cycles;

/*DPI exported tasks*/
extern "C" void runtest(int runcycles);
extern "C" void do_SDR();
extern "C" void do_SIR();

/*DPI inported tasks*/

extern "C" int init_server_cpp() {
   std::cout 
         << "\n\nInitialising MAST VPI CPP server\n\n";
	start_server();
	 
 return 1;
}

extern "C" int do_runtest() {
  int cycles[4]={12,4,7,25};
  static int i=0;
   std::cout 
         << "\n Generating "<< runtest_cycles << " clock cycles\n\n";
	 runtest(cycles[i++]);
    i=i%4; 
	 
 return 1;
}


extern "C" int get_scanbits()
{
     std::cout  << "DPI: get_scanbits\n" ;   
 return cur_bits;
}
 
extern "C" void get_data_to_SUT (svOpenArrayHandle SV_data_to_SUT)
{
   /*NB: even if only 8 bits are needed for SV BYTE type, DPI uses an integer pointer*/
   /*   char variables can be directly assigned without casting */
   int *byte_p; 
   byte_p = (int*)svGetArrayPtr(SV_data_to_SUT); 

     for (int i= svLow(SV_data_to_SUT,1); i <= svHigh(SV_data_to_SUT,1); i++) 
      {
      byte_p[i]=DPI_data_to_SUT[i];
      }
}

extern "C" void send_data_from_SUT(int nbits, svOpenArrayHandle SV_data_from_SUT)
{
     std::cout  << "DPI: send_data_from_SUT, received "<< std::dec<< nbits <<" bits\n" ;   
   int *byte_p; 
   byte_p = (int*)svGetArrayPtr(SV_data_from_SUT); 
     for (int i= svLow(SV_data_from_SUT,1); i <= svHigh(SV_data_from_SUT,1); i++) 
      {
      DPI_data_from_SUT[i]=byte_p[i];
      std::cout  << "DPI: DPI_data_from_SUT : "<< i<<" is: "<<std::hex<<(int)DPI_data_from_SUT[i]<<"\n" ;
      }

 return;
}

#define EXAMPLE_BYTES 10

extern "C" int test_do_SDR() {

  char first_val=0xA9;
  DPI_data_to_SUT = (unsigned char *) calloc(sizeof(char),EXAMPLE_BYTES);
  DPI_data_from_SUT = (unsigned char *) calloc(sizeof(char),EXAMPLE_BYTES);
  cur_bits=10;


   for (int i= 0; i <= 10; i++) 
      {
      DPI_data_to_SUT[i] =first_val+i;
      std::cout  << "DPI_data_to_SUT : "<< i<<" is: "<<std::hex<<(int)DPI_data_to_SUT[i]<<"\n" ;
      }
 
 //prepare data
 do_SDR();
 //retrieve data
}

extern "C" int test_do_SIR() {

  char first_val=0x01;
  DPI_data_to_SUT = (unsigned char *) calloc(sizeof(char),EXAMPLE_BYTES);
  DPI_data_from_SUT = (unsigned char *) calloc(sizeof(char),EXAMPLE_BYTES);
  cur_bits=8;


   for (int i= 0; i <= 10; i++) 
      {
      DPI_data_to_SUT[i] =first_val+i;
//      std::cout  << "DPI_data_to_SUT : "<< i<<" is: "<<std::hex<<(int)DPI_data_to_SUT[i]<<"\n" ;
      }
 
 //prepare data
 do_SIR();
 //retrieve data
}
