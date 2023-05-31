--------------------------------------------------------------------------------
--                                                                            --
-- File name : cSIB.vhd                                                       --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 08/11/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity cSIB is
	generic(
		secure : boolean:=false);
	port(
		clk    : in  std_logic;
		rstn    : in  std_logic;
		-- STD scan chain
		STD_CSU_CE : in std_logic;
		STD_CSU_SE : in std_logic;
		STD_CSU_UE : in std_logic;
		STD_CSU_Select	: in  std_logic;
		TDI : in  std_logic;
		TDO : out std_logic;
		TO_Select : out std_logic;
		TO_TDO : out std_logic;
		FROM_TDI : in std_logic;
		--Secure Scan chain;
		SEC_CSU_UE	: in  std_logic;
		SEC_CSU_SE	: in  std_logic;
		SEC_TDI		: in  std_logic;
		SEC_TDO		: out std_logic);
end cSIB;

Architecture Behaviour of cSIB is
-----D�claration-des-signaux-----
	signal std_shifted : std_logic;
	signal std_state   : std_logic;
	signal sec_shifted : std_logic;
	signal sec_state   : std_logic;
	signal mux_TDI     : std_logic;
	signal fstate      : std_logic;
	signal rst         : std_logic;
begin
    rst <= not rstn;

	TDO 	<= std_shifted;
	TO_Select	<= fstate and STD_CSU_Select;
	TO_TDO	<= TDI;
	SEC_TDO	<= sec_shifted;

	fstate	<= std_state and sec_state;
	mux_TDI	<= FROM_TDI when fstate='1' else TDI;

	process(clk)
	begin
		if rising_edge(clk) then
			std_shifted <= std_shifted;
			std_state	<= std_state;
			if rst='1' then
				std_shifted <= '0';
				std_state	<= '0';
			elsif STD_CSU_Select='1' then
				if STD_CSU_CE='1' then
					std_shifted <= std_state;
				elsif STD_CSU_UE='1' then
					std_state <= std_shifted;
				elsif STD_CSU_SE='1' then
					std_shifted <= mux_TDI;
				end if;
			end if;
		end if;
	end process;

	UNSEC_G : IF not secure GENERATE
		sec_state <= '1';
		sec_shifted <= SEC_TDI;
	END GENERATE;

	SEC_G : IF secure GENERATE
		process(clk)
		begin
			if rising_edge(clk) then
				if rst='1' then
					sec_shifted <= '0';
					sec_state   <= '0';
				elsif SEC_CSU_UE='1' then
					sec_shifted <= sec_shifted;
					sec_state	<= sec_shifted;
				elsif SEC_CSU_SE = '1' then
					sec_shifted	<= SEC_TDI;
					sec_state	<= sec_state;
				end if;
			end if;
		end process;
	END GENERATE;
end Behaviour;
