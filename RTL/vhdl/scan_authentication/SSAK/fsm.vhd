----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 20.04.2018 10:07:31
-- Design Name: 
-- Module Name: FSM - MAE
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

entity FSM is
    Port ( clk : in STD_LOGIC;
		   rst : in STD_LOGIC;
           sel : in STD_LOGIC;
           updateV : in STD_LOGIC;
           confKeyV : in STD_LOGIC;
           SHAv : in STD_LOGIC;
		   respCheck : in STD_LOGIC;
           muxCde : out STD_LOGIC;
           SSC_shift : out STD_LOGIC;
           SSC_update : out STD_LOGIC;
           GeneDef : out STD_LOGIC;
		   rstSSC : out std_logic;
		   compare : out STD_LOGIC;
           confSave : out STD_LOGIC;
		   BLch		: in  STD_LOGIC;
		   BLmatch	: in  STD_LOGIC;
		   sucess : out std_logic;
		   failed : out std_logic;
           rstn : out STD_LOGIC);
end FSM;

architecture MAE of FSM is
	type etats is (INIT,DG,WAIT1,CONF,WAIT_CONF_KEY,WAIT_RSP,COMP,appSSC,D_LOCK);
-------------------------Déclaration-des-signaux-------------------------
	signal etat_c : etats;                        --ï¿½tat courant de la FSM
	signal etat_s : etats;                        --Prochain ï¿½tat de la FSM
	signal s_su   : std_logic;
	signal DefG   : std_logic;
begin
	GeneDef <= DefG;
	rstSSC <= DefG;
    sucess <= s_su;
	process(clk)
	begin
		if rising_edge(clk) then
			if rst='1' then
				etat_c <= INIT;
				s_su	<= '0';
			else
				etat_c <= etat_s;
				if etat_s = appSSC then
					s_su <= '1';
				else
					s_su <= s_su;
				end if;
			end if;
		end if;
	end process;
	
	process(sel,updateV,confKeyV,SHAv,respCheck,etat_c,BLmatch)
	begin
		case etat_c is
			-------------MACHINE-A-ETAT-----------------

			----------Case INIT----------
			when INIT =>
				etat_s <= DG;
				muxCde <= '0';
				SSC_shift <= '0';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '0';
			-----------------------------

			-----------Case DG-----------
			when DG =>
				if sel='1' then
					etat_s <= WAIT1;
				else
					etat_s <= DG;
				end if;
				muxCde <= '0';
				SSC_shift <= '0';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '1';
				compare <= '0';
				rstn <= '1';
			-----------------------------

			------------------Case WAIT1------------------
			when WAIT1 =>
				if updateV='1' then
					etat_s <= CONF;
				else
					etat_s <= WAIT1;
				end if;
				muxCde <= '0';
				SSC_shift <= '0';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '1';
			----------------------------------------------

			-----------Case CONF-----------
			when CONF =>
				if BLmatch = '1' then
					etat_s <= D_LOCK;
				else
					etat_s <= WAIT_CONF_KEY;
				end if;
				muxCde <= '0';
				SSC_shift <= '0';
				confSave <= '1';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '1';
			-------------------------------

			----------------Case WAIT_CONF_KEY----------------
			when WAIT_CONF_KEY =>
				if BLmatch = '1' then
					etat_s <= D_LOCK;
				elsif confKeyV='1' then
					etat_s <= WAIT_RSP;
				else
					etat_s <= WAIT_CONF_KEY;
				end if;
				muxCde <= '0';
				SSC_shift <= '1';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '1';
			--------------------------------------------------

			-------------------Case WAIT_RSP-------------------
			when WAIT_RSP =>
				if BLmatch = '1' then
					etat_s <= D_LOCK;
				elsif SHAv='1' and updateV='1' then
					etat_s <= COMP;
				else
					etat_s <= WAIT_RSP;
				end if;
				muxCde <= '1';
				SSC_shift <= '1';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '1';
			---------------------------------------------------

			-----------Case COMP-----------
			when COMP =>
				if respCheck='0' then
					etat_s <= D_LOCK;
				else
					etat_s <= appSSC;
				end if;
				muxCde <= '1';
				SSC_shift <= '1';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '1';
				rstn <= '1';
			-------------------------------

			-----Case appSSC-----
			when appSSC =>
				etat_s <= D_LOCK;
				muxCde <= '1';
				SSC_shift <= '0';
				confSave <= '0';
				SSC_update <= '1';
				DefG <= '0';
				compare <= '0';
				rstn <= '1';
			---------------------

			---------Case D_LOCK------
			when D_LOCK =>
				if sel = '1' then
					etat_s <= INIT;
				else
					etat_s <= D_LOCK;
				end if;
				muxCde <= '0';
				SSC_shift <= '0';
				confSave <= '0';
				SSC_update <= '0';
				DefG <= '0';
				compare <= '0';
				rstn <= '0';
			--------------------------

			---------FIN-MACHINE-A-ETAT-----------------
		end case;
	end process;

end MAE;

