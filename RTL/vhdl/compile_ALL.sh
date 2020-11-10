 #!/bin/bash
 
vhd_files=( 
        "common/utilities.vhd"
        "common/txt_util.vhd" 
        "common/MAST_config.vhd"
        "Examples/JTAG/JTAG_configuration.vhd"  
        "common/types.vhd"
        "Examples/JTAG/bs_elements.vhd"  
        "common/JTAG_package.vhd"  
        "common/exchange_registers.vhd"
        "Examples/JTAG/slave_tap.vhd"  
        "Examples/JTAG/MIB_tutorial.vhd" 
        "Examples/JTAG/tutorial_1.vhd"

        "scan_encryption/trivium.vhd"
        "scan_encryption/trivium_streamer.vhd"
        "scan_encryption/trivium_testcase.vhd"
        "scan_encryption/double_trivium_testcase.vhd"
        "scan_encryption/nested_trivium_testcase.vhd"

        "scan_authentication/Example/cSIB.vhd"
        "scan_authentication/SSAK/data_types.vhd"
        "scan_authentication/SSAK/fonctions.vhd"
        "scan_authentication/SSAK/FSM2.vhd"
        "scan_authentication/CryptoProc/AES/AXISbuffer.vhd"
        "scan_authentication/SSAK/AXIS_ip/axismux.vhd"
        "scan_authentication/SSAK/TNRGfx.vhd"
        "scan_authentication/SSAK/l_key.vhd"
        "scan_authentication/SSAK/sscdriver.vhd"
        "scan_authentication/SSAK/AXIS_ip/axisdemux.vhd"
        "scan_authentication/SSAK/AXIS_ip/compres.vhd"
        "scan_authentication/SSAK/AXIS_ip/interface.vhd"
        "scan_authentication/SSAK/SSAK.vhd"
        "scan_authentication/CryptoProc/AES/aes128Pkg.vhd"
        "scan_authentication/CryptoProc/AES/sbox.vhd"
        "scan_authentication/CryptoProc/AES/subWord.vhd"
        "scan_authentication/CryptoProc/AES/keyExpansion.vhd"
        "scan_authentication/CryptoProc/AES/subMatrix.vhd"
        "scan_authentication/CryptoProc/AES/mixColumn.vhd"
        "scan_authentication/CryptoProc/AES/mixMatrix.vhd"
        "scan_authentication/CryptoProc/AES/cipherRound.vhd"
        "scan_authentication/CryptoProc/AES/aes128.vhd"
        "scan_authentication/CryptoProc/AES/AES128_AXIS_Wrapper.vhd"
        "scan_authentication/Example/ScanSegment.vhd"
        "scan_authentication/Example/PRNG128.vhd"
        "scan_authentication/Example/PRNG_gest.vhd"
        "scan_authentication/Example/topTestDesing.vhd"

        "scan_secure/scan_secure.vhd"

        "Examples/JTAG/SUT_Package.vhd" 
        "Examples/JTAG/SUT.vhd" 
        "AI_for_simulation/master_tapfsm.vhd"
        "AI_for_simulation/SVF_Master_TAP.vhd"  
        "scan_encryption/SVF_Simuation_top_encryption.vhd"

        "scan_authentication/Example/bench/benchTopDemo.vhd"
        "Examples/JTAG/SVF_Simuation_top.vhd"
)

for file in "${vhd_files[@]}"
do
  vcom $file
  [ $? -eq 0 ] ||  exit 1 
done
