Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_arith.all;

entity bs_cell is
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic;
     P_out : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end  bs_cell;
  
architecture behav of  bs_cell is
 
 signal Shift_reg,next_Shift_reg : std_logic;
 signal Update_reg,next_Update_reg : std_logic;
 
 
 begin

 P_out <= Update_reg when mode = '1' else P_in;
 TDO  <= Shift_reg;
 
 seq: process(clk,rst)
  begin
   if rising_edge(clk) then
    if rst = '1' then
     Shift_reg <= '0';
     Update_reg<= '0';
    else
     Shift_reg <= next_Shift_reg;
     Update_reg<= next_Update_reg;
    end if;
   end if;
  end process;
 
 
 shift_comb: process(Sel,SH_en,CA_en,TDI,Shift_reg,P_in)
 begin
  next_Shift_reg <= Shift_reg;
  if Sel = '1' then
    if CA_en = '1' then
     next_Shift_reg <= P_in;
    elsif SH_en = '1' then  
     next_Shift_reg <= TDI;
    end if; 
  end if;
 end process;

 update_comb: process(Sel,UP_en,Shift_reg,Update_reg)
 begin
  next_Update_reg<=Update_reg;
  if Sel = '1' then
    if UP_en = '1' then
     next_Update_reg <= Shift_reg;
    end if;
  end if;
 end process;

end;

Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;

entity bs_register is
 generic (size : integer := 1);
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic_vector(size-1 downto 0);
     P_out : out std_logic_vector(size-1 downto 0);
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end  bs_register;
  
architecture behav of  bs_register is

component bs_cell 
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic;
     P_out : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end  component;

signal internal_chain :std_logic_vector(size downto 0);

begin
internal_chain(0) <= TDI;
TDO <= internal_chain(size);

   BS_REG: 
   for i in 0 to size-1 generate
      CELL : bs_cell port map
   ( clk =>clk,   
     rst =>rst,
     TDI => internal_chain(i),
     TDO  => internal_chain(i+1),
     P_in =>P_in(size-1-i) ,
     P_out =>P_out(size-1-i),
     mode =>mode,
     SH_en =>SH_en,
     CA_en =>CA_en,
     UP_en =>UP_en,
     Sel =>Sel);
   end generate BS_REG;

 
end;

Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;

entity bs_register_nocapture is
 generic (size : integer := 1);
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     SH_en : in  std_logic;
     UP_en : in  std_logic;
     P_out : out std_logic_vector(size-1 downto 0);
     Sel   : in  std_logic
   );
 end  bs_register_nocapture;

architecture behav of bs_register_nocapture is

component bs_register 
 generic (size : integer := 1);
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic_vector(size-1 downto 0);
     P_out : out std_logic_vector(size-1 downto 0);
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end component;

 signal P_in_dummy  : std_logic_vector(size-1 downto 0);
  
begin

 BS_REG_NOCA : bs_register generic map(size)
 port map
   ( clk, 
     rst, 
     TDI,
     TDO,
     P_in_dummy,
     P_out,
     '1',
     SH_en,
     '0',
     UP_en,
     Sel
   ); 

end;


Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;

entity SIB is
 port
   ( TCK   : in  std_logic;
     RST  : in  std_logic;
     SI   : in  std_logic;
     SO   : out std_logic;
     SEL  : in  std_logic;
     CE   : in  std_logic;
     UE   : in  std_logic;
     SE   : in  std_logic;
     
     toSI   : out std_logic;
     fromSO : in  std_logic;
     toSEL  : out  std_logic;
     toCE   : out std_logic;
     toUE   : out std_logic;
     toSE   : out std_logic;
     toTCK  : out std_logic;
     toRST  : out std_logic
   );
 end  SIB;
  
architecture behav of  SIB is
 
 signal Shift_reg,next_Shift_reg : std_logic;
 signal Update_reg,next_Update_reg : std_logic;
 
 
 begin

 toTCK <= TCK; --no clock gating to avoid timing problems on FPGA

 toSI <= Shift_reg ; 
 SO  <= fromSO when Update_reg = '1' else Shift_reg ;
 
 toSEL  <= SEL when Update_reg = '1' else '0';
 toCE   <= CE  when Update_reg = '1' else '0';  
 toUE   <= UE  when Update_reg = '1' else '0';
 toSE   <= SE  when Update_reg = '1' else '0'; 
 toRST  <= RST when Update_reg = '1' else '0'; 
 
 seq: process(TCK,RST)
  begin
   if rising_edge(TCK) then
    if RST = '1' then
     Shift_reg <= '0';
     Update_reg<= '0';
    else
     Shift_reg <= next_Shift_reg;
     Update_reg<= next_Update_reg;
    end if;
   end if;
  end process;
 

 shift_comb: process(SEL,SE,CE,SI,Shift_reg)
 begin
  next_Shift_reg <= Shift_reg;
  if SEL = '1' then
    if SE = '1' then  
     next_Shift_reg <= SI;
    end if; 
  end if;
 end process;

 update_comb: process(SEL,UE,Shift_reg,Update_reg)
 begin
  next_Update_reg<=Update_reg;
  if SEL = '1' then
    if UE = '1' then
     next_Update_reg <= Shift_reg;
    end if;
  end if;
 end process;

end;


Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;
use ieee.std_logic_arith.all;
library work;
use work.utilities.all;


---------------------------------------------------
-- Implementation of a binary-coded MIB
--
-- in coherence with SIT notation
-- when reg_before = true the control register is placed before the mux
-- when reg_before = false the control register is placed after the mux
--
---------------------------------------------------
entity MIB is
 generic (size :integer := 1;
          reg_before : boolean := true);
 port
   ( TCK   : in  std_logic;
     RST  : in  std_logic;
     SI   : in  std_logic;
     SO   : out std_logic;
     SEL  : in  std_logic;
     CE   : in  std_logic;
     UE   : in  std_logic;
     SE   : in  std_logic;
     
     toSI   : out std_logic_vector(size downto 1);
     fromSO : in  std_logic_vector(size downto 1);
     toSEL  : out  std_logic_vector(size downto 1);
     toCE   : out std_logic_vector(size downto 1);
     toUE   : out std_logic_vector(size downto 1);
     toSE   : out std_logic_vector(size downto 1);
     toTCK  : out std_logic_vector(size downto 1);
     toRST  : out std_logic_vector(size downto 1)
   );
 end  MIB;
  
architecture behav of  MIB is
 
 signal MIB_select : std_logic_vector(log2roundup(size) downto 1) := (others => '0');
 signal MIB_selvalue : natural range 0 to size;
 signal  MIB_Out : std_logic;
 signal  MIB_In : std_logic;
 
component bs_cell 
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic;
     P_out : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end  component;

component bs_register 
 generic (size : integer := 1);
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic_vector(size-1 downto 0);
     P_out : out std_logic_vector(size-1 downto 0);
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end component;
 
       
 begin

 toTCK <= (others => TCK); --no clock gating to avoid timing problems on FPGA

 MIB_selvalue <=  conv_integer(unsigned(MIB_select));


 ctrl_reg_before_mux: if reg_before = true generate
   toSI <= (others => MIB_Out) ; 
   MIB_In <= SI;
    SO     <= MIB_Out when MIB_selvalue=0 else fromSO(MIB_selvalue);
 end generate;

 ctrl_reg_after_mux: if reg_before = false generate
   toSI <= (others => SI) ; 
   MIB_In <= SI when MIB_selvalue=0 else fromSO(MIB_selvalue);
   SO     <= MIB_Out ;
 end generate;
 
 sel_process : process(SEL,CE,UE,SE,RST,MIB_selvalue)
 begin
 toSEL  <= (others => '0');
 toCE   <= (others => '0');  
 toUE   <= (others => '0');
 toSE   <= (others => '0'); 
 toRST  <= (others => '0'); 

 if (MIB_selvalue > 0) then
  toSEL(MIB_selvalue)   <= SEL ;
  toCE (MIB_selvalue)   <= CE ;  
  toUE (MIB_selvalue)   <= UE ;
  toSE (MIB_selvalue)   <= SE ; 
  toRST (MIB_selvalue)  <= RST;
 end if; 
 end process;
 
-- toSEL  <= SEL when MIB_select(1) = '1' else '0';
-- toCE   <= CE  when MIB_select(1) = '1' else '0';  
-- toUE   <= UE  when MIB_select(1) = '1' else '0';
-- toSE   <= SE  when MIB_select(1) = '1' else '0'; 
-- toRST  <= RST when MIB_select(1) = '1' else '0'; 
 
MIB_ctrl_reg: bs_register 
    generic map (size => log2roundup(size))
    port map
   ( clk =>TCK,   
     rst =>RST,
     TDI => MIB_in,
     TDO  => MIB_Out,
     P_in => MIB_select ,
     P_out =>MIB_select,
     mode =>'1',
     SH_en =>SE,
     CA_en =>'0',
     UP_en =>UE,
     Sel =>SEL);


end;

Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;

entity MUX_Exclusive is
 port
   ( TCK  : in  std_logic;
     RST  : in  std_logic;
     SO   : out std_logic;
     SEL  : in  std_logic;
     CE   : in  std_logic;
     UE   : in  std_logic;
     SE   : in  std_logic;
     
     SI_0   : in  std_logic;
     SEL_0  : out  std_logic;
     CE_0   : out  std_logic;
     UE_0   : out  std_logic;
     SE_0   : out  std_logic;
     
     SI_1   : in  std_logic;
     SEL_1  : out  std_logic;
     CE_1   : out  std_logic;
     UE_1   : out  std_logic;
     SE_1   : out  std_logic

   );
 end  MUX_Exclusive;
  
architecture behav of  MUX_Exclusive is
 
 signal Mux_ctrl : std_logic;
 
 signal Mux_input : std_logic;
 
component bs_cell 
 port
   ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     P_in  : in  std_logic;
     P_out : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
 end  component;

 begin



 SEL_0  <= SEL when Mux_ctrl = '0' else '0';
 CE_0   <= CE  when Mux_ctrl = '0' else '0';  
 UE_0   <= UE  when Mux_ctrl = '0' else '0';
 SE_0   <= SE  when Mux_ctrl = '0' else '0'; 

 SEL_1  <= SEL when Mux_ctrl = '1' else '0';
 CE_1   <= CE  when Mux_ctrl = '1' else '0';  
 UE_1   <= UE  when Mux_ctrl = '1' else '0';
 SE_1   <= SE  when Mux_ctrl = '1' else '0'; 

 Mux_input <= SI_1 when Mux_ctrl = '1' else SI_0;

Mux_ctrl_reg: bs_cell port map
   ( clk =>TCK,   
     rst =>RST,
     TDI => Mux_input,
     TDO  => SO,
     P_in => Mux_ctrl ,
     P_out =>Mux_ctrl,
     mode =>'1',
     SH_en =>SE,
     CA_en =>'0',
     UP_en =>UE,
     Sel =>SEL);
     
     
end;
