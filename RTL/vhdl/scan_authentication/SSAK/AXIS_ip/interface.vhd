----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 26.04.2018 09:57:26
-- Design Name: 
-- Module Name: Interface - IJTAG
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Interface is
	Generic(
		s_segment : integer);
	Port(
		rstn : in std_logic;
		
		TCLK : in  std_logic;
		--
		SI_TMS : in  std_logic;
		SI_TD : in  std_logic;
		SI_update : in  std_logic;
		SI_shift : in  std_logic;
		SI_capture : in  std_logic;
		--
--		SO_TMS : out std_logic;
		SO_TD : out std_logic;
--		SO_update : out std_logic;
--		SO_shift : out std_logic;
--		SO_capture : out std_logic;
		
		Pin : in  std_logic_vector(s_segment-1 downto 0);
		Pout: out std_logic_vector(s_segment-1 downto 0);
		Uout: out std_logic);

end Interface;

architecture IJTAG of Interface is

----------------Déclaration-des-signaux----------------
	signal RegAD : std_logic_vector(s_segment-1 downto 0);
--	signal RegF  : std_logic_vector(s_segment-1 downto 0);
begin
--	Pout <= RegF;
	
	--SO_TCLK <= TCLK;
--	SO_TMS <= SI_TMS;
	SO_TD <= RegAD(0);
--	SO_update <= SI_update;
--	SO_shift <= SI_shift;
--	SO_capture <= SI_capture;
	
	process(TCLK)
	begin
		IF rising_edge(TCLK) then
			Uout	<= '0';
			if rstn = '0' then
				RegAD 	<= (others => '0');
				Pout	<= (others => '0');
				Uout	<= '0';
			elsif SI_TMS = '1' then
				if SI_update = '1' then
					RegAD	<= RegAD;
					Pout	<= RegAD;
					Uout	<= '1';
				elsif SI_capture = '1' then
					RegAD	<= Pin;
				elsif SI_shift = '1' then
					RegAD	<= SI_TD & RegAD(s_segment-1 downto 1);
				end if;
			end if;
		end if;
	end process;
end IJTAG;


