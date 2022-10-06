////////////////////////////////////////////////////////////
//  File        : function.h                              //
//  Author      : Clément TARDY                           //
//  Status      : Intern at TIMA in summer 2022           //
////////////////////////////////////////////////////////////
#ifndef FTDI_JTAG_FUNCTIONS
#define FTDI_JTAG_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftdi.h>

typedef struct jtag_context_t
{
    int clock;
    unsigned char buf[1];
}*jtag_context;


void shift_char(unsigned char * buf, int n, int c);

void print_char(unsigned char * buf, int n);

void my_ftdi_TMS_high(unsigned char * buf);

void my_ftdi_TDO_high(unsigned char * buf);

void my_ftdi_TRSTN_high(unsigned char * buf);

void my_ftdi_TMS_low(unsigned char * buf);

void my_ftdi_TDO_low(unsigned char * buf);

void my_ftdi_clk(unsigned char * buf);

int my_ftdi_sir(jtag_context jtag,unsigned char * buf_data, unsigned char * buf_read, int n,struct ftdi_context *ftdi);

int my_ftdi_sdr(jtag_context jtag,unsigned char * buf_data, unsigned char * buf_read, int n,struct ftdi_context *ftdi);

void my_ftdi_clk_n_cycle(jtag_context jtag, int cycle, struct ftdi_context *ftdi);

struct ftdi_context * my_ftdi_start(int * err_ftdi);

void my_ftdi_free(struct ftdi_context *ftdi);

#endif //FTDI_JTAG_FUNCTIONS
