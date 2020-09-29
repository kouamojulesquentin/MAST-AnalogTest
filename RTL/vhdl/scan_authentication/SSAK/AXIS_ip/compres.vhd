----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 25.04.2018 14:52:22
-- Design Name: 
-- Module Name: CompRes - Cmp
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

entity CompRes is
	Generic(
		s_cmp : integer);
    Port ( 		
		S0_AXIS_TVALID	: in  STD_LOGIC;
		S0_AXIS_TREADY	: out STD_LOGIC;
		S0_AXIS_TDATA	: in  STD_LOGIC_VECTOR(s_cmp - 1 downto 0);
		S1_AXIS_TVALID	: in  STD_LOGIC;
		S1_AXIS_TREADY	: out STD_LOGIC;
		S1_AXIS_TDATA	: in  STD_LOGIC_VECTOR(s_cmp - 1 downto 0);
		
		repValue: out std_logic_vector(s_cmp-1 downto 0); --Value of the compared response
		resV    : out STD_LOGIC; --Comparaison is finished
		res		: out STD_LOGIC);
end CompRes;

architecture Cmp of CompRes is

-----Déclaration-des-signaux-----
	signal CV  : std_logic;
--	signal CR  : std_logic;
	signal CD  : std_logic;
begin
	S0_AXIS_TREADY <= S1_AXIS_TVALID;
	S1_AXIS_TREADY <= S0_AXIS_TVALID;
	res <= CD;
	resV <= CV;
	-- S0_AXIS_TREADY <= CV;
	-- S1_AXIS_TREADY <= CV;
	
	CV <= S0_AXIS_TVALID and S1_AXIS_TVALID;
	CD <= CV when S0_AXIS_TDATA = S1_AXIS_TDATA else '0';
	repValue <= S0_AXIS_TDATA;
end Cmp;
