onerror {resume}
vsim -novopt work.benchtopdemo
quietly WaveActivateNextPane {} 0
add wave -noupdate -group BENCH /benchtopdemo/ctrl
add wave -noupdate -group BENCH /benchtopdemo/tclk
add wave -noupdate -group BENCH /benchtopdemo/clk
add wave -noupdate -group BENCH /benchtopdemo/rstn
add wave -noupdate -group BENCH /benchtopdemo/CSU_SE
add wave -noupdate -group BENCH /benchtopdemo/CSU_CE
add wave -noupdate -group BENCH /benchtopdemo/CSU_UE
add wave -noupdate -group BENCH /benchtopdemo/CSU_UE_Select
add wave -noupdate -group BENCH /benchtopdemo/TDI
add wave -noupdate -group BENCH /benchtopdemo/TDO
add wave -noupdate -group BENCH /benchtopdemo/PLAIN_TVALID
add wave -noupdate -group BENCH /benchtopdemo/PLAIN_TREADY
add wave -noupdate -group BENCH /benchtopdemo/PLAIN_TDATA
add wave -noupdate -group BENCH /benchtopdemo/Key_TVALID
add wave -noupdate -group BENCH /benchtopdemo/Key_TREADY
add wave -noupdate -group BENCH /benchtopdemo/Key_TDATA
add wave -noupdate -group BENCH /benchtopdemo/ENCR_TVALID
add wave -noupdate -group BENCH /benchtopdemo/ENCR_TREADY
add wave -noupdate -group BENCH /benchtopdemo/ENCR_TDATA
add wave -noupdate -group BENCH /benchtopdemo/resultat
add wave -noupdate -group BENCH /benchtopdemo/AuthStatut
add wave -noupdate -group BENCH /benchtopdemo/Defi
add wave -noupdate -group BENCH /benchtopdemo/eos
add wave -noupdate -group {SIBs states} -label SSAK_SIB_STATE /benchtopdemo/DUT/SIB_ssak/std_state
add wave -noupdate -group {SIBs states} -label aSIB_unlock /benchtopdemo/DUT/aSIB/sec_state
add wave -noupdate -group {SIBs states} -label aSIB_state /benchtopdemo/DUT/aSIB/std_state
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {1211 ns} 0}
quietly wave cursor active 1
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
WaveRestoreZoom {0 ns} {2100 ns}
restart -force
run -all
