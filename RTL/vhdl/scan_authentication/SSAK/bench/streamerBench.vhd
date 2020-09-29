--------------------------------------------------------------------------------
--                                                                            --
-- File name : streamerBench.vhd                                              --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 04/11/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity streamerBench is
	generic(
		size : integer --Maximum size of the vector to be stream
	);
	port(
		clk    : in  std_logic;
		rstn   : in  std_logic;
		-- Intput bus
		S_AXIS_TVALID	: in  std_logic;
		S_AXIS_TREADY	: out std_logic;
		S_AXIS_TDATA	: in  std_logic_vector(size - 1 downto 0);
		S_AXIS_TLAST    : in  std_logic:='0';
		S_AXIS_TUSER    : in  integer;--Amount of bit to send
		--ScanChain
		SC_TD	: out std_logic;
		SC_TMS	: out std_logic;
		SC_SE	: out std_logic;
		SC_UE   : out std_logic;
		SC_CE 	: out std_logic
	);
end streamerBench;

Architecture Test of streamerBench is
begin

	process
		variable memLast : std_logic;
	begin
		if rstn = '0' then
			S_AXIS_TREADY <= '0';
			SC_TD	<= '0';
			SC_TMS	<= '0';
			SC_SE	<= '0';
			SC_UE	<= '0';
			SC_CE	<= '0';
		end if;
		wait until rising_edge(clk) and rstn='1' and S_AXIS_TVALID='1';
		SC_TMS  <= '1';
		SC_CE	<= '1';
		memLast	:= S_AXIS_TLAST;
		wait until rising_edge(clk);
		SC_CE 	<= '0';
		SC_SE  	<= '1';
		for i in 0 to S_AXIS_TUSER-1 LOOP
			SC_TD <= S_AXIS_TDATA(i);
			wait until rising_edge(clk);
		end loop;
		SC_SE 	<= '0';
		SC_UE	<= '1';
		S_AXIS_TREADY <= '1';
		wait until rising_edge(clk);
		SC_UE	<= '0';
		S_AXIS_TREADY <= '0';
		SC_TMS <= not memLast;
	end process;
end Test;

