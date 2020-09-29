----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 16.05.2018 10:46:16
-- Design Name: 
-- Module Name: SecureReg - Behavioral
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

entity SecureReg is
    Port ( clk : in STD_LOGIC;
           rstn: in STD_LOGIC; 
           sel : in STD_LOGIC;
           shi : in STD_LOGIC;
           i   : in STD_LOGIC;
           o   : out STD_LOGIC;
           g   : out STD_LOGIC;
           u   : in STD_LOGIC);
end SecureReg;

architecture Behavioral of SecureReg is
    signal m : std_logic;
begin
    o <= m;
    SHIFT_PROC : process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				m <= '0';
			elsif shi = '1' and sel='1' then
				m <= i;
			else
				m <= m;
			end if;
		end if;
	end process SHIFT_PROC;
	
	UPD_PROC : process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				g <= '0';
			elsif u = '1' and sel='1' then
				g <= m;
			end if;
		end if;
	end process UPD_PROC;
end Behavioral;
