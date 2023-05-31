-----------------------------------------
-- Auteur      : Vincent Reynaud
-- Fichier     : LSKR.vhd LSIB Key Register
-- Date        : 14/11/2018
-----------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity LSKR is
	generic(
		size : integer);
	port(
		SI : in std_logic;
		SE : in std_logic;
		SEL : in std_logic;
		RST : in std_logic;
		TCK : in std_logic;
		SO : out std_logic;
		CE : in std_logic;
		UE : in std_logic;
---		DI : in std_logic_vector(dataWidth-1 downto 0);
		DO : out std_logic_vector(size-1 downto 0));
end LSKR;

architecture warp of LSKR is

	constant dataWidth : integer := size;
-----------------Déclaration-des-signaux-----------------
	signal rstn     : std_logic;
	signal TabShift : std_logic_vector(dataWidth downto 0);
begin
    rstn <= not RST;
	TabShift(size)	<= SI;
	SO <= TabShift(0);
	DO <= TabShift(size-1 downto 0);
	ALL_CELLS : FOR I IN 0 TO dataWidth-1 GENERATE
        process(TCK)
		begin
			if rising_edge(TCK) then
				TabShift(I) <= TabShift(I);
				if RST='1' then
					TabShift(I) <= '0';
				elsif SEL = '1' then
					if CE = '1' then
						TabShift(I) <= '0';
					elsif SE='1' then
						TabShift(I) <= TabShift(I+1);
					end if;
				end if;
			end if;
		end process;
	END GENERATE;
end warp;