--  
--  Filename    : internal_coding.vhd	                                          
--  Designer    : Michele Portolan
--						                                                                                                 --
--  Description: Internal coding used for registers and instructions
--	

Library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;
use work.JTAG_configuration.all;
library work;

package types is

type coding is (Binary, Binary_noidle,One_Hot,One_Hot_noidle);

end types;

