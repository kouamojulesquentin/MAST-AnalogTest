#!/bin/bash
 
WORK=./work

[ ! -d "$WORK" ] && vlib $WORK
 
vhd_files=( 
        "common/utilities.vhd"
        "common/txt_util.vhd" 
        "common/MAST_config.vhd"
        "SUT_Examples/JTAG/JTAG_configuration.vhd"  
        "common/types.vhd"
        "SUT_Examples/JTAG/bs_elements.vhd"  
        "common/JTAG_package.vhd"  
        "common/exchange_registers.vhd"
        "SUT_Examples/JTAG/slave_tap.vhd"  
        "SUT_Examples/JTAG/MIB_tutorial.vhd" 
        "SUT_Examples/JTAG/tutorial_1.vhd"


        "SUT_Examples/JTAG/SUT_Package.vhd" 
        "SUT_Examples/JTAG/SUT.vhd" 
        "AI_for_simulation/master_tapfsm.vhd"
        "AI_for_simulation/SVF_Master_TAP.vhd"  

        "SUT_Examples/JTAG/SVF_Simuation_top.vhd"


        "scan_encryption/trivium.vhd"
        "scan_encryption/SVF_Simuation_top_encryption.vhd"

)

for file in "${vhd_files[@]}"
do
  vcom $file
  [ $? -eq 0 ] ||  exit 1 
done
