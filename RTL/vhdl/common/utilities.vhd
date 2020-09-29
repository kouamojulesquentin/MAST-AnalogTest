--  
--  Filename    : utilities.vhd	                                          
--  Designer    : Michele Portolan
--						                                                                                                 --
--  Description: Miscellanous types and functions for easier coding
--	

Library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;
library work;

package utilities is

  FUNCTION log2roundup (
      data_value : INTEGER)
    RETURN INTEGER;
end utilities;

package body utilities is
  FUNCTION log2roundup (
      data_value : INTEGER)
    RETURN INTEGER IS

      VARIABLE width       : INTEGER := 0;
      VARIABLE cnt         : INTEGER := 1;
    BEGIN
      IF (data_value <= 1) THEN
        width   := 1;
      ELSE
        WHILE (cnt < data_value) LOOP
          width := width + 1;
          cnt   := cnt *2;
        END LOOP;
      END IF;

      RETURN width;
   END log2roundup;
end;
