--------------------------------------------------------------------------------
--                                                                            --
-- File name : StreamRegister.vhd                                             --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 01/08/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity StreamRegister is
	generic(
		size : integer);
	port(
		clk    : in  std_logic;
		rstn   : in  std_logic;
		-- Intput bus
		S_AXIS_TVALID	: in  std_logic;
		S_AXIS_TREADY	: out std_logic;
		S_AXIS_TDATA	: in  std_logic_vector(size - 1 downto 0);
		-- Scan Inter
		KeyStream : out std_logic;
		KeyStreamValid : out std_logic;
		TMS : in std_logic;
		SE : in std_logic;
		
		-- Encryption order
		en_Encr : in std_logic -- Enable the key stream encryption output
		);

end StreamRegister;

Architecture Behaviour of StreamRegister is
-------------------------Déclaration-des-signaux-------------------------
	signal KeyShiftMemory : std_logic_vector(size-1 downto 0);
	signal bitCounter     : integer range 0 to size-1;
	signal busy           : std_logic;
	signal smv            : std_logic;                    --shift mem valid
	--BufferOutput
	signal buff_TVALID    : std_logic;
	signal buff_TREADY    : std_logic;
	signal buff_TDATA     : std_logic_vector(size-1 downto 0);
begin
	busy <= '0' when bitCounter=0 else '1';
	KeyStream <= KeyShiftMemory(0) and en_Encr;
	KeyStreamValid <= smv;
	
	buff_TREADY <= not busy;
	
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				bitCounter <= 0;
				KeyShiftMemory <= (others => '0');
				smv <= '0';
			elsif bitCounter=0 then
				KeyShiftMemory <= buff_TDATA;
				smv <= buff_TVALID;
				if buff_TVALID='1' then
					bitCounter <= size-1;
				end if;
			elsif '1' = ( TMS and SE ) then
				KeyShiftMemory <= "0" & KeyShiftMemory(size-1 downto 1);
				bitCounter <= bitCounter-1;
			end if;
		end if;
	end process;
    
    BUFFin : entity work.AXISReg generic map(
        s_data        => size)
                                 port map(
        clk           => clk,
        rstn          => rstn,
        S_AXIS_TVALID => S_AXIS_TVALID,
        S_AXIS_TREADY => S_AXIS_TREADY,
        S_AXIS_TDATA  => S_AXIS_TDATA,
        M_AXIS_TVALID => buff_TVALID,
        M_AXIS_TREADY => buff_TREADY,
        M_AXIS_TDATA  => buff_TDATA);
end Behaviour;