--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   ./vhd/P1687_testcase.vhd
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

entity P1687_testcase is
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
end P1687_testcase;

architecture behav of P1687_testcase is

--Generic signals used in the configurable SUT
signal internal_chain : std_logic_vector(0 to 20);
signal toSEL  : std_logic_vector(1 to MAX_LEVELS);
signal toCE   : std_logic_vector(1 to MAX_LEVELS);
signal toUE   : std_logic_vector(1 to MAX_LEVELS);
signal toSE   : std_logic_vector(1 to MAX_LEVELS);
signal toTCK  : std_logic_vector(1 to MAX_LEVELS);

--------------------------
 --signal for PO capture

signal static_out : std_logic_vector(63 downto 0);
signal static_in : std_logic_vector(63 downto 0);
signal dynamic_out : std_logic_vector(127 downto 0);

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


 --one 64-bit "static" register, followed by a SIB connecting to a 128-bit "dynamic" 
static_reg: bs_register generic map (size => 64)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => TDI,
     TDO   => internal_chain(0),
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => static_out,
     P_in => static_in --(others => '1')
     ); 

---------------------------------
static_reg_log : output_connection generic map (data_size => 64,
                                        output_file => "static_reg.out")
  port map
   (
     data_in => static_out,
     Sel => SEL_delay(0),
     UP_en => UE_delay(0)
   );

static_reg_in : input_connection generic map (data_size => 64,
                                        input_file => "static_reg.in")
  port map
   (
     data_out => static_in,
     Sel =>  Sel,
     CA_en => CA_en
   );
---------------------------------

SIB_0 : SIB port map(   
     TCK  => clk,
     RST  => rst, 
     SI   => internal_chain(0),
     SO   => TDO,
     SEL  => Sel,
     CE   => CA_en,
     UE   => UP_en,
     SE   => SH_en,
     
     toSI   => internal_chain(1),
     fromSO => internal_chain(2),
     toSEL  => toSEL(1),
     toCE   => toCE(1),
     toUE   => toUE(1),
     toSE   => toSE(1),
     toTCK  => toTCK(1),
     toRST  => toRST(1)
     );

dynamic_reg: bs_register generic map (size => 128)
 port map
   ( clk   => toTCK(1),
     rst   => toRST(1), 
     TDI   => internal_chain(1),
     TDO   => internal_chain(2),
     mode => mode,
     SH_en => toSE(1),
     UP_en => toUE(1),
     Sel   => toSEL(1),
     CA_en => toCE(1),
     P_out => dynamic_out,
     P_in => (others => '1')
     ); 

---------------------------------
dynamic_reg_log : output_connection generic map (data_size => 128,
                                        output_file => "dynamic_reg.out")
  port map
   (
     data_in => dynamic_out,
     Sel => SEL_delay(1),
     UP_en => UE_delay(1)
   );
---------------------------------

end;
 
