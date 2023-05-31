--------------------------------------------------------------------------------
--                                                                            --
-- File name : benchTopDemo.vhd                                               --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 12/12/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity benchTopDemo is
--  port(
--       Clk  : in  std_logic;
--       RSTN : in  std_logic;
--       );
end benchTopDemo;

Architecture Tester of benchTopDemo is
	type ijtagC is (Normal,Test,s0,s1,C,U);
	constant SSAK   : std_logic_vector(127 downto 0):=x"72c4358f5a8a07af3d0f7d560a872a2b";
	constant config : std_logic_vector(127 downto 0):=x"00000000000000000000000000000001";
--------------------------------------D�claration-des-signaux--------------------------------------
	signal ctrl         : ijtagC := Normal;
	signal tclk, clk    : std_logic := '0';
	signal rstn         : std_logic := '0';
	signal CSU_SE       : std_logic := '0';
	signal CSU_CE       : std_logic := '0';
	signal CSU_UE       : std_logic := '0';
	signal CSU_Select          : std_logic := '0';
	signal TDI          : std_logic := '0';
	signal TDO          : std_logic := '0';
	--	signal KS           : std_logic := '0';
	--	signal KSV          : std_logic := '0';
	signal PLAIN_TVALID : std_logic := '0';
	signal PLAIN_TREADY : std_logic := '0';
	signal PLAIN_TDATA  : std_logic_vector(127 downto 0) := x"00000000000000000000000000000000";
	signal Key_TVALID   : std_logic := '0';
	signal Key_TREADY   : std_logic := '0';
	signal Key_TDATA    : std_logic_vector(127 downto 0) := x"00000000000000000000000000000000";
	signal ENCR_TVALID  : std_logic := '0';
	signal ENCR_TREADY  : std_logic := '0';
	signal ENCR_TDATA   : std_logic_vector(127 downto 0) := x"00000000000000000000000000000000";
	signal resultat     : std_logic_vector(15 downto 0);
	signal AuthStatut   : std_logic_vector(127 downto 0);
	signal Defi         : std_logic_vector(127 downto 0);
	signal eos          : std_logic := '0';                    --end of simulation, it stops the clock

--	signal Defi_SVF   : std_logic_vector(127 downto 0):=
--	    "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101011111111100001101010110100000011111";

                    
begin
	clk <= eos or not clk after 1 ns;
	tclk <= clk;
	rstn <= '0', '1' after 10 ns;
	-------SignalMan----------------------
	CSU_Select <= '0' when ctrl=Normal else '1';
	CSU_CE <= '1' when ctrl=C  	else '0';
	CSU_SE <= '1' when ctrl=s0 	else '1' when ctrl=s1 else '0';
	CSU_UE <= '1' when ctrl=U  	else '0';
	TDI	   <= '1' when ctrl=s1	else '0';
	--------------------------------------



    process
	begin
		wait until rising_edge(clk) and rstn='0';
		ctrl <= Test;
		wait until rising_edge(clk) and rstn='1';

		---------------Ouverture du sib ssak---------
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s1;
		wait until rising_edge(clk);
		ctrl <= U;
		wait until rising_edge(clk);
		ctrl <= Test;
		wait until rising_edge(clk);
		wait until rising_edge(clk);

		-----------------Config/Defi-------------------
		wait until rising_edge(clk);
		ctrl <= C;
		wait until rising_edge(clk);
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s1;
		wait until rising_edge(clk);
		for i in 0 to 127 loop
			if config(i)='1' then
				ctrl <= s1;
			else
				ctrl <= s0;
			end if;
			wait until rising_edge(clk);
			defi(i) <= TDO;
		end loop;
		ctrl <= U;
		wait until rising_edge(clk);
		ctrl <= Test;

		-----------------Resp---------------
		Key_TDATA <= SSAK;
		Key_TVALID <= '1';
		PLAIN_TVALID	<= '1';
		PLAIN_TDATA <= defi;
--		PLAIN_TDATA <= Defi_SVF;
		wait until rising_edge(clk);
		Key_TVALID <= '0';
		PLAIN_TVALID	<= '0';
		wait until rising_edge(clk) and ENCR_TVALID='1';
		ctrl	<= C;
		wait until rising_edge(clk);
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s1;
		wait until rising_edge(clk);
		for i in 0 to 127 loop
			if ENCR_TDATA(i)='1' then
				ctrl <= s1;
			else
				ctrl <= s0;
			end if;
			wait until rising_edge(clk);
		end loop;
		ENCR_TREADY <= '1';
		ctrl <= U;

		---Check AUTH 1
		wait until rising_edge(clk);
		ctrl <= Normal;
		wait until rising_edge(clk);
		ctrl <= C;
		wait until rising_edge(clk);
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s1;
		wait until rising_edge(clk);
		ctrl <= s0;

		for i in 0 to 127 loop
		  wait until rising_edge(clk);
		  AuthStatut(i) <= TDO;
		end loop;
		ctrl <= U;
		wait until rising_edge(clk);

		---Check AUTH 2
		ctrl <= Normal;
		wait until rising_edge(clk);
		ctrl <= C;
		wait until rising_edge(clk);
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s0;
		wait until rising_edge(clk);
		ctrl <= s0;

		for i in 0 to 127 loop
		  wait until rising_edge(clk);
		  AuthStatut(i) <= TDO;
		end loop;
		ctrl <= U;
		wait until rising_edge(clk);

		-------Collect the data from 16bit secure vect--------
		---Open aSIB
		ctrl <= Normal;
		wait until rising_edge(clk);
		ctrl <= C;
		wait until rising_edge(clk);
		ctrl <= s1; --aSIB
		wait until rising_edge(clk);
		ctrl <= s0; -- SSAK_SIB
		wait until rising_edge(clk);
		ctrl <= U;
		wait until rising_edge(clk);
		---read secured data
		ctrl <= Normal;
		wait until rising_edge(clk);
		ctrl <= C;
		wait until rising_edge(clk);
		ctrl <= s0; --aSIB
		wait until rising_edge(clk);
		ctrl <= s0;
		for i in 0 to 15 loop
			wait until rising_edge(clk);
			resultat(i) <= TDO;
		end loop;
		ctrl <= s0; --SSAK_SIB
		wait until rising_edge(clk);
		ctrl <= U;
		wait until rising_edge(clk);
		ctrl <= Normal;
		wait until rising_edge(clk);
		---


		--------End-of-simulation---
		eos <= '1';
		wait;
	end process;






    DUT : entity work.topTestDesing port map(
        tclk   => tclk,
        rstn   => rstn,
        CSU_SE => CSU_SE,
        CSU_CE => CSU_CE,
        CSU_UE => CSU_UE,
        CSU_Select    => CSU_Select,
        TDI    => TDI,
        TDO    => TDO);

    CRYPTO : entity work.AES128_AXIS_Wrapper generic map(
        id_size          => 1,
        defaut_key_behav => '0')
                                             port map(
        clk              => clk,
        rstn             => rstn,
        S_AXIS_TVALID    => PLAIN_TVALID,
        S_AXIS_TREADY    => PLAIN_TREADY,
        S_AXIS_TDATA     => PLAIN_TDATA,
        S_AXIS_TID => "0",
        Key_AXIS_TVALID  => Key_TVALID,
        Key_AXIS_TREADY  => Key_TREADY,
        Key_AXIS_TDATA   => Key_TDATA,
        M_AXIS_TVALID    => ENCR_TVALID,
        M_AXIS_TREADY    => ENCR_TREADY,
        M_AXIS_TDATA     => ENCR_TDATA);

end Tester;
