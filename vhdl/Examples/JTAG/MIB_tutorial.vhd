--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   ./vhd/Debug_Mux.vhd
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
use work.types.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values

entity MIB_tutorial_testcase is
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
end MIB_tutorial_testcase;

architecture behav of MIB_tutorial_testcase is

------------------------------------------------------
-- Configuration Parameters
-- NB: must be coherent with MIB_Tutorial.sit
constant REGISTER_SIZE : integer := 8;
constant MUX_SIZE : integer := 3;
constant SELECTOR_CODING : coding:= One_Hot;
constant REGISTER_BEFORE_MUX : boolean := false;
------------------------------------------------------

--Generic signals used in the configurable SUT
--signal internal_chain : std_logic;
signal SEL_mux  : std_logic_vector(0 to MUX_SIZE-1);
signal CE_mux   : std_logic_vector(0 to MUX_SIZE-1);
signal UE_mux   : std_logic_vector(0 to MUX_SIZE-1);
signal SE_mux   : std_logic_vector(0 to MUX_SIZE-1);


signal MUX_in   : std_logic_vector(0 to MUX_SIZE-1);

signal MUX_subchains_in   : std_logic_vector(1 to MUX_SIZE);
signal MUX_subchains_out   : std_logic_vector(1 to MUX_SIZE);
--------------------------
 --signal for PO capture


type LB_array is array (1 to MUX_SIZE) of std_logic_vector(REGISTER_SIZE-1 downto 0);

signal Register_LB : LB_array;

signal Register_0_LB : std_logic_vector(REGISTER_SIZE-1 downto 0);
signal Register_1_LB : std_logic_vector(REGISTER_SIZE-1 downto 0);
signal Register_2_LB : std_logic_vector(REGISTER_SIZE-1 downto 0);
signal Register_3_LB : std_logic_vector(REGISTER_SIZE-1 downto 0);

--Generic signals used in the configurable SUT
signal top_level_chain : std_logic;
signal toSEL  : std_logic_vector(1 to MUX_SIZE);
signal toCE   : std_logic_vector(1 to MUX_SIZE);
signal toUE   : std_logic_vector(1 to MUX_SIZE);
signal toSE   : std_logic_vector(1 to MUX_SIZE);
signal toTCK  : std_logic_vector(1 to MUX_SIZE);
signal toRST  : std_logic_vector(1 to MUX_SIZE);

component MIB 
 generic (size :integer := 1;
          reg_before : boolean := true;
	  selector: coding:= Binary);
 port
   ( TCK   : in  std_logic;
     RST  : in  std_logic;
     SI   : in  std_logic;
     SO   : out std_logic;
     SEL  : in  std_logic;
     CE   : in  std_logic;
     UE   : in  std_logic;
     SE   : in  std_logic;
     
     toSI   : out std_logic_vector(1 to size);
     fromSO : in  std_logic_vector(1 to size);
     toSEL  : out  std_logic_vector(1 to size);
     toCE   : out std_logic_vector(1 to size);
     toUE   : out std_logic_vector(1 to size);
     toSE   : out std_logic_vector(1 to size);
     toTCK  : out std_logic_vector(1 to size);
     toRST  : out std_logic_vector(1 to size)
   );
 end  component;

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

------------------------------------------------------------------
--------------------init end
------------------------------------------------------------------


MIB_0 : MIB 
     generic map (size => MUX_SIZE,
     		  reg_before => REGISTER_BEFORE_MUX,
	          selector=> SELECTOR_CODING)
     port map(   
     TCK  => clk,
     RST  => rst, 
     SI   => TDI,--top_level_chain,
     SO   => TDO,
     SEL  => Sel,
     CE   => CA_en,
     UE   => UP_en,
     SE   => SH_en,
     
     toSI   => MUX_subchains_in,
     fromSO => MUX_subchains_out,
     toSEL  => toSEL,
     toCE   => toCE,
     toUE   => toUE,
     toSE   => toSE,
     toTCK  => toTCK,
     toRST  => toRST
     );


subchains: for i in 1 to MUX_SIZE generate

register_subchain: bs_register generic map (size => REGISTER_SIZE)
port map
   ( clk   => toTCK(i),
     rst   => rst,--toRST(1), 
     TDI   => MUX_subchains_in(i),
     TDO   => MUX_subchains_out(i),
     mode => mode,
     SH_en => toSE(i),
     UP_en => toUE(i),
     Sel   => toSEL(i),
     CA_en => toCE(i),
     P_out => Register_LB(i),
     P_in => Register_LB(i) --(others => '1')
     ); 
end generate;

end;
 

