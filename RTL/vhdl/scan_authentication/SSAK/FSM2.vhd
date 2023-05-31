--------------------------------------------------------------------------------
--                                                                            --
-- File name : FSM2.vhd                                                       --
-- Author : Vincent Reynaud                                                   --
-- Creation date : 13/12/2019                                                 --
--                                                                            --
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;

entity FSM2 is
	port(
		clk    : in  std_logic;
		rst_n   : in  std_logic;
		---INPUT-----
		defV   	: in std_logic;
		updateV : in std_logic;
		confNull: in std_logic;
		ssakV	: in std_logic;
		SHAv	: in std_logic;
		rCheck	: in std_logic;
		capture	: in std_logic;
		sel 	: in std_logic;
		---OUTPUT-----
		muxCde		: out std_logic;
		SSC_shift	: out std_logic;
		SSC_update	: out std_logic;
		GeneDef		: out std_logic;
		rstSSC		: out std_logic;
		compare		: out std_logic;
		confSave	: out std_logic;
		sucess      : out std_logic;
		failed		: out std_logic;
		rstn		: out std_logic);
end FSM2;

Architecture MAE of FSM2 is
	type etats is (INIT,IDLE,WAIT_CONF,SCONF,RESET,WAIT_SSAK,WAIT_RSP,CMP,APP_SSC,CONNECTED_act,CONNECTED,NO_CO_act,NO_CO);
-----Déclaration-des-signaux-----
	signal etat_c : etats;
	signal etat_s : etats;
begin

	process(clk)
	begin
		if rising_edge(clk) then
			if rst_n='0' then
				etat_c <= INIT;
				sucess <= '0';
				failed <= '0';
			else
				etat_c <= etat_s;
				if etat_c=RESET then
					sucess <= '0';
				elsif etat_c=IDLE then
					failed <= '0';
				elsif etat_c = APP_SSC then
					sucess <= '1';
				elsif etat_c = NO_CO_act then
					failed <= '1';
				end if;
			end if;
		end if;
	end process;


	process(etat_c,defV,updateV,confNull,ssakV,SHAv,rCheck,capture,sel)
	begin
		case etat_c is
		-------------MACHINE-A-ETAT-----------------

		-----------Case INIT-----------
		when INIT =>
			etat_s <= IDLE;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '0';
		-------------------------------
		
		-----------Case IDLE-----------
		when IDLE =>
			if defV='1' then
				etat_s <= WAIT_CONF;
			else
				etat_s <= IDLE;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '1';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		-------------------------------

		---------Case WAIT_CONF---------
		when WAIT_CONF =>
			if updateV='1' then
				etat_s <= SCONF;
			else
				etat_s <= WAIT_CONF;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		--------------------------------

		-----------Case SCONF-----------
		when SCONF =>
			if confNull='0' then
				etat_s <= WAIT_SSAK;
			else
				etat_s <= RESET;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '1';
--			sucess <= '0';
			rstn <= '1';
		--------------------------------

		------Case RESET------
		when RESET =>
			etat_s <= IDLE;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '1';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		----------------------

		---------Case WAIT_SSAK---------
		when WAIT_SSAK =>
			if ssakV='1' then
				etat_s <= WAIT_RSP;
			else
				etat_s <= WAIT_SSAK;
			end if;
			muxCde <= '0';
			SSC_shift <= '1';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		--------------------------------

		-----------Case WAIT_RSP-----------
		when WAIT_RSP =>
			if SHAv='1' and updateV='1' then
				etat_s <= CMP;
			else
				etat_s <= WAIT_RSP;
			end if;
			muxCde <= '1';
			SSC_shift <= '1';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		-----------------------------------

		-----------Case CMP-----------
		when CMP =>
			if rCheck='1' then
				etat_s <= APP_SSC;
			else
				etat_s <= NO_CO_act;
			end if;
			muxCde <= '1';
			SSC_shift <= '1';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '1';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		------------------------------

		--------Case APP_SSC--------
		when APP_SSC =>
			etat_s <= CONNECTED_act;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '1';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '1';
			rstn <= '1';
		----------------------------

		----------Case CONNECTED_act----------
		when CONNECTED_act =>
			if capture='1' and sel='0' then
				etat_s <= CONNECTED;
			else
				etat_s <= CONNECTED_act;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '1';
			rstn <= '1';
		--------------------------------------

		--------Case CONNECTED--------
		when CONNECTED =>
			if sel='1' then
				etat_s <= INIT;
			else
				etat_s <= CONNECTED;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '1';
			rstn <= '1';
		------------------------------

		-----------Case NO_CO_act-----------
		when NO_CO_act =>
			if capture='1' and sel='0' then
				etat_s <= NO_CO;
			else
				etat_s <= NO_CO_act;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		------------------------------------

		----------Case NO_CO----------
		when NO_CO =>
			if sel='1' then
				etat_s <= IDLE;
			else
				etat_s <= NO_CO;
			end if;
			muxCde <= '0';
			SSC_shift <= '0';
			SSC_update <= '0';
			GeneDef <= '0';
			rstSSC <= '0';
			compare <= '0';
			confSave <= '0';
--			sucess <= '0';
			rstn <= '1';
		------------------------------

		---------FIN-MACHINE-A-ETAT-----------------
		end case;
	end process;
end MAE;


