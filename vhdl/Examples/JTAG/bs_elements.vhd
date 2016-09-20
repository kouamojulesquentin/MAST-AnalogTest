Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;

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
