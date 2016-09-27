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
use work.MAST_write.all;
use work.MAST_config.all;

package Master_TAP_package is

  component Master_TAP
    generic ( input_SVF_dir : string := "./inputs/";
              input_SVF_file : string := "data_to_shift.svf";
             output_SVF_dir : string := "./outputs/";
             output_SVF_file : string := "data_from_SUT.dat");
    port (
       	  TDI		: out std_logic;
			  TCK    : out std_logic;
			  TMS    : out std_logic;
           TRSTN  : out std_logic;
			  TDO		: in std_logic
	   );       
  end component;
end Master_TAP_package;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_write.all;
use work.MAST_config.all;

Entity Master_TAP is
    generic ( input_SVF_dir : string := "./inputs/";
              input_SVF_file : string := "data_to_shift.svf";
             output_SVF_dir : string := "./outputs/";
             output_SVF_file : string := "data_from_SUT.dat");
    port ( TDI		: out std_logic;
			  TCK : out std_logic;
			  TMS    : out std_logic;
           TRSTN  : out std_logic;
			  TDO		: in std_logic
	   );       
end Master_TAP;

Architecture behavior of Master_TAP is

  signal current_state: extended_tap_states;  
  signal next_state: extended_tap_states;

  signal old_state,next_old_state: extended_tap_states;

  signal start_state,shift_state,end_state: tap_states;  
  signal next_start_state,next_shift_state,next_end_state,in_shift_state: tap_states;
                                        
  signal new_start_state,new_shift_state,new_end_state : tap_states;

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
  signal shifting,reset_shifted : std_logic;

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
signal start_scan  : std_logic := '0';
signal next_vector_length : integer;

signal data_to_scan  : std_logic;
signal data_from_scan: std_logic;

signal busy           : std_logic;
signal errorn         : std_logic;
signal is_shifting    : std_logic;
signal finished       : std_logic;
signal shift_no_data   : std_logic:='0';
   -- Clock period definitions
   constant Clk_period : time := 10 ns;

 constant polling_interval : time := 1 ms;

signal  Clk :  std_logic;
signal Resetn : std_logic;        

  type log_status_type is (idle,active);
 signal log_status, next_log_status : log_status_type;
 signal toggle : std_logic;
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

function hex_to_bin(c: character) return bit_vector is 
    variable nibble: bit_vector(0 to 3);
    begin
      case c is
        when '0' => 
            nibble := "0000";
        when '1' => 
            nibble := "0001";
        when '2' => 
            nibble := "0010";
        when '3' => 
            nibble := "0011";
        when '4' => 
            nibble := "0100";
        when '5' => 
            nibble := "0101";
        when '6' => 
            nibble := "0110";
        when '7' => 
            nibble := "0111";
        when '8' => 
            nibble := "1000";
        when '9' => 
            nibble := "1001";
        when 'A' => 
            nibble := "1010";
        when 'B' => 
            nibble := "1011";
        when 'C' => 
            nibble := "1100";
        when 'D' => 
            nibble := "1101";
        when 'E' => 
            nibble := "1110";
        when 'F' => 
            nibble := "1111";
        when others =>
            nibble := "0000";
    end case;
   return nibble;
  end hex_to_bin;
			  
begin

   --reset
    Resetn <= '0', '1' after 100 ns;
	 -- Clock process definitions
   Clk_process :process
   begin
		Clk <= '0';
		wait for Clk_period/2;
		Clk <= '1';
		wait for Clk_period/2;
   end process;

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
--Reading from a text file to find SVF commands
 --as of now, only SDR and SIR are supported
   SVF_interpreter: process
   file gen_file : TEXT open READ_MODE is input_SVF_dir&input_SVF_file;
--   file gen_file : TEXT open READ_MODE is "./inputs/data_to_shift.txt";
   variable input_line : LINE;
   variable data_to_send: string(32 downto 1);
	variable line_size : integer;
	
   variable command: string(4 downto 1);
	variable whitespace : character;
	variable read_ok : boolean;
	variable vector_length : integer;
	variable n_bytes : integer;
	variable this_bit : bit;
 	variable this_vector_p : vector_p;
 	variable vector_hex_p : string_p;
	variable this_nibble : bit_vector(0 to 3);
	variable i,k: integer;
	variable vector_bit: integer;
	variable cur_nibble: integer;
    begin		
   	start_scan <= '0';  
		wait until resetn = '1';
	  read_loop: loop	
	  polling_loop: while (endfile(gen_file)) loop
	          --wait for a new SVF instruction
		  wait for polling_interval;
	         end loop;
      
                readline(gen_file,input_line);
		line_size := input_line'high;
		if line_size = 0 then
		 next read_loop;
		end if;
		report "Read a line of size " & integer'image(line_size);
		read(input_line,command);
		wait for Clk_period*4;
		case (command) is
		 when "SDR " =>   report "found an SDR";
							new_start_state <= Run_Test_Idle;
							new_shift_state <= Shift_DR;
							new_end_state   <= Run_Test_Idle;
		 when "SIR " =>   report "found an SIR";
							new_start_state <= Run_Test_Idle;
							new_shift_state <= Shift_IR;
							new_end_state   <= Run_Test_Idle;
		 when others =>  next read_loop;
		end case;
		
		read(input_line,vector_length,read_ok);
		if (read_ok = FALSE) then
		  next read_loop;
      end if;		
		   report "Vector size is "&integer'image(vector_length);
      
		read(input_line,command);
		if (command /= " TDI" ) then
		  next read_loop;
      end if;
		read(input_line,whitespace);
		if (whitespace /= '(' ) then
		  next read_loop;
      end if;
		
		report "Reading TDI vector";

		this_vector_p := new bit_vector(0 to vector_length-1);
		vector_bit := 0;

                n_bytes := vector_length/8;
		if (vector_length mod 8 /= 0) then n_bytes :=n_bytes+1; end if;
		vector_hex_p := new string(1 to n_bytes*2);
		read(input_line,vector_hex_p(1 to n_bytes*2),read_ok);
		
		--convert Hex to binary vector
		--NB: in SVF, the rightmost bit is shifted first, so we have to start from the last nibble
		i := n_bytes*2;
		while (i >0) loop
		  cur_nibble := i;
		  this_nibble := hex_to_bin(vector_hex_p(cur_nibble));
		  k:=0;
		  while(k <4) loop
		  --one bit at a time to check for incomplete nibbles
		  if (vector_bit < vector_length) then
		   this_vector_p(vector_bit) := this_nibble(3-k);
		   vector_bit := vector_bit+1;
		   end if;
		  k:=k+1;
		  end loop;	   
		  i:=i-1;
		end loop;

		report "TDI vector read";
		
     	--check for correct SVF format
		read(input_line,whitespace);
		if (whitespace /= ')' ) then
		report "Error in SVF format, expected ) found "&whitespace;
		 next read_loop;
      end if;
     	read(input_line,whitespace);
		if (whitespace /= ';' ) then
		report "Error in SVF format, expected ; found "&whitespace;
		  next read_loop;
      end if;
		
		report "SVF command read successfully";
		
		
		------------------------------------
		--A new SVF instruction is ready to be executed:
		     --prepare scan length
		     next_vector_length <= vector_length;
		     --scan states already assigned: nothing to do
			  --prepare data to be scanned
		  
		  --toggle start_scan to start a scan operation
		  start_scan <= '1';  
		  wait for Clk_period *2;
		  start_scan <='0';
		  
		  loop
		   if (shifted < scanlength ) then
		    
		     data_to_scan <= to_stdulogic(this_vector_p(shifted));
		   end if;
			wait on shifted;
			if (finished = '1') then
			 exit;
			end if; 
		  end loop;
		  --wait for end before reading next line
     end loop; 
     wait;
   end process;
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
  --Data receveid during SHFIT_DR or SHIFT_IR from the SUT is logged one scan cycle per line
   --saved during FALLING EDGE to assure signals have been updated
log_seq: process (Resetn, Clk)
  begin
    if (Resetn = '0')
       then log_status <= idle;
       toggle <= '1';
    elsif (Clk'event and (Clk = '0'))
       then log_status <= next_log_status;
       toggle <=  not toggle;
    end if;
  end process;

--the master FSM is actually one cycle ahead of TMS as it generates it
 --for logging we therefore need to delay the state

data_logger: process(log_status,toggle) is
 file log_file : TEXT open WRITE_MODE is output_SVF_dir&output_SVF_file;
 variable display : LINE;
 variable i : integer;
 variable shift_state : tap_states;
 variable cur_bit : integer;
 variable first_execution : integer:=0;
 variable output_vector_p : string_p;
  begin
 if (first_execution=0) then
   init_target(output_SVF_dir&output_SVF_file);
   first_execution := 1;
 end if;
 case log_status is
  when idle =>
   if (old_state = Shift_IR) or (old_state = Shift_DR) then
     next_log_status <= active;
	shift_state := old_state;
	cur_bit := 1;
	 --Format line start
	 shift_state := old_state;
	 output_vector_p := new string(1 to next_vector_length);
	 for i in 1 to next_vector_length loop
	     output_vector_p(i) := 'X'; end loop;
	 --Writing first bit
	output_vector_p(next_vector_length):=chr(TDO);
  else
     next_log_status <= idle;
   end if;  
  when active =>
   if (old_state = Shift_IR) or (old_state = Shift_DR) then
	if (next_vector_length-cur_bit>0) then
	 output_vector_p(next_vector_length-cur_bit):=chr(TDO);
	end if;
	cur_bit := cur_bit+1;
      next_log_status <= active;
     else
        write_string(output_vector_p(1 to next_vector_length));
	flush_string;
        cur_bit := 0;
        next_log_status <= idle;
     end if;
  end case;
 end process;
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

  
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------



  data_from_scan <= from_TDO;
  
 next_TDI_delay <=  '0' when shift_no_data='1' else int_to_TDI;  
 
  --ASSIGNING TAP SIGNALS
 TDI <= TDI_delay;
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
   to_TDI <= '-';
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
