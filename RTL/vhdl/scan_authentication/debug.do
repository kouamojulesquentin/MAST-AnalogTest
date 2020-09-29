onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -height 15 -label IR /svf_simulation_top/SUT_top/IR_reg/BS_REG_NOCA/P_out
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/reset_chains
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/TDO
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/slave_TAP_0/TDI
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/select_DR_chain(1)
add wave -noupdate -divider SUT
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/TDI
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/TDO
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/fstate
add wave -noupdate -divider SSAK_SIB
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/clk
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/rstn
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/STD_CSU_Select
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/STD_CSU_CE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/STD_CSU_UE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/STD_CSU_SE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/TDI
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/TDO
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/SIB_ssak/fstate
add wave -noupdate -divider {SSAK Controller}
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/Interface_0/RegAD
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/authorizedSsib
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/CRY_KEY_AXIS_TDATA
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/CRY_PLAIN_AXIS_TDATA
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/CRY_ENC_AXIS_TDATA
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/CRY_IV_AXIS_TDATA
add wave -noupdate -divider aSIB
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/clk
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/rstn
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/STD_CSU_CE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CSU_CE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/STD_CSU_UE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CSU_UE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/STD_CSU_SE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CSU_SE
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/STD_CSU_Select
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CSU_Select
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/TDI
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/TDO
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/std_state
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/sec_state
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/aSIB/fstate
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/RNG_port
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/DefG
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/defGFA
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/RDef/M_AXIS_TVALID
add wave -noupdate -height 15 /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/RDef/M_AXIS_TREADY
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_SSAK_EXAMPLE/SUT/CONTROLLER/RDef/M_AXIS_TDATA
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {615 ns} 0}
configure wave -namecolwidth 220
configure wave -valuecolwidth 248
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
WaveRestoreZoom {0 ns} {2592 ns}
