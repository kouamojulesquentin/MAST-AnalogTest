
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_write.all;
use work.MAST_config.all;
use work.master_tapfsm_package.all;

package SVF_Master_TAP_package is
component SVF_Master_TAP 
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
end component;
end SVF_Master_TAP_package ;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_write.all;
use work.MAST_config.all;
use work.master_tapfsm_package.all;

Entity SVF_Master_TAP is
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
end SVF_Master_TAP;

Architecture behavior of SVF_Master_TAP is

  
 signal shifted :integer;

type vector_p is access bit_vector;
type string_p is access string;
signal start_scan  : std_logic := '0';
signal next_vector_length : integer;

signal data_to_scan  : std_logic;
signal data_from_scan: std_logic;

signal is_shifting    : std_logic;
signal finished       : std_logic;

   -- Clock period definitions
   constant Clk_period : time := 10 ns;

 constant polling_interval : time := 1 ms;

signal  Clk :  std_logic;
signal Resetn : std_logic;        

  signal old_state,next_old_state: extended_tap_states;

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

  function vec2str(vec: std_logic_vector) return string is
    variable stmp: string(vec'left+1 downto 1);
    begin
        for i in vec'reverse_range loop
            if (vec(i) = 'U') then
                stmp(i+1) := 'U';
            elsif (vec(i) = 'X') then
                stmp(i+1) := 'X';
            elsif (vec(i) = '0') then
                stmp(i+1) := '0';
            elsif (vec(i) = '1') then
                stmp(i+1) := '1';
            elsif (vec(i) = 'Z') then
                stmp(i+1) := 'Z';
            elsif (vec(i) = 'W') then
                stmp(i+1) := 'W';
            elsif (vec(i) = 'L') then
                stmp(i+1) := 'L';
           elsif (vec(i) = 'H') then
                stmp(i+1) := 'H';
           else
                stmp(i+1) := '-';
            end if;
        end loop;
    return stmp;
    end vec2str;
    			  
  function vec2str(vec: bit_vector) return string is
    variable stmp: string(vec'left+1 downto 1);
    begin
        for i in vec'reverse_range loop
            if (vec(i) = '0') then
                stmp(i+1) := '0';
            elsif (vec(i) = '1') then
                stmp(i+1) := '1';
           else
                stmp(i+1) := '-';
            end if;
        end loop;
    return stmp;
    end vec2str;			  

signal	tap_command: std_logic_vector(1 downto 0);

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
		                  tap_command <= "01";
		 when "SIR " =>   report "found an SIR";
		                  tap_command <= "10";								 when others =>  tap_command <= "00"; next read_loop;
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
--		report to_hstring(this_nibble);
		  k:=0;
		  while(k <4) loop
		  --one bit at a time to check for incomplete nibbles
		  if (vector_bit < vector_length) then
		   this_vector_p(vector_bit) := this_nibble(3-k);
--		   report "Bit "&integer'image(vector_bit)&":"&bit'image(this_nibble(3-k));
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
		   if (shifted < next_vector_length ) then
		    
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
 variable output_vector_debug : std_logic_vector(1 to 300);
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
	 output_vector_debug(next_vector_length-cur_bit):=TDO;
	end if;
	cur_bit := cur_bit+1;
      next_log_status <= active;
     else
        report "data_logger: received string "&output_vector_p(1 to next_vector_length);
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


TAP_master: master_tapfsm 
   port map(
	   Clk,
	   Resetn,
           start_scan,
	   --one-hot coding of possible commands
	  tap_command,
          next_vector_length,
	  finished,
	  shifted,
	 data_to_scan,
	 data_from_scan,
	 is_shifting,
	 old_state,
              TDI,
	    TCK,
	   TMS,
           TRSTN,
	  TDO
   );

 
end behavior;
