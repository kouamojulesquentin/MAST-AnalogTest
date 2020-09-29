----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12:05 29/01/2018 
-- Design Name: 
-- Module Name: AXISmux - axistech
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
library work;
use work.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity AXISmux is
	generic(
		s_data	: integer;
		s_user	: integer:=1;
		s_dest	: integer:=1;
		synch   : boolean := false;
		auto	: boolean := true
		);
	port(		
		clk				: in std_logic;
		rstn			: in std_logic;
		cde				: in std_logic:='0';
		
		S0_AXIS_TVALID	: in  std_logic;
		S0_AXIS_TREADY	: out std_logic;
		S0_AXIS_TDATA	: in  std_logic_vector(s_data - 1 downto 0):=(others => '0');
		S0_AXIS_TUSER	: in  std_logic_vector(s_user - 1 downto 0):=(others => '0');
		S0_AXIS_TDEST	: in  std_logic_vector(s_dest - 1 downto 0):=(others => '0');
		
		S1_AXIS_TVALID	: in  std_logic;
		S1_AXIS_TREADY	: out std_logic;
		S1_AXIS_TDATA	: in  std_logic_vector(s_data - 1 downto 0):=(others => '0');
		S1_AXIS_TUSER	: in  std_logic_vector(s_user - 1 downto 0):=(others => '0');
		S1_AXIS_TDEST	: in  std_logic_vector(s_dest - 1 downto 0):=(others => '0');
		
		M_AXIS_TVALID	: out std_logic;
		M_AXIS_TREADY	: in  std_logic;
		M_AXIS_TDATA	: out std_logic_vector(s_data - 1 downto 0);
		M_AXIS_TUSER	: out std_logic_vector(s_user - 1 downto 0);
		M_AXIS_TDEST	: out std_logic_vector(s_dest - 1 downto 0));
end AXISmux;

architecture axistech of AXISmux is

-----------------Déclaration-des-signaux-----------------
	signal sI1_TVALID : std_logic;
	signal sI1_TREADY : std_logic;
	signal sI1_TDATA  : std_logic_vector(s_data-1 downto 0);
	signal sI1_TUSER  : std_logic_vector(s_user-1 downto 0);
	signal sI1_TDEST  : std_logic_vector(s_dest-1 downto 0);
	signal sI2_TVALID : std_logic;
	signal sI2_TREADY : std_logic;
	signal sI2_TDATA  : std_logic_vector(s_data-1 downto 0);
	signal sI2_TUSER  : std_logic_vector(s_user-1 downto 0);
	signal sI2_TDEST  : std_logic_vector(s_dest-1 downto 0);
begin
	COMBINATOIRE : IF not synch GENERATE
		sI1_TVALID     <= S0_AXIS_TVALID;
		S0_AXIS_TREADY <= sI1_TREADY;
		sI1_TDATA      <= S0_AXIS_TDATA;
		sI1_TUSER      <= S0_AXIS_TUSER;
		sI1_TDEST      <= S0_AXIS_TDEST;
		sI2_TVALID     <= S1_AXIS_TVALID;
		S1_AXIS_TREADY <= sI2_TREADY;
		sI2_TDATA      <= S1_AXIS_TDATA;
		sI2_TUSER      <= S1_AXIS_TUSER;
		sI2_TDEST      <= S1_AXIS_TDEST;
	END GENERATE;
	
	SEQUENTIEL : IF synch GENERATE
        
        R0 : entity work.AXISReg generic map(
            s_data        => s_data,
            s_user        => s_user,
            s_dest        => s_dest)
                     port map(
            clk           => clk,
            rstn          => rstn,
            S_AXIS_TVALID => S0_AXIS_TVALID,
            S_AXIS_TREADY => S0_AXIS_TREADY,
            S_AXIS_TDATA  => S0_AXIS_TDATA,
            S_AXIS_TUSER  => S0_AXIS_TUSER,
            S_AXIS_TDEST  => S0_AXIS_TDEST,
            M_AXIS_TVALID => sI1_TVALID,
            M_AXIS_TREADY => sI1_TREADY,
            M_AXIS_TDATA  => sI1_TDATA,
            M_AXIS_TUSER  => sI1_TUSER,
            M_AXIS_TDEST  => sI1_TDEST);
        
        R1 : entity work.AXISReg generic map(
            s_data        => s_data,
            s_user        => s_user,
            s_dest        => s_dest)
                     port map(
            clk           => clk,
            rstn          => rstn,
            S_AXIS_TVALID => S1_AXIS_TVALID,
            S_AXIS_TREADY => S1_AXIS_TREADY,
            S_AXIS_TDATA  => S1_AXIS_TDATA,
            S_AXIS_TUSER  => S1_AXIS_TUSER,
            S_AXIS_TDEST  => S1_AXIS_TDEST,
            M_AXIS_TVALID => sI2_TVALID,
            M_AXIS_TREADY => sI2_TREADY,
            M_AXIS_TDATA  => sI2_TDATA,
            M_AXIS_TUSER  => sI2_TUSER,
            M_AXIS_TDEST  => sI2_TDEST);
	END GENERATE;
	
	MODE_AUTO : IF auto GENERATE
		M_AXIS_TVALID <= 	sI1_TVALID when sI1_TVALID = '1' else
							sI2_TVALID when sI2_TVALID = '1' else '0';
		M_AXIS_TDATA <= 	sI1_TDATA when sI1_TVALID = '1' else
							sI2_TDATA when sI2_TVALID = '1' else
							(others => '0');
		M_AXIS_TUSER <= 	sI1_TUSER when sI1_TVALID = '1' else
							sI2_TUSER when sI2_TVALID = '1' else
							(others => '0');
		M_AXIS_TDEST <= 	sI1_TDEST when sI1_TVALID = '1' else
							sI2_TDEST when sI2_TVALID = '1' else 
							(others => '0');
		sI1_TREADY <= 		M_AXIS_TREADY;-- when sI1_TVALID = '1' else '0';
		sI2_TREADY <= 		M_AXIS_TREADY and not sI1_TVALID;
	END GENERATE;
	
	MODE_MANU : IF not auto GENERATE
		M_AXIS_TVALID	<= sI1_TVALID when cde = '0' else sI2_TVALID; 
		M_AXIS_TDATA	<= sI1_TDATA  when cde = '0' else sI2_TDATA;
		M_AXIS_TUSER	<= sI1_TUSER  when cde = '0' else sI2_TUSER;
		M_AXIS_TDEST	<= sI1_TDEST  when cde = '0' else sI2_TDEST;
		sI1_TREADY		<= M_AXIS_TREADY and not cde;
		sI2_TREADY		<= M_AXIS_TREADY and cde;
	END GENERATE;
end axistech;
