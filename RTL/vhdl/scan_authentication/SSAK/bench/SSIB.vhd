----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.04.2018 14:54:44
-- Design Name: 
-- Module Name: SSIB - Behavioral
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

entity SSIB is
	Generic(
	    debug : boolean := false;
		secured : boolean := false);
    Port ( 
        tclk : in STD_LOGIC;
		rstn : in STD_LOGIC;
--		SI_TCLK : in  std_logic;
		SI_TMS : in  std_logic;
		SI_TD : in  std_logic;
		SI_update : in  std_logic;
		SI_shift : in  std_logic;
		SI_capture : in  std_logic;
--		SO_TCLK : out std_logic;
		SO_TMS : out std_logic;
		SO_TD : out std_logic;
		SO_update : out std_logic;
		SO_shift : out std_logic;
		SO_capture : out std_logic;
--		TO_TCLK : out std_logic;
		TO_TMS : out std_logic;
		TO_TD : out std_logic;
		TO_update : out std_logic;
		TO_shift : out std_logic;
		TO_capture : out std_logic;
		FROM_TD : in std_logic;

		   --Secure part
--		SSI_TCLK : in  std_logic;
		--SSI_TMS : in  std_logic;
		SSI_TD : in  std_logic := '0';
		SSI_update : in  std_logic := '0';
		SSI_shift : in  std_logic := '0';
		--SSI_capture : in  std_logic;
--		SSO_TCLK : out std_logic;
		--SSO_TMS : out std_logic;
		SSO_TD : out std_logic;
		SSO_update : out std_logic;
		SSO_shift : out std_logic;
		--SSO_capture : out std_logic--;
		
		debug_SIB_state : out std_logic

		   );
end SSIB;

architecture Behavioral of SSIB is
---------------Déclaration-Des-Composants----------------
-- composant : SecureReg
    component SecureReg
            Port (
            clk : in STD_LOGIC ;
            rstn : in STD_LOGIC ;
            sel : in STD_LOGIC ;
            shi : in STD_LOGIC ;
            i : in STD_LOGIC ;
            o : out STD_LOGIC ;
            g : out STD_LOGIC ;
            u : in STD_LOGIC ) ;
    end component;

-------------------------------------Déclaration-des-signaux-------------------------------------
--	signal shiftReg       : std_logic;                     --signal du registre à décalage
	signal gostReg        : std_logic;                     --signal fantôme du SIB
	signal SImux1         : std_logic;                     --multiplexeur entrée 1
	signal SImux2         : std_logic;                     --multiplexeur entrée 2
	signal SOmux          : std_logic;                     --multiplexeur sortie
	signal CDEmux         : std_logic;                     --multiplexeur commande
	signal SecureGostReg  : std_logic;                     --signal fantôme du SIB sécurisé
--	signal TCLK           : std_logic;                     --Signal d'horloge
	signal TMS            : std_logic;
	signal TD             : std_logic;
	signal update         : std_logic;
	signal shift          : std_logic;
	signal capture        : std_logic;
begin
    ------DEBUG--------
    debug_SIB_state <= CDEmux when debug else '0';


--	TCLK <= SI_TCLK;
	TMS <= SI_TMS;
	TD <= SI_TD;
	update <= SI_update;
	shift <= SI_shift;
	capture <= SI_capture;
	
--	SO_TCLK <= TCLK;
    SO_TMS <= TMS;
--    SO_TD <= TD;
    SO_update <= update;
    SO_shift <= shift;
    SO_capture <= capture;

--    TO_TCLK <= TCLK;
--    TO_TMS <= TMS;
--    TO_TD <= TD;
    TO_update <= update and CDEmux and SI_TMS;
    TO_shift <= shift and CDEmux and SI_TMS;
    TO_capture <= capture and CDEmux and SI_TMS;

--    SSO_TCLK <= SSI_TCLK;
--    SSO_TMS <= SSI_TMS;
--    SSO_TD <= SSI_TD;
    SSO_update <= SSI_update;
    SSO_shift <= SSI_shift;
--    SSO_capture <= SSI_capture;
		-- process(tclk)
		-- begin
			-- if rising_edge(tclk) then
				-- if rstn='0' then
					-- gostReg 	<= '0';
					-- shiftReg 	<= '0';
				-- elsif TMS = '0' then
					-- gostReg 	<= gostReg;
					-- shiftReg	<= shiftReg;
				-- elsif shift = '1' then
					-- gostReg 	<= gostReg;
					-- shiftReg	<= SOmux;
				-- elsif update = '1' then
					-- gostReg 	<= shiftReg;
					-- shiftReg	<= shiftReg;
				-- else
					-- gostReg 	<= gostReg;
					-- shiftReg	<= shiftReg;
				-- end if;
			-- end if;
		-- end process;
	--Non sécurisé
	SIB_MODE : IF not secured GENERATE
		CDEmux <= gostReg;
		SSO_TD <= '0';
	END GENERATE;
	
	S2IB_MODE : IF secured GENERATE
		CDEmux 	<= SecureGostReg and gostReg;
		--SSO_TMS       <= SSI_TMS;
        
        sreg : SecureReg port map(
            clk  => TCLK,
            rstn => rstn,
            sel  => '1',
            shi  => SSI_shift,
            i    => SSI_TD,
            o    => SSO_TD,
            g    => SecureGostReg,
            u    => SSI_update);
	END GENERATE;
	--Multiplexeur
	SOmux <= SImux1 when CDEmux = '0' else SImux2;
	
	TO_TMS <= CDEmux and SI_TMS;
	SImux1 	<= SI_TD;
	SImux2 	<= FROM_TD;
	TO_TD	<= SImux1;
    
    shReg : SecureReg port map(
        clk  => TCLK,
        rstn => rstn,
        sel  => TMS,
        shi  => shift,
        i    => SOmux,
        o    => SO_TD,
        g    => gostReg,
        u    => SI_update);
end Behavioral;









