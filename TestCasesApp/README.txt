-------------------------------------------------------------------

TestCasesApp is a multiple-purpose binary able to use architectures described in SIT files, or internally declared.

At the opposite of binaries in the MastExample_CPP directory, it is possible to force different parameters via line commands instead of editing the SIT file.

The TestCasesApp binary must be launched with at least one of the following options:

|	Options																			| Available parameters
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------							
| Test case: -tc|--test_case									| SIT_File: Use a SIT-described architecture. Corresponding file must be passed via the 'Test case option' option.
| 																						| ZyboSPI: A simple SPI access interface with one 8-bit data register.
| 																						| Wrapper_1500 : A 1500-wrapper architecture.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------							
| Test case options: -tco|--test_case_options	|	--name zybo: use a simple algorithm incrementing the data register value from 0 to 0x37A
|																							| <filename>.sit: when used with the SIT_File test_case option, give the path of the requested SIT file.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
|	Protocol: -p|--protocol											|	Loopback (default): simple loopback protocol, what you send is what you get.
|																							| SVF_Emulation: a SVF emulation. What you send is what you get.
|																							|	SVF_Simulation: a SVF simulation protocol, using a third-party RTL simulator. Require ModelSim.
|																							|								The RTL model simulates a JTAG controller.
|																							| 
|																							|	I2C_Emulation: a I2C protocol emulation. What you send is what you get.
|																							|
|																							|	OpenOCD: JTAG protocol via OpenOCD (Linux only). Adapter configuration file must be passed as protocol option.
|																							|						OpenOCD must be precompiled for MAST, according to ../openocd/openocd-builder/README.txt
|																							|
|																							|	SPI: SPI protocol via libftdispi (Linux only, require a FT2232H adapter).
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
| Protocol options: -po|--protocol_options		| <adapter_configuration_filename>.cfg: for OpenOCD only. Declares the path of the JTAG adapter configuration file.
|																							|																				See ../openocd/openocd-builder/README.txt for further information.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
|	Loop count: -lc|--loop_count								|	<integer value>: in SIT_File test cases, give how many times the algorithm must be executed.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
| Print graph: -pg|--print_graph							| <true> or <false>: if true, generate a diagram of the SUT.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
| Print graph file: -pgf|--print_graph_file		| <path to a new .pgf file>: create and fill a PGF-format file, giving a visual diagram of the SUT.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
| Help: -h|--help|-\?													|	Gives a summary of all the information given in this table.
|---------------------------------------------|-----------------------------------------------------------------------------------------------------------------
ML505 prototype board with JTAG: ./TestCasesApp -tc SIT_File -tco "--name ml505 -f ../../MastExample_CPP/SIT/ml505_demo.sit"

ML505 prototype board with SPI: ./TestCasesApp -tc ZyboSPI -p SPI
