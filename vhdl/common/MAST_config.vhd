-- Package TAPFSM_package defines the constants and types used in TAPFSM
--  

Library IEEE;
use IEEE.STD_LOGIC_1164.all;

package MAST_config is
  ---IEEE1149.1 states 
  
 constant IR_SIZE : integer := 8; --common to all Use cases for simplicity

 constant MAX_DR_CHAINS : integer := 12;
 constant MAX_REGISTERS : integer := 20;
 constant MAX_LEVELS : integer := 10;
  
  type target_SUT_type is (LOOPBACK,TUTORIAL_1,IEEE1687,AMS);

  constant target_SUT :target_SUT_type :=TUTORIAL_1;
--  constant target_SUT :target_SUT_type :=LOOPBACK;
--  constant target_SUT :target_SUT_type :=IEEE1687;
--  constant target_SUT :target_SUT_type :=AMS;
 
  
  constant MAX_BS_REGISTERS : integer := 10;
end;
