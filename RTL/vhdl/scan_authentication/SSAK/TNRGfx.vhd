--------------------------------------------------------------------------------
--                                                                            --
-- File name : TNRGfx.vhd                                                     --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 23/04/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity TNRGfx is
   generic(
		size : integer);
   port(
		mem : out std_logic_vector(size-1 downto 0)
       );
end TNRGfx;

Architecture Fx of TNRGfx is
	constant rom : std_logic_vector(255 downto 0) := x"8668955D3BAD6206DD7B16A7C81EC8A37B25D9C8A7365C096E9924025A3FFB03";
begin
	mem <= rom(size-1 downto 0);
end Fx;

