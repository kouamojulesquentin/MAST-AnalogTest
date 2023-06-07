vsim work.svf_simulation_top_encryption
onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -height 15 /svf_simulation_top_encryption/Resetn
add wave -noupdate -height 15 /svf_simulation_top_encryption/Clk
add wave -noupdate -height 15 /svf_simulation_top_encryption/rstn_master
add wave -noupdate -height 15 /svf_simulation_top_encryption/Sampling_TDI
add wave -noupdate -height 15 /svf_simulation_top_encryption/Shifting
add wave -noupdate -height 15 /svf_simulation_top_encryption/keystream
add wave -noupdate -height 15 /svf_simulation_top_encryption/TDI_plain
add wave -noupdate -height 15 /svf_simulation_top_encryption/TDI_cipher
add wave -noupdate -height 15 /svf_simulation_top_encryption/TDO_plain
add wave -noupdate -height 15 /svf_simulation_top_encryption/TDO_cipher
add wave -noupdate -height 15 /svf_simulation_top_encryption/TCK
add wave -noupdate -height 15 /svf_simulation_top_encryption/TMS
add wave -noupdate -height 15 /svf_simulation_top_encryption/TRSTN
add wave -noupdate -height 15 /svf_simulation_top_encryption/init_cntrl
add wave -noupdate -height 15 /svf_simulation_top_encryption/CNTRL
add wave -noupdate -height 15 /svf_simulation_top_encryption/KEY
add wave -noupdate -height 15 /svf_simulation_top_encryption/IV
add wave -noupdate -height 15 /svf_simulation_top_encryption/TM
add wave -noupdate -divider TAP
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/ShiftIR
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/ShiftDR
add wave -noupdate -height 15 -label IR /svf_simulation_top_encryption/SUT_top/IR_reg/P_out
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/select_IR
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/select_DR
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/select_BYP
add wave -noupdate -height 15 /svf_simulation_top_encryption/SUT_top/select_DR_chain
add wave -noupdate -divider SUT
add wave -noupdate -height 15 -label Register_INPUT_Value -radix hexadecimal /svf_simulation_top_encryption/SUT_top/SUT_TUTORIAL_1/SUT/static_reg/P_in
add wave -noupdate -height 15 -label Register_CURRENT_value -radix hexadecimal /svf_simulation_top_encryption/SUT_top/SUT_TUTORIAL_1/SUT/static_reg/P_out
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {999171 ns} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {999050 ns} {1000050 ns}
run 11800 ns
