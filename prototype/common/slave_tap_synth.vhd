--  
--  Filename    : slave_TAP_synth.vhd	                                          
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

package slave_TAP_synth_package is

  component slave_TAP_synth
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
end slave_TAP_synth_package;

library IEEE;
use IEEE.std_logic_1164.all;
use work.JTAG_package.all;

Entity slave_TAP_synth is
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
end slave_TAP_synth;

Architecture behavior of slave_TAP_synth is
  --
  -- 16 states TAP FSM
  --

  signal current_state: tap_states;  
  signal next_state: tap_states;
  signal clock_DR_enable,clock_IR_enable : std_logic := '0'; 
 
 signal received_data : std_logic_vector(31 downto 0);                                       

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

end behavior;
