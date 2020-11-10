----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11.12.2019 18:11:04
-- Design Name: 
-- Module Name: scan_secure - Behavioral
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

entity scan_secure is
	Port (
		tclk : in std_logic;
		rstn : in std_logic;
               SysResetn: in std_logic; --System Level Reset for Trivium Initialization
              Trivium_ready : out std_logic;
		
		CSU_SE : in  std_logic;
		CSU_CE : in  std_logic;
		CSU_UE : in  std_logic;
		CSU_Select : in  std_logic;
		
		TDI : in  std_logic;
		TDO : out std_logic
		
	);
end scan_secure;

architecture Behavioral of scan_secure is
constant size : integer :=128;
-------------------------------Déclaration-des-signaux-------------------------------
	--Plain_AXIS
	signal Plain_AXIS_TVALID : std_logic;
	signal Plain_AXIS_TREADY : std_logic;
	signal Plain_AXIS_TDATA  : std_logic_vector(size-1 downto 0);
	--Key_AXIS
	signal Key_AXIS_TVALID   : std_logic;
	signal Key_AXIS_TREADY   : std_logic;
	signal Key_AXIS_TDATA    : std_logic_vector(size-1 downto 0);
	-- --IV_AXIS
	-- signal IV_AXIS_TVALID    : std_logic;
	-- signal IV_AXIS_TREADY    : std_logic;
	-- signal IV_AXIS_TDATA     : std_logic_vector(size-1 downto 0);
	--ENCR_AXIS
	signal ENCR_AXIS_TVALID  : std_logic;
	signal ENCR_AXIS_TREADY  : std_logic;
	signal ENCR_AXIS_TDATA   : std_logic_vector(size-1 downto 0);
	----
	signal SSAK_Select          : std_logic;
	signal SSAK_TDI          : std_logic;
	signal SSAK_TDO          : std_logic;
	signal a_CSU_Select             : std_logic;
	signal a_TDI             : std_logic;
	signal a_TDO             : std_logic;
	signal TDm               : std_logic;
	signal SEC_TDI           : std_logic;
	signal SEC_SE            : std_logic;
	signal SEC_UE            : std_logic;
	signal RNV               : std_logic_vector(size-1 downto 0);    --RandomNumberVector
--signal enableEncryption  : std_logic;
	signal PDI               : std_logic_vector(15 downto 0):=x"0123";
	-- -----ENCRTDAT
	signal PD0               : std_logic_vector(15 downto 0);
	-- signal plain_TDI         : std_logic;
	-- signal plain_TDO         : std_logic;
	-- signal KeySTR            : std_logic;
begin
 Trivium_ready <= '1';
 
    PDI <= x"0123";
    
    SIB_ssak : entity work.cSIB generic map(
        secure     => false)
                                port map(
        clk        => tclk,
        rstn       => rstn,
        STD_CSU_CE => CSU_CE,
        STD_CSU_SE => CSU_SE,
        STD_CSU_UE => CSU_UE,
        STD_CSU_Select        => CSU_Select,
        TDI        => TDI,
        TDO        => TDm,
        TO_Select     => SSAK_Select,
        TO_TDO     => SSAK_TDI,
        FROM_TDI   => SSAK_TDO,
        SEC_CSU_UE => '0',
        SEC_CSU_SE => '0',
        SEC_TDI    => '0');
    
    CONTROLLER : entity work.SSAK generic map(
        s_Interface                    => size,
        s_SSC                          => 1,
        RNG_fx                         => false,
        RNG_lfsr                       => false,
        RNG_ext                        => true,
        enableScanEncryptionManagement => false)
                                  port map(
        tclk                           => tclk,
        rst_n                          => rstn,
        SI_TMS                         => SSAK_Select,
        SI_TD                          => SSAK_TDI,
        SI_update                      => CSU_UE,
        SI_shift                       => CSU_SE,
        SI_capture                     => CSU_CE,
        SO_TD                          => SSAK_TDO,
        SSO_TD                         => SEC_TDI,
        SSO_update                     => SEC_UE,
        SSO_shift                      => SEC_SE,
        RNG_port                       => RNV,
        CRY_KEY_AXIS_TVALID            => KEY_AXIS_TVALID,
        CRY_KEY_AXIS_TREADY            => KEY_AXIS_TREADY,
        CRY_KEY_AXIS_TDATA             => KEY_AXIS_TDATA,
        CRY_PLAIN_AXIS_TVALID          => PLAIN_AXIS_TVALID,
        CRY_PLAIN_AXIS_TREADY          => PLAIN_AXIS_TREADY,
        CRY_PLAIN_AXIS_TDATA           => PLAIN_AXIS_TDATA,
        CRY_ENC_AXIS_TVALID            => ENCR_AXIS_TVALID,
        CRY_ENC_AXIS_TREADY            => ENCR_AXIS_TREADY,
        CRY_ENC_AXIS_TDATA             => ENCR_AXIS_TDATA);
    
    AESnSTR : entity work.AES128_AXIS_Wrapper generic map(
        id_size          => 1,
        defaut_key_behav => '0')
                                              port map(
        clk              => tclk,
        rstn             => rstn,
        S_AXIS_TVALID    => Plain_AXIS_TVALID,
        S_AXIS_TREADY    => Plain_AXIS_TREADY,
        S_AXIS_TDATA     => Plain_AXIS_TDATA,
        S_AXIS_TID       => "0",
        use_last_key     => '0',
        Key_AXIS_TVALID  => Key_AXIS_TVALID,
        Key_AXIS_TREADY  => Key_AXIS_TREADY,
        Key_AXIS_TDATA   => Key_AXIS_TDATA,
        M_AXIS_TVALID    => ENCR_AXIS_TVALID,
        M_AXIS_TREADY    => ENCR_AXIS_TREADY,
        M_AXIS_TDATA     => ENCR_AXIS_TDATA);
    
    aSIB : entity work.cSIB generic map(
        secure     => true)
                            port map(
        clk        => tclk,
        rstn       => rstn,
        STD_CSU_CE => CSU_CE,
        STD_CSU_SE => CSU_SE,
        STD_CSU_UE => CSU_UE,
        STD_CSU_Select => CSU_Select,
        TDI        => TDm,
        TDO        => TDO,
        TO_Select     => a_CSU_Select,
        TO_TDO     => a_TDI,
        FROM_TDI   => a_TDO,
        SEC_CSU_UE => SEC_UE,
        SEC_CSU_SE => SEC_SE,
        SEC_TDI    => SEC_TDI);
    
    A : entity work.ScanSegment generic map(
        size   => 16)
                                port map(
        clk    => tclk,
        rstn   => rstn,
        CSU_SE => CSU_SE,
        CSU_UE => CSU_UE,
        CSU_CE => CSU_CE,
        CSU_Select    => a_CSU_Select,
        TDI    => a_TDI,
        TDO    => a_TDO,
        PDI    => PDI);
    
    TRNG : entity work.PRNG_gest port map(
        clk     => tclk,
        rstn    => rstn,
        RandVal => RNV);
end Behavioral;
