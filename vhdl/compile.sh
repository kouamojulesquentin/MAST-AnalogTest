export INSTALL_HOME=/softslin/modelsim10_4/modeltech/
 source $INSTALL_HOME/examples/c_posix/setup/setup_compiler_and_linker_paths_gcc.sh 

echo $CC $CC_OPTION Modelsim_FLI/MAST_write.c
eval $CC $CC_OPTION Modelsim_FLI/MAST_write.c
echo $LD MAST_write.so MAST_write.o $MTIPLILIB
eval $LD MAST_write.so MAST_write.o $MTIPLILIB

vcom Modelsim_FLI/MAST_write.vhd
vcom common/MAST_config.vhd
vcom Examples/JTAG/JTAG_configuration.vhd  
vcom common/types.vhd
vcom Examples/JTAG/bs_elements.vhd  
vcom common/JTAG_package.vhd  
vcom common/txt_util.vhd 
vcom common/exchange_registers.vhd
vcom AI_for_simulation/AI_JTAG_SVF.vhd 
vcom Examples/JTAG/slave_tap.vhd  
vcom Examples/JTAG/P1687_testcase.vhd 
vcom Examples/JTAG/AMS_testcase.vhd 
vcom Examples/JTAG/Top_TAP_bench.vhd 
