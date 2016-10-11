-------------------------------------------------------------------------------------
--	LCD supervisor for ML505 board, with update transmitted via an internal JTAG TAP
--
--	The LCD displays a VU-meter, regarding the value set into the register after
--	a JTAG update.
--
--	This demo code is copyright 2016 Grenoble-INP TIMA // All rights reserved.
--
--	version: 1.0
--  16/09/2016 - NG
--
-------------------------------------------------------------------------------------

library ieee;

use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;

library unisim;
use unisim.vcomponents.all ;

entity jtag_lcd_driver is
	generic(
		register_size : integer := 8;
		CLK_FREQ		: positive := 100000000
	);
	port(
		tck			:	in	std_logic;
		clk			:	in 	std_logic;
		resetn		:	in 	std_logic;
		update_data_left	:	in 	std_logic_vector(register_size-1 downto 0);
		update_data_right	:	in 	std_logic_vector(register_size-1 downto 0);
		we_update_left 	:   in 	std_logic;
		we_update_right	:   in 	std_logic;

		-- LCD Interface
		lcdData		:   out	std_logic_vector(3 downto 0);
		lcdRS		:   out	std_logic;
		lcdRW		:   out	std_logic;
		lcdE		:   out	std_logic
	);
end jtag_lcd_driver;

architecture jtag_lcd_driver_a of jtag_lcd_driver is

	-- Declaring internal components

	-- LCD driver
	component LCDDriver4Bit is
		generic (CLK_FREQ		: positive := 100000000);			-- Frequency of clk input in Hz
		port (	clk			    : in    std_logic;
				resetn		    : in	std_logic;

				-- Screen Buffer Interface
				dIn			    : in	std_logic_vector(7 downto 0);
				charNum		    : in	integer range 0 to 39;
				wEn			    : in	std_logic;
				refresh         : in	std_logic;

				-- LCD Interface
				lcdData		    : out	std_logic_vector(3 downto 0);
				lcdRS		    : out	std_logic;
				lcdRW			: out	std_logic;
				lcdE			: out	std_logic
		);
      --    lcd_display : in string(1 to 32));
	end component;
	
	-- Clock domain division FIFO
	component jtag_to_lcd_fifo IS
      port (
        rst : IN STD_LOGIC;
        wr_clk : IN STD_LOGIC;
        rd_clk : IN STD_LOGIC;
        din : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        wr_en : IN STD_LOGIC;
        rd_en : IN STD_LOGIC;
        dout : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        full : OUT STD_LOGIC;
        empty : OUT STD_LOGIC
      );
    end component;

	-- Interconnection signals
	signal s_re_fifo_left, s_re_fifo_right  :	std_logic;
	signal s_to_lcd_din		    :	std_logic_vector(7 downto 0);
	signal s_fifo_byte_out_left	    :	std_logic_vector(7 downto 0);
	signal s_fifo_byte_out_right    :	std_logic_vector(7 downto 0);
	signal s_to_lcd_charnum	    :	integer range 0 to 39;
	signal s_to_lcd_wen		    :	std_logic;
	signal s_to_lcd_refresh	    :	std_logic;

	signal s_fifo_reset		    :	std_logic;
	signal s_fifo_is_empty_left, s_fifo_is_empty_right 	    :   std_logic;
	signal s_from_lcd_data_out  :   std_logic_vector(3 downto 0);

	-- constants
		-- LCD diplayed characters
	constant displayed_vu_meter_char    :   std_logic_vector(7 downto 0) := "11111111"; -- We want the LCD display a big black square when the level is present at a given coordinate.
	constant displayed_vu_meter_blank   :   std_logic_vector(7 downto 0) := "11111110"; -- Otherwise, we want the LCD display a big white (empty) square when the level is not present.
	constant displayed_vu_left_letter   :   std_logic_vector(7 downto 0) := "01001100"; -- First letter on the first line is a 'L' to show we are looking at the left channel.
	constant displayed_vu_right_letter  :   std_logic_vector(7 downto 0) := "01010010"; -- First letter on the second line is a 'R' to explain the given amplitude is the right channel one.

	constant value_shift    :   integer range 0 to 7 := 4;  -- Amplitude is scaled onto the 16-bit VU-meter dynamic. This is done via 256/16 = 16, with right logical shifts;
	
	constant LCD_line_size  :   integer := 16;              -- LCD line size is 16.
	
	constant NB_CLK_PER_TICK :  integer := CLK_FREQ/100000; -- 1 tick every 10 ms
	
	-- FSM state machine states definition and dedicated registers
	type fsm_states is (S_Idle, S_GetLevel, S_CalculateVUlevel,
											 S_UpdateChars, S_Refresh);
	signal state_d, state_q : fsm_states;
	
	-- Local registers
	signal jtag_updated_register_left_d, jtag_updated_register_left_q : std_logic_vector(register_size-1 downto 0);
	signal jtag_updated_register_right_d, jtag_updated_register_right_q : std_logic_vector(register_size-1 downto 0);
	signal current_char_pos_d, current_char_pos_q : integer range 1 to 2*(LCD_line_size);
	
	signal vu_level_left_d, vu_level_left_q, vu_level_right_d, vu_level_right_q : integer range 0 to LCD_line_size;
	
	signal refresh_waiting_ticks_d, refresh_waiting_ticks_q : integer range 0 to NB_CLK_PER_TICK+1;
	
	
	-- TCK-clock domain update delay register
	signal update_req_left_d, update_req_left_q, update_req_right_d, update_req_right_q : std_logic;  -- Update signal is sent by the slave_TAP component to the bs_register to request the update at the next TCK clock rising edge. 
	                                                -- As we also use the same signal to write in our own FIFO, there is a need to add a one TCK clock period delay between the write-enable in the FIFO
	                                                -- and its corresponding data.

begin

	lcd_driver : LCDDriver4Bit
	generic map (CLK_FREQ => CLK_FREQ)
	port map(
			clk 		=> clk,
			resetn	    => resetn,
			
			dIn 		=> s_to_lcd_din,
			charNum     => s_to_lcd_charnum,
			wEn 		=> s_to_lcd_wen,
			refresh     => s_to_lcd_refresh,

			lcdData     => lcdData,
			lcdRS 	    => lcdRS,
			lcdRW 	    => lcdRW,
			lcdE 		=> lcdE
		);

	-- Time-domain dividing FIFO.
  clk_domain_border_fifo_left : jtag_to_lcd_fifo
      port map(
        rst     => s_fifo_reset,
        wr_clk  => tck,
        rd_clk  => clk,
        din     => update_data_left,
        wr_en   => update_req_left_q,
        rd_en   => s_re_fifo_left,
        dout    => s_fifo_byte_out_left,
        full    => open,
        empty   => s_fifo_is_empty_left
      );
      
    -- Time-domain dividing FIFO.
  clk_domain_border_fifo_right : jtag_to_lcd_fifo
      port map(
        rst     => s_fifo_reset,
        wr_clk  => tck,
        rd_clk  => clk,
        din     => update_data_right,
        wr_en   => update_req_right_q,
        rd_en   => s_re_fifo_right,
        dout    => s_fifo_byte_out_right,
        full    => open,
        empty   => s_fifo_is_empty_right
      );

	s_fifo_reset <= not resetn;


    tck_sync : process(tck)
    begin
        if tck'event and tck = '1' then
            if resetn = '0' then
                update_req_left_q <= '0';
                update_req_right_q <= '0';
            else
                update_req_left_q <= update_req_left_d;
                update_req_right_q <= update_req_right_d;
            end if;
        end if;
    end process;
    
    comb_tck : process(we_update_left, we_update_right)
    begin
        update_req_left_d <= we_update_left;
        update_req_right_d <= we_update_right;
        
--        if we_update = '1' then
--            update_req_d <= '1';
--        end if;
    end process;
        
            				

	sync : process(clk)
	begin
		if clk'event and clk = '1' then
			if resetn = '0' then					-- Synchronous reset.
				state_q                 <= S_Idle;
				jtag_updated_register_left_q <= (others => '0');
				jtag_updated_register_right_q <= (others => '0');
				current_char_pos_q      <= 1;
				vu_level_left_q         <= 0;
				vu_level_right_q        <= 0;
				refresh_waiting_ticks_q <= 0;
			else
				state_q                 <= state_d;
				jtag_updated_register_left_q <= jtag_updated_register_left_d;
				jtag_updated_register_right_q <= jtag_updated_register_right_d;
				current_char_pos_q      <= current_char_pos_d;
				vu_level_left_q         <= vu_level_left_d;
				vu_level_right_q        <= vu_level_right_d;
				refresh_waiting_ticks_q <= refresh_waiting_ticks_d;
			end if;
		end if;
	end process;

	comb: process(state_q, jtag_updated_register_left_q, jtag_updated_register_right_q, s_fifo_is_empty_left, s_fifo_is_empty_right, s_fifo_byte_out_left, s_fifo_byte_out_right,
	current_char_pos_q, vu_level_left_q, vu_level_right_q, refresh_waiting_ticks_q)
	    variable vu_meter_level_left, vu_meter_level_right : std_logic_vector((register_size-1) downto 0);
	begin
	
		-- setting idle values for internal signals
		s_to_lcd_din 		<= (others => '0');
		s_to_lcd_charNum    <= current_char_pos_q-1;
		s_to_lcd_wen		<= '0';
		s_to_lcd_refresh 	<= '0';
		s_re_fifo_left			<= '0';
		s_re_fifo_right			<= '0';
		
		s_from_lcd_data_out <= (others => '0');

		-- avoiding latches...
		state_d                 <= state_q;
		jtag_updated_register_left_d <= jtag_updated_register_left_q;
		jtag_updated_register_right_d <= jtag_updated_register_right_q;
		vu_level_left_d         <= vu_level_left_q;
		vu_level_right_d        <= vu_level_right_q;
		current_char_pos_d      <= current_char_pos_q;
		
		refresh_waiting_ticks_d <= refresh_waiting_ticks_q;
	

		case state_q is
			when S_Idle =>

				if (s_fifo_is_empty_left = '0') and (s_fifo_is_empty_right = '0') then						 	-- Create an one CLK period delay, ensuring the values
					state_d <= S_GetLevel; 	                            -- are properly progated one clock period after the 
					s_re_fifo_left <= '1';									-- read request.
					s_re_fifo_right <= '1';

				end if;

			when S_GetLevel	=>                                          -- FIFO has propagated the output value.
				jtag_updated_register_left_d <= s_fifo_byte_out_left;   -- Now we update copy the value into our local register
				jtag_updated_register_right_d <= s_fifo_byte_out_right;
				state_d <= S_CalculateVUlevel;

			when S_CalculateVUlevel =>                                  -- This step scales the amplitude value
				current_char_pos_d <= 1;                                -- within the VU-meter range.
				
				vu_meter_level_left := (others => '0');
				vu_meter_level_right := (others => '0');
				
				if value_shift > 0 then                                 -- Scaling is processed with a right 
				                                                        -- logical shift
    				vu_meter_level_left := ((register_size-1) downto (register_size-value_shift) => '0') 
    				                &jtag_updated_register_left_q((register_size-1) downto value_shift);
    				                
                    vu_meter_level_right := ((register_size-1) downto (register_size-value_shift) => '0') 
    				                &jtag_updated_register_right_q((register_size-1) downto value_shift);
                else
                    vu_meter_level_left := jtag_updated_register_left_q;
                    vu_meter_level_right := jtag_updated_register_right_q;
                end if;
                
				vu_level_left_d <= to_integer(unsigned(vu_meter_level_left));
				vu_level_right_d <= to_integer(unsigned(vu_meter_level_right));
				
				state_d <= S_UpdateChars;
				
			when S_UpdateChars =>
				
				s_to_lcd_wEn <= '1';
					
				if current_char_pos_q <= LCD_line_size then             -- That's the first line.

                    if current_char_pos_q = 1 then
                        s_to_lcd_din <= displayed_vu_left_letter;
                        s_from_lcd_data_out <= displayed_vu_left_letter(3 downto 0);
                        
				    elsif current_char_pos_q <= vu_level_left_q+1 then
				    
					    s_to_lcd_din <= displayed_vu_meter_char;
					    s_from_lcd_data_out <= displayed_vu_meter_char(3 downto 0);
					    
				    else
					    s_to_lcd_din <= displayed_vu_meter_blank;
					    s_from_lcd_data_out <= displayed_vu_meter_blank(3 downto 0);
				    end if;
				    
			    elsif current_char_pos_q <= 2*LCD_line_size then        -- That's the second line
			    
			        if current_char_pos_q = LCD_line_size+1 then
                        s_to_lcd_din <= displayed_vu_right_letter;
                        s_from_lcd_data_out <= displayed_vu_right_letter(3 downto 0);
                        
				    elsif current_char_pos_q <= LCD_line_size+vu_level_right_q+1 then
					    s_to_lcd_din <= displayed_vu_meter_char;
					    s_from_lcd_data_out <= displayed_vu_meter_char(3 downto 0);
					    
				    else
					    s_to_lcd_din <= displayed_vu_meter_blank;
					    s_from_lcd_data_out <= displayed_vu_meter_blank(3 downto 0);
					    
				    end if;
				    
				-- else -- TODO: for out-of-display slots
				    
			    end if;

				current_char_pos_d <= current_char_pos_q + 1;
				
				if current_char_pos_q = 2*LCD_line_size then            -- EoL
					state_d <= S_Refresh;
					refresh_waiting_ticks_d <= 0;
				end if;


			when S_Refresh =>
				s_to_lcd_refresh <= '1';
				
				refresh_waiting_ticks_d <= refresh_waiting_ticks_q + 1;
				
				if refresh_waiting_ticks_q = NB_CLK_PER_TICK then
				    state_d <= S_Idle;
				    refresh_waiting_ticks_d <= 0;
			    end if;

		end case;
		
    end process;
				
end jtag_lcd_driver_a;
