--------------------------------------------------------------------------------
--                                                                            --
-- File name : AES128_AXIS_Wrapper.vhd                                        --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 19/08/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity AES128_AXIS_Wrapper is
	generic(
		id_size : integer;
		defaut_key_behav:std_logic);
	port(
		clk    : in  std_logic;
		rstn   : in  std_logic;
		-- Intput bus
		S_AXIS_TVALID	: in  std_logic;
		S_AXIS_TREADY	: out std_logic;
		S_AXIS_TDATA	: in  std_logic_vector(127 downto 0);
		S_AXIS_TID		: in  std_logic_vector(id_size-1 downto 0);
		-- Key
		use_last_key    : in  std_logic:=defaut_key_behav;
		Key_AXIS_TVALID	: in  std_logic;
		Key_AXIS_TREADY	: out std_logic;
		Key_AXIS_TDATA	: in  std_logic_vector(127 downto 0);
		-- Output bus
		M_AXIS_TVALID	: out std_logic;
		M_AXIS_TREADY	: in  std_logic;
		M_AXIS_TDATA	: out std_logic_vector(127 downto 0);
		M_AXIS_TID		: out std_logic_vector(id_size-1 downto 0));
end AES128_AXIS_Wrapper;

Architecture AXIStech of AES128_AXIS_Wrapper is
-------------------Déclaration-des-signaux-------------------
	signal OUTBUF_TVALID : std_logic;
	signal OUTBUF_TREADY : std_logic;
	signal OUTBUF_TDATA  : std_logic_vector(127 downto 0);
	signal OUTBUF_TID    : std_logic_vector(id_size-1 downto 0);
	--sig_out
	signal SIG_M_TVALID  : std_logic;
	signal SIG_M_TREADY  : std_logic;
	signal SIG_M_TDATA   : std_logic_vector(127 downto 0);
	signal SIG_M_TID     : std_logic_vector(id_size-1 downto 0);
	--ctrl signals
	signal start_encr    : std_logic;
	signal rst           : std_logic;
	signal newKey        : std_logic;
	signal Busy          : std_logic;
	signal lastBusy      : std_logic;
begin
	OUTBUF_TVALID <= lastBusy and not Busy;
	start_encr 	  <= OUTBUF_TREADY and S_AXIS_TVALID and (Key_AXIS_TVALID or use_last_key);
	newKey		  <= start_encr and Key_AXIS_TVALID;
	rst 		  <= not rstn;
	S_AXIS_TREADY <= start_encr;
	Key_AXIS_TREADY <= start_encr;
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				lastBusy <= '0';
				OUTBUF_TID <= (others => '0');
			else
				lastBusy <= Busy;
				if start_encr='1' then
					OUTBUF_TID <= S_AXIS_TID;
				end if;
			end if;
		end if;
	end process;
	
    M_AXIS_TVALID <= SIG_M_TVALID;
	SIG_M_TREADY <= M_AXIS_TREADY;
	M_AXIS_TDATA <= SIG_M_TDATA;
	M_AXIS_TID <= SIG_M_TID;
    oREG : entity work.AXISReg generic map(
        s_data        => 128,
        s_id          => id_size)
                               port map(
        clk           => clk,
        rstn          => rstn,
        S_AXIS_TVALID => OUTBUF_TVALID,
        S_AXIS_TREADY => OUTBUF_TREADY,
        S_AXIS_TDATA  => OUTBUF_TDATA,
        S_AXIS_TID    => OUTBUF_TID,
        M_AXIS_TVALID => SIG_M_TVALID,
        M_AXIS_TREADY => SIG_M_TREADY,
        M_AXIS_TDATA  => SIG_M_TDATA,
        M_AXIS_TID    => SIG_M_TID);
    
    wrapedAES : entity work.aes128 port map(
        Clk_CI          => clk,
        Reset_RBI       => rstn,
        Start_SI        => start_encr,
        NewCipherkey_SI => newKey,
        Busy_SO         => Busy,
        Plaintext_DI    => S_AXIS_TDATA,
        Cipherkey_DI    => Key_AXIS_TDATA,
        Ciphertext_DO   => OUTBUF_TDATA);
end AXIStech;











