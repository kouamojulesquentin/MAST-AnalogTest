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
use work.AI_JTAG_SVF_package.all;
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.MAST_write.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY SVF_Simuation_top IS
END SVF_Simuation_top;
 
ARCHITECTURE behavior OF SVF_Simuation_top IS 
 
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

component tutorial_1_testcase 
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
end component;

component SIB_tutorial_testcase
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
end component;

component AMS_testcase 
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
end component;

component DEBUG_MUX_testcase 
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
end component;

component MIB_tutorial_testcase 
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
end component;

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
 
  main_tap: AI_JTAG_SVF 
    generic map (input_SVF_dir => "./"
                 ,input_SVF_file => "data_to_rtl.svf" 
		 ,output_SVF_dir => "./"
                 ,output_SVF_file => "data_from_rtl.dat" 
              )
    port map (
       	  TDI => TDI,
			  TCK => TCK,
			  TMS => TMS,
           TRSTN => TRSTN,
			  TDO => TDO
	   );   



 slave_TAP_0 : slave_TAP 
    port  map ( 
	        --TAP Signalq
			  TCK => TCK,
           TMS  => TMS,
           TRSTN  => TRSTN,
           TDI => TDI, --NB!
           TDO => TDO, --NB!
			   --debug signals
           IS_SHIFTING  => IS_SHIFTING,
           IS_IDLE => IS_IDLE,
        --Scan chain control signals
	   reset => reset_chains,
 	   Select_IR  => Select_IR,
 	   Select_DR  => Select_DR,
	   ClockIR  => ClockIR,
	   ShiftIR  => ShiftIR,
	   UpdateIR  => UpdateIR,
	   ClockDR  => ClockDR,
	   ShiftDR  => ShiftDR,
	   UpdateDR  => UpdateDR,
	   CaptureDR => CaptureDR,
	   to_scan_chain => to_scan_chain,
	  from_scan_chain =>from_scan_chain

	   );       
IR_reg : bs_register_nocapture generic map (size => IR_SIZE)
 port map
   ( clk   => TCK, --ClockIR,
     rst   => reset_chains, 
     TDI   => to_scan_chain,
     TDO   => from_IR,
     SH_en => ShiftIR,
     UP_en => UpdateIR,
     Sel   => select_IR,
     P_out => IR_value
     );

BYP_reg : bs_cell port map
    ( clk  => TCK, --ClockDR,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_BYP,
     P_in  => '0',
     P_out => open,
     mode  => '0',
     SH_en => ShiftDR,
     CA_en => '0',
     UP_en => UpdateIR,
     Sel   => select_BYP
   );



DR_Mux : process(IR_value,from_IR,from_DR,Select_DR,Select_IR)
 begin
  select_BYP <= '0';
  select_DR_chain <= (others=> '0');
  if (Select_IR = '1') then
    from_scan_chain <= from_IR;
  elsif(Select_DR = '1') then
   case (IR_value) is 
    when "1111" => 
               select_BYP <= '1';
	       from_scan_chain <= from_BYP;
    when others =>
        select_DR_chain(to_integer(unsigned(IR_value)))<='1';  
        from_scan_chain <= from_DR(to_integer(unsigned(IR_value)));
   end case; 
  end if;  	 
    
 end process;

--Generating SUT based on the configuration in MAST_config
--from

DR_loopback: if target_SUT = LOOPBACK generate
 chain_loop: for n in 1 to MAX_DR_CHAINS generate
    from_DR(n) <= to_scan_chain;
  end generate;
end generate;


SUT_TUTORIAL_1: if target_SUT = TUTORIAL_1 generate

SUT : tutorial_1_testcase  port map 
   ( clk   => TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1)
   );
end generate;


SUT_IEE1687: if target_SUT = SIB_tutorial generate

SUT : SIB_tutorial_testcase  port map 
   ( clk   => TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1)
   );

end generate;

SUT_AMS: if target_SUT = AMS generate

SUT : AMS_testcase  port map 
   ( clk   => TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1)
   );

end generate;

SUT_MIB_tutorial: if target_SUT = MIB_tutorial generate

SUT : MIB_tutorial_testcase  port map 
   ( clk   => TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1)
   );
end generate;

SUT_DEBUG_MUX: if target_SUT = DEBUG_MUX generate

SUT : DEBUG_MUX_testcase  port map 
   ( clk   => TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1)
   );

end generate;

END;
