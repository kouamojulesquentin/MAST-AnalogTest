--  
--  Filename    : tapfsm.vhd	                                          
---						                                                                                                 --
--  Description: Finite State Machine 
--               This is designed to mirror the FSM in the TAP controller of the 
--               System Under Test and control in which state it is             
	   
--                       
--  Inputs: TCK, TMS, TRSTN
--  Output: TCK, TMS, TRSN       
--

Library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_config.all;

package master_tapfsm_package is

  component master_tapfsm
     port (
	   Clk : in std_logic;
	   Resetn : in std_logic;        
          start_scan  : in std_logic;
	   --one-hot coding of possible commands
	  tap_command: in std_logic_vector(1 downto 0);
          next_vector_length : in integer;
	  scan_finished : out std_logic;
	  current_bit : out integer;
	 data_to_scan  : in std_logic;
	 data_from_scan: out std_logic;
	 is_shifting: out std_logic;
         state_to_log: out extended_tap_states;


       	  TDI		: out std_logic;
			  TCK    : out std_logic;
			  TMS    : out std_logic;
           TRSTN  : out std_logic;
			  TDO		: in std_logic
	   );       
  end component;
end master_tapfsm_package;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_config.all;

Entity master_tapfsm is
    port ( 
	   Clk : in std_logic;
	   Resetn : in std_logic;        
            start_scan  : in std_logic;
	   --one-hot coding of possible commands
	  tap_command: in std_logic_vector(1 downto 0);
          next_vector_length : in integer;
	   scan_finished : out std_logic;
	  current_bit : out integer;
	 data_to_scan  : in std_logic;
	 data_from_scan: out std_logic;
	 is_shifting: out std_logic;
         state_to_log: out extended_tap_states;

              TDI		: out std_logic;
			  TCK : out std_logic;
			  TMS    : out std_logic;
           TRSTN  : out std_logic;
			  TDO		: in std_logic
	   );       
end master_tapfsm;

Architecture behavior of master_tapfsm is

  signal current_state: extended_tap_states;  
  signal next_state: extended_tap_states;

  signal old_state,next_old_state: extended_tap_states;

  signal new_start_state,new_shift_state,new_end_state : tap_states;

  signal start_state,shift_state,end_state: tap_states;  
  signal next_start_state,next_shift_state,next_end_state,in_shift_state: tap_states;
                                        
  signal shift_pause : std_logic;
  signal to_TLR, to_RTI : std_logic;
  
  signal traverse_DR_IR : std_logic_vector(1 downto 0);

  type ctrl_states is (idle, reach_start,reach_shift,shift, reach_end,end_reached);
  
  signal current_ctrl,next_ctrl : ctrl_states;
  signal enable_TCK,enable_TAPFSM : std_logic;
  
  signal int_errorn,next_int_errorn : std_logic;
  signal int_TMS,next_int_TMS : std_logic;
  signal next_enable_TCK,delayed_enable_TCK : std_logic;
  signal int_to_TDI : std_logic;
  
  signal scanlength, next_scanlength :integer;
  signal shifted, next_shifted :integer;
  signal shifting : std_logic;

 signal load_scanlength,load_state : std_logic;
  
 function set_traverse(state : in tap_states) return std_logic_vector is
  variable traverse_DR, traverse_IR : std_logic;
  begin 
    if (state >= Select_IR_Scan) and (state <= Update_IR) then
    	traverse_IR := '1';
    	traverse_DR := '0';
    else
    --defaut: traverse DR
    	traverse_DR := '1';
	traverse_IR := '0';
    end if;
    
    return traverse_DR & traverse_IR;
  end;
 
  --aliases for cleaner and more readable code
alias from_TDO        is TDO;
alias to_TDI  	      is int_to_TDI;

--1 clock delay element to synchronise TDI output
--with slave state machine
signal TDI_delay,next_TDI_delay : std_logic;

type vector_p is access bit_vector;
type string_p is access string;

signal busy           : std_logic;
signal errorn         : std_logic;
--signal is_shifting    : std_logic;
signal finished       : std_logic;
signal shift_no_data   : std_logic:='0';
   -- Clock period definitions

		  
begin
scan_finished <= finished;
current_bit <= shifted;
state_to_log <= old_state;

cmd_selector:process(tap_command)
 begin
  shift_no_data <= '0';
  
  case (tap_command) is
        when "01" => --SDR
		new_start_state <= Run_Test_Idle;
		new_shift_state <= Shift_DR;
		new_end_state   <= Run_Test_Idle;
        when "10" => --SIR
		new_start_state <= Run_Test_Idle;
		new_shift_state <= Shift_IR;
		new_end_state   <= Run_Test_Idle;
        when others =>
		new_start_state <= Run_Test_Idle;
		new_shift_state <= Run_Test_Idle;
		new_end_state   <= Run_Test_Idle;
  end case;
 end process;



------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------


  data_from_scan <= from_TDO;
  
 next_TDI_delay <=  '0' when shift_no_data='1' else int_to_TDI;  
 
  --ASSIGNING TAP SIGNALS
 TDI <= TDI_delay;
-- TDI <= '0' when shift_no_data='1' else int_to_TDI; 
 TCK <= Clk and next_enable_TCK;
 
 --delayed half a clock cyle
 --to assure data is stable on rising edge of TCK
delay: process (Resetn, Clk)
  begin
    if (Resetn = '0')
       then 
	  TDI_delay <= '0';
    elsif (Clk'event and (Clk = '1'))
       then 
	  TDI_delay <= next_TDI_delay;
    end if;
  end process;
   
  sequential_fsm: process (Resetn, Clk)
  begin
    if (Resetn = '0')
       then 
          current_state <= First_state;
	  old_state <= First_state; 
  --what was the use of "First_state"?
    elsif (Clk'event and (Clk = '1') and (enable_TAPFSM = '1'))
       then 
          current_state <= next_state;
	  old_state <= next_old_state;
    end if;
  end process;
  
  next_old_state <= current_state;
  
  sequential_ctrl: process (Resetn, Clk)
  begin
    if (Resetn = '0')
       then 
	  start_state <= Run_Test_Idle;
	  shift_state <= Shift_DR;
	  end_state   <= Run_Test_Idle;
	  int_errorn  <= '1';
	  int_TMS <= '1';
	  enable_TCK <= '0';
          delayed_enable_TCK <= '0';
	  scanlength <= 0;
    elsif (Clk'event and (Clk = '1'))
       then 
	  start_state <= next_start_state;
	  shift_state <= next_shift_state;
	  end_state   <= next_end_state;
	  int_TMS <= next_int_TMS;
	  enable_TCK <= next_enable_TCK; 
	  delayed_enable_TCK <= enable_TCK; 
	  scanlength <= next_scanlength;
	  if (load_state = '1' ) then
	    int_errorn  <= '1'; --reset errorn when shift is updated
	  else 
	    int_errorn  <= next_int_errorn;
	  end if;  
    end if;
  end process;
 
  sequential_counter: process (Resetn, Clk)
   begin
    if (Resetn = '0') then 
     shifted <= 0;
    elsif (Clk'event and (Clk = '1')) then 
      shifted <= next_shifted;
    end if;	  
   end process;

  --TMS is buffered to avoid glitches 
  --the result is a one-cycle delay in the commanded TAP FSM
 TMS <= int_TMS;
 errorn <= int_errorn;

  ctrl_sequ: process (Resetn, Clk)
  begin
    if (Resetn = '0')
       then 
          current_ctrl <= idle;
	  --shift_pause <= '0'; 
    elsif (Clk'event and (Clk = '1')) then
          current_ctrl <= next_ctrl; 
	  --shift_pause <= next_shift_pause;
    end if;
  end process;

 in_shift_state <= new_shift_state;

 combinatorial_ctrl: process(current_ctrl, current_state,start_scan, 
                             next_state,shifted,data_to_scan)
 --Process charged of controlling the TAP FSM
 begin
   next_int_errorn  <= int_errorn;
   load_scanlength  <= '0';
   load_state       <= '0';
  -- to_TDI <= '-'; --commented to avoid X in VCS
   to_TDI <= '0';
   shifting <= '0';
   is_shifting <= '0';
   finished <= '0';

   next_shifted <= shifted;

   if (int_errorn = '1') then
   case current_ctrl is
    when  idle => 
    	   busy <= '0';
	   next_enable_TCK <= '0';
           enable_TAPFSM <= '0';
           next_shifted <= 0;
	   
	   if (start_scan = '1') then
	     if (is_allowed_shift(in_shift_state)=false) then 
	       next_ctrl <= idle;
	       next_int_errorn  <= '0'; --raise error
	      else
	     busy <= '1'; --setting busy for data_arbiter
             load_scanlength  <= '1';
             load_state       <= '1';
	       if (shift_state = current_state) then
	         next_ctrl <= shift;
	       else 
	        if (start_state /= shift_state) then
	          next_ctrl <= reach_start;
		  busy <= '1';
	        else	 
	           next_ctrl <= reach_shift;
	        end if;  
	       end if;
	       traverse_DR_IR <= set_traverse(start_state);
               shift_pause <= is_shift_pause(start_state); 
	       to_TLR <= is_TLR(start_state);
	       to_RTI <= is_RTI(start_state);
	     end if;
	   else
	     traverse_DR_IR <= (others => '0');
	     shift_pause <= '0';
             next_ctrl <= idle;
	     to_TLR <= '0';
	     to_RTI <= '0';
	   end if;    
  when  reach_start => 
    	   busy <= '1';
	   next_enable_TCK <= '1';
	   enable_TAPFSM <= '1';
             if (current_state /= start_state) then
	        traverse_DR_IR <= set_traverse(start_state);
		shift_pause <= is_shift_pause(start_state); 
	        to_TLR <= is_TLR(start_state);
  	        to_RTI <= is_RTI(start_state);
                next_ctrl <= reach_start;
	     else
                shift_pause <= is_shift_pause(shift_state); 
	        to_TLR <= is_TLR(shift_state);
  	        to_RTI <= is_RTI(shift_state);
                traverse_DR_IR <= set_traverse(shift_state);
	        next_ctrl <= reach_shift;  
	     end if; 
    when  reach_shift => 
    	   busy <= '1';
	   next_enable_TCK <= '1';
           enable_TAPFSM <= '1';
	   shift_pause <= is_shift_pause(shift_state); 
           to_TLR <= is_TLR(shift_state);
  	   to_RTI <= is_RTI(shift_state);
	   
	   if (next_state /= shift_state) then
	      traverse_DR_IR <= set_traverse(shift_state);
	      next_ctrl <= current_ctrl;
	     else
	      traverse_DR_IR <= (others => '0');
	      next_ctrl <= shift;  
	        --Prepare to shift first bit
	      next_shifted <= 0;
	      --scanning data read from file
--	      to_TDI <= data_to_scan;	      
--	     shifting <= '1';
--	     is_shifting <= '1';
	   end if; 
	   
    when  shift => 
	   busy <= '1';
	   enable_TAPFSM <= '1';
           --Stop traversal and do shift
	   traverse_DR_IR <= (others => '0');
	   
	   next_enable_TCK <= '1';

	   shift_pause <= is_shift_pause(end_state); 
           to_TLR <= is_TLR(end_state);
  	   to_RTI <= is_RTI(end_state);
	   
	      --do shift
	      --NB: one=cycle delay not introduced yet!
	   next_shifted <= shifted+1;

	      --scanning data read from file
	      to_TDI <= data_to_scan;	      

	     shifting <= '1';
	     is_shifting <= '1';
	   
	   if (shifted = scanlength-1) then
	     --shift last bit and change state
 	    if (current_state /= end_state) then
	      traverse_DR_IR <= set_traverse(end_state);
	      next_ctrl <= reach_end;
	     else
	      enable_TAPFSM <= '0';
	      traverse_DR_IR <= (others => '0');
	      next_ctrl <= idle; 
	      finished <= '1'; 
	     end if;
	    else
	     --continue shifting
	      next_ctrl <= shift;
           end if; 
	   
    when  reach_end => 
 	   next_shifted <= shifted+1; --left accumulating to help file read process
    	   busy <= '1';
	   next_enable_TCK <= '1';
	   enable_TAPFSM <= '1';
	   shift_pause <= is_shift_pause(end_state); 
           to_TLR <= is_TLR(end_state);
  	   to_RTI <= is_RTI(end_state);
           if (next_state /= end_state) then
              traverse_DR_IR <= set_traverse(end_state);
	      next_ctrl <= current_ctrl;
           else
	      traverse_DR_IR <= (others => '0');
	      next_ctrl <= end_reached;
	      finished <= '1';   
	   end if; 

 --state to allow one extra TCK cycle
    when  end_reached => 
	      traverse_DR_IR <= (others => '0');
	      next_ctrl <= idle;
	   
   end case;
  end if;
 end process;
 
 --Processes charged of updating the state registers

 scanlength_ctrl: process(load_scanlength,scanlength,next_vector_length)
 begin
  next_scanlength  <= scanlength;	  
   if (load_scanlength = '1') then
	   next_scanlength <= next_vector_length;
   end if;

  end process;
 
 scanstates_ctrl: process(load_state, start_state,shift_state,end_state,
                           new_start_state,new_shift_state,new_end_state)
 begin
   if (load_state = '1') then
	
          next_start_state <= new_start_state;
          next_shift_state <= new_shift_state;
          next_end_state   <= new_end_state;
   else
          next_start_state <= start_state;
          next_shift_state <= shift_state;
          next_end_state   <= end_state;
   end if;
  end process;


  combinational_tap: process (traverse_DR_IR, current_state,enable_TAPFSM)
  --
  -- 17 states FSM used to mirror and control a standard 16-state 1149.1 TAP FSM
  --
  variable traverse_DR, traverse_IR : std_logic;
  begin
   TRSTN <= '1';   
   next_state <= current_state;
   next_int_TMS <= int_TMS;

    traverse_DR := traverse_DR_IR (1);
    traverse_IR := traverse_DR_IR (0);

   if (enable_TAPFSM = '1') then
    case current_state is
      when     First_state  =>
         TRSTN <= '0'; 
         next_state <= Test_Logic_Reset;
         next_int_TMS <= '1';

      when Test_Logic_Reset  =>
	      if (traverse_IR = '1') or (traverse_DR = '1') then
                        next_int_TMS <= '0';
                        next_state <= Run_Test_Idle;
                      else 
                        next_int_TMS <= '1';
                        next_state <= current_state;
                      end if;
                           
      when Run_Test_Idle  =>
		      if (traverse_IR = '1') or (traverse_DR = '1') 
                         then 
			    next_int_TMS <= '1';
			    next_state <= Select_DR_Scan;
                         else 
			    next_int_TMS <= '0';
			    next_state <= current_state;
                      end if;
                      
      when Select_DR_Scan  =>
		      if (traverse_IR = '1') or (to_TLR = '1')
                         then
			   next_int_TMS <= '1'; 
			   next_state <= Select_IR_Scan;
                         else 
			     next_int_TMS <= '0';
			     next_state <= Capture_DR;
                      end if;
                      
      when Select_IR_Scan  =>
		      if (traverse_IR = '1') or (to_TLR = '0')
			 then 
			    next_int_TMS <= '0';
			    next_state <= Capture_IR;
                         else 
			    next_int_TMS <= '1';
			    next_state <= Test_Logic_Reset;
                      end if;
		           
      when Capture_IR  => 
		      if (shift_pause = '0') or (traverse_DR = '1')
                         then 
			    next_int_TMS <= '1';
			    next_state <= Exit1_IR;
                         else 
			    next_int_TMS <= '0';
			    next_state <= Shift_IR;
                      end if;
		      
      when Shift_IR  => 
		      if (traverse_IR = '1') or (traverse_DR = '1')
                         then 
			    next_int_TMS <= '1';
			    next_state <= Exit1_IR;
                         else 
			    next_int_TMS <= '0';
			    next_state <= current_state;
                      end if;
                      
      when Exit1_IR  => 
		      if (shift_pause = '0')  or (traverse_DR = '1')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Update_IR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Pause_IR;
                      end if;
                           
      when Pause_IR  => 
		      if (traverse_IR = '1') or (traverse_DR = '1')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Exit2_IR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= current_state;
                      end if;
                           
      when Exit2_IR  => 
		      if (shift_pause = '0')  or (traverse_DR = '1')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Update_IR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Shift_IR;
                      end if;
                      
      when Update_IR  =>
		      if (to_RTI = '0')
                         then 
			    next_int_TMS <= '1';
			    next_state <= Select_DR_Scan;
                         else 
			    next_int_TMS <= '0';
			    next_state <= Run_Test_Idle;
                      end if;
                      
      when Capture_DR  =>
		      if (shift_pause = '0') or (traverse_IR = '1')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Exit1_DR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Shift_DR;
                      end if;
                      
      when Shift_DR  => 
		      if (traverse_IR = '1') or (traverse_DR = '1')
                         then 
                            next_int_TMS <= '1';
			    next_state <= Exit1_DR;
                         else
			    next_int_TMS <= '0'; 
                            next_state <= current_state;
                      end if;
                      
      when Exit1_DR  => 
		      if (shift_pause = '0') or (traverse_IR = '1') 
                         then  
			   next_int_TMS <= '1';
			   next_state <= Update_DR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Pause_DR;
                      end if;
                           
      when Pause_DR  => 
		      if (traverse_IR = '1') or (traverse_DR = '1')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Exit2_DR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= current_state;
                      end if;
                           
      when Exit2_DR  => 
		      if (shift_pause = '0') or (traverse_IR = '1') 
                         then 
			   next_int_TMS <= '1';
			   next_state <= Update_DR;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Shift_DR;
                      end if;
                      
      when Update_DR  =>
		      if (to_RTI= '0')
                         then 
			   next_int_TMS <= '1';
			   next_state <= Select_DR_Scan;
                         else 
			   next_int_TMS <= '0';
			   next_state <= Run_Test_Idle;
                      end if;
    end case;
   else --(enable_TAPFSM = '0') 
    if ( current_state = First_state) then
         TRSTN <= '0'; 
         next_state <= Test_Logic_Reset;
         next_int_TMS <= '1';
    end if;
    
   end if;
  end process;


 
end behavior;
