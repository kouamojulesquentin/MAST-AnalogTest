------- Pre-requirements for OpenOCD with MAST ------------------
MAST requires OpenOCD as a third-party library. Linux users may build this library by following the procedures explaind in
openocd/openocd-builder/README.txt

----------- Configuring OpenOCD with MAST -----------------------

MAST directly embeds JTAG boundary scan functions for OpenOCD.
Hence, there is no need to declare the SUT architecture in a cfg file.
However, the adapter must still be declared in a .cfg file.

Please check http://openocd.org/doc/html/Debug-Adapter-Configuration.html#Debug-Adapter-Configuration
for how to build your adapter cfg file.

An example is provided through the openocd/openocd-ft2232.cfg file and can be used with any FT2232H
adaptater with few modifications.

For example, please note that the reset signal 'ftdi_layout_signal nTRST' has been declared for
the Dangerous Prototypes Breakout-Board ( http://dangerousprototypes.com/docs/FT2232_breakout_board ).
