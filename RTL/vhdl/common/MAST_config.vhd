-- Package TAPFSM_package defines the constants and types used in TAPFSM
--  

Library IEEE;
use IEEE.STD_LOGIC_1164.all;

package MAST_config is
  ---IEEE1149.1 states 
  

 constant MAX_DR_CHAINS : integer := 12;
 constant MAX_REGISTERS : integer := 20;
 constant MAX_LEVELS : integer := 10;
  
  type target_SUT_type is (LOOPBACK,TUTORIAL_1, MIB_tutorial,SSAK_Example, Trivium_Streamer, Double_Trivium_Streamer, Nested_Trivium_Streamer,Secure_Scan);

 -- To select a target System Under Test, uncomment the related line and comment
 -- all others

-- constant target_SUT :target_SUT_type :=MIB_tutorial;
--  constant target_SUT :target_SUT_type :=TUTORIAL_1;
--  constant target_SUT :target_SUT_type :=LOOPBACK;
--  constant target_SUT :target_SUT_type :=SSAK_Example;
--  constant target_SUT :target_SUT_type :=Trivium_Streamer;
--  constant target_SUT :target_SUT_type :=Double_Trivium_Streamer;
--  constant target_SUT :target_SUT_type := Nested_Trivium_Streamer;
  constant target_SUT :target_SUT_type := Secure_Scan;

 constant IR_SIZE : integer := 4;
  
  constant MAX_BS_REGISTERS : integer := 10;
end;
