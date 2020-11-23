--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   Trivium_Streamer
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
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values

entity double_trivium_testcase is
 port  ( clk   : in  std_logic;
     rst  : in  std_logic; --Chain Reset
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end double_trivium_testcase;

architecture behav of double_trivium_testcase is

--Generic signals used in the configurable SUT
signal internal_chain : std_logic_vector(0 to 20);
signal toSEL  : std_logic_vector(1 to MAX_LEVELS);
signal toCE   : std_logic_vector(1 to MAX_LEVELS);
signal toUE   : std_logic_vector(1 to MAX_LEVELS);
signal toSE   : std_logic_vector(1 to MAX_LEVELS);
signal toTCK  : std_logic_vector(1 to MAX_LEVELS);

--------------------------
 --signal for PO capture
 
constant before_bits : integer := 12;
constant protected_bits : integer := 16;
constant after_bits : integer := 8;


signal before_out : std_logic_vector(before_bits-1 downto 0);
signal before_in : std_logic_vector(before_bits-1 downto 0);

signal protected_out : std_logic_vector(protected_bits-1 downto 0);
signal protected_in : std_logic_vector(protected_bits-1 downto 0);

signal after_out : std_logic_vector(after_bits-1 downto 0);
signal after_in : std_logic_vector(after_bits-1 downto 0);

signal SEL_delay,next_SEL_delay  : std_logic_vector(0 to MAX_LEVELS);
signal UE_delay, next_UE_delay   : std_logic_vector(0 to MAX_LEVELS);
signal SEL_int,next_SEL_int  : std_logic_vector(0 to MAX_LEVELS);
signal UE_int, next_UE_int   : std_logic_vector(0 to MAX_LEVELS);
signal toRST  : std_logic_vector(1 to MAX_LEVELS);


signal before_streamer, after_streamer : std_logic;
signal streamer_input, streamer_output : std_logic;
signal protected_TDI, protected_TDO : std_logic;

signal after_TDI, after_TDO : std_logic;
signal before_TDI, before_TDO : std_logic;

component trivium_streamer 
 port  ( clk   : in  std_logic;
     rst  : in  std_logic; --Chain Reset
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;
     Trivium_busy : out std_logic; --'1' when resetting the Chyper

     KEY    : in  std_logic_vector(79 downto 0);
     IV     : in std_logic_vector(79 downto 0);
    
     --External connections
     TDI_before_streamer   : in  std_logic;
     TDO_after_streamer   : out std_logic;

     --Protected segment
     protected_TDI   : out  std_logic;
     protected_TDO   : in std_logic;

     --ShiftEnable is needed to count shifted bits for keystream generation
     SH_en : in  std_logic 
   );
end component;

signal second_protected_out : std_logic_vector(protected_bits-1 downto 0);
signal second_after_out : std_logic_vector(after_bits-1 downto 0);
signal second_after_TDI: std_logic;
signal second_protected_TDI: std_logic;
signal second_protected_TDO: std_logic;
signal second_after_TDO: std_logic;

signal    KEY    : std_logic_vector(79 downto 0) ;
signal   IV     : std_logic_vector(79 downto 0) ;

signal Trivium_ready_1,Trivium_ready_2 : std_logic;

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
--   if rising_edge(clk) then
   if clk'event then
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
  
before_register: bs_register generic map (size => before_bits)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => TDI,
     TDO   => before_TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => before_out,
     P_in => before_in --(others => '1')
     ); 
before_in <= x"123";

--Streamer instatiation

KEY <= X"0F62B5085BAE0154A7FA";
IV  <=X"288FF65DC42B92F960C7";

Trivium_ready <= Trivium_ready_1 and Trivium_ready_2;

Streamer : trivium_streamer 
 port map ( 
     clk => clk,
     rst => rst,
     SysResetn => SysResetn,
     Trivium_ready => Trivium_ready_1,
   Trivium_busy => open,

     KEY    =>  KEY,
     IV     => IV,
  
       --External connections
     TDI_before_streamer => before_TDO,
     TDO_after_streamer  => after_TDI,

     --Protected segment
     protected_TDI  =>  protected_TDI,
     protected_TDO  =>  protected_TDO,

     --ShiftEnable is needed to count shifted bits for keystream generation
     SH_en => SH_en
   );
 
protected_register: bs_register generic map (size => protected_bits)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => protected_TDI,
     TDO   => protected_TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => protected_out,
     P_in => protected_in --(others => '1')
     ); 
protected_in <= x"3456";

---------------------------------
---------------------------------

after_register: bs_register generic map (size => after_bits)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => after_TDI,
     TDO   => after_TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => after_out,
     P_in => after_in --(others => '1')
     ); 
after_in <= x"78";


--------------Second Streamer
second_Streamer : trivium_streamer 
 port map ( 
     clk => clk,
     rst => rst,
     SysResetn => SysResetn,
     Trivium_ready => Trivium_ready_2,


     KEY    =>  KEY,
     IV     => IV,

     --External connections
     TDI_before_streamer => after_TDO,
     TDO_after_streamer  => second_after_TDI,

     --Protected segment
     protected_TDI  =>  second_protected_TDI,
     protected_TDO  =>  second_protected_TDO,

     --ShiftEnable is needed to count shifted bits for keystream generation
     SH_en => SH_en
   );
 
second_protected_register: bs_register generic map (size => protected_bits)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => second_protected_TDI,
     TDO   => second_protected_TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => second_protected_out,
     P_in => protected_in --(others => '1')
     ); 

second_after_register: bs_register generic map (size => after_bits)
 port map
   ( clk   => clk, 
     rst   => rst, 
     TDI   => second_after_TDI,
     TDO   => second_after_TDO,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => second_after_out,
     P_in => after_in --(others => '1')
     ); 

---------------------------------
---------------------------------

 TDO <= second_after_TDO;

end;
 
