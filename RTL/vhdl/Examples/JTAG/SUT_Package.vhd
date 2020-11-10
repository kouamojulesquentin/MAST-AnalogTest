--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:40:57 11/02/2015
-- Design Name:   
-- Module Name:   ./vhd/SUT_Package
-- Project Name:  MAST_JTAG
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use STD.textio.all;

library work;
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.exchange_registers.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
package SUT_Package is

component tutorial_1_testcase 
 port  ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end component;

component MIB_tutorial_testcase 
 port  ( clk   : in  std_logic;
     rst  : in  std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end component;

component topTestDesing 
	Port (
		tclk : in std_logic;
		rstn : in std_logic;
		
		CSU_SE : in  std_logic;
		CSU_CE : in  std_logic;
		CSU_UE : in  std_logic;
		CSU_Select : in  std_logic;
		
		TDI : in  std_logic;
		TDO : out std_logic
		
--		KS : out std_logic;
--		KSV : out std_logic
	);
end component;

component trivium_testcase
 port  ( clk   : in  std_logic;
     rst  : in  std_logic;
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end component;

component double_trivium_testcase
 port  ( clk   : in  std_logic;
     rst  : in  std_logic; --Chain Reset
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end component;

component nested_trivium_testcase
 port  ( clk   : in  std_logic;
     rst  : in  std_logic; --Chain Reset
     SysResetn: in std_logic; --System Level Reset for Trivium Initialization
     Trivium_ready : out std_logic;
     TDI   : in  std_logic;
     TDO   : out std_logic;
     mode  : in  std_logic;
     SH_en : in  std_logic;
     CA_en : in  std_logic;
     UP_en : in  std_logic;
     Sel   : in  std_logic
   );
end component;

component scan_secure
	Port (
		tclk : in std_logic;
		rstn : in std_logic;
		
		CSU_SE : in  std_logic;
		CSU_CE : in  std_logic;
		CSU_UE : in  std_logic;
		CSU_Select : in  std_logic;
		
		TDI : in  std_logic;
		TDO : out std_logic
		
	);
end component;


END;
