--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   proto_top.vhd
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

library work;
use work.slave_TAP_synth_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 Library  UNISIM;
use  UNISIM.vcomponents.all;

ENTITY proto_top IS
 port (
        	  TDI		: in std_logic;
			  TCK    : in std_logic;
			  TMS    : in std_logic;
           TRSTN  : in std_logic;
			  TDO	: out	 std_logic;
			    GPIO_DIP_SW : in std_logic_vector(8 downto 1);
				 
			    GPIO_LED : out std_logic_vector(7 downto 0)
);

END proto_top;
 
ARCHITECTURE behavior OF proto_top IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
   

	
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

signal SEL_delay,next_SEL_delay  : std_logic_vector(0 to MAX_LEVELS);
signal UE_delay, next_UE_delay   : std_logic_vector(0 to MAX_LEVELS);
signal SEL_int,next_SEL_int  : std_logic_vector(0 to MAX_LEVELS);
signal UE_int, next_UE_int   : std_logic_vector(0 to MAX_LEVELS);

component tutorial_1_synth 
 port  ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic;
     static_out : out std_logic_vector(11 downto 0);
     static_in : in std_logic_vector(11 downto 0)
   );
end component;


signal     static_out : std_logic_vector(11 downto 0);
signal     static_in : std_logic_vector(11 downto 0);



signal int_tck: std_logic;

BEGIN


tckbuf: IBUFG
generic  map  (
IOSTANDARD  =>  "DEFAULT")
port  map  (
O  =>  int_TCK,  --  Clock  buffer  output
I  =>  TCK    --  Clock  buffer  input  (connect  directly  to  top-level  port) 
);

 slave_TAP_0 : slave_TAP_synth 
    port  map ( 
	        --TAP Signalq
			  TCK => int_TCK,
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
   ( clk   => int_TCK, --ClockIR,
     rst   => reset_chains, 
     TDI   => to_scan_chain,
     TDO   => from_IR,
     SH_en => ShiftIR,
     UP_en => UpdateIR,
     Sel   => select_IR,
     P_out => IR_value
     );

BYP_reg : bs_cell port map
    ( clk  => int_TCK, --ClockDR,
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
    when "11111111" => 
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

SUT : tutorial_1_synth  port map 
   ( clk   => int_TCK,
     rst   => reset_chains,
     TDI   => to_scan_chain,
     TDO   => from_DR(1),
     mode  => '1',
     SH_en => ShiftDR,
     CA_en => CaptureDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1),
     static_out => static_out,
     static_in => static_in
   );
	
	static_in(7 downto 0) <=   GPIO_DIP_SW;

			    GPIO_LED <= static_out(7 downto 0);
end generate;


END;
