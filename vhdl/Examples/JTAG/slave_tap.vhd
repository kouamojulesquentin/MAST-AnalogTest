--  
--  Filename    : tapfsm.vhd	                                          
--  Designer    : Chen-Huan Chiang      			
--  Modified by : Michele Portolan
--						                                                                                                 --
--  Description: Finite State Machine for the Boundary-Scan TAP Controller.   
--               This is designed for ASP decoder to identify TAP idle states. 
--               TMS is 1 when TAP in TLR and is 0 for the rest of idle_states. 
	   
--                       
--  Inputs: TCK, TMS, TRSTN
--  Output: IS_SHIFTING, IS_IDLE 
--          (IS_SHIFTING='1' when TAP is in a  shifting state -- Shift_IR or Shift_DR)
--          (IS_IDLE='1' when TAP in one of the 4 idle_states -- Test_Logic_Reset,Run_Test_Idle,Pause_DR,Pause_IR)
--
-- Package TAPFSM_package defines the constants and types used in TAPFSM
--  

--Library IEEE;
--use IEEE.STD_LOGIC_1164.all;

--package JTAG_package is
  ---IEEE1149.1 states 
--  type tap_states is (Test_Logic_Reset, Run_Test_Idle,
--                      Select_DR_Scan, Capture_DR, Shift_DR, Exit1_DR, 
--                      Pause_DR, Exit2_DR, Update_DR,
--                      Select_IR_Scan, Capture_IR, Shift_IR, Exit1_IR, 
--                      Pause_IR, Exit2_IR, Update_IR);
--end JTAG_package;
--
Library IEEE,STD;
use IEEE.STD_LOGIC_1164.all;
use work.JTAG_package.all;
use STD.textio.all;

package slave_TAP_package is

  component slave_TAP
    generic ( output_filename : string := "./outputs/tap_log.txt");
    port (
         --TAP interface
	 TCK: in std_logic;        
         TMS: in std_logic;        
         TRSTN: in std_logic;  
         TDI: in std_logic;        
         TDO: out std_logic;        
	 --debug signals
         IS_SHIFTING: out std_logic;      
         IS_IDLE: out std_logic;
	  --Scan Chain control signal
	 reset : out std_logic; 
         Select_DR : out std_logic;
         Select_IR : out std_logic;
	 ClockIR : out std_logic;
	 ShiftIR : out std_logic;
	 UpdateIR : out std_logic;
	 ClockDR : out std_logic;
	 ShiftDR : out std_logic;
	 UpdateDR : out std_logic;
	 CaptureDR: out std_logic;
	 to_scan_chain : out std_logic;
	 from_scan_chain : in std_logic
	   );       
  end component;
end slave_TAP_package;

library IEEE;
use IEEE.std_logic_1164.all;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;

Entity slave_TAP is
    generic ( output_filename : string := "./outputs/tap_log.txt");
     port ( 
         --TAP interface
	 TCK: in std_logic;        
         TMS: in std_logic;        
         TRSTN: in std_logic;  
         TDI: in std_logic;        
         TDO: out std_logic;        
	 --debug signals
         IS_SHIFTING: out std_logic;      
         IS_IDLE: out std_logic;
	  --Scan Chain control signal
	 reset : out std_logic; 
         Select_DR : out std_logic;
         Select_IR : out std_logic;
	 ClockIR : out std_logic;
	 ShiftIR : out std_logic;
	 UpdateIR : out std_logic;
	 ClockDR : out std_logic;
	 ShiftDR : out std_logic;
	 CaptureDR : out std_logic;
	 UpdateDR : out std_logic;
	 to_scan_chain : out std_logic;
	 from_scan_chain : in std_logic
	 );       
end slave_TAP;

Architecture behavior of slave_TAP is
  --
  -- 16 states TAP FSM
  --

  signal current_state: tap_states;  
  signal next_state: tap_states;
  signal clock_DR_enable,clock_IR_enable : std_logic := '0'; 
 
 signal received_data : std_logic_vector(31 downto 0);                                       

  type log_status_type is (idle,active);

 signal log_status, next_log_status : log_status_type;
 signal toggle : std_logic;

 signal gen_status, next_gen_status : log_status_type;
 signal gen_toggle : std_logic;

   function print_state(state: tap_states) return string is
    variable s: string(1 to 16);
    begin
    case state is
      when Test_Logic_Reset  =>s := "Test_Logic_Reset";
      when Run_Test_Idle     =>s := "Run_Test_Idle   ";
      when Select_DR_Scan    =>s := "Select_DR_Scan  ";
      when Select_IR_Scan    =>s := "Select_IR_Scan  ";
      when Capture_IR        =>s := "Capture_IR      ";
      when Shift_IR          =>s := "Shift_IR        "; 
      when Exit1_IR          =>s := "Exit1_IR        "; 
      when Pause_IR          =>s := "Pause_IR        ";
      when Exit2_IR          =>s := "Exit2_IR        "; 
      when Update_IR         =>s := "Update_IR       "; 
      when Capture_DR        =>s := "Capture_DR      ";  
      when Shift_DR          =>s := "Shift_DR        ";  
      when Exit1_DR          =>s := "Exit1_DR        ";  
      when Pause_DR          =>s := "Pause_DR        ";  
      when Exit2_DR          =>s := "Exit2_DR        ";  
      when Update_DR         =>s := "Update_DR       "; 
    end case;
   return s;
   end print_state;

constant first_spaces : string(1 to 18) :="                  ";

  function chr2std_logic(sl: character) return std_logic is
      variable c: std_logic;
      begin
       case sl is
         when 'U' => c:= 'U';
         when 'X' => c:= 'X';
         when '0' => c:= '0';
         when '1' => c:= '1';
         when 'Z' => c:= 'Z';
         when 'W' => c:= 'W';
         when 'L' => c:= 'L';
         when 'H' => c:= 'H';
         when '-' => c:= '-';
         when others => c:= 'X';
       end case;
      return c;
     end chr2std_logic;

type string_p is access string;

begin
  
   TDO <= from_scan_chain;
   to_scan_chain <= TDI;
  
  sequential: process (TRSTN, TCK)
  begin
    if (TRSTN = '0')
       then current_state <= Test_Logic_Reset;
    elsif (TCK'event and (TCK = '1'))
       then current_state <= next_state;
    end if;
  end process;
  
 
  combinational: process (TMS,current_state)
    -- qout[1] for IS_TLR_to_RTI
    -- qout[0] for IS_IDLE 
    variable qout: std_logic_vector (1 downto 0);                                      
  begin
    
    Select_DR <= '0';
    Select_IR <= '0';
    ShiftDR <= '0';
    ShiftIR <= '0';
    UpdateDR <= '0';
    UpdateIR <= '0';
    CaptureDR <= '0';
    IS_SHIFTING <= '0';
	 next_state <= current_state;
    reset <= '0';
    case current_state is
      when Test_Logic_Reset  =>
                      clock_DR_enable <='0';
                      clock_IR_enable <='0';
		      reset <= '1';
                      if (TMS = '1') then 
                        qout := "01"; 
                        next_state <= current_state;
                      else 
                        qout := "11";
                        next_state <= Run_Test_Idle;
                      end if;
                           
      when Run_Test_Idle  => qout := "01"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='0';
                      if (TMS = '1')
                         then next_state <= Select_DR_Scan;
                         else next_state <= current_state;
                      end if;
                      
      when Select_DR_Scan  => qout := "00"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='0';
                      if (TMS = '1')
                         then next_state <= Select_IR_Scan;
                         else next_state <= Capture_DR;
                      end if;
                      
      when Select_IR_Scan  => qout := "00"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='0';
		      Select_IR <= '1';
                      if (TMS = '1')
                         then next_state <= Test_Logic_Reset;
                         else next_state <= Capture_IR;
                      end if;
		           
      when Capture_IR  => qout := "00"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='1'; --Rising edge to capture data
		      Select_IR <= '1';
                      if (TMS = '1')
                         then next_state <= Exit1_IR;
                         else next_state <= Shift_IR;
                      end if;
		      
      when Shift_IR  => qout := "00"; 
		      Select_IR <= '1';
                      clock_DR_enable <='0';
                      clock_IR_enable <='1'; --Shifting
                      ShiftIR <= '1';
		      IS_SHIFTING <= '1';
		      if (TMS = '1')
                         then next_state <= Exit1_IR;
                         else next_state <= current_state;
                      end if;
                      
      when Exit1_IR  => qout := "00"; 
		      Select_IR <= '1';
                      clock_DR_enable <='0';
                      clock_IR_enable <='1';
                      --ShiftIR <= '1';
		      if (TMS = '1')
                         then next_state <= Update_IR;
                         else next_state <= Pause_IR;
                      end if;
                           
      when Pause_IR  => qout := "01"; 
		      Select_IR <= '1';
                      clock_DR_enable <='0';
                      clock_IR_enable <='1';
                      if (TMS = '1')
                         then next_state <= Exit2_IR;
                         else next_state <= current_state;
                      end if;
                           
      when Exit2_IR  => qout := "00"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='1';
		      Select_IR <= '1';
                      if (TMS = '1')
                         then next_state <= Update_IR;
                         else next_state <= Shift_IR;
                      end if;
                      
      when Update_IR  => qout := "00"; 
                      clock_DR_enable <='0';
                      clock_IR_enable <='1';
                      UpdateIR <='1';
		      Select_IR <= '1';
                      if (TMS = '1')
                         then next_state <= Select_DR_Scan;
                         else next_state <= Run_Test_Idle;
                      end if;
                      
      when Capture_DR  => qout := "00"; 
		      Select_DR <= '1';
		      CaptureDR <= '1';
                     clock_DR_enable <='1'; --Rising edge to capture data
                      clock_IR_enable <='0';
                      if (TMS = '1')then
                          next_state <= Exit1_DR;
                      else 
							    next_state <= Shift_DR;
                      end if;
                      
      when Shift_DR  => qout := "00"; 
	                clock_DR_enable <='1'; --Shifting
                      clock_IR_enable <='0';
		      Select_DR <= '1';
		               IS_SHIFTING <= '1';
			if (TMS = '1')     then 
			  --still one bit to shift
	                   ShiftDR <= '1';
                           clock_DR_enable <='1';
                           next_state <= Exit1_DR;
			 else 
	                   ShiftDR <= '1';
                            clock_DR_enable <='1'; --Shifting
                            next_state <= Shift_DR;
                      end if;
		 
                      
      when Exit1_DR  => qout := "00"; 
		      Select_DR <= '1';
		      clock_DR_enable <='1';
                      clock_IR_enable <='0';
                      --ShiftDR <= '1';
                      clock_IR_enable <='0';
                      if (TMS = '1')
                         then  next_state <= Update_DR;
                         else next_state <= Pause_DR;
                      end if;
                           
      when Pause_DR  => qout := "01"; 
		      Select_DR <= '1';
		      clock_DR_enable <='1';
                      clock_IR_enable <='0';
                      if (TMS = '1')
                         then next_state <= Exit2_DR;
                         else next_state <= current_state;
                      end if;
                           
      when Exit2_DR  => qout := "00"; 
		      Select_DR <= '1';
		      clock_DR_enable <='1';
                      clock_IR_enable <='0';
                      if (TMS = '1')
                         then next_state <= Update_DR;
                         else next_state <= Shift_DR;
                      end if;
                      
      when Update_DR  => qout := "00"; 
		      Select_DR <= '1';
		      clock_DR_enable <='1';
                      clock_IR_enable <='0';
                      UpdateDR <='1';
                      if (TMS = '1')
                         then next_state <= Select_DR_Scan;
                         else next_state <= Run_Test_Idle;
                      end if;
    end case;

    IS_IDLE <= qout(0);
  end process;

    clockDR <= TCK and clock_DR_enable;
    clockIR <= TCK and clock_IR_enable;

------------------------------------------------------------------------
--                                                                    --
-- Process in charge of logging received data                         --
--                                                                    --
-- As shifting can occur on any state, incoming data is detected      --
-- as TDI leaving '-'                                                 --
--                                                                    --
--  Data is saved in file ./outputs/tap_log.txt                                 --
--                                                                    --
-- NB: Post-synteshis simulation needs TDI to be brought to '-' when  --
--    non active to assure this process works!                        --
--                                                                    --
------------------------------------------------------------------------

log_seq: process (TRSTN, TCK)
  begin
    if (TRSTN = '0')
       then log_status <= idle;
       toggle <= '1';
    elsif (TCK'event and (TCK = '1'))
       then log_status <= next_log_status;
       toggle <=  not toggle;
    end if;
  end process;

log_fsm: process(log_status,toggle) is
 file log_file : TEXT open WRITE_MODE is "slave_tap_log.dat";
 variable out_line : LINE;
 variable i : integer;
 variable shift_state : tap_states;
 variable cur_bit : integer;
 begin
 case log_status is
  when idle =>
   if (current_state = Shift_IR) or (current_state = Shift_DR) then
--   if (TDI = '0') or (TDI = '1') then
     next_log_status <= active;
	shift_state := current_state;
	cur_bit := 1;
	 --Format line start
	 shift_state := current_state;
	 write(out_line,print_state(shift_state)&": ");
         write(out_line,chr(TDI));
  else
     next_log_status <= idle;
   end if;  
  when active =>
     if (current_state = Shift_IR) or (current_state = Shift_DR) then
--   if (TDI = '0') or (TDI = '1') then
	cur_bit := cur_bit+1;
       write(out_line,chr(TDI));
      next_log_status <= active;
     else
        writeline(log_file,out_line);
        cur_bit := 0;
        next_log_status <= idle;
     end if;
  end case;
 end process;

end behavior;
