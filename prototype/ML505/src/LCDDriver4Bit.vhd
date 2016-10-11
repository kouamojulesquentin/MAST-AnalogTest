-- LCD Driver Module for driving HD44780 Controller
-- A. Greensted, July 2010

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity LCDDriver4Bit is
	generic (CLK_FREQ		: positive := 100000000);			-- Frequency of clk input in Hz
	port (	clk			: in		std_logic;
				resetn			: in		std_logic;

				-- Screen Buffer Interface
				dIn			: in		std_logic_vector(7 downto 0);
				charNum		: in		integer range 0 to 39;
				wEn			: in		std_logic;
				refresh     	: in		std_logic;

				-- LCD Interface
				lcdData		: out		std_logic_vector(3 downto 0);
				lcdRS			: out		std_logic;
				lcdRW			: out		std_logic;
				lcdE			: out		std_logic
	);
end LCDDriver4Bit;

architecture Structural of LCDDriver4Bit is

	-- LCD interface constants
	constant DATA_CODE		: std_logic := '1';
	constant INSN_CODE		: std_logic := '0';

	-- Tick Generation, generates a tick pulse every 10us
	constant TICK_MAX			: integer := CLK_FREQ / 100000;
	signal tick					: std_logic;

	-- Delay times in units of 10 us
	constant WARMUP_DELAY	: integer := 2000;	-- 2000:	20ms
	constant INIT_DELAY		: integer := 500;		-- 500:	5ms
	constant CHAR_DELAY		: integer := 10;		-- 10:	100us

	signal timer				: integer range 0 to WARMUP_DELAY;

	type INIT_ROM_TYPE is array (0 to 11) of std_logic_vector(3 downto 0);
	constant initROM			: INIT_ROM_TYPE := (	b"0011",	-- Init
									b"0011",	-- Init
									b"0011",	-- Init
									b"0010",	-- Init
									b"0010",	b"1000",		-- Function Set: 4 bit, 2 lines, 5x7 characters
									b"0000",	b"1000",		-- Display On/Off Control: Display off, Cursor off, Blink off
									b"0000", b"1100",		-- Display On/Off Control: Display on, Cursor off, Blink off
									b"0000", b"0110");	-- Entry Mode Set: Auto increment cursor, don't shift display


	constant out_of_LCD_chars : integer := 4; --number of characters lost between 1st and 2nd line
	constant Line_size : integer := 20; --number of characters of a line
	constant last_char_1st_line : integer := Line_size-1; 
	constant last_char_2nd_line : integer := 2*Line_size-1; 

	constant last_LCD_char_1st_line : integer := last_char_1st_line - out_of_LCD_chars; 
	constant last_LCD_char_2nd_line : integer := last_char_2nd_line - out_of_LCD_chars; 

	type CHAR_RAM_TYPE is array(0 to last_char_2nd_line) of std_logic_vector(7 downto 0);
	signal charRAM				: CHAR_RAM_TYPE := (	0=>x"4D", 1=>x"2E", 2=>x"50", 3=>x"6F", 4=>x"72", 5=>x"74",
									6=>x"6F", 7=>x"6C", 8=>x"61", 9=>x"6E",  
									20=>x"57",21=>x"69", 22=>x"69", 23=>x"FE",  --Wii
									24=>x"63", 25=>x"6F", 26=>x"6E",27=>x"74", 28=>x"72", 
									29=>x"6F",30=>x"6C", 31=>x"6C", 32=>x"65",
									33=>x"72",--Controller
--									20=>x"4D", 21=>x"61", 22=>x"79", 23=>x"FE", 24=>x"FE", --MAY
--									25=>x"32", 26=>x"30", 27=>x"31", 28=>x"31", --2011
									others=>x"A0");

	signal setLine				: std_logic;
	signal lineNum				: integer range 0 to 1;
	signal initialising		: std_logic;
	signal nibble				: std_logic;

	signal initROMPointer	: integer range 0 to INIT_ROM_TYPE'high;
	signal charRAMPointer	: integer range 0 to CHAR_RAM_TYPE'high;

	type STATE_TYPE is (STARTUP,IDLE,DELAY, STAGE1, STAGE2, STAGE3);
	signal state				: STATE_TYPE;
	signal refresh_done : std_logic;
	
	signal LCD_display_vector : std_logic_vector(7 downto 0);
	
begin
lcdRW <= '0';

--LCD_display_idx_conv:process(charRAMPointer)
-- variable LCD_display_idx 	: integer;
-- begin
--   LCD_display_vector <= x"2E";
--   if (charRAMPointer < 16) then
--    LCD_display_idx := charRAMPointer+1;
--    LCD_display_vector <= ASCII2STDLOGIC(lcd_display(LCD_display_idx));
--   elsif ((charRAMPointer >= 20 ) and (charRAMPointer < 36)) then
--      LCD_display_idx := charRAMPointer-3;
--      LCD_display_vector <= ASCII2STDLOGIC(lcd_display(LCD_display_idx));
--   end if;
-- end process;


TickGen : process(clk)
	variable tickCount : integer range 0 to TICK_MAX-1;
begin
	if (clk'event and clk='1') then
		if (resetn = '0') then
			tickCount := 0;
			tick <= '0';

		elsif (tickCount = TICK_MAX-1) then
			tickCount := 0;
			tick <= '1';

		else
			tickCount := tickCount + 1;
			tick <= '0';

		end if;
	end if;
end process;

CharRAMWrite : process(clk)
begin
	if (clk'event and clk='1') then
	  if (resetn = '0') then
	    charRAM <=( 0=>x"4D", 1=>x"2E", 2=>x"50", 3=>x"6F", 4=>x"72", 5=>x"74",
		        6=>x"6F", 7=>x"6C", 8=>x"61", 9=>x"6E",  
		        20=>x"57",21=>x"69", 22=>x"69", 23=>x"FE",  --Wii
		        24=>x"63", 25=>x"6F", 26=>x"6E",27=>x"74", 28=>x"72", 
		        29=>x"6F",30=>x"6C", 31=>x"6C", 32=>x"75",
			33=>x"72",
			others=>x"A0");

	  elsif (wEn='1') then
	     if (charNum <= last_LCD_char_1st_line) then
			charRAM(charNum) <= dIn;
	     elsif (charNum <= last_LCD_char_1st_line*2+1) then
	         --displacement to take care of non-displayed characters
                 charRAM(charNum+out_of_LCD_chars) <= dIn;
	     else	 
	         --wrap to beginning if too many chars
		  charRAM(charNum-(last_LCD_char_1st_line*2+2)) <= dIn;
	     end if;		
	  end if;
	end if;
end process;

Controller : process (clk)
begin
	if (clk'event and clk='1') then

		if (resetn='0') then
			timer				<= WARMUP_DELAY;
			initROMPointer <= 0;
			charRAMPointer <= 0;

			lcdRS				<= INSN_CODE;
			lcdE				<= '0';
			lcdData			<= (others => '0');

			nibble			<= '0';
			initialising	<= '1';
			setLine			<= '0';
			lineNum			<= 0;
			state				<= STARTUP;
			refresh_done <= '0';

		elsif (tick='1') then

			case state is

				-- Lanch initialisation at startup
				when STARTUP =>
					if (timer=0) then
						state <= STAGE1;
					else
						timer <= timer - 1;
					end if;
					

				when IDLE =>
				 refresh_done <= '0';
				  --Start to write if new charachter arrives, or to skip the out-of-lcd chars
				  if (refresh = '1')  --or 
--				      ((charRAMPointer >= last_LCD_char_1st_line) and (charRAMPointer <= last_char_1st_line) )
				        then 
				    	state <= DELAY;
				  else
				        state <= IDLE;
				 end if;		

				-- Provide delay to allow instruciton to execute
				when DELAY =>
					if (timer=0) then
						state <= STAGE1;
					else
						timer <= timer - 1;
					end if;

				-- Set the LCD data
				-- Set the LCD RS
				-- Initialise the timer with the required delay
				when STAGE1 =>
					if (initialising='1') then
						timer		<= INIT_DELAY;
						lcdRS		<= INSN_CODE;
						lcdData	<= initROM(initROMPointer);

					elsif (setLine='1') then
						timer		<= CHAR_DELAY;
						lcdRS		<= INSN_CODE;

						if (nibble='0') then
							case lineNum is
								when 0 => lcdData	<= b"1000";	-- x00
								when 1 => lcdData	<= b"1100";	-- x40
							end case;
						else
							lcdData <= b"0000";
						end if;

					else
						timer		<= CHAR_DELAY;
						lcdRS		<= DATA_CODE;

						if (nibble <= '0') then
							lcdData	<= charRAM(charRAMPointer)(7 downto 4);
	--						lcdData	<= LCD_display_vector(7 downto 4);
							
						else
							lcdData	<= charRAM(charRAMPointer)(3 downto 0);
--							lcdData	<= LCD_display_vector(3 downto 0);
						end if;
					end if;

					state	<= STAGE2;

				-- Set lcdE (latching RS and RW)
				when STAGE2 =>
					if (initialising='1') then
						if (initROMPointer=INIT_ROM_TYPE'high) then
							initialising <= '0';
						else
							initROMPointer	<= initROMPointer + 1;
						end if;

					elsif (nibble='1') then
						nibble <= '0';
					
						if (setLine='1') then
							setLine <= '0';

						else

							if (charRAMPointer=last_char_1st_line) then
								setLine <= '1';
								lineNum <= 1;

							elsif (charRAMPointer=last_char_2nd_line) then
								setLine <= '1';
								lineNum <= 0;
							end if;

							if (charRAMPointer=CHAR_RAM_TYPE'high) then
								charRAMPointer <= 0;
								refresh_done <= '1';
							else
								charRAMPointer <= charRAMPointer + 1;
							end if;

						end if;

					else
						nibble <= '1';

					end if;

					lcdE	<= '1';
					state	<= STAGE3;

				-- Clear lcdE (latching data)
				when STAGE3 =>
					lcdE	<= '0';

					-- No delay required between nibbles
					if (nibble = '1') then
						state <= STAGE1;
					else
					  if (refresh_done = '1') then
					    state	<= IDLE;
					  else
						state	<= DELAY;
					  end if;	
					end if;

			end case;
		end if;
	end if;
end process;

end Structural;
