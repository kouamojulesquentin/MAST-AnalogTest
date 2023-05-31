----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 25.04.2018 14:11:06
-- Design Name: 
-- Module Name: AXISdemux - RVlogic
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

entity AXISdemux is
	generic(
		s_data	: integer;
		s_user	: integer:=1;
		s_dest	: integer:=1;
		cmdBit	: integer:=0;
		synch   : boolean := false);
    Port ( 
		clk : in STD_LOGIC;
		rstn : in STD_LOGIC;
		S_AXIS_TVALID	: in  std_logic;
		S_AXIS_TREADY	: out std_logic;
		S_AXIS_TDATA	: in  std_logic_vector(s_data-1 downto 0);
		S_AXIS_TUSER	: in  std_logic_vector(s_user-1 downto 0):=(others=> '0');
		S_AXIS_TDEST	: in  std_logic_vector(s_dest-1 downto 0):=(others=> '0');

		M0_AXIS_TVALID	: out std_logic;
		M0_AXIS_TREADY	: in  std_logic;
		M0_AXIS_TDATA	: out std_logic_vector(s_data-1 downto 0);
		M0_AXIS_TUSER	: out std_logic_vector(s_user-1 downto 0);
		M0_AXIS_TDEST	: out std_logic_vector(s_dest-1 downto 0);

		M1_AXIS_TVALID	: out std_logic;
		M1_AXIS_TREADY	: in  std_logic;
		M1_AXIS_TDATA	: out std_logic_vector(s_data-1 downto 0);
		M1_AXIS_TUSER	: out std_logic_vector(s_user-1 downto 0);
		M1_AXIS_TDEST	: out std_logic_vector(s_dest-1 downto 0)
		   );
end AXISdemux;

architecture RVlogic of AXISdemux is
-----------------------Déclaration-des-signaux-----------------------
	signal buffOut0_AXIS_TVALID : std_logic;
	signal buffOut0_AXIS_TREADY : std_logic;
	signal buffOut0_AXIS_TDATA  : std_logic_vector(s_data - 1 downto 0);
	signal buffOut0_AXIS_TUSER  : std_logic_vector(s_user - 1 downto 0);
	signal buffOut0_AXIS_TDEST  : std_logic_vector(s_dest - 1 downto 0);
	signal buffOut1_AXIS_TVALID : std_logic;
	signal buffOut1_AXIS_TREADY : std_logic;
	signal buffOut1_AXIS_TDATA  : std_logic_vector(s_data - 1 downto 0);
	signal buffOut1_AXIS_TUSER  : std_logic_vector(s_user - 1 downto 0);
	signal buffOut1_AXIS_TDEST  : std_logic_vector(s_dest - 1 downto 0);
begin
	buffOut0_AXIS_TVALID <= S_AXIS_TVALID and not S_AXIS_TDEST(cmdBit);
	buffOut0_AXIS_TDATA <= S_AXIS_TDATA;
	buffOut0_AXIS_TUSER <= S_AXIS_TUSER;
	buffOut0_AXIS_TDEST <= S_AXIS_TDEST;
	
	buffOut1_AXIS_TVALID <= S_AXIS_TVALID and S_AXIS_TDEST(cmdBit);
	buffOut1_AXIS_TDATA <= S_AXIS_TDATA;
	buffOut1_AXIS_TUSER <= S_AXIS_TUSER;
	buffOut1_AXIS_TDEST <= S_AXIS_TDEST;

	S_AXIS_TREADY <= buffOut0_AXIS_TREADY when S_AXIS_TDEST(cmdBit)='0' else buffOut1_AXIS_TREADY;
	
	MEM_OUT : IF synch GENERATE        
        Ro0 : entity work.AXISReg generic map(
            s_data        => s_data,
            s_user        => s_user,
            s_dest        => s_dest)
                      port map(
            clk           => clk,
            rstn          => rstn,
            S_AXIS_TVALID => buffOut0_AXIS_TVALID,
            S_AXIS_TREADY => buffOut0_AXIS_TREADY,
            S_AXIS_TDATA  => buffOut0_AXIS_TDATA,
            S_AXIS_TDEST  => buffOut0_AXIS_TDEST,
            S_AXIS_TUSER  => buffOut0_AXIS_TUSER,
            M_AXIS_TVALID => M0_AXIS_TVALID,
            M_AXIS_TREADY => M0_AXIS_TREADY,
            M_AXIS_TDATA  => M0_AXIS_TDATA,
            M_AXIS_TDEST  => M0_AXIS_TDEST,
            M_AXIS_TUSER  => M0_AXIS_TUSER);
        
        Ro1 : entity work.AXISReg generic map(
            s_data        => s_data,
            s_user        => s_user,
            s_dest        => s_dest)
                      port map(
            clk           => clk,
            rstn          => rstn,
            S_AXIS_TVALID => buffOut1_AXIS_TVALID,
            S_AXIS_TREADY => buffOut1_AXIS_TREADY,
            S_AXIS_TDATA  => buffOut1_AXIS_TDATA,
            S_AXIS_TUSER  => buffOut1_AXIS_TUSER,
            S_AXIS_TDEST  => buffOut1_AXIS_TDEST,
            M_AXIS_TVALID => M1_AXIS_TVALID,
            M_AXIS_TREADY => M1_AXIS_TREADY,
            M_AXIS_TDATA  => M1_AXIS_TDATA,
            M_AXIS_TUSER  => M1_AXIS_TUSER,
            M_AXIS_TDEST  => M1_AXIS_TDEST);
	END GENERATE;
	
	DIRECT_OUT : IF not synch GENERATE
		M0_AXIS_TVALID <= buffOut0_AXIS_TVALID;
		buffOut0_AXIS_TREADY <= M0_AXIS_TREADY;
		M0_AXIS_TDATA <= buffOut0_AXIS_TDATA;
		M0_AXIS_TUSER <= buffOut0_AXIS_TUSER;
		M0_AXIS_TDEST <= buffOut0_AXIS_TDEST;
		M1_AXIS_TVALID <= buffOut1_AXIS_TVALID;
		buffOut1_AXIS_TREADY <= M1_AXIS_TREADY;
		M1_AXIS_TDATA <= buffOut1_AXIS_TDATA;
		M1_AXIS_TUSER <= buffOut1_AXIS_TUSER;
		M1_AXIS_TDEST <= buffOut1_AXIS_TDEST;
	END GENERATE;
end RVlogic;







