This directory contains example SIT files for different examples, and an example configuration file. 
In the provided Cmakefile, they are installed in ./cmake_debug/Install when running "make". 

------------------------------------
QUICKSTART
 - go to the installation directory ./cmake_debug/Install/Examples
  - run the default example (JTAG.sit):
   ./Mast -c=Examples.yml -s=./SIT/JTAG.sit
  - This runs the PDL-1 routine "Console_Incr", defined in <root>/Examples/PDL/Examples.cpp
  
    - A full execution trace can be found in the Mast.log file
    - The Emulation.log file contains the commands for the top-level Interface computed by MAST
 
 - to run the PDL against another Network, you can either 
       - edit Examples.yml to point to the desired file 
       		(a detailed information on the configuration file is given in the Documentation) 
       - override the configuration using a command-line option. Ex: 
       		 ./Mast -c=Examples.yml -s=./SIT/1500_JTAG.sit
	  --Command line options be found either in the Documention or 
	        by using the --help flag	 

         
------------------------------------------------------------------------------------------------------------
 The C++Wrapped PDL 1 files are found in the mastopensource/Examples/PDL directory
 in particular, Console_Incr is defined in Examples.cpp
 
 These files are compiled together with MAST and are provided in the .so shared files of the Install Directory
 
------------------------------------------------------------------------
DETAILS of the provided Hardware Description files

- Examples using ICL: 

- This ICL example is equivalent to JTAG.sit (Referencing the "Console_Incr" PDL-1 routine) : 
./Mast -c=Examples.yml --icl_list=ICL/JTAG/JTAG.lst

- ICL example emulating the usage of Leds (source file mastopensource/Examples/PDL/ICL_Leds.cpp)
./Mast -c=ICL_Leds.yml --icl_list=ICL/ICL_Leds/ICL_Leds.lst
 

- Examples using SIT: 
 
- Examples referencing the "Console_Incr" PDL-1 routine (source file mastopensource/Examples/PDL/Examples.cpp): 
JTAG.sit
JTAG_Double.sit
MIB_Example.sit
Module_12.sit
SIB_Tutorial.sit
ml505_demo.sit

- Examples of multiple-file SIT
1500_JTAG.sit
1500_multi.sit
Flat_Chain.sit

-- Application of P1687.1 concepts as in paper [1]
JTAG_FTDI.sit
JTAG_to_I2C.sit
Brocade.sit
Dummys.sit
Empty.sit
I2C.sit
STIL.sit
TPSP.sit

--Scan encryption as in paper [2]
Trivium.sit

--Examples of concurrent execution of PDL-1 algorithms
./Concurrent/Module_8.sit
./Concurrent/Random.sit
./Concurrent/Random_hierarchical.sit
./Concurrent/Random_hierarchical_deep.sit
./Concurrent/Random_hierarchical_stress.sit
./Concurrent/Random_horizontal.sit
./Concurrent/Random_horizontal_stress.sit
./Concurrent/Replica_100x8.sit


- Topology example with no PDL
1500_Wrapper_4xRegs_12xBits.sit

- Example of a P1687.2-like system expressed using P1687.1 (please see dedicated Documentation)
LDO.sit

- Interactive demo (FFT on audio signal)
ml505_kissfft.sit


------------------------------------
Reference papers:
[1] Portolan M., "Automated Test Flow: the Present and the Future", IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems (IEEE-TCAD), December 2019,  DOI:10.1109/TCAD.2019.2961328


[2] M. Portolan, E. Valea, P. Maistri, G. Di Natale, "Flexible and Portable Management of Secure Scan Implementations Exploiting P1687.1 Extensions", IEEE Design & Test on 30/9.2021, DOI : 10.1109/MDAT.2021.3117875 

