-- Package TAPFSM_package defines the constants and types used in TAPFSM
--  

Library IEEE;
use IEEE.STD_LOGIC_1164.all;

package JTAG_package is
  ---IEEE1149.1 states 
  
  
  type extended_tap_states is (First_state,
  		      Test_Logic_Reset, Run_Test_Idle,
                      Select_DR_Scan, Capture_DR, Shift_DR, Exit1_DR, 
                      Pause_DR, Exit2_DR, Update_DR,
                      Select_IR_Scan, Capture_IR, Shift_IR, Exit1_IR, 
                      Pause_IR, Exit2_IR, Update_IR);
  subtype tap_states is extended_tap_states range Test_Logic_Reset to Update_IR;
  subtype DR_states is tap_states range Select_DR_Scan to Update_DR;
  subtype IR_states is tap_states range Select_IR_Scan to Update_IR;

  constant TAP_CODE_WIDTH : integer := 4;

  constant TEST_LOGIC_RESET_CODE: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0000";
  constant RUN_TEST_IDLE_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0001";
  constant SELECT_DR_SCAN_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0010";
  constant CAPTURE_DR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0011";
  constant SHIFT_DR_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0100";
  constant EXIT1_DR_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0101";
  constant PAUSE_DR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0110";
  constant EXIT2_DR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "0111";
  constant UPDATE_DR_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1000";
  constant SELECT_IR_SCAN_CODE  : std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1001";
  constant CAPTURE_IR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1010";
  constant SHIFT_IR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1011";
  constant EXIT1_IR_CODE 	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1100";
  constant PAUSE_IR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1101";
  constant EXIT2_IR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1110";
  constant UPDATE_IR_CODE  	: std_logic_vector(TAP_CODE_WIDTH-1 downto 0) := "1111";
 
 constant NO_IN_STATE    : std_logic_vector (1 downto 0) := "00";
 constant IN_START_STATE : std_logic_vector (1 downto 0) := "01";
 constant IN_SHIFT_STATE : std_logic_vector (1 downto 0) := "10";
 constant IN_END_STATE   : std_logic_vector (1 downto 0) := "11";

 function is_shift_pause (state : in tap_states) return std_logic;
 function is_TLR (state : in tap_states) return std_logic;
 function is_RTI (state : in tap_states) return std_logic;
 function is_allowed_shift (state : in tap_states) return boolean;
 function to_state_type(state : in std_logic_vector(TAP_CODE_WIDTH-1 downto 0)) return tap_states;


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
 end  component;

component bs_register_nocapture
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
 end  component;

component SIB 
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
 end  component;

end;

package body JTAG_package is

 function is_shift_pause (state : in tap_states) return std_logic is
   begin
    if (state = Shift_IR) or (state = Pause_IR) or 
       (state = Shift_DR) or (state = Pause_DR) then
    	return '1';
    else
        return '0';
    end if;		
   end;
 function is_TLR (state : in tap_states) return std_logic is
   begin
    if (state = Test_Logic_Reset)  then
    	return '1';
    else
        return '0';
    end if;		
   end;

 function is_RTI (state : in tap_states) return std_logic is
   begin
    if (state = Run_Test_Idle) then
    	return '1';
    else
        return '0';
    end if;		
   end;

  function is_allowed_shift (state : in tap_states) return boolean is
   --Checks that the shift state is one of the 6 stable states
   --of the 1149.1 FSM
   begin
    if (state = Test_Logic_Reset) or (state = Run_Test_Idle) or
       (state = Shift_DR) or (state = Pause_DR) or
       (state = Shift_IR) or (state = Pause_IR)   then
    	return true;
    else
        return false;
    end if;		
   end;

 function to_state_type(state : in std_logic_vector(TAP_CODE_WIDTH-1 downto 0)) return tap_states is
 begin
  case (state) is
    when TEST_LOGIC_RESET_CODE 	=> return Test_Logic_Reset; 
    when RUN_TEST_IDLE_CODE 	=> return Run_Test_Idle;
    when SELECT_DR_SCAN_CODE 	=> return Select_DR_Scan;
    when CAPTURE_DR_CODE 	=> return Capture_DR;
    when SHIFT_DR_CODE 		=> return Shift_DR;
    when EXIT1_DR_CODE 		=> return Exit1_DR;
    when PAUSE_DR_CODE 		=> return Pause_DR;
    when EXIT2_DR_CODE 		=> return Exit2_DR;
    when UPDATE_DR_CODE 	=> return Update_DR;
    when SELECT_IR_SCAN_CODE 	=> return Select_IR_Scan;
    when CAPTURE_IR_CODE 	=> return Capture_IR;
    when SHIFT_IR_CODE 		=> return Shift_IR;
    when EXIT1_IR_CODE 		=> return Exit1_IR;
    when PAUSE_IR_CODE 		=> return Pause_IR;
    when EXIT2_IR_CODE 		=> return Exit2_IR;
    when UPDATE_IR_CODE 	=> return Update_IR;
    when others => return Test_Logic_Reset; --should never come here, but line needed by Modelsim
  end case;
 
 end;

end;
