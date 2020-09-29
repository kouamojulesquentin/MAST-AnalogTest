----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 20.04.2018 10:07:31
-- Design Name: 
-- Module Name: SSCdriver - Behavioral
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SSCdriver is
	Generic(
		s_Interface : integer;
		s_SSC : integer
	);
    Port ( clk : in STD_LOGIC;
           rstn : in STD_LOGIC;
           Di : in STD_LOGIC_VECTOR (s_Interface-1 downto 0);
           save : in STD_LOGIC;
           shift : in STD_LOGIC;
		   removeAuth : in std_logic;
		   imageData : out STD_LOGIC_VECTOR(s_Interface-1 downto 0);
		   imageValid : out STD_LOGIC;
		   imageReady : in std_logic;
		   updateAuth : in std_logic;
		   ssibUnlocked : out std_logic;
		   SSS : out STD_LOGIC;
		   SSU : out STD_LOGIC;
           SSO : out STD_LOGIC;
           SSI : in STD_LOGIC);
end SSCdriver;

architecture Behavioral of SSCdriver is
	type states is (IDLE,RST_SETUP,RST_SHIFT,RST_UPDATE,NORMAL_SETUP,NORMAL_SHIFT,NORMAL_UPDATE,WAIT_READ_IMG);
-----------------------D�claration-des-signaux-----------------------
	signal etat_c     : states;                       --current state
	signal etat_s     : states;                       --next state
	signal memoire    : std_logic_vector(s_Interface-1 downto 0);
	signal memoire_s  : std_logic_vector(s_Interface-1 downto 0);
	signal compteur   : integer range 0 to S_SSC;     --current counter
	signal compteur_s : integer range 0 to S_SSC;     --next counter
begin
	SSO <= memoire(0);
	imageData <= memoire;
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				ssibUnlocked <= '0';
			elsif updateAuth = '1' and etat_c = NORMAL_UPDATE then
				ssibUnlocked <= '1';
			elsif etat_c = RST_UPDATE then
				ssibUnlocked <= '0';
			end if;
		end if;
	end process;
	
	
	
	process(clk)
	begin
		if rising_edge(clk) then
			if rstn='0' then
				etat_c <= IDLE;
				compteur <= 0;
				memoire <= (others => '0');
			else
				etat_c <= etat_s;
				compteur <= compteur_s;
				memoire <= memoire_s;
			end if;
		end if;
	end process;
	
	process(Di,save,shift,removeAuth,updateAuth,SSI,compteur,memoire,etat_c,imageReady)
	begin
		case etat_c is
			-------------MACHINE-A-ETAT-----------------
			
			---------------------Case IDLE---------------------
			when IDLE =>
				if removeAuth='1' then
					etat_s <= RST_SETUP;
				elsif save='1' then
					etat_s <= NORMAL_SETUP;
				else
					etat_s <= IDLE;
				end if;
				compteur_s <= 0;
				SSS <= '0';
				SSU <= '0';
				memoire_s <= std_logic_vector(to_unsigned(1,s_Interface));
				imageValid <= '0';
			---------------------------------------------------

			--------Case RST_SETUP--------
			when RST_SETUP =>
				etat_s <= RST_SHIFT;
				compteur_s <= s_ssc;
				SSS <= '0';
				SSU <= '0';
				memoire_s <= (others=>'0');
				imageValid <= '0';
			------------------------------

			-----------Case RST_SHIFT-----------
			when RST_SHIFT =>
				if compteur>0 then
					etat_s <= RST_SHIFT;
				else
					etat_s <= RST_UPDATE;
				end if;
				if compteur>0 then
					compteur_s <= compteur-1;
				else
					compteur_s <= compteur;
				end if;
				SSS <= '1';
				SSU <= '0';
				memoire_s <= (others=>'0');
				imageValid <= '0';
			------------------------------------

			--------Case RST_UPDATE--------
			when RST_UPDATE =>
				etat_s <= IDLE;
				compteur_s <= 0;
				SSS <= '0';
				SSU <= '1';
				memoire_s <= (others=>'0');
				imageValid <= '0';
			-------------------------------

			-------------------Case NORMAL_SHIFT-------------------
			when NORMAL_SHIFT =>
				if compteur>0 then
					etat_s <= NORMAL_SHIFT;
				elsif removeAuth='1' then
					etat_s <= RST_SETUP;
				else
					etat_s <= NORMAL_UPDATE;
				end if;
				if compteur>0 and shift='1' then
					compteur_s <= compteur-1;
				else
					compteur_s <= compteur;
				end if;
				if compteur>0 and shift='1' then
					SSS <= '1';
				else
					SSS <= '0';
				end if;
				SSU <= '0';
				if compteur>0 and shift='1' then
					memoire_s <= SSI & memoire(s_Interface-1 downto 1);
				else
					memoire_s <= memoire;
				end if;
				imageValid <= '0';
			-------------------------------------------------------

			------------------Case NORMAL_UPDATE------------------
			when NORMAL_UPDATE =>
				if updateAuth='1' then
					etat_s <= IDLE;
				elsif removeAuth='1' then
					etat_s <= RST_SETUP;
				else
					etat_s <= NORMAL_UPDATE;
				end if;
				compteur_s <= 0;
				SSS <= '0';
				if updateAuth='1' then
					SSU <= '1';
				else
					SSU <= '0';
				end if;
				memoire_s <= memoire;
				imageValid <= '0';
			------------------------------------------------------

			-----Case NORMAL_SETUP-----
			when NORMAL_SETUP =>
				etat_s <= WAIT_READ_IMG;
				compteur_s <= s_ssc;
				SSS <= '0';
				SSU <= '0';
				memoire_s <= Di;
				imageValid <= '0';
			---------------------------

			---------Case WAIT_READ_IMG---------
			when WAIT_READ_IMG =>
				if imageReady='1' then
					etat_s <= NORMAL_SHIFT;
				else
					etat_s <= WAIT_READ_IMG;
				end if;
				compteur_s <= compteur;
				SSS <= '0';
				SSU <= '0';
				memoire_s <= memoire;
				imageValid <= '1';
			------------------------------------

			---------FIN-MACHINE-A-ETAT-----------------

		end case;
	end process;	
end Behavioral;


