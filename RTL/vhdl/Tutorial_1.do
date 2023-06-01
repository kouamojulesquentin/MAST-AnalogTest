onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {Top-Level TAP Signals}
add wave -noupdate -height 15 /svf_simulation_top/TDI
add wave -noupdate -height 15 /svf_simulation_top/TCK
add wave -noupdate -height 15 /svf_simulation_top/TMS
add wave -noupdate -height 15 /svf_simulation_top/TRSTN
add wave -noupdate -height 15 /svf_simulation_top/TDO
add wave -noupdate -divider {System Under Test}
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_TUTORIAL_1/SUT/static_out
add wave -noupdate -height 15 -radix hexadecimal /svf_simulation_top/SUT_top/SUT_TUTORIAL_1/SUT/static_in
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {5001225 ns} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 86
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
WaveRestoreZoom {4984921 ns} {5017529 ns}
