----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12:05 29/01/2018 
-- Design Name: 
-- Module Name: AXISReg - axistech
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

entity AXISReg is
	generic(
	    breakLoop  : boolean := false;
		init	   : boolean := false;
		skipIfEmpty : boolean := false;
		s_data	   : integer:=8;
		s_id	   : integer:=0;
		s_user	   : integer:=0;
		s_dest	   : integer:=0;
		def_LAST   : boolean := false;  
		def_KEEP   : boolean := false;  
		def_STRB   : boolean := false; 
		def_id	   : integer:=0;
		def_user   : integer:=0;
		def_data   : integer:=0;
		def_dest   : integer:=0);
	port(
		clk		: in  std_logic;
		rstn	: in  std_logic;
		
		S_AXIS_TVALID : in   std_logic;
		S_AXIS_TREADY : out  std_logic;
		S_AXIS_TDATA  : in   std_logic_vector(s_data-1 downto 0);
		S_AXIS_TID    : in   std_logic_vector(s_id-1 downto 0):=(others => '0');
		S_AXIS_TDEST  : in   std_logic_vector(s_dest-1 downto 0):=(others => '0');
		S_AXIS_TLAST  : in   std_logic:='0';
		S_AXIS_TKEEP  : in   std_logic:='0';
		S_AXIS_TSTRB  : in   std_logic:='0';
		S_AXIS_TUSER  : in   std_logic_vector(s_user-1 downto 0):=(others => '0');
		--
		M_AXIS_TVALID : out  std_logic;
		M_AXIS_TREADY : in   std_logic;
		M_AXIS_TDATA  : out  std_logic_vector(s_data-1 downto 0);
		M_AXIS_TID    : out  std_logic_vector(s_id-1 downto 0);
		M_AXIS_TDEST  : out  std_logic_vector(s_dest-1 downto 0);
		M_AXIS_TLAST  : out  std_logic;
		M_AXIS_TKEEP  : out  std_logic;
		M_AXIS_TSTRB  : out  std_logic;
		M_AXIS_TUSER  : out  std_logic_vector(s_user-1 downto 0)
		--
		);
end AXISReg;

architecture axistech of AXISReg is
-----------------------------------------Déclaration-des-signaux-----------------------------------------
	signal valide          : std_logic;                    --Données stockées valides
	signal sent            : std_logic;                    --Données lues par le suivant
	signal memorize        : std_logic;                    --Indique aux registres de mémoriser les données
	signal AutorInp        : std_logic;                    --Ready & autorise la capture de donnée
	--Bus intermédiaire de sortie
	signal INT_TVALID      : std_logic;
	signal INT_TREADY      : std_logic;
	signal INT_TDATA       : std_logic_vector(s_data-1 downto 0);
	signal INT_TID         : std_logic_vector(s_id-1 downto 0);
	signal INT_TDEST       : std_logic_vector(s_dest-1 downto 0);
	signal INT_TLAST       : std_logic;
	signal INT_TKEEP       : std_logic;
	signal INT_TSTRB       : std_logic;
	signal INT_TUSER       : std_logic_vector(s_user-1 downto 0);
	signal Input_Valid_sig : std_logic;
begin
	SKIP : IF skipIfEmpty GENERATE
		signal skipCond : std_logic;
	begin
		skipCond    	<= INT_TREADY and not INT_TVALID;
		M_AXIS_TVALID	<= S_AXIS_TVALID when skipCond='1' else INT_TVALID;
		INT_TREADY  	<= M_AXIS_TREADY;
		M_AXIS_TDATA	<= S_AXIS_TDATA when skipCond='1' else INT_TDATA;
		M_AXIS_TID  	<= S_AXIS_TID when skipCond='1' else INT_TID;
		M_AXIS_TDEST	<= S_AXIS_TDEST when skipCond='1' else INT_TDEST;
		M_AXIS_TLAST	<= S_AXIS_TLAST when skipCond='1' else INT_TLAST;
		M_AXIS_TKEEP	<= S_AXIS_TKEEP when skipCond='1' else INT_TKEEP;
		M_AXIS_TSTRB	<= S_AXIS_TSTRB when skipCond='1' else INT_TSTRB;
		M_AXIS_TUSER	<= S_AXIS_TUSER when skipCond='1' else INT_TUSER;
		Input_Valid_sig	<= S_AXIS_TVALID and not skipCond;
	END GENERATE;
	
	noSKIP : IF not skipIfEmpty GENERATE
		M_AXIS_TVALID	<=INT_TVALID;
		INT_TREADY		<=M_AXIS_TREADY;
		M_AXIS_TDATA	<=INT_TDATA;
		M_AXIS_TID  	<=INT_TID;
		M_AXIS_TDEST	<=INT_TDEST;
		M_AXIS_TLAST	<=INT_TLAST;
		M_AXIS_TKEEP	<=INT_TKEEP;
		M_AXIS_TSTRB	<=INT_TSTRB;
		M_AXIS_TUSER	<=INT_TUSER;
		Input_Valid_sig	<= S_AXIS_TVALID;
	END GENERATE;
	
    FAST : IF not breakLoop GENERATE
        AutorInp <= sent;
    END GENERATE;
    
    UNLOOP : IF breakLoop GENERATE
        AutorInp <= not valide;
    end GENERATE;
    
	sent <= INT_TREADY or not valide;
	S_AXIS_TREADY <= AutorInp;
	memorize <= AutorInp and Input_Valid_sig;
	INT_TVALID <= valide;
	
	PVAL : process(clk) is
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				if init then
					valide <= '1';
				else
					valide <= '0';
				end if;
			elsif sent = '1' then
				valide <= memorize;
			else valide <= valide;
			end if;
		end if;
	end process PVAL;
	
	OthVal : process(clk)
	begin
		if rising_edge(clk) then
			if rstn = '0' then
				INT_TID		<= STD_LOGIC_VECTOR(to_unsigned(def_ID,s_ID));
				INT_TDEST	<= STD_LOGIC_VECTOR(to_unsigned(def_DEST,s_DEST)) ;  
				INT_TUSER	<= STD_LOGIC_VECTOR(to_unsigned(def_USER,s_USER));
				INT_TDATA	<= STD_LOGIC_VECTOR(to_unsigned(def_DATA,s_DATA));
			    if def_LAST then INT_TLAST	<= '1';
                else INT_TLAST    <= '0';end if;  
                if def_KEEP then INT_TKEEP    <= '1';
                else INT_TKEEP    <= '0';end if;  
                if def_STRB then INT_TSTRB    <= '1';
                else INT_TSTRB    <= '0';end if;  
			elsif memorize = '1' then
				if s_ID = 0 then
					INT_TID	<= (others => '0');
				else
					INT_TID	<= S_AXIS_TID;
				end if;    
				if s_DEST = 0 then
					INT_TDEST	<= (others => '0');
				else
					INT_TDEST	<= S_AXIS_TDEST;
				end if;   
				if s_USER = 0 then
					INT_TUSER	<= (others => '0');
				else
					INT_TUSER	<= S_AXIS_TUSER;
				end if;
				if s_DATA = 0 then
					INT_TDATA	<= (others => '0');
				else
					INT_TDATA	<= S_AXIS_TDATA;
				end if;
				INT_TLAST	<= S_AXIS_TLAST ; 
                INT_TKEEP    <= S_AXIS_TKEEP  ;
                INT_TSTRB    <= S_AXIS_TSTRB ;
			end if;
		end if;
	end process;
	
end axistech;