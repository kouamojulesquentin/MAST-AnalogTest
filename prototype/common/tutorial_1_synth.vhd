--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   ./vhd/tutorial_1.vhd
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

entity tutorial_1_synth is
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
end tutorial_1_synth;

architecture behav of tutorial_1_synth is

--Generic signals used in the configurable SUT
signal internal_chain : std_logic_vector(0 to 20);
signal toSEL  : std_logic_vector(1 to MAX_LEVELS);
signal toCE   : std_logic_vector(1 to MAX_LEVELS);
signal toUE   : std_logic_vector(1 to MAX_LEVELS);
signal toSE   : std_logic_vector(1 to MAX_LEVELS);
signal toTCK  : std_logic_vector(1 to MAX_LEVELS);

--------------------------
 --signal for PO capture


signal SEL_delay,next_SEL_delay  : std_logic_vector(0 to MAX_LEVELS);
signal UE_delay, next_UE_delay   : std_logic_vector(0 to MAX_LEVELS);
signal SEL_int,next_SEL_int  : std_logic_vector(0 to MAX_LEVELS);
signal UE_int, next_UE_int   : std_logic_vector(0 to MAX_LEVELS);
signal toRST  : std_logic_vector(1 to MAX_LEVELS);

begin
------------------------------------------------------------------
 --Initialization  of helpers and signals, common to all testcases
------------------------------------------------------------------

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


 --one 12-bit "reg" register
  
static_reg: bs_register generic map (size => 12)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => TDI,
     TDO   => TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => static_out,
     P_in => static_in --(others => '1')
     ); 

end;
 
