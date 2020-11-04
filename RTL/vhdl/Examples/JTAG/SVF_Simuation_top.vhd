--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   ./vhd/SVF_Simuation_top.vhd
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
use work.SVF_Master_TAP_package.all;
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY SVF_Simulation_top IS
END SVF_Simulation_top;
 
ARCHITECTURE behavior OF SVF_Simulation_top IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
   

   --Inputs
   signal Resetn : std_logic := '0';
   signal Clk : std_logic := '0';

   -- Clock period definitions
   constant Clk_period : time := 10 ns;


signal       	  TDI		: std_logic;
signal			  TCK    : std_logic;
signal			  TMS    : std_logic;
signal           TRSTN  : std_logic;
signal			  TDO		: std_logic;
	
signal reset_chains : std_logic;
signal           IS_SHIFTING: std_logic;
signal           IS_IDLE: std_logic;
signal 	   Select_Reg : std_logic; --1 Data Registers, 0 IR
signal	   ClockIR : std_logic;
signal	   ShiftIR : std_logic;
signal	   UpdateIR : std_logic;
signal	   ClockDR : std_logic;
signal	   ShiftDR : std_logic;
signal	   UpdateDR : std_logic;
signal	 to_scan_chain :  std_logic;
signal	 from_scan_chain : std_logic;
signal CaptureDR: std_logic;
signal	 from_IR : std_logic;
signal	 from_BYP : std_logic;
signal	 select_IR : std_logic;
signal	 select_DR : std_logic;

signal	 select_BYP : std_logic;

signal IR_value : std_logic_vector(IR_SIZE-1 downto 0); 
signal select_DR_chain : std_logic_vector(MAX_DR_CHAINS-1 downto 0); 

signal	 from_DR : std_logic_vector(1 to MAX_DR_CHAINS);

--Generic signals used in the configurable SUT
signal internal_chain : std_logic_vector(0 to 20);
signal toSEL  : std_logic_vector(1 to MAX_LEVELS);
signal toCE   : std_logic_vector(1 to MAX_LEVELS);
signal toUE   : std_logic_vector(1 to MAX_LEVELS);
signal toSE   : std_logic_vector(1 to MAX_LEVELS);
signal toTCK  : std_logic_vector(1 to MAX_LEVELS);
signal toRST  : std_logic_vector(1 to MAX_LEVELS);

--------------------------
 --signal for PO capture

signal static_out : std_logic_vector(63 downto 0);
signal static_in : std_logic_vector(63 downto 0);
signal dynamic_out : std_logic_vector(127 downto 0);

signal SEL_delay,next_SEL_delay  : std_logic_vector(0 to MAX_LEVELS);
signal UE_delay, next_UE_delay   : std_logic_vector(0 to MAX_LEVELS);
signal SEL_int,next_SEL_int  : std_logic_vector(0 to MAX_LEVELS);
signal UE_int, next_UE_int   : std_logic_vector(0 to MAX_LEVELS);

signal Shifting:  std_logic;
signal SUT_ready : std_logic;

signal TAP_Resetn : std_logic;
 
component SUT
     port ( 
         --Environment signals
	 Clk : in std_logic; --Free Running system clock
	 SysResetn: in std_logic; --System Level Reset
	 SUT_ready : out std_logic; --Synchronization signal to IP initialization
         --TAP interface
	 TCK: in std_logic;        
         TMS: in std_logic;        
         TRSTN: in std_logic;  
         TDI: in std_logic;        
         TDO: out std_logic
	 );        
END component;


BEGIN
 

   -- Clock process definitions
   Clk_process :process
   begin
		Clk <= '0';
		wait for Clk_period/2;
		Clk <= '1';
		wait for Clk_period/2;
   end process;
 
  Resetn <= '0', '1' after 100 ns;
 
  main_tap: SVF_Master_TAP
    generic map (input_SVF_dir => "./"
                 ,input_SVF_file => "data_to_rtl.svf" 
		 ,output_SVF_dir => "./"
                 ,output_SVF_file => "data_from_rtl.dat" 
              )
    port map (
       Clk => Clk,
        Resetn => TAP_Resetn, --Resetn
  	  TCK => TCK,
      	  TDI => TDI,
	  TMS => TMS,
           TRSTN => TRSTN,
	  TDO => TDO,
	  Shifting=> Shifting
	   );   


  SUT_top : SUT 
   port map (
         --simulation signals
	 Clk => Clk,
	 SysResetn =>Resetn,
	 SUT_ready =>SUT_ready,
        --TAP interface
       	  TDI => TDI,
	  TCK => TCK,
	  TMS => TMS,
          TRSTN => TRSTN,
	  TDO => TDO
   );

TAP_Resetn <= Resetn and SUT_ready; --Wait for SUT initialization before starting Master TAP


END;
