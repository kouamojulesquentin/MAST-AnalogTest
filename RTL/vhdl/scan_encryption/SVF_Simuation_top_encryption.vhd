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
 
ENTITY SVF_Simulation_top_encryption IS
END SVF_Simulation_top_encryption;
 
ARCHITECTURE behavior OF SVF_Simulation_top_encryption IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
   

--Inputs
signal Resetn : std_logic := '0';
signal Clk : std_logic := '0';

signal rstn_master:  std_logic := '0';

-- Clock period definitions
constant Clk_period : time := 10 ns;


signal       	TDI_plain	  : std_logic;
signal        TDI_cipher  : std_logic;
signal			  TCK    : std_logic;
signal			  TMS    : std_logic;
signal           TRSTN  : std_logic;
signal			  TDO_plain  : std_logic;
signal        TDO_cipher : std_logic;
	
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

-------------
-- Input signals for the Trivium
signal init_cntrl: std_logic_vector(1 downto 0);
signal CNTRL  : std_logic_vector(1 downto 0);
signal KEY    : std_logic_vector(79 downto 0);
signal IV     : std_logic_vector(79 downto 0);

signal keystream : std_logic;

signal Sampling_TDI : std_logic;

signal TM:  std_logic;

signal Shifting:  std_logic;

 
component SUT
     port ( 
         --simulation signals
	 Clk : in std_logic;
        --TAP interface
	 TCK: in std_logic;        
         TMS: in std_logic;        
         TRSTN: in std_logic;  
         TDI: in std_logic;        
         TDO: out std_logic
	 );        
END component;

component trivium is
    Port ( SYS_CLK : in  STD_LOGIC; --System or User clock
      CNTRL : in STD_LOGIC_VECTOR(1 downto 0); --Control Bus: Manages Key/IV Loading, Init, and general operation
      KEY  : in  STD_LOGIC_VECTOR(79 downto 0); --Secret 80-bit key input port
      IV   : in  STD_LOGIC_VECTOR(79 downto 0); --80-bit Initialization vector input port
      KEY_OUT : out  STD_LOGIC);    --Cipher stream output
end component;

function reverse_any_vector (a: in std_logic_vector)
return std_logic_vector is
  variable result: std_logic_vector(a'RANGE);
  alias aa: std_logic_vector(a'REVERSE_RANGE) is a;
begin
  for i in aa'RANGE loop
    result(i) := aa(i);
  end loop;
  return result;
end; -- function reverse_any_vector


BEGIN
 
 --reset
 Resetn <= '0', '1' after 100 ns;
 
-- Test mode signal should not be activated before the initialization of the stream cipher is finished
 TM     <= '0', '1' after Clk_period*1300;
 rstn_master <= TM and Resetn;

 -- Clock process definitions
 Clk_process :process
 begin
  Clk <= '0';
  wait for Clk_period/2;
  Clk <= '1';
  wait for Clk_period/2;
 end process;

  init_proc: process
  begin
    wait for 100 ns;
      key <= X"0F62B5085BAE0154A7FA";
      iv  <= X"288FF65DC42B92F960C7";
--      key <= reverse_any_vector(X"0F62B5085BAE0154A7FA");
--      iv  <= reverse_any_vector(X"288FF65DC42B92F960C7");
      init_cntrl <= "10";
    wait for Clk_period;
      init_cntrl <= "11";
    wait for Clk_period*1152;
      init_cntrl <= "00";
    wait;
  end process;

  chipher_cntrl_sel: process(TM, init_cntrl,Sampling_TDI)
  begin 
    if ( TM = '1' ) then
      if (Sampling_TDI = '1') then 
        CNTRL <= "01";
      else
	CNTRL <= "00";
      end if;	
    else
      CNTRL <= init_cntrl;
    end if;
  end process;

  main_tap: SVF_Master_TAP
    generic map (input_SVF_dir => "./"
                 ,input_SVF_file => "data_to_rtl.svf" 
		 ,output_SVF_dir => "./"
                 ,output_SVF_file => "data_from_rtl.dat" 
              )
    port map (

        Clk => Clk,
        Resetn => rstn_master,
       	  TDI => TDI_cipher,
			  TCK => TCK,
			  TMS => TMS,
           TRSTN => TRSTN,
			  TDO => TDO_cipher,
			  Shifting=> Shifting,
			  Sampling_TDI => Sampling_TDI
	   );   


  SUT_top : SUT 
   port map (
         --simulation signals
	 Clk => Clk,
        --TAP interface
       	  TDI => TDI_plain,
	  TCK => TCK,
	  TMS => TMS,
          TRSTN => TRSTN,
	  TDO => TDO_plain
   );

  cipher: trivium
  port map(
    
    SYS_CLK => Clk,
    CNTRL   => cntrl,
    KEY     => key,
    IV      => iv,
    KEY_OUT => keystream

  );

 
  TDI_plain <= TDI_cipher xor keystream;
  TDO_cipher <= TDO_plain xor keystream;


  --TDI_plain <= TDI_cipher;
  --TDO_cipher <= TDO_plain;
 
END;
