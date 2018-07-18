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


int start_server ();


void set_DPI_bitsCount(int bitsCount);
void set_DPI_data_to_SUT(unsigned char *data_to_SUT);
void set_DPI_data_from_SUT(unsigned char *data_from_SUT);

/*DPI exported tasks */
extern "C" void runtest(int runcycles);
extern "C" void do_SDR();
extern "C" void do_SIR(int bitsCount);

/*
DPI inported tasks

extern "C" int init_server_cpp();

extern "C" int do_runtest();

extern "C" int get_scanbits();
 
extern "C" void get_data_to_SUT (svOpenArrayHandle SV_data_to_SUT);

extern "C" void send_data_from_SUT(int nbits, svOpenArrayHandle data_from_SUT);

extern "C" int test_do_SDR();
*/
