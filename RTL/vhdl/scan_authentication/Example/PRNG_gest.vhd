  
---------------------------------------------
--#File prng_tb.vhd
--#Author: Rodrigo Wuerdig
--#Contact: rodrigo.wuerdig@acad.pucrs.br
---------------------------------------------
--This PRNG uses Fibonacci LFSRs with a 
--estimated period of 3.40282366920938463463374607431768211455 Ã— 10^38 clock cycles
--
--#expression: 
--    X^128 + X^126 + X^101 + X^99 + 1
--
--http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
---------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity PRNG_gest is
	port(
		clk  : in  std_logic;
		rstn : in  std_logic;
		RandVal : out std_logic_vector(127 downto 0));

end PRNG_gest;

architecture interface of PRNG_gest is
-------------Déclaration-des-signaux-------------
	signal clock    : std_logic;
	signal seed     : std_logic_vector(31 downto 0);
	signal reset    : std_logic;
	signal seed_sel : std_logic;
	signal lastRstn : std_logic;
begin

	reset <= not rstn;
	clock <= clk;
	
	process(clk)
	begin
		if rising_edge(clk) then
			lastRstn <= rstn;
		end if;
	end process;
	
	seed_sel <= rstn and not lastRstn;
	seed <= x"30061173";
	
    PRNG : entity work.PRNG port map(
        clk_in   => clock,
        reset_in => reset,
        random   => RandVal,
        seed     => seed,
        seed_sel => seed_sel);

end interface;






