# Script de compilation modelsim pour benchtopdemo
if [ ! -d work ]
then
	vlib work
fi
vcom Example/cSIB.vhd
vcom SSAK/data_types.vhd
vcom SSAK/fonctions.vhd
vcom SSAK/FSM2.vhd
vcom CryptoProc/AES/AXISbuffer.vhd
vcom SSAK/AXIS_ip/axismux.vhd
vcom SSAK/TNRGfx.vhd
vcom SSAK/l_key.vhd
vcom SSAK/sscdriver.vhd
vcom SSAK/AXIS_ip/axisdemux.vhd
vcom SSAK/AXIS_ip/compres.vhd
vcom SSAK/AXIS_ip/interface.vhd
vcom SSAK/SSAK.vhd
vcom CryptoProc/AES/aes128Pkg.vhd
vcom CryptoProc/AES/sbox.vhd
vcom CryptoProc/AES/subWord.vhd
vcom CryptoProc/AES/keyExpansion.vhd
vcom CryptoProc/AES/subMatrix.vhd
vcom CryptoProc/AES/mixColumn.vhd
vcom CryptoProc/AES/mixMatrix.vhd
vcom CryptoProc/AES/cipherRound.vhd
vcom CryptoProc/AES/aes128.vhd
vcom CryptoProc/AES/AES128_AXIS_Wrapper.vhd
vcom Example/ScanSegment.vhd
vcom Example/PRNG128.vhd
vcom Example/PRNG_gest.vhd
vcom Example/topTestDesing.vhd
vcom Example/bench/benchTopDemo.vhd
echo use command \'vsim -do benchTopDemo.do\'
