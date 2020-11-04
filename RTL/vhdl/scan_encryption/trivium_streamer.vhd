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

entity trivium_testcase is
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
end trivium_testcase;

architecture behav of trivium_testcase is

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
constant protected_bits : integer := 13;
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

-- Input signals for the Trivium
signal init_cntrl: std_logic_vector(1 downto 0);
signal CNTRL  : std_logic_vector(1 downto 0);
signal KEY    : std_logic_vector(79 downto 0);
signal IV     : std_logic_vector(79 downto 0);

signal keystream_s : std_logic;

signal TM:  std_logic;

signal nbit: integer := 0;
signal current_keystream: std_logic_vector(79 downto 0);
signal current_plaintext: std_logic_vector(79 downto 0);

-- Clock period definitions for trivium initialization
constant Clk_period : integer := 10; --10 ns

component trivium is
    Port ( SYS_CLK : in  STD_LOGIC; --System or User clock
      CNTRL : in STD_LOGIC_VECTOR(1 downto 0); --Control Bus: Manages Key/IV Loading, Init, and general operation
      KEY  : in  STD_LOGIC_VECTOR(79 downto 0); --Secret 80-bit key input port
      IV   : in  STD_LOGIC_VECTOR(79 downto 0); --80-bit Initialization vector input port
      KEY_OUT : out  STD_LOGIC);    --Cipher stream output
end component;

signal trivum_rst_count, next_trivum_rst_count : integer;

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
     TDO   => before_streamer,
     mode => mode,
     SH_en => SH_en,
     UP_en => UP_en,
     Sel   => Sel,
     CA_en => CA_en,
     P_out => before_out,
     P_in => before_in --(others => '1')
     ); 
before_in <= x"123";

---------------------------------
--Trivium Streamer BEGIN
 
streamer_input <= before_streamer xor keystream_s;

trivium_counter: process(clk)
 begin
  if falling_edge(clk) then
   if (SysResetn = '0') then
      trivum_rst_count <= 0;
      else
      trivum_rst_count<= next_trivum_rst_count;
   end if;   
  end if;    
 end process;
 
  init_proc: process(trivum_rst_count)
  begin
    next_trivum_rst_count <= trivum_rst_count+1;
    key <= X"0F62B5085BAE0154A7FA";
    iv  <= X"288FF65DC42B92F960C7";
    init_cntrl <= "00";
    TM     <= '0';
    Trivium_ready <= '0';
  
   if (trivum_rst_count = 1) then
       init_cntrl <= "10";
   end if;    
   if (trivum_rst_count > 1) and (trivum_rst_count < 1154)then
      init_cntrl <= "11";
   end if;    

   if (trivum_rst_count >= 1300) then
       TM     <= '1';
       next_trivum_rst_count <= trivum_rst_count; --stop incrementing counter 
      Trivium_ready <= '1'; --Trivium chyper ready for usage
   end if;    

  
  end process;

  chipher_cntrl_sel: process(TM, init_cntrl,SH_en)
  begin 
    if ( TM = '1' ) then
      if (SH_en = '1') then 
        CNTRL <= "01";
      else
	CNTRL <= "00";
      end if;	
    else
      CNTRL <= init_cntrl;
    end if;
  end process;

 keystream_dump : process(clk)
  begin
   if rising_edge(clk) then
    if SH_en = '0' then
      nbit <= 0;
      current_keystream <= (others => '0');
      current_plaintext <= (others => '0');
    else
      current_keystream(nbit) <=   keystream_s;
      current_plaintext(nbit) <=   streamer_input;
      nbit <= nbit+1;
    end if;
   end if;
  end process;

  cipher: trivium
  port map(
    
    SYS_CLK => Clk,
    CNTRL   => cntrl,
    KEY     => key,
    IV      => iv,
    KEY_OUT => keystream_s

  );

protected_TDI <= streamer_input;
---------------------------------

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
protected_in <= '1'&x"456";

streamer_output <= protected_TDO xor keystream_s; 


--Trivium Streamer END
---------------------------------
---------------------------------

after_TDI <= streamer_output;

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


-- TDO <= after_TDO;
 TDO <= streamer_output; --Bypass After Register

end;
 
