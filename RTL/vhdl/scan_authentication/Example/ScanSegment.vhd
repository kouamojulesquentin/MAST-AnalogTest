--------------------------------------------------------------------------------
--                                                                            --
-- File name : ScanSegment.vhd                                                --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 08/11/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity ScanSegment is
	generic(
		size : integer:=1);
	port(
		clk    : in  std_logic;
		rstn   : in  std_logic;
		CSU_SE	: in  std_logic;
		CSU_UE	: in  std_logic;
		CSU_CE	: in  std_logic;
		CSU_Select 	: in  std_logic;
		TDI 	: in  std_logic;
		TDO 	: out std_logic;
		PDI 	: in  std_logic_vector(size-1 downto 0);
		PDO 	: out std_logic_vector(size-1 downto 0));
end ScanSegment;

Architecture newArch of ScanSegment is
-------------Déclaration-des-signaux-------------
	signal mem   : std_logic_vector(size-1 downto 0);
	signal shift : std_logic_vector(size downto 0);
begin
	shift(size) <= TDI;
	TDO <= shift(0);
	PDO <= mem;
	
	CASELP : FOR I IN 0 TO size-1 GENERATE
		process(clk)
		begin
			if rising_edge(clk) then
				if rstn='0' then
					mem(I) <= '0';
					shift(I) <= '0';
				elsif CSU_Select='1' then
					if CSU_CE='1' then
						shift(I) <= mem(I);
					elsif CSU_SE='1' then
						shift(I) <= shift(I+1);
					elsif CSU_UE = '1' then
						mem(I) <= shift(I);
					end if;
				else
					mem(I) <= PDI(I);
				end if;
			end if;
		end process;
	END GENERATE;
						
end newArch;


