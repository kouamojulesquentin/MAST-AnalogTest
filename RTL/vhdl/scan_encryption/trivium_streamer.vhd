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

entity trivium_streamer is
 port  ( clk   : in  std_logic;
     rst  : in  std_logic; --Chain Reset
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;

     --External connections
     TDI_before_streamer   : in  std_logic;
     TDO_after_streamer   : out std_logic;

     --Protected segment
     protected_TDI   : out  std_logic;
     protected_TDO   : in std_logic;

     --ShiftEnable is needed to count shifted bits for keystream generation
     SH_en : in  std_logic 
   );
end trivium_streamer;

architecture behav of trivium_streamer is


signal streamer_input, streamer_output : std_logic;

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
signal current_chypertext: std_logic_vector(79 downto 0);
signal current_fromSUT: std_logic_vector(79 downto 0);

component trivium is
    Port ( SYS_CLK : in  STD_LOGIC; --System or User clock
      CNTRL : in STD_LOGIC_VECTOR(1 downto 0); --Control Bus: Manages Key/IV Loading, Init, and general operation
      KEY  : in  STD_LOGIC_VECTOR(79 downto 0); --Secret 80-bit key input port
      IV   : in  STD_LOGIC_VECTOR(79 downto 0); --80-bit Initialization vector input port
      KEY_OUT : out  STD_LOGIC);    --Cipher stream output
end component;

signal trivum_rst_count, next_trivum_rst_count : integer;

begin

---------------------------------
-- Trivium Streamer BEGIN
 

--Counter for initialization
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

--observation process (should be removed in sythesis)
 keystream_dump : process(clk)
  begin
   if rising_edge(clk) then
    if SH_en = '0' then
      nbit <= 0;
      current_keystream <= (others => '0');
      current_plaintext <= (others => '0');
      current_chypertext<= (others => '0');
      current_fromSUT<= (others => '0');
    else
      current_keystream(nbit) <=   keystream_s;
      current_plaintext(nbit) <=   streamer_input;
      current_chypertext(nbit) <=   TDI_before_streamer;
      current_fromSUT(nbit) <=   after_TDO;
      nbit <= nbit+1;
    end if;
   end if;
  end process;

--Chyper engine
  cipher: trivium
  port map(
    
    SYS_CLK => Clk,
    CNTRL   => cntrl,
    KEY     => key,
    IV      => iv,
    KEY_OUT => keystream_s

  );


protected_TDI <= streamer_input; --internal signal for debugging

---------------------------------
--Input encoding
streamer_input <= TDI_before_streamer xor keystream_s;

--Output encoding
streamer_output <= protected_TDO xor keystream_s; 


--Trivium Streamer END
---------------------------------
---------------------------------

TDO_after_streamer <= streamer_output;


end;
 
