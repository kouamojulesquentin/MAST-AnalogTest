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
--use work.txt_util.all;
use work.MAST_write.all;
use work.MAST_config.all;

package exchange_registers is

component output_connection 
    generic ( output_dir : string := "./Matlab_exchange/";
              output_file : string := "cal_measure_1.txt";
	      data_size : integer := 8);
    port ( 
	  data_in : in std_logic_vector(data_size-1 downto 0);
	  Sel  : in  std_logic;
	  UP_en : in  std_logic
	   );       
end component;

component input_connection 
    generic ( input_dir : string := "./Matlab_exchange/";
              input_file : string := "cal_value_1.txt";
	      data_size : integer := 8);
    port ( 
	  data_out : out std_logic_vector(data_size-1 downto 0);
	  Sel  : in  std_logic;
	  CA_en : in  std_logic
	   );       
end component;

end;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_write.all;
use work.MAST_config.all;

Entity output_connection is
    generic ( output_dir : string := "./Matlab_exchange/";
              output_file : string := "cal_measure_1.txt";
	      data_size : integer := 8);
    port ( 
	  data_in : in std_logic_vector(data_size-1 downto 0);
	  Sel  : in  std_logic;
	  UP_en : in  std_logic
	   );       
end output_connection;

Architecture behavior of output_connection is

			  
begin


----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

data_logger: process(data_in,UP_en) is
 file log_file : TEXT open WRITE_MODE is output_dir&output_file;
 variable out_line, display : LINE;
 variable first_execution : integer:=0;
 variable log_idf : integer;
 begin
 if logs_init_done = TRUE then
   if (first_execution=0) then
    log_idf:=init_log_file(output_dir&output_file);
    first_execution := 1;
   end if;
 
   if (UP_en='1') and (Sel = '1') then
        write(out_line,"Data to log: "&str(data_in));
        write_log_line(log_idf,str(data_in));
        writeline(output,out_line);
   end if;	
  end if;
 end process;
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------


end behavior;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
library work;
use work.JTAG_package.all;
use STD.textio.all;
use work.txt_util.all;
use work.MAST_config.all;

Entity input_connection is
    generic ( input_dir : string := "./Matlab_exchange/";
              input_file : string := "cal_value_1.txt";
	      data_size : integer := 8);
    port ( 
	  data_out : out std_logic_vector(data_size-1 downto 0);
	  Sel  : in  std_logic;
	  CA_en : in  std_logic
	   );       
end input_connection;

Architecture behavior of input_connection is

			  
begin


----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

data_reader: process(Sel,CA_en) is
 file input_stream : TEXT open READ_MODE is input_dir&input_file;
 variable input_line, display : LINE;
 variable line_size : integer;
 variable data_string: string(data_size downto 1);
 variable data_read : std_logic_vector(data_size-1 downto 0);
 begin
   if (CA_en='1') and (Sel = '1') then
       readline(input_stream,input_line);
       line_size := input_line'high;
       read(input_line,data_string);
       for i in data_size downto 1 loop
         case (data_string(i)) is
	   when '0' =>data_read(i-1):= '0';
	   when '1' =>data_read(i-1):= '1';
	   when others =>data_read(i-1):= 'X';
	 end case;  
       end loop;
      data_out <= data_read;
   end if;	
 end process;
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------


end behavior;
