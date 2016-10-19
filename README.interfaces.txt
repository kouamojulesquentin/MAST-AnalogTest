1) OpenOCD has two documentations, once for the binary deamon and one (Doxygen) for the source code : 
- Daoemon Documentation : http://openocd.org/doc/html/index.html
- source code documentation (Doxygen): http://openocd.org/doc/doxygen/html/index.html

Most of the JTAG source code is in src/jtag/
- jtag.h describe the functions exported by the JTAG stack
- core.c contains the main functions manimulating the JTAG FSM

The JTAG Stack initialization can be found in src/openocd.c , function openocd_thread().


2) libftdispi is available at https://github.com/ynsta/libftdispi
 A configuration and usage example is given in https://github.com/ynsta/libftdispi/blob/master/src/spitest.c
