--  
--  Filename    : internal_coding.vhd	                                          
--  Designer    : Michele Portolan
--						                                                                                                 --
--  Description: Internal coding used for PIPPO registers and instructions
--	

Library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;
use work.JTAG_configuration.all;
library work;

package types is


constant reg_depth : integer := 32;
constant word_size : integer := 32;

type dp_memory_in_type is record
        ncsA      : std_logic;        -- not chip select 1
        addrA     : std_logic_vector( SRAM_addr_width-1 downto 0 );
        data_inA  : std_logic_vector( word_size-1 downto 0 );
        nweA      : std_logic;        -- not write enable
        noeA      : std_logic;        -- not output enable 
        ncsB      : std_logic;        -- not chip select 1
        addrB     : std_logic_vector( SRAM_addr_width-1 downto 0 );
        data_inB  : std_logic_vector( word_size-1 downto 0 );
        nweB      : std_logic;        -- not write enable
        noeB      : std_logic;        -- not output enable 
end record;

type dp_memory_out_type is record
        data_outA : std_logic_vector( word_size-1 downto 0 );
        data_outB : std_logic_vector( word_size-1 downto 0 );
end record;


type tapfsm_in_type is record
	   data_to_scan : std_logic;
	   --1149.1 TAP signals
	   from_TDO     :   std_logic;
	   --Control signals from PIPPO IU
	   start_scan    : std_logic;
	   scan_register :std_logic;
	   shift_no_data: std_logic;
	   --Register data from arbiter
	   reg_scanlength :  std_logic_vector(word_size-1 downto 0);
	   reg_scanstates :  std_logic_vector(word_size-1 downto 0);
	   input_scan_reg :  std_logic_vector(word_size-1 downto 0);
end record;

type tapfsm_out_type is record
	   data_from_scan : std_logic;
	   --1149.1 TAP signals
	   TCK        : std_logic;        
           TMS        : std_logic;        
           TRSTN      : std_logic;
	   to_TDI     : std_logic;
	   busy       : std_logic; 
	   errorn     : std_logic;
	   is_shifting   : std_logic;
	   finished   : std_logic;
	   --Register control signals for arbiter
	   read_scanlength : std_logic;
	   read_scanstates : std_logic;
	   --debug signals
	   cur_tapctrl_state : std_logic_vector(2 downto 0);
	   cur_tapfsm_state : std_logic_vector(4 downto 0);
end record;

type reg_arbiter_in_type is record
	IU_busy       : std_logic; --IU is working
	scan_register : std_logic;
	--registers used for register-based scan operations
	source		:  std_logic_vector(reg_depth-1 downto 0) ;
	destination	:  std_logic_vector(reg_depth-1 downto 0) ;
	mask		:  std_logic_vector(reg_depth-1 downto 0) ;
	expected	:  std_logic_vector(reg_depth-1 downto 0) ;
	--TAP-related signals
	TAP_shifting   : std_logic;
	--IO Registers
       data_to_ram : std_logic_vector(WORD_SIZE-1 downto 0);
	
end record;

type reg_arbiter_out_type is record
	--IO Register control
       data_from_ram : std_logic_vector(WORD_SIZE-1 downto 0);
end record;

type obuf_in_type is record
  data_from_ram  : std_logic_vector(WORD_SIZE-1 downto 0);        
  resetn	 : std_logic;        
  clk		 : std_logic; 
  new_data 	 : std_logic; --new data coming from ram
  oe 		 : std_logic; --send data to tap
end record;

type obuf_out_type is record
  new_set       : std_logic; --active register have changed
  data_to_tap	: std_logic;       
 end record;

type ibuf_in_type is record
   data_from_tap : std_logic;        
   resetn	 : std_logic;        
   clk		 : std_logic; 
   new_data 	 : std_logic; --new data coming from tap
end record;

type ibuf_out_type is record
   oe 		 : std_logic; --data ready for ram
   data_to_ram   : std_logic_vector(WORD_SIZE-1 downto 0);
 end record;

end types;

