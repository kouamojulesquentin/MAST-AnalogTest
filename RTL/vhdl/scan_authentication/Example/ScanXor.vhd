----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11.12.2019 15:23:27
-- Design Name: 
-- Module Name: ScanXor - Behavioral
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

entity ScanXor is
    Port ( TDI : in STD_LOGIC;
           TDO : out STD_LOGIC;
           KeyStream : in STD_LOGIC;
           Enable : in STD_LOGIC);
end ScanXor;

architecture Behavioral of ScanXor is

begin
    TDO <= TDI xor KeyStream when Enable='1' else TDI;
end Behavioral;
