////////////////////////////////////////////////////////////
//  File        : function.h                              //
//  Author      : Clément TARDY                           //
//  Status      : Intern at TIMA in summer 2022           //
////////////////////////////////////////////////////////////
#include "function.h"

#include <experimental/string_view>
#include <sstream>
#include "g3log/g3log.hpp"


#define TCK 0x01
#define TDI 0x02
#define TDO 0x04
#define TMS 0x08
#define TRSTN 0x10

//Allows to shift unsigned char 
void shift_char(unsigned char * buf, int n, int c){
    int carry = 0;                      
    int tmp = c;                        
    int k = n % 8 ? n/8 : n/8 -1 ;      
    int l = n % 8;                      
    unsigned char tab[8] = {0x80,0x01,0x02,0x04,0x08,0x10,0x20,0x40};
    int i;
    for(i = k; i >= 0 ; i--){
        carry  = buf[i] % 2 ? 1 : 0;
        buf[i] = buf[i] >> 1;
        if(i != k) {
        buf[i] = tmp ? buf[i] | 0x80 : buf[i] & ~0x80;
        }
        else {
            buf[i] = tmp ? buf[i] | tab[l] : buf[i] & ~tab[l];
        }
        tmp = carry;
    }
}
/*
//Allows to print a unsigned char
void print_char(unsigned char * buf, int n){
    int i;
    int k = n % 8 ? n/8 : n/8 -1 ;
    printf("0x");
    for(i = k;i>= 0; i--){
        printf("%x.",buf[i]);
    }
    printf("\n");
}
*/

//TMS takes the high value
void my_ftdi_TMS_high(unsigned char * buf){
    buf[0] = buf[0] | TMS;
}
//TDI takes the high value
void my_ftdi_TDO_high(unsigned char * buf){
    buf[0] = buf[0] | TDO;
}

//TRSTN takes the high value
void my_ftdi_TRSTN_high(unsigned char * buf){
    buf[0] = buf[0] | TRSTN;
}

//TMS takes the low value
void my_ftdi_TMS_low(unsigned char * buf){
    buf[0] = buf[0] & ~TMS;
}

// TDO takes the low value
void my_ftdi_TDO_low(unsigned char * buf){
    buf[0] = buf[0] & ~TDO;
}

// change the value of clk you need two changes for one cycle
void my_ftdi_clk(unsigned char * buf){
    buf[0] = (buf[0] & TCK) ? ~TCK & buf[0] : TCK | buf[0];
}

// allows to do a SIR with n shift
//  jtag    : structur for the context  of the jtag (e.g buff and clokc)
//          buf   : current bit-bang value of pins
//          clock : clock of the JTAG
//  buf_data: usigned char pre allocated to send the data to the TAP by the TDI 
//  buf_read: usigned char pre allocated to store the data received by the TDO fill right (e.g at least the size of buf_data)
//  ftdi    : variable that allows to select the right chip ftdi
//  n       : number of shift (e.g length of the IR_reg)
int my_ftdi_sir(jtag_context jtag,
                 unsigned char * buf_data, 
                 unsigned char * buf_read, int n,
                 struct ftdi_context *ftdi){
//-------- Variable-----------
int i;
int f = 0;
unsigned char buf_pins[1];
int carry;
//--------1 cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TRSTN_high(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------2 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_high(jtag->buf); //TMS High
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------3 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------4 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_low(jtag->buf); //TMS Low
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------4 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------5 cycle-------------
for ( i = 0; i < n; i++)
{
    my_ftdi_clk(jtag->buf);
    f = ftdi_write_data(ftdi, jtag->buf, 1);
    if (f < 0){
        //fprintf(stderr,"write failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"write failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
	return -1;
    }
    f = ftdi_read_pins(ftdi,buf_pins);
    if (f < 0){
        //fprintf(stderr,"read failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"read failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
        return -1;
    }
    carry = buf_pins[0] & TDO ? 1 : 0;
    if(i != n){
        shift_char(buf_read,n,carry);
    }
    usleep(jtag->clock);
    my_ftdi_clk(jtag->buf);
    jtag->buf[0] = buf_data[i/8] % 2 ? jtag->buf[0] | TDI : jtag->buf[0] & ~TDI;
    f = ftdi_write_data(ftdi,jtag->buf, 1);
    if (f < 0){
        //fprintf(stderr,"write failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"write failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
	return -1;
    }
    usleep(jtag->clock);
    buf_data[i/8] = buf_data[i/8] >> 1;
}
//print_char(buf_read,n);

//--------6+n cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_high(jtag->buf); //TMS High
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------7+n cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
//--------8+n cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_low(jtag->buf); //TMS Low
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi, jtag->buf, 1);
usleep(jtag->clock);
return 0;
}


// allows to do a SDR with n shift
//  jtag    : structur for the context  of the jtag (e.g buff and clokc)
//          buf   : current bit-bang value of pins
//          clock : clock of the JTAG
//  buf_data: usigned char pre allocated to send the data to the TAP by the TDI 
//  buf_read: usigned char pre allocated to store the data received by the TDO fill right (e.g at least the size of buf_data)
//  ftdi    : variable that allows to select the right chip ftdi
//  n       : number of shift (e.g length of the DR_reg)
int my_ftdi_sdr(jtag_context jtag,
                 unsigned char * buf_data, 
                 unsigned char * buf_read, int n,
                 struct ftdi_context *ftdi){
//-------- Variable-----------
int i;
int f = 0;
unsigned char buf_pins[1];
int carry;
//--------1 cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TRSTN_high(jtag->buf); // Resetn high !!
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------2 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_high(jtag->buf); //TMS High
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------3 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_TMS_low(jtag->buf); //TMS Low
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------4 cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------5 cycle-------------
for ( i = 0; i < n; i++)
{
    my_ftdi_clk(jtag->buf);
    f = ftdi_write_data(ftdi,jtag->buf, 1);
    if (f < 0){
        //fprintf(stderr,"write failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"write failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
        return -1;
    }
    f = ftdi_read_pins(ftdi,buf_pins);
    if (f < 0){
        //fprintf(stderr,"read failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"read failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
        return -1;
    }
    carry = buf_pins[0] & TDO ? 1 : 0;
    if(i != n){
        shift_char(buf_read,n,carry);
    }
    usleep(jtag->clock);
    my_ftdi_clk(jtag->buf);
    jtag->buf[0] = buf_data[i/8] % 2 ?jtag->buf[0] | TDI :jtag->buf[0] & ~TDI;
    f = ftdi_write_data(ftdi,jtag->buf, 1);
    if (f < 0){
        //fprintf(stderr,"write failed for 0x%x, error %d (%s)\n",jtag->buf[0],f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"write failed for 0x"+std::to_string(jtag->buf[0])+", error "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
        return -1;
    }
    usleep(jtag->clock);
    buf_data[i/8] = buf_data[i/8] >> 1;
}
//print_char(buf_read,n);

//--------5+n cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_high(jtag->buf); //TMS High
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------6+n cycle-------------
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
//--------7+n cycle-------------
my_ftdi_clk(jtag->buf);
my_ftdi_TMS_low(jtag->buf); //TMS Low
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
my_ftdi_clk(jtag->buf);
ftdi_write_data(ftdi,jtag->buf, 1);
usleep(jtag->clock);
return 0;
}

//Allows to do n cycle of clock
void my_ftdi_clk_n_cycle(jtag_context jtag, int cycle, struct ftdi_context *ftdi){
    int i;
    for ( i = 0; i < cycle; i++)
    {
    my_ftdi_TRSTN_high(jtag->buf);
    my_ftdi_clk(jtag->buf);
    ftdi_write_data(ftdi, jtag->buf, 1);
    usleep(jtag->clock);
    my_ftdi_clk(jtag->buf);
    ftdi_write_data(ftdi, jtag->buf, 1);
    usleep(jtag->clock);
    }
}

//Allows to start the connection with the FTDI
struct ftdi_context * my_ftdi_start(int * err_ftdi){
    struct ftdi_context *ftdi;
    *err_ftdi = 0;
    int f;
     if ((ftdi = ftdi_new()) == 0)
    {
        //fprintf(stderr, "ftdi_new failed\n");
	LOG(ERROR_LVL)<<"ftdi_new failed";
        *err_ftdi = -1;
        return NULL;
    }
    f = ftdi_usb_open(ftdi, 0x0403, 0x6011);
    if (f < 0 && f != -5)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        LOG(ERROR_LVL)<<"unable to open ftdi device: "+std::to_string(f)+" ("+ftdi_get_error_string(ftdi)+")";
        *err_ftdi = 1;
    }
//    printf("ftdi open succeeded: %d\n",f);
//    printf("enabling bitbang mode\n");
    LOG(DEBUG)<<"ftdi open succeeded: "+std::to_string(f);
    LOG(DEBUG)<<"enabling bitbang mode";
    ftdi_set_bitmode(ftdi, 0xFF, BITMODE_BITBANG);
    usleep(5 * 100000);
    return ftdi;
}

//Allows to end the comunication with the FTDI
void my_ftdi_free(struct ftdi_context *ftdi){
    //printf("\n");
    //printf("disabling bitbang mode\n");
    LOG(DEBUG)<<"disabling bitbang mode";
    ftdi_disable_bitbang(ftdi);
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
}
