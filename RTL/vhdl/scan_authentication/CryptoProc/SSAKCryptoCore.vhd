--------------------------------------------------------------------------------
--                                                                            --
-- File name : SSAKCryptoCore.vhd                                             --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 09/12/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity SSAKCryptoCore is
	generic(
		size : integer:=128);--Size of cryptoprocessor vectors
	port(
		clk    : in  std_logic;
		rstn   : in  std_logic	;	
		--
		Plain_TVALID : in   std_logic;
		Plain_TREADY : out  std_logic;
		Plain_TDATA  : in   std_logic_vector(size-1 downto 0);
		--
		Key_TVALID : in   std_logic;
		Key_TREADY : out  std_logic;
		Key_TDATA  : in   std_logic_vector(size-1 downto 0);
		--
		IV_TVALID : in   std_logic;
		IV_TREADY : out  std_logic;
		IV_TDATA  : in   std_logic_vector(size-1 downto 0);
		--
		ENCR_TVALID : out  std_logic;
		ENCR_TREADY : in   std_logic;
		ENCR_TDATA  : out  std_logic_vector(size-1 downto 0);
		--
--		CSU_CE : in  std_logic;
		CSU_SE : in  std_logic;
--		CSU_UE : in  std_logic;
		KeyStr : out std_logic;
		KeyStrV: out std_logic;
		TMS	   : in  std_logic;
		--
		Encryption_mode : in std_logic);
end SSAKCryptoCore;

Architecture CryCore of SSAKCryptoCore is
	type etats is(INIT,START_CRY,WFOR_CRY,RET_CRY,START_STR,WFOR_STR,RET_STR,LOOP_STR);
--------------------------Déclaration-des-signaux--------------------------
	signal etat_c         : etats;
	signal etat_s         : etats;
	--aesP AXIS
	signal aesP_TVALID    : std_logic;
	signal aesP_TREADY    : std_logic;
	signal aesP_TDATA     : std_logic_vector(size-1 downto 0);
	--aesK AXIS
	signal aesK_TVALID    : std_logic;
	signal aesK_TREADY    : std_logic;
	signal aesK_TDATA     : std_logic_vector(size-1 downto 0);
	--aesE AXIS
	signal aesE_TVALID    : std_logic;
	signal aesE_TREADY    : std_logic;
	signal aesE_TDATA     : std_logic_vector(size-1 downto 0);
	--bitStr AXIS
	signal bitStr_TVALID  : std_logic;
	signal bitStr_TREADY  : std_logic;
	signal bitStr_TDATA   : std_logic_vector(size-1 downto 0);
	--FSM signals.
	signal start_cond_cry : std_logic;
	signal start_cond_str : std_logic;
	signal keepKey        : std_logic;
	signal resMemory      : std_logic_vector(size-1 downto 0);
	--buffedP AXIS
	signal buffedP_TVALID : std_logic;
	signal buffedP_TREADY : std_logic;
	signal buffedP_TDATA  : std_logic_vector(size-1 downto 0);
	signal buffedKeepKey  : std_logic;                    --KeepThePreviousKey
	--buffedK AXIS
	signal buffedK_TVALID : std_logic;
	signal buffedK_TREADY : std_logic;
	signal buffedK_TDATA  : std_logic_vector(size-1 downto 0);
begin
	start_cond_cry <= Plain_TVALID and aesP_TREADY and Key_TVALID and aesK_TREADY;
	start_cond_str <= IV_TVALID and aesP_TREADY and Key_TVALID and aesK_TREADY and Encryption_mode;
	ENCR_TDATA	   <= aesE_TDATA;
	aesK_TDATA	   <= Key_TDATA;
	bitStr_TDATA   <= aesE_TDATA;
	
    cProc : entity work.AES128_AXIS_Wrapper generic map(
        id_size          => 1,
        defaut_key_behav => '0')
                                            port map(
        clk              => clk,
        rstn             => rstn,
        S_AXIS_TVALID    => buffedP_TVALID,
        S_AXIS_TREADY    => buffedP_TREADY,
        S_AXIS_TDATA     => buffedP_TDATA,
        S_AXIS_TID       => "0",
        use_last_key     => buffedKeepKey,
        Key_AXIS_TVALID  => buffedK_TVALID,
        Key_AXIS_TREADY  => buffedK_TREADY,
        Key_AXIS_TDATA   => buffedK_TDATA,
        M_AXIS_TVALID    => aesE_TVALID,
        M_AXIS_TREADY    => aesE_TREADY,
        M_AXIS_TDATA     => aesE_TDATA);
	
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				resMemory<=(others=>'0');
			elsif bitStr_TVALID='1' then
				resMemory <= aesE_TDATA;
			end if;
		end if;
	end process;
	
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				etat_c <= INIT;
			else
				etat_c <= etat_s;
			end if;
		end if;
	end process;
	
	process(start_cond_str,start_cond_cry,Plain_TDATA,ENCR_TREADY,IV_TDATA,Encryption_mode,aesP_TREADY,resMemory,etat_c,aesE_TVALID,bitStr_TREADY)
	begin
		case etat_c is
		-------------MACHINE-A-ETAT-----------------

		-----------------------Case INIT-----------------------
		when INIT =>
			if start_cond_cry='1' then
				etat_s <= START_CRY;
			elsif start_cond_str='1' then
				etat_s <= START_STR;
			else
				etat_s <= INIT;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '0';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= (others=>'-');
			keepKey <= '0';
			aesK_TVALID <= '0';
		-------------------------------------------------------

		---------Case START_CRY---------
		when START_CRY =>
			etat_s <= WFOR_CRY;
			Plain_TREADY <= '1';
			Key_TREADY <= '1';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '1';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= Plain_TDATA;
			keepKey <= '0';
			aesK_TVALID <= '1';
		--------------------------------

		---------------------Case WFOR_CRY---------------------
		when WFOR_CRY =>
			if aesE_TVALID='1' then
				etat_s <= RET_CRY;
			else
				etat_s <= WFOR_CRY;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '0';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= (others=>'-');
			keepKey <= '0';
			aesK_TVALID <= '0';
		-------------------------------------------------------

		----------Case RET_CRY----------
		when RET_CRY =>
			if ENCR_TREADY='1' then
				etat_s <= INIT;
			else
				etat_s <= RET_CRY;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '1';
			aesP_TVALID <= '0';
			aesE_TREADY <= ENCR_TREADY;
			bitStr_TVALID <= '0';
			aesP_TDATA <= (others=>'-');
			keepKey <= '0';
			aesK_TVALID <= '0';
		--------------------------------

		------Case START_STR------
		when START_STR =>
			etat_s <= WFOR_STR;
			Plain_TREADY <= '0';
			Key_TREADY <= '1';
			IV_TREADY <= '1';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '1';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= IV_TDATA;
			keepKey <= '0';
			aesK_TVALID <= '1';
		--------------------------

		----------Case WFOR_STR----------
		when WFOR_STR =>
			if aesE_TVALID='1' then
				etat_s <= RET_STR;
			else
				etat_s <= WFOR_STR;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '0';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= (others=>'-');
			keepKey <= '0';
			aesK_TVALID <= '0';
		---------------------------------

		---------------------Case RET_STR---------------------
		when RET_STR =>
			if Encryption_mode='0' then
				etat_s <= INIT;
			elsif bitStr_TREADY='1' then
				etat_s <= LOOP_STR;
			else
				etat_s <= RET_STR;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '0';
			aesE_TREADY <= bitStr_TREADY;
			bitStr_TVALID <= '1';
			aesP_TDATA <= (others=>'-');
			keepKey <= '0';
			aesK_TVALID <= '0';
		------------------------------------------------------

		--------------------Case LOOP_STR--------------------
		when LOOP_STR =>
			if Encryption_mode='0' then
				etat_s <= INIT;
			elsif aesP_TREADY='1' then
				etat_s <= WFOR_STR;
			else
				etat_s <= LOOP_STR;
			end if;
			Plain_TREADY <= '0';
			Key_TREADY <= '0';
			IV_TREADY <= '0';
			ENCR_TVALID <= '0';
			aesP_TVALID <= '1';
			aesE_TREADY <= '0';
			bitStr_TVALID <= '0';
			aesP_TDATA <= resMemory;
			keepKey <= '1';
			aesK_TVALID <= '0';
		-----------------------------------------------------

		---------FIN-MACHINE-A-ETAT-----------------
		end case;
	end process;
	
    
    SKG : entity work.StreamRegister generic map(
        size           => size)
                                     port map(
        clk            => clk,
        rstn           => rstn,
        S_AXIS_TVALID  => bitStr_TVALID,
        S_AXIS_TREADY  => bitStr_TREADY,
        S_AXIS_TDATA   => bitStr_TDATA,
        KeyStream      => KeyStr,
        KeyStreamValid => KeyStrV,
        TMS            => TMS,
        SE             => CSU_SE,
        en_Encr        => Encryption_mode);
    
    REG_PLAIN : entity work.AXISReg generic map(
        s_data        => size)
                                    port map(
        clk           => clk,
        rstn          => rstn,
        S_AXIS_TVALID => aesP_TVALID,
        S_AXIS_TREADY => aesP_TREADY,
        S_AXIS_TDATA  => aesP_TDATA,
        S_AXIS_TKEEP  => keepKey,
        M_AXIS_TVALID => buffedP_TVALID,
        M_AXIS_TREADY => buffedP_TREADY,
        M_AXIS_TDATA  => buffedP_TDATA,
        M_AXIS_TKEEP  => buffedKeepKey);
    
    REG_KEY : entity work.AXISReg generic map(
        s_data        => size)
                                  port map(
        clk           => clk,
        rstn          => rstn,
        S_AXIS_TVALID => aesK_TVALID,
        S_AXIS_TREADY => aesK_TREADY,
        S_AXIS_TDATA  => aesK_TDATA,
        M_AXIS_TVALID => buffedK_TVALID,
        M_AXIS_TREADY => buffedK_TREADY,
        M_AXIS_TDATA  => buffedK_TDATA);
end CryCore;






















