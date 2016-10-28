-------------------------------------------------------------------------------------
--	MAST ML505 JTAG and SPI units demonstrator.
--
--	This top design provides JTAG registers to acquire audio values from the embedded AD1981B ADC
--  and to display left and right channel amplitudes on the LCD screen.
--
--  It also provides a SPI slave unit, with one data register. 
--  See new_jtag_slave.vhd for operation guidelines.
--
--	This code is copyright 2016 Grenoble-INP TIMA // All rights reserved.
--
-------------------------------------------
--  CHANGELOG:
--  06/10/2016: First version - Niels G.
--
--------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
USE IEEE.numeric_std.ALL;
use IEEE.std_logic_textio.all;
library  UNISIM;
use  UNISIM.vcomponents.all;

library work;
use work.slave_TAP_package.all;
use work.JTAG_package.all;
use work.MAST_config.all;
use work.MAST_write.all;
use work.exchange_registers.all;

entity kiss_fft is
 port (
    -- Clock and reset inputs
    resetn		        : in    std_logic;
    clk		            : in    std_logic;      -- clk for LCD controller
    clk_27mhz           : in    std_logic;      -- clk for the ADC controller (from U8 oscillator)
		  
	-- JTAG TAP signals (from/to JTAG adapter)
	jtagTCK             : in    std_logic;
	jtagRST             : in    std_logic;
	jtagTMS             : in    std_logic;
	jtagTDI             : in    std_logic;
	jtagTDO             : out   std_logic;
	
	-- SPI slave signals (from/to SPI master)
	
	spi_sclk            : in std_logic;
	spi_mosi            : in std_logic;
	spi_miso            : out std_logic;
	spi_ssn             : in std_logic;     -- SPI slave select signal
	
	-- ac97 interface signals (from/to the AD1981B ADC)
    ac97_sdata_o        : out   std_logic;
    ac97_sdata_i        : in    std_logic;
    ac97_sync           : out   std_logic;
    ac97_bitclk         : in    std_logic;
    ac97_reset          : out   std_logic;

	--LCD Screen signals
	lcdData		        : out	std_logic_vector(3 downto 0);
	lcdRS		        : out	std_logic;
	lcdRW		        : out	std_logic;
	lcdE		        : out	std_logic;
	
	-- Useful I/Os
	switches	        : in    std_logic_vector(7 downto 0);
	refresh_screen	    : in    std_logic;
	refresh_mouse_pb	: in    std_logic;
    LED_out             : out   std_logic_vector(7 downto 0);
	lcd_LED             : out   std_logic
 	);
end entity;

architecture kiss_fft_a of kiss_fft is


component jtag_lcd_pll is
   port ( CLKIN1_IN     : in    std_logic;      -- 100MHz clock from the X1 oscillator
          RST_IN        : in    std_logic;      -- reset signal
          CLKOUT0_OUT   : out   std_logic;      -- 100MHz clock
          CLKOUT1_OUT   : out   std_logic;      -- 50MHz clock
          LOCKED_OUT    : out   std_logic);
end component;


-------------LCD screen
constant CLK_FREQ		: positive := 100000000;	-- Input Clock Frequency (clk)

-- JTAG-to-LCD clock domain gateway + VU-metre manager
-- 
-- After a JTAG register update, this component scales the value onto the VU-metre range,
-- and refreshes the LCD display. See jtag_lcd_driver.vhd for further details.
component jtag_lcd_driver is
	generic(
		register_size : integer := 8;
		CLK_FREQ		: positive := 100000000
	);
	port(
		tck			    : in	std_logic;
		clk			    : in 	std_logic;
		resetn		    : in 	std_logic;
		update_data_left	:	in 	std_logic_vector(register_size-1 downto 0);
		update_data_right	:	in 	std_logic_vector(register_size-1 downto 0);
		we_update_left 	: in 	std_logic;
		we_update_right	: in 	std_logic;

		-- LCD Interface
		lcdData		    : out		std_logic_vector(3 downto 0);
		lcdRS		    : out		std_logic;
		lcdRW		    : out		std_logic;
		lcdE		    : out		std_logic
	);
end component;

-- SPI slave component
--
-- This SPI slave provides one data register that may be read or written.
-- First, the master transmits one byte requesting a read or write, then
-- followed by d_width sclk rising edges.
-- Master must send 0x90 as write command, and 0x80 as read command.
component spi_slave_io IS
  GENERIC(
    cpol    : STD_LOGIC := '0';  --spi clock polarity mode
    cpha    : STD_LOGIC := '0';  --spi clock phase mode
    d_width : INTEGER := 8);     --data width in bits
  PORT(
    sclk         : IN     STD_LOGIC;  --spi clk from master
    clk_driver   : IN     STD_LOGIC;  --downstream IP clock (SPI driver)
    reset_n      : IN     STD_LOGIC;  --active low reset
    ss_n         : IN     STD_LOGIC;  --active low slave select
    mosi         : IN     STD_LOGIC;  --master out, slave in
    reg_data_out : OUT    STD_LOGIC_VECTOR(d_width-1 DOWNTO 0);  --receive register output to logic
    reg_data_in  : IN     STD_LOGIC_VECTOR(d_width-1 DOWNTO 0);  --receive register output to logic
    miso         : OUT    STD_LOGIC := 'Z'
  ); --master in, slave out
END component;

-- ADC-to-JTAG register interface
--
-- This component handles the DC97 protocol supported by the on-board AD1981B ADC.
-- It receives each sample the ADC provides, divides the clock domains (ADC to JTAG), and makes the data ready
-- to be transmitted via JTAG when the CAPTURE state is enabled.
--
-- This interface uses the 'Line In' signals as audio signal inputs.
component dc97_jtag_interface is
    generic(
        i_reg_size : integer := 8
    );
    port(
        tck             : in std_logic;         -- Incoming JTAG clock
        clk             : in std_logic;         -- DC97 processor clock (27MHz)
        reset           : in std_logic;         -- reset signal. Internal behaviour makes it synchronous and active high.
        
        -- ac97 interface signals
        ac97_sdata_o    : out std_logic;
        ac97_sdata_i    : in  std_logic;
        ac97_sync       : out std_logic;
        ac97_bitclk     : in  std_logic;
        ac97_reset      : out std_logic;
        
        -- JTAG interface signals
        left_sampled_data   : out std_logic_vector(i_reg_size-1 downto 0);
        right_sampled_data  : out std_logic_vector(i_reg_size-1 downto 0)
    );
end component;

signal clk_100mhz, clk_50mhz : std_logic;

signal rst : std_logic;

-- JTAG signals


constant MAX_DR_CHAINS : integer := 12;
constant MAX_REGISTERS : integer := 20;
constant MAX_LEVELS : integer := 10;

signal reset_chains : std_logic;
signal IS_SHIFTING: std_logic;
signal IS_IDLE: std_logic;
signal Select_Reg : std_logic; --1 Data Registers, 0 IR
signal ClockIR : std_logic;
signal ShiftIR : std_logic;
signal UpdateIR : std_logic;
signal ClockDR : std_logic;
signal ShiftDR : std_logic;
signal UpdateDR : std_logic;
signal to_scan_chain :  std_logic;
signal from_scan_chain : std_logic;
signal CaptureDR: std_logic;
signal from_IR : std_logic;
signal from_BYP : std_logic;
signal select_IR : std_logic;
signal select_DR : std_logic;

signal select_BYP : std_logic;

signal IR_value : std_logic_vector(10 downto 0); 
signal select_DR_chain : std_logic_vector(MAX_DR_CHAINS-1 downto 0); 

signal from_DR : std_logic_vector(1 to MAX_DR_CHAINS);

signal s_jtag_update : std_logic_vector(15 downto 0);
signal s_jtag_update_en : std_logic;

signal s_jtag_capture_lcd : std_logic_vector(15 downto 0) := (others => '0');


-- LCD signals

constant LCD_SIZE : integer := 32;

signal lcd_refresh : std_logic;

signal lcd_wEn : std_logic;

signal 	debug_LED,LED	: std_logic_vector(7 downto 0);

signal jtag_update_d, jtag_update_q : std_logic_vector(15 downto 0);

-- DC97 protocol handler (ADC) signals

signal dc97_left_channel, dc97_right_channel : std_logic_vector(7 downto 0);
signal s_jtag_capture_adc : std_logic_vector(15 downto 0);

-- General-Purposes signals
signal s_from_spi_reg_val : std_logic_vector(7 downto 0);
signal s_to_spi_reg_val: std_logic_vector(7 downto 0);

signal s_to_spi: std_logic_vector(31 downto 0);
signal s_from_spi: std_logic_vector(31 downto 0);

type demo_target_type is (JTAG,SPI);

constant demo_target :demo_target_type :=JTAG;


begin

rst <= not resetn;                          -- Internal positive reset signal.

lcd_LED <= lcd_wEn;

jtag_top_pll : jtag_lcd_pll
   port map (
        CLKIN1_IN   => clk,
        RST_IN      => rst,
        CLKOUT0_OUT => clk_100mhz,
        CLKOUT1_OUT => open,
        LOCKED_OUT  => open
      );

------------------------------------ JTAG area


pslave_TAP_0 : slave_TAP 
    port  map ( 
        --TAP Signalq
	    TCK         => jtagTCK,
        TMS         => jtagTMS,
        TRSTN       => jtagRST,
        TDI         => jtagTDI, --NB!
        TDO         => jtagTDO, --NB!
		   --debug signals
        IS_SHIFTING => open,
        IS_IDLE     => open,
        --Scan chain control signals
	    reset       => reset_chains,
 	    Select_IR   => Select_IR,
 	    Select_DR   => Select_DR,
	    ClockIR     => ClockIR,
	    ShiftIR     => ShiftIR,
	    UpdateIR    => UpdateIR,
	    ClockDR     => ClockDR,
	    ShiftDR     => ShiftDR,
	    UpdateDR    => UpdateDR,
	    CaptureDR   => CaptureDR,
	    to_scan_chain => to_scan_chain,
	    from_scan_chain =>from_scan_chain

	   );       
IR_reg : bs_register_nocapture generic map (size => 11)
 port map
   ( clk            => jtagTCK, --ClockIR,
     rst            => reset_chains, 
     TDI            => to_scan_chain,
     TDO            => from_IR,
     SH_en          => ShiftIR,
     UP_en          => UpdateIR,
     Sel            => select_IR,
     P_out          => IR_value
     );

BYP_reg : bs_cell port map
    ( clk           => jtagTCK, --ClockDR,
     rst            => reset_chains,
     TDI            => to_scan_chain,
     TDO            => from_BYP,
     P_in           => '0',
     P_out          => open,
     mode           => '0',
     SH_en          => ShiftDR,
     CA_en          => '0',
     UP_en          => UpdateIR,
     Sel            => select_BYP
   );



DR_Mux : process(IR_value,from_IR,from_DR,Select_DR,Select_IR)
 begin
  select_BYP <= '0';
  select_DR_chain <= (others=> '0');
  if (Select_IR = '1') then
    from_scan_chain <= from_IR;
  elsif(Select_DR = '1') then
   case (IR_value) is 
    when "11111111111" => 
               select_BYP <= '1';
	       from_scan_chain <= from_BYP;
    when others =>
        select_DR_chain(to_integer(unsigned(IR_value)))<='1';  
        from_scan_chain <= from_DR(to_integer(unsigned(IR_value)));
   end case; 
  end if;  	 
    
 end process;
 
FFT_JTAG_SPI_on_LED: if demo_target = JTAG generate
static_reg0: bs_register generic map (size => 16)
 port map
   ( clk   => jtagTCK, --ClockIR,
     rst   => reset_chains, 
     TDI   => to_scan_chain,
     TDO   => from_DR(1), -- internal_chain(0),
     mode => '1',
     SH_en => ShiftDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(1),
     CA_en => CaptureDR,
     P_out => s_jtag_update,
     P_in => s_jtag_capture_lcd 
    ); 

static_reg1: bs_register generic map (size => 16)
 port map
   ( clk   => jtagTCK, --ClockIR,
     rst   => reset_chains, 
     TDI   => to_scan_chain,
     TDO   => from_DR(2), -- internal_chain(0),
     mode  => '1',
     SH_en => ShiftDR,
     UP_en => UpdateDR,
     Sel   => select_DR_chain(2),
     CA_en => CaptureDR,
     P_out => open,
     P_in => s_jtag_capture_adc 
    ); 

------------------------------------ SPI area
spi_unit : spi_slave_io
  generic map(
        d_width  => 8)
  port map(
        sclk        => spi_sclk,
        clk_driver  => clk_100mhz,
        reset_n     => resetn,
        ss_n        => spi_ssn,
        mosi        => spi_mosi,
        reg_data_out    => s_from_spi_reg_val,
        reg_data_in    => s_to_spi_reg_val,
        miso        => spi_miso
      );

------------------------------------ LED area
LED_out <= s_from_spi_reg_val;
--LED_out(3) <= '0';
end generate;

all_SPI: if demo_target = SPI generate

spi_unit : spi_slave_io
  generic map(
        d_width  => 32)
  port map(
        sclk        => spi_sclk,
        clk_driver  => clk_100mhz,
        reset_n     => resetn,
        ss_n        => spi_ssn,
        mosi        => spi_mosi,
        reg_data_out    => s_from_spi,
        reg_data_in    => s_to_spi,
        miso        => spi_miso
      );
s_jtag_update <= s_from_spi(31 downto 16);
s_to_spi(15 downto 0) <= s_jtag_capture_adc;

LED_out <= s_from_spi(31 downto 24); --to avoid ISE errors

end generate;

------------------------------------ LCD area
JTAG_to_LCD_driver : jtag_lcd_driver
	generic map(
		register_size => 8,
		CLK_FREQ => 100000000
	)
	port map(
		tck			=> jtagTCK,
		clk			=> clk_100mhz,
        resetn      => resetn,
		update_data_left	=> s_jtag_update(15 downto 8),
		update_data_right	=> s_jtag_update(7 downto 0),
		we_update_left 	=> s_jtag_update_en,
		we_update_right	=> s_jtag_update_en,

		-- LCD Interface
		lcdData		=>  lcdData,
		lcdRS		=>  lcdRs,
		lcdRW       =>  lcdRW,
		lcdE        =>  lcdE
	);


------------------------------------ AD1981B ADC area
dc97_interface : dc97_jtag_interface
    generic map(
        i_reg_size => 8
    )
    port map(
        tck         => clk_100mhz,--jtagTCK,
        clk         => clk_27mhz,
        reset       => rst,
        
        -- ac97 interface signals
        ac97_sdata_o => ac97_sdata_o,
        ac97_sdata_i => ac97_sdata_i,
        ac97_sync    => ac97_sync,
        ac97_bitclk  => ac97_bitclk,
        ac97_reset   => ac97_reset,
        
        -- JTAG interface signals
        left_sampled_data   => dc97_left_channel,
        right_sampled_data  => dc97_right_channel
    );
    
	s_jtag_capture_adc <= dc97_left_channel & dc97_right_channel;
   
    
	-- TODO: BELOW: OBSOLETE? As we now use the LEDs to replicate the SPI data register content
sync_jtag_reg : process(jtagTCK)
begin
    if jtagTCK = '1' and jtagTCK'event then
        if rst = '1' then
            jtag_update_q <= (others => '0');
        else
            jtag_update_q <= jtag_update_d;
        end if;
    end if;
end process;

s_jtag_update_en <= UpdateDR;

comb_jtag_reg : process(jtag_update_q, s_jtag_update_en, s_jtag_update)
begin
    jtag_update_d <= jtag_update_q;
    
    s_jtag_capture_lcd <= jtag_update_q;
    
    if s_jtag_update_en = '1' then
        jtag_update_d <= s_jtag_update;
    end if;
end process;

end kiss_fft_a;
