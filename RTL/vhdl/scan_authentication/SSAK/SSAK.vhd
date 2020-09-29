----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date: 23.04.2018 11:03:30
-- Design Name:
-- Module Name: AuthCont - RVL
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
 library work;
 use work.data_types.all;
 use work.fonctions.ALL;

entity SSAK is
	Generic(
		s_Interface : integer := 256; --Size of interface, SSAK, Challenge and response
		s_SSC : integer := 16; -- Lengh of the secure scan chain
--		s_UcdeMem : integer := 32;
		RNG_fx : boolean := true;--Constant Value
		RNG_lfsr : boolean := false;--LFSR generation
		RNG_ext : boolean := false;--External generator
		imp_FH : boolean := false;
		imp_SHA256 : boolean := true;
		enableScanEncryptionManagement : boolean := false--Generate initialisation vector for scan encryption.
	);
    Port (
		tclk : in STD_LOGIC;
		rst_n : in STD_LOGIC;
		--
		SI_TMS : in  std_logic;
		SI_TD : in  std_logic;
		SI_update : in  std_logic;
		SI_shift : in  std_logic;
		SI_capture : in  std_logic;
		--
--		SO_TMS : out std_logic;
		SO_TD : out std_logic;
--		SO_update : out std_logic;
--		SO_shift : out std_logic;
--		SO_capture : out std_logic;
		--
		SSO_TMS : out std_logic;
		SSO_TD : out std_logic;
		SSO_update : out std_logic;
		SSO_shift : out std_logic;
		--
		RNG_port : in STD_LOGIC_VECTOR(s_Interface-1 downto 0) := (others => '0');
		--
		authorizedSsib : out std_logic;
		-----Cryptoprocessor-Interface-------
		CRY_KEY_AXIS_TVALID : out  std_logic;
		CRY_KEY_AXIS_TREADY : in   std_logic;
		CRY_KEY_AXIS_TDATA  : out  std_logic_vector(s_Interface-1 downto 0);
		--
		CRY_PLAIN_AXIS_TVALID : out  std_logic;
		CRY_PLAIN_AXIS_TREADY : in   std_logic;
		CRY_PLAIN_AXIS_TDATA  : out  std_logic_vector(s_Interface-1 downto 0);
		--
		CRY_ENC_AXIS_TVALID : in   std_logic;
		CRY_ENC_AXIS_TREADY : out  std_logic;
		CRY_ENC_AXIS_TDATA  : in   std_logic_vector(s_Interface-1 downto 0);
		--
		CRY_IV_AXIS_TVALID : out  std_logic;
		CRY_IV_AXIS_TREADY : in   std_logic:='0';
		CRY_IV_AXIS_TDATA  : out  std_logic_vector(s_Interface-1 downto 0)
		--
--		BL_id : in  STD_LOGIC_VECTOR(s_cdeUser-1 downto 0);
--		BL_iv : in  STD_LOGIC;
--		BL_od : out STD_LOGIC_VECTOR(s_cdeUser-1 downto 0);
--		BL_ov : out STD_LOGIC;
--		BL_oa : out STD_LOGIC_VECTOR(addrBL - 1 downto 0)--(log2(s_UcdeMem)-1 downto 0)
		);
end SSAK;

architecture Controller of SSAK is
	constant nulSvec         : std_logic_vector(s_Interface-3 downto 0):=(others=> '0');
-----------------------------------D�claration-des-signaux-----------------------------------
	signal memConfigV            : std_logic;
	signal memConfigR            : std_logic;
	signal memConfigD            : std_logic_vector(s_Interface-1 downto 0);
	-----Defi-----
	signal DefiV                 : std_logic;
	signal DefiR                 : std_logic;
	signal DefiD                 : std_logic_vector(s_Interface-1 downto 0);
	-----Mux0-----
	signal Mux0V                 : std_logic;
	signal Mux0R                 : std_logic;
	signal Mux0D                 : std_logic_vector(s_Interface-1 downto 0);
	signal Mux1V                 : std_logic;
	signal Mux1R                 : std_logic;
	signal Mux1D                 : std_logic_vector(s_Interface-1 downto 0);
	----Merged----
	signal MergedV               : std_logic;
	signal MergedR               : std_logic;
	signal MergedD               : std_logic_vector(s_Interface*2-1 downto 0);
	-----SHA-----
	signal SHAV                  : std_logic;
	signal SHAR                  : std_logic;
	signal SHAD                  : std_logic_vector(s_Interface-1 downto 0);
	---LocalKey---
	signal LocalKeyV             : std_logic;
	signal LocalKeyR             : std_logic;
	signal LocalKeyD             : std_logic_vector(s_Interface-1 downto 0);
	---ConfKey---
	signal ConfKeyV              : std_logic;
	signal ConfKeyR              : std_logic;
	signal ConfKeyD              : std_logic_vector(s_Interface-1 downto 0);
	----Update----
	signal UpdateV               : std_logic;
	signal UpdateR               : std_logic;
	signal UpdateD               : std_logic_vector(s_Interface-1 downto 0);
	----dmux0----
	signal dmux0v                : std_logic;
	signal dmux0r                : std_logic;
	signal dmux0d                : std_logic_vector(s_Interface-1 downto 0);
	----dmux1----
	signal dmux1v                : std_logic;
	signal dmux1r                : std_logic;
	signal dmux1d                : std_logic_vector(s_Interface-1 downto 0);
	-----ResI-----
	signal ResId                 : std_logic;
	signal ResIv                 : std_logic;
	---CdeUser---
	signal CdeUserV              : std_logic;
	signal CdeUserR              : std_logic;
	--signal CdeUserD          : std_logic_vector(s_cdeUser-1 downto 0);
	--------- Signaux de contr�le -------
	signal muxCde                : std_logic;
	signal DefG                  : std_logic;
	signal defGFA                : std_logic;
	signal confSave              : std_logic;
	signal shiftOrder            : std_logic;
	signal TRN                   : std_logic_vector(s_Interface-1 downto 0);
	signal compare               : std_logic;
	signal rstn                  : std_logic;
	signal rst                   : std_logic;
	signal ParaFrIt              : std_logic_vector(s_Interface-1 downto 0);
	signal ParaTiIt              : std_logic_vector(s_Interface-1 downto 0);
	signal Update                : std_logic;
	signal sSelect               : std_logic;
	--signal writeItr        : std_logic;
	--signal extrCdeUser       : std_logic_vector(s_cdeUser-1 downto 0);
	signal BlackList_check       : std_logic;
	signal BlackList_present     : std_logic;
	signal ACKmessage            : std_logic_vector(s_Interface-1 downto 0);
	signal sucess                : std_logic;                    --Sucess of the authentication
	signal failed                : std_logic;                    --AUTH Failed
	signal SecureUpdatePermition : std_logic;
	signal rstSSC                : std_logic;
	--initialisation vector signals
	signal s_iv_tvalid           : std_logic;
	signal s_iv_tready           : std_logic;
	signal s_iv_tdata            : std_logic_vector(s_Interface-1 downto 0);
	----
	signal configNull            : std_logic;
begin
	defGFA <= DefG and SI_capture;
    process(UpdateD)
		variable tmpConfNull : std_logic;
	begin
		tmpConfNull := '1';
		for i in 0 to s_SSC-1 loop
			if UpdateD(i)='1' then
				tmpConfNull := '0';
			else
				tmpConfNull := tmpConfNull;
			end if;
		end loop;
		configNull <= tmpConfNull;
	end process;


    rst <= not rst_n;
--- Interface Driver ----------
	process(tclk)
	begin
		if rising_edge(tclk) then
			if rstn = '0' then
				UpdateV <= '0';
				UpdateD <= (others => '0');
			elsif Update = '1' then
				UpdateV <= '1';
				UpdateD <= ParaFrIt;
			elsif UpdateR = '1' then
				UpdateV <= '0';
			end if;
		end if;
	end process;
	sSelect <= SI_TMS;

--	ParaTiIt <= TRN when (failed='0') and ('0'= sucess) else ACKmessage;
	ParaTiIt <= DefiD when (failed='0') and ('0'= sucess) else ACKmessage;

	ACKmessage	<= nulSvec & failed & sucess;

	UpdateR <= confSave or compare;
------------------------------



--- SSO ------------
SSO_TMS <= SI_TMS;
---------------------
    BlackList_check <='0';
	BlackList_present <= '0';
    MAE : entity work.FSM2 port map(
        clk        => tclk,
        rst_n      => rst_n,
        defV       => DefiV,
        updateV    => UpdateV,
        confNull   => configNull,
        ssakV      => ConfKeyV,
        SHAv       => dmux1v,
        rCheck     => ResId,
        capture    => SI_capture,
        sel        => SI_TMS,
        muxCde     => muxCde,
        SSC_shift  => shiftOrder,
        SSC_update => SecureUpdatePermition,
        GeneDef    => DefG,
        rstSSC     => rstSSC,
        compare    => compare,
        confSave   => confSave,
        sucess     => sucess,
        failed     => failed,
        rstn       => rstn);

    MUX0 : entity work.AXISmux generic map(
        s_data         => s_Interface,
        auto           => false)
                   port map(
        clk            => tclk,
        rstn           => rstn,
        cde            => muxCde,
        S0_AXIS_TVALID => LocalKeyV,
        S0_AXIS_TREADY => LocalKeyR,
        S0_AXIS_TDATA  => LocalKeyD,
        S1_AXIS_TVALID => ConfKeyV,
        S1_AXIS_TREADY => ConfKeyR,
        S1_AXIS_TDATA  => ConfKeyD,
        M_AXIS_TVALID  => Mux0V,
        M_AXIS_TREADY  => Mux0R,
        M_AXIS_TDATA   => Mux0D);

    MUX1 : entity work.AXISmux generic map(
        s_data         => s_Interface,
        auto           => false)
                   port map(
        clk            => tclk,
        rstn           => rstn,
        cde            => muxCde,
        S0_AXIS_TVALID => memConfigV,
        S0_AXIS_TREADY => memConfigR,
        S0_AXIS_TDATA  => memConfigD,
        S1_AXIS_TVALID => DefiV,
        S1_AXIS_TREADY => DefiR,
        S1_AXIS_TDATA  => DefiD,
        M_AXIS_TVALID  => Mux1V,
        M_AXIS_TREADY  => Mux1R,
        M_AXIS_TDATA   => Mux1D);

  -- Connection for external crypto function
	NO_IV_GEN : IF not enableScanEncryptionManagement GENERATE
		CRY_KEY_AXIS_TVALID <= Mux0V;
		Mux0R <= CRY_KEY_AXIS_TREADY;
		CRY_KEY_AXIS_TDATA  <= Mux0D;
	END GENERATE;

	CRY_PLAIN_AXIS_TVALID <= Mux1V;
	Mux1R <= CRY_PLAIN_AXIS_TREADY;
	CRY_PLAIN_AXIS_TDATA  <= Mux1D;

	SHAV <= CRY_ENC_AXIS_TVALID;
	CRY_ENC_AXIS_TREADY <= SHAR;
	SHAD <= CRY_ENC_AXIS_TDATA;
----------------------------------------

    -- MERGE0 : entity work.AXIS_MERGE generic map(
        -- size1 => s_Interface,
        -- size2 => s_Interface)
                        -- port map(
        -- I1D   => Mux0D,
        -- I1V   => Mux0V,
        -- I1R   => Mux0R,
        -- I2D   => Mux1D,
        -- I2V   => Mux1V,
        -- I2R   => Mux1R,
        -- O1D   => MergedD,
        -- O1V   => MergedV,
        -- O1R   => MergedR);

	-- GENE_FAKE_HASH : IF imp_FH GENERATE
		-- SHA0 : entity work.FakeHash generic map(
			-- N             => s_Interface)
						-- port map(
			-- clk           => tclk,
			-- rstn          => rstn,
			-- S_AXIS_TVALID => MergedV,
			-- S_AXIS_TREADY => MergedR,
			-- S_AXIS_TDATA  => MergedD,
			-- M_AXIS_TVALID => SHAV,
			-- M_AXIS_TREADY => SHAR,
			-- M_AXIS_TDATA  => SHAD);
	-- END GENERATE;

	-- GENE_SHA256 : IF imp_SHA256 GENERATE

        -- SHA1 : entity work.AXIS_SHA port map(
            -- clk           => tclk,
            -- rstn          => rstn,
            -- S_AXIS_TVALID => MergedV,
            -- S_AXIS_TREADY => MergedR,
            -- S_AXIS_TDATA  => MergedD,
            -- M_AXIS_TVALID => SHAV,
            -- M_AXIS_TREADY => SHAR,
            -- M_AXIS_TDATA  => SHAD);
	-- END GENERATE;

    RLK : entity work.AXISReg generic map(
        s_data        => s_Interface)
                  port map(
        clk           => tclk,
        rstn          => rstn,
        S_AXIS_TVALID => dmux0v,
        S_AXIS_TREADY => dmux0r,
        S_AXIS_TDATA  => dmux0d,
        M_AXIS_TVALID => ConfKeyV,
        M_AXIS_TREADY => ConfKeyR,
        M_AXIS_TDATA  => ConfKeyD);

	MODE_RNG_FIX : IF RNG_fx GENERATE
        TNRG_0 : entity work.TNRGfx generic map(
            size => s_Interface)
                                    port map(
            mem  => TRN);
    END GENERATE;

	MODE_RNG_LFSR : IF RNG_lfsr GENERATE

--        LFSR : entity work.LFSR_RNG generic map(
--            size   => s_Interface,
--            nbCoef => 5,
--            coefs  => (0,1,8,11,22,0,0,0,0,0),
--            ini    => 1)
--                        port map(
--            clk    => tclk,
--            rstn   => rstn,
--            RN     => TRN);
	END GENERATE;

	MODE_RNG_EXT : IF RNG_ext GENERATE
		TRN	<= RNG_port;
	END GENERATE;

	IV_GEN : IF enableScanEncryptionManagement GENERATE
		signal memOfSSAK : std_logic_vector(s_Interface-1 downto 0);
		signal IVvalid : std_logic;
	begin
		process(tclk)
		begin
			if rising_edge(tclk) then
				if rstn='0' then
					memOfSSAK <= (others=>'-');
				elsif ConfKeyV='1' then
					memOfSSAK <= ConfKeyD;
				end if;
			end if;
		end process;

        REG_IV : entity work.AXISReg generic map(
            skipIfEmpty   => true,
            s_data        => s_Interface)
                                     port map(
            clk           => tclk,
            rstn          => rstn,
            S_AXIS_TVALID => s_iv_tvalid,
            S_AXIS_TREADY => s_iv_tready,
            S_AXIS_TDATA  => s_iv_tdata,
            M_AXIS_TVALID => IVvalid,
            M_AXIS_TREADY => CRY_IV_AXIS_TREADY,
            M_AXIS_TDATA  => CRY_IV_AXIS_TDATA);
		CRY_IV_AXIS_TVALID <= IVvalid;

        MUX2 : entity work.AXISmux generic map(
            s_data         => s_Interface,
            auto           => true)
                                   port map(
            clk            => tclk,
            rstn           => rstn,
            S0_AXIS_TVALID => Mux0V,
            S0_AXIS_TREADY => Mux0R,
            S0_AXIS_TDATA  => Mux0D,
            S1_AXIS_TVALID => IVvalid,
            S1_AXIS_TDATA  => memOfSSAK,
            M_AXIS_TVALID  => CRY_KEY_AXIS_TVALID,
            M_AXIS_TREADY  => CRY_KEY_AXIS_TREADY,
            M_AXIS_TDATA   => CRY_KEY_AXIS_TDATA);
	END GENERATE;

    L_key_0 : entity work.keyROM generic map(
        key_size => s_Interface)
                                 port map(
        clk      => tclk,
        rstn     => rstn,
        mem      => LocalKeyD,
        memValid => LocalKeyV,
        memReady => LocalKeyR);

    DriverSSC : entity work.SSCdriver generic map(
        s_Interface  => s_Interface,
        s_SSC        => s_SSC)
                                      port map(
        clk          => tclk,
        rstn         => rstn,
        Di           => UpdateD,
        save         => confSave,
        shift        => shiftOrder,
        removeAuth   => rstSSC,
        imageData    => memConfigD,
        imageValid   => memConfigV,
        imageReady   => memConfigR,
        updateAuth   => SecureUpdatePermition,
        ssibUnlocked => authorizedSsib,
        SSS          => SSO_shift,
        SSU          => SSO_update,
        SSO          => SSO_TD,
        SSI          => '0');

    RDef : entity work.AXISReg generic map(
        s_data        => s_Interface)
                               port map(
        clk           => tclk,
        rstn          => rstn,
        S_AXIS_TVALID => defGFA,
        S_AXIS_TDATA  => TRN,
        M_AXIS_TVALID => DefiV,
        M_AXIS_TREADY => DefiR,
        M_AXIS_TDATA  => DefiD);

    DMUX0 : entity work.AXISdemux generic map(
        s_data          => s_Interface)
                      port map(
        clk             => tclk,
        rstn            => rstn,
        S_AXIS_TVALID   => SHAV,
        S_AXIS_TREADY   => SHAR,
        S_AXIS_TDATA    => SHAD,
        S_AXIS_TDEST(0) => muxCde,
        M0_AXIS_TVALID  => dmux0v,
        M0_AXIS_TREADY  => dmux0r,
        M0_AXIS_TDATA   => dmux0d,
        M1_AXIS_TVALID  => dmux1v,
        M1_AXIS_TREADY  => dmux1r,
        M1_AXIS_TDATA   => dmux1d);

    CMP : entity work.CompRes generic map(
        s_cmp          => s_Interface)
                              port map(
        S0_AXIS_TVALID => dmux1v,
        S0_AXIS_TREADY => dmux1r,
        S0_AXIS_TDATA  => dmux1d,
        S1_AXIS_TVALID => compare,
        S1_AXIS_TDATA  => UpdateD,
        repValue       => s_iv_tdata,
        resV           => s_iv_tvalid,
        res            => ResId);

    Interface_0 : entity work.Interface generic map(
        s_segment  => s_Interface)
                                        port map(
        rstn       => rstn,
        TCLK       => tclk,
        SI_TMS     => SI_TMS,
        SI_TD      => SI_TD,
        SI_update  => SI_update,
        SI_shift   => SI_shift,
        SI_capture => SI_capture,
        SO_TD      => SO_TD,
        Pin        => ParaTiIt,
        Pout       => ParaFrIt,
        Uout       => Update);

end Controller;


