--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/06/2016
-- Design Name:   
-- Module Name:   ./vhd/AMS_testcase.vhd
-- Project Name:  MAST_JTAG
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use STD.textio.all;

library work;
use work.master_TAP_package.all;
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.MAST_write.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values

entity AMS_testcase is
 port  ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end AMS_testcase;

architecture behav of AMS_testcase is


constant MEASURE_WIDTH : integer := 5;
constant CALIBRATION_WIDTH : integer := 5;
constant N_HARMONICS : integer := 4;

constant MAX_CHAIN_ELEMENTS : integer := N_HARMONICS*2 +3;

--Generic signals used in the configurable SUT
signal internal_chain : std_logic_vector(0 to MAX_CHAIN_ELEMENTS);

signal toSEL  : std_logic_vector(1 to MAX_LEVELS);
signal toCE   : std_logic_vector(1 to MAX_LEVELS);
signal toUE   : std_logic_vector(1 to MAX_LEVELS);
signal toSE   : std_logic_vector(1 to MAX_LEVELS);
signal toTCK  : std_logic_vector(1 to MAX_LEVELS);

--------------------------
 --signal for PI capture
constant IN_FILENAME_LENGTH : integer := 16;
constant OUT_FILENAME_LENGTH : integer := 17;

type in_exchange_files_names is array (1 to N_HARMONICS) of 
					string(1 to IN_FILENAME_LENGTH);
type out_exchange_files_names is array (1 to N_HARMONICS) of 
					string(1 to OUT_FILENAME_LENGTH);
					
constant measure_files : in_exchange_files_names := (
		"cal_measure_1.in",
		"cal_measure_2.in",
		"cal_measure_3.in",
		"cal_measure_4.in");
		
constant calibration_files : out_exchange_files_names := (
		"calibration_1.out",
		"calibration_2.out",
		"calibration_3.out",
		"calibration_4.out");
		

type measure_array is array (1 to N_HARMONICS) of 			
			std_logic_vector(MEASURE_WIDTH-1 downto 0);
signal measure : measure_array;

type calibration_array is array (1 to N_HARMONICS) of 			
			std_logic_vector(CALIBRATION_WIDTH-1 downto 0);
signal calibration : calibration_array;


signal SEL_delay,next_SEL_delay  : std_logic_vector(0 to MAX_LEVELS);
signal UE_delay, next_UE_delay   : std_logic_vector(0 to MAX_LEVELS);
signal SEL_int,next_SEL_int  : std_logic_vector(0 to MAX_LEVELS);
signal UE_int, next_UE_int   : std_logic_vector(0 to MAX_LEVELS);
signal toRST  : std_logic_vector(1 to MAX_LEVELS);

begin
------------------------------------------------------------------
 --Initialization  of helpers and signals, common to all testcases
------------------------------------------------------------------
 
  --Logging helpers 
  log_process: process
  begin
      set_n_log_files(MAX_BS_REGISTERS);
      logs_init_done <= TRUE;
     wait;
  end process;

 --data takes a cycle to be diplayed on PO, so we delay the signals
   next_SEL_int(0) <= Sel;
   next_SEL_int(1 to MAX_LEVELS) <= toSEL;
   next_UE_int(0) <=Up_en;
   next_UE_int(1 to MAX_LEVELS) <= toUE;
   next_SEL_delay <= SEL_int;
   next_UE_delay <= UE_int;
   
 sig_delay: process(clk)
  begin
   if rising_edge(clk) then
    SEL_delay <= next_SEL_delay;
    UE_delay  <= next_UE_delay;
    SEL_int <= next_SEL_int;
    UE_int  <= next_UE_int;
  end if;
 end process;

------------------------------------------------------------------
--------------------init end
------------------------------------------------------------------

internal_chain(0) <= TDI;


------------------------------------------------------------------
--------------------MEASURE REGISTERS
------------------------------------------------------------------

measure_registers: for i in 1 to N_HARMONICS generate
 --Generating the Measure registers

 measure_reg: bs_register generic map (size => MEASURE_WIDTH)
  port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => internal_chain(i-1),
     TDO   => internal_chain(i),
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => open,
     P_in => measure(i)
     ); 

 measure_reg_in : input_connection generic map (data_size => MEASURE_WIDTH,
                                        input_file => measure_files(i))
  port map
   (
     data_out => measure(i),
     Sel =>  Sel,
     CA_en => CA_en
   );

end generate;


------------------------------------------------------------------
--------------------CALIBRATION REGISTERS
------------------------------------------------------------------


calibration_registers: for i in 1 to N_HARMONICS generate
 --Generating the Measure registers

 measure_reg: bs_register generic map (size => MEASURE_WIDTH)
  port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => internal_chain(N_HARMONICS+i-1),
     TDO   => internal_chain(N_HARMONICS+i),
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => calibration(i),
     P_in => (others=> '0') --unused
     ); 


 calibration_reg_log : output_connection generic map 
				(data_size => CALIBRATION_WIDTH,
                                 output_file => calibration_files(i))
  port map
   (
     data_in => calibration(i),
     Sel => SEL_delay(0),
     UP_en => UE_delay(0)
   );

end generate;

TDO <= internal_chain(N_HARMONICS + N_HARMONICS);
------------------------------------------------------------------
--------------------OUTPUT CONNECTION
------------------------------------------------------------------


end;
 
