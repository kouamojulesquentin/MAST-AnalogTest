-------------------------------------------------------------------------------------
--	AC97 protocol handler-to-JTAG interface, essentially for the AD1981 ADC.
--
--	The AC97 protocol handler is written by Javier Valcarce (Universidad Rey Juan Carlos)
--  based on a source code from MIT http://web.mit.edu/6.111/www/f2008/handouts/labs/lab4.html
--
--	This handler code is copyright 2016 Grenoble-INP TIMA // All rights reserved.
--
--	version: 1.0
--  06/10/2016 - NG
--
-------------------------------------------------------------------------------------

library ieee;

use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;

library unisim;
use unisim.vcomponents.all ;

entity dc97_jtag_interface is
    generic(
        i_reg_size : integer := 8
    );
    port(
        tck     : in std_logic;     -- Incoming JTAG clock
        clk     : in std_logic;     -- DC97 processor clock (27MHz)
        reset   : in std_logic;     -- reset signal. Internal behaviour makes it synchronous and active high.
        
        -- ac97 interface signals
        ac97_sdata_o : out std_logic;
        ac97_sdata_i : in  std_logic;
        ac97_sync    : out std_logic;
        ac97_bitclk  : in  std_logic;
        ac97_reset   : out std_logic;
        
        -- JTAG interface signals
        left_sampled_data   : out std_logic_vector(i_reg_size-1 downto 0);
        right_sampled_data  : out std_logic_vector(i_reg_size-1 downto 0)
    );
end dc97_jtag_interface;

architecture dc97_jtag_interface_a of dc97_jtag_interface is

    -- DC97 component declaration
    component dc97 is
      port (
        n_reset      : in  std_logic;
        clk          : in  std_logic;
        -- ac97 interface signals
        ac97_sdata_o : out std_logic;
        ac97_sdata_i : in  std_logic;
        ac97_sync    : out std_logic;
        ac97_bitclk  : in  std_logic;
        ac97_reset   : out std_logic;
        --
        ready        : out std_logic; -- one-cycle pulse
        L_o          : in  std_logic_vector(15 downto 00);
        R_o          : in  std_logic_vector(15 downto 00);
        L_i          : out std_logic_vector(15 downto 00);
        R_i          : out std_logic_vector(15 downto 00);
        cmd_addr     : in  std_logic_vector(07 downto 00);
        cmd_data     : in  std_logic_vector(15 downto 00));
    end component;
    
    -- DC97 command handler component declaration
    component dc97cmd is
      port (
        clk      : in  std_logic;
        ready    : in  std_logic;
        cmd_addr : out std_logic_vector(07 downto 00);
        cmd_data : out std_logic_vector(15 downto 00);
        volume   : in  std_logic_vector(04 downto 00);  -- 01000
        source   : in  std_logic_vector(02 downto 00)); --000 = Mic, 100=LineIn
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
    
    signal n_reset : std_logic;
    signal s_left_channel_out, s_right_channel_out : std_logic_vector(15 downto 0);
    signal s_ready : std_logic;
    
    signal cmd_addr : std_logic_vector(7 downto 0);
    signal cmd_data : std_logic_vector(15 downto 0);
    
    signal jtag_left_d, jtag_left_q, jtag_right_d, jtag_right_q : std_logic_vector(i_reg_size-1 downto 0);
    
    signal s_fifo_left_dout, s_fifo_right_dout : std_logic_vector(i_reg_size-1 downto 0);
    
    signal s_fifo_is_empty_left, s_fifo_is_empty_right : std_logic;
    
    signal s_fifo_re_left, s_fifo_re_right : std_logic;
    
    type fsm_states is (S_Idle, S_Read);
    
    signal state_d, state_q : fsm_states;
    
begin


    n_reset <= not reset;
    
    -- DC97 components instantiation
      dc97_phy : dc97
      port map (
            n_reset => n_reset,
            clk => clk,
            -- ac97 interface signals
            ac97_sdata_o => ac97_sdata_o,
            ac97_sdata_i => ac97_sdata_i,
            ac97_sync    => ac97_sync,
            ac97_bitclk  => ac97_bitclk,
            ac97_reset   => ac97_reset,
	        --
	        ready        => s_ready,
            L_o          => (others => '0'), 		-- to   headphone L
            R_o          => (others => '0'), 		-- to   headphone R
            L_i          => s_left_channel_out, 		-- from Mic L
            R_i          => s_right_channel_out,  		-- from Mic R
            cmd_addr     => cmd_addr,
            cmd_data     => cmd_data
        );
 
 dc97_protocol : dc97cmd 
     port map (
        clk     => clk,
        ready    => s_ready,
        cmd_addr => cmd_addr,
        cmd_data => cmd_data,
        volume   => "01000", 
        source   => "100"
    );  
	 
  -- (000 = mic) (100 = line in)	 
--  source <= srcselect & "00";

	-- Time-domain dividing FIFO.
  clk_domain_border_fifo_left : jtag_to_lcd_fifo
      port map(
        rst     => reset,
        wr_clk  => clk,
        rd_clk  => tck,
        din     => s_left_channel_out(15 downto 8),
        wr_en   => s_ready,
        rd_en   => s_fifo_re_left,
        dout    => s_fifo_left_dout,
        full    => open,
        empty   => s_fifo_is_empty_left
      );
      
  clk_domain_border_fifo_right : jtag_to_lcd_fifo
      port map(
        rst     => reset,
        wr_clk  => clk,
        rd_clk  => tck,
        din     => s_right_channel_out(15 downto 8),
        wr_en   => s_ready,
        rd_en   => s_fifo_re_right,
        dout    => s_fifo_right_dout,
        full    => open,
        empty   => s_fifo_is_empty_right
      );
      
  sync_tck : process(tck)
  begin
    if tck'event and tck = '1' then
        if reset = '1' then
            state_q     <= S_Idle;
            jtag_left_q <= (others => '0');
            jtag_right_q <= (others => '0');
        else
            state_q     <= state_d;
            jtag_left_q <= jtag_left_d;
            jtag_right_q <= jtag_right_d;
        end if;
    end if;
  end process;
  
  comb_tck : process(s_fifo_is_empty_left, s_fifo_is_empty_right, s_fifo_left_dout, s_fifo_right_dout, jtag_left_q, jtag_right_q, state_q)
  begin
  
    -- setting component outputs
    left_sampled_data   <= jtag_left_q;
    right_sampled_data  <= jtag_right_q;
  
    -- avoiding latches
    jtag_left_d         <= jtag_left_q;
    jtag_right_d        <= jtag_right_q;
    state_d             <= state_q;
    
    -- FSM output signals idle states (i.e. when they are not overriden by a state)
    s_fifo_re_left      <= '0';
    s_fifo_re_right     <= '0';
    
    case state_q is
        when S_Idle =>
            if (s_fifo_is_empty_left = '0') and (s_fifo_is_empty_right = '0') then      -- If both FIFOs has new data, request a read.
                s_fifo_re_left <= '1';
                s_fifo_re_right <= '1';
                state_d     <= S_Read;
            end if;
            
        when S_Read =>
            jtag_left_d     <= s_fifo_left_dout;                                        -- Then update jtag_left and jtag_right registers.
            jtag_right_d    <= s_fifo_right_dout;
            state_d         <= S_Idle;
            
    end case;
            
	end process;
end dc97_jtag_interface_a;
