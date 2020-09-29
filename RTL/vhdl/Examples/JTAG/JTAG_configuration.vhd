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

package JTAG_configuration is

----------------------------------------------------------------------------
-- log2 tables
----------------------------------------------------------------------------

type log2arr is array(1 to 64) of integer;
constant log2  : log2arr := (0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
				5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,others => 6);
constant log2x : log2arr := (1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
				5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,others => 6);

--------------------------------------------------------
--PIPPO configuration options and values
--------------------------------------------------------

constant SRAM_WORD_SIZE : integer := 8;

constant word_size : natural := 32;
constant word_nbytes : natural := word_size / 8;

constant immediate_width: natural := 22;

constant SRAM_size_kb : integer := 32;
constant SRAM_addr_width : integer := 16;

type tech is (general, simulation, series_7);

constant target  : tech := series_7;
--constant target_string : string :=  "7SERIES";
constant target_string : string :=  "SPARTAN6";
--constant target  : tech := simulation;

-------------------------------------------------
-- This is the ration betwen the system clock  --
-- and the (slower) TCK, used by the data      --
-- arbiter to synchronise the two clock domains--
-------------------------------------------------
constant TCK_ratio : integer := 5; --Max: 11, after only zeros are scanned out
				    -- 6 does not work....check synch of SRAM
				    --accesses
end JTAG_configuration;

