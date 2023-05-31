onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/ctrl
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/tclk
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/clk
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/CSU_SE
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/CSU_CE
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/CSU_UE
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/TDI
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/TDO
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/PLAIN_TVALID
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/PLAIN_TREADY
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/PLAIN_TDATA
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/Key_TVALID
add wave -noupdate -height 15 -expand -group BENCH -height 15 -radix hexadecimal /benchtopdemo/ENCR_TDATA
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/Key_TREADY
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/Key_TDATA
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/ENCR_TVALID
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/ENCR_TREADY
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/resultat
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/AuthStatut
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/Defi
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/eos
add wave -noupdate -height 15 -expand -group BENCH -height 15 /benchtopdemo/rstn
add wave -noupdate -height 15 -expand -group {SIBs states} -height 15 -label SSAK_SIB_STATE /benchtopdemo/DUT/SIB_ssak/std_state
add wave -noupdate -height 15 -expand -group {SIBs states} -height 15 -label aSIB_unlock /benchtopdemo/DUT/aSIB/sec_state
add wave -noupdate -height 15 -expand -group {SIBs states} -height 15 -label aSIB_state /benchtopdemo/DUT/aSIB/std_state
add wave -noupdate -height 15 /benchtopdemo/DUT/aSIB/SEC_CSU_UE
add wave -noupdate -height 15 /benchtopdemo/DUT/aSIB/SEC_CSU_SE
add wave -noupdate -height 15 /benchtopdemo/DUT/aSIB/SEC_TDI
add wave -noupdate -height 15 /benchtopdemo/DUT/aSIB/SEC_TDO
add wave -noupdate -height 15 /benchtopdemo/DUT/CONTROLLER/authorizedSsib
add wave -noupdate -height 15 -radix hexadecimal /benchtopdemo/DUT/CONTROLLER/Interface_0/RegAD
add wave -noupdate -height 15 -radix hexadecimal /benchtopdemo/DUT/CONTROLLER/Interface_0/Pout
add wave -noupdate -height 15 /benchtopdemo/DUT/CONTROLLER/DriverSSC/updateAuth
add wave -noupdate -height 15 /benchtopdemo/DUT/CONTROLLER/MAE/etat_c
add wave -noupdate -height 15 /benchtopdemo/DUT/CONTROLLER/MAE/etat_s
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {583 ns} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 262
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
WaveRestoreZoom {568 ns} {598 ns}
