Examples JTAG and their respective SIT files

MIB_tutorial: examples of using a BINARY MIB

  PREFACE: 
  The VHDL can be used together with the algorithm defined in  Tutorials/MIB_Tutorial
  For this, the ./compile.sh script creates a symbolic link to the installed Tutorial files and to the Mast executable
  The MIB_tutorial can then be launched with the command: 
   ./Mast -c=./MIB_Tutorial/MIB_Tutorial.yml

  - in the VHDL, MUX_SIZE controls the number of paths controlled by the MIB
  - in the VHDL, REGISTER_SIZE controls the size of the identical registers accessed by the MIB
  - the instantiated MIB has always a binary coding
  - the position of the control mux with respect to the mux can be changed 
     thanks to the "reg_before" parameter (see MIB comments)
     
  - NB: when the VHDL is changed, the SIB must be modified accordingly 
  - in the SIT directories there are three reference description files: 
  
    * MIB_Tutorial.sit : file referenced by MIB_Tutorial.yml
    * MIB_Tutorial_post.sit : 3-way MIB with control register after the mux
    * MIB_Tutorial_pre.sit : 3-way MIB with control register before the mux
    * MIB_Tutorial_SIB.sit : 1-way MIB collapsed as a SIB with control register 	
                             before the mux. 
                       NB: in this case, modify MIB_Tutorial.cpp to reference only reg_1

Debug_Mux: example for work with Politecnico (functional verification of muxes) based on MIB_tutorial

  PREFACE: after running compile.sh, it can be run with 
   ./Mast -c=./Debug_mux/Debug_mux.yml
        
