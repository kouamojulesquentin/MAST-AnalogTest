--
-- Copyright 1991-2012 Mentor Graphics Corporation
--
-- All Rights Reserved.
--
-- THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
-- MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
--   

library ieee;
use ieee.std_logic_1164.all;

package MAST_write is

    attribute foreign : string;

    procedure write_string(
      vhdl_array            : IN string
    );
    attribute foreign of write_string : procedure is "write_string MAST_write.so";

    procedure flush_string;
    attribute foreign of flush_string : procedure is "flush_string MAST_write.so";

    procedure init_target(
      filename            : IN string
    );
    attribute foreign of init_target : procedure is "init_target MAST_write.so";

    procedure set_n_log_files ( 
    new_n_log_files : IN integer
    );
    attribute foreign of set_n_log_files : procedure is "set_n_log_files MAST_write.so";
    
    function init_log_file(
      filename            : IN string
    ) return integer;
    attribute foreign of init_log_file : function is "init_log_file MAST_write.so";

   procedure write_log_line (
    n_log : IN integer;
    vhdl_array            : IN string
   ); 
  attribute foreign of write_log_line : procedure is "write_log_line MAST_write.so";
 
  signal logs_init_done : boolean := FALSE;
 
end;

package body MAST_write is

    procedure write_string(
      vhdl_array        : IN string
    ) is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
    end;

    procedure flush_string is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
    end;

    procedure init_target(
      filename            : IN string
    ) is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
    end;

    procedure set_n_log_files ( 
    new_n_log_files : IN integer
    )is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
    end;

    function init_log_file(
      filename            : IN string
    ) return integer is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
      return 0;
    end;

   procedure write_log_line (
    n_log : IN integer;
    vhdl_array            : IN string
   )is
    begin
      assert false report "ERROR: foreign subprogram not called" severity note;
    end; 

end;
