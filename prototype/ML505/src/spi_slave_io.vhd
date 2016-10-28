--------------------------------------------------------------------------------
--
--  SPI slave IP, for implementation on MAST demontration prototypes.
--
--  (c) September 2016, Grenoble INP / TIMA. All rights reserved.
--
-------------------------------------------
--  USER GUIDE:
--
--  The SPI master device may request a read or a write by sending a first byte via the MOSI
--  signal. Sending 0x80 requests a write, while 0x90 requests a read.
--
-------------------------------------------
--  CHANGELOG:
--  22/09/2016: First version - Niels G.
--
--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

library unisim;
use unisim.vcomponents.all ;

ENTITY spi_slave_io IS
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
END spi_slave_io;

ARCHITECTURE logic OF spi_slave_io IS

    component jtag_to_lcd_fifo IS
      PORT (
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
    END component;
    
  -- useful constants
  constant read_command : std_logic_vector(7 downto 0) := x"90";
  constant write_command : std_logic_vector(7 downto 0) := x"80";
  -- signals and registers  
  
  -- SPI domain clock signals
  SIGNAL mode    : STD_LOGIC;  --groups modes by clock polarity relation to data
  SIGNAL clk     : STD_LOGIC;  --clock

  type fsm_state_spi is (S_Idle, S_GetCommand, S_ParseCommand, S_Read, S_Write, S_WaitEndOfCS);  
  signal spi_i_nb_bits_d, spi_i_nb_bits_q : integer range 0 to (8 + d_width + 1);
  signal spi_command_d, spi_command_q : std_logic_vector(7 downto 0);
  signal spi_mosi_d, spi_mosi_q : std_logic_vector(d_width-1 downto 0);
  signal spi_state_d, spi_state_q : fsm_state_spi;
  
  signal spi_miso_d, spi_miso_q : std_logic_vector(d_width-1 downto 0);
  signal spi_miso_buffer_d, spi_miso_buffer_q : std_logic_vector(d_width-1 downto 0);
  signal reg_data_d, reg_data_q : std_logic_vector(d_width-1 downto 0);
  
  signal en_miso_buf_d, en_miso_buf_q : boolean;
  
  signal miso_read_req_d, miso_read_req_q : boolean;
  
  -- cross-clock domains fifo signals
  signal fifo_miso_data_in, fifo_miso_data_out, fifo_mosi_data_in, fifo_mosi_data_out : std_logic_vector((d_width - 1) downto 0);
  signal fifo_miso_we, fifo_mosi_we : std_logic;
  signal fifo_miso_re, fifo_mosi_re : std_logic;
  signal fifo_miso_empty, fifo_mosi_empty : std_logic;
  signal fifo_reset : std_logic;
  
  -- data registers clock domain
  type fsm_state_driver is (S_Reset, S_Startup, S_Idle, S_UpdateReg, S_UpdateMISO);
  signal driver_state_d, driver_state_q : fsm_state_driver;
  signal i_nb_clk_wait_d, i_nb_clk_wait_q : integer range 0 to 7;
  
  constant n_banks : INTEGER := d_width/8;
  signal s_fifo_miso_empty, s_fifo_mosi_empty : std_logic_vector(n_banks-1 downto 0);
  
BEGIN
  
  --adjust clock so writes are on rising edge and reads on falling edge
  mode <= cpol XOR cpha;  --'1' for modes that write on rising edge
  WITH mode SELECT
    clk <= sclk WHEN '1',
           NOT sclk WHEN OTHERS;


    -- Instantiating FIFOs           
    fifo_reset <= not reset_n;

fifos: for bank in 0 to n_banks-1 generate
    fifo_mosi : jtag_to_lcd_fifo
      PORT map (
        rst => fifo_reset,
        wr_clk => sclk,
        rd_clk => clk_driver,
        din => fifo_mosi_data_in(7+(8*bank) downto 0+(8*bank)),
        wr_en => fifo_mosi_we,
        rd_en => fifo_mosi_re,
        dout => fifo_mosi_data_out(7+(8*bank) downto 0+(8*bank)),
        full => open,
        empty => s_fifo_mosi_empty(bank)
      );
      
    fifo_miso : jtag_to_lcd_fifo
      PORT map (
        rst => fifo_reset,
        wr_clk => clk_driver,
        rd_clk => sclk,
        din => fifo_miso_data_in(7+(8*bank) downto 0+(8*bank)),
        wr_en => fifo_miso_we,
        rd_en => fifo_miso_re,
        dout => fifo_miso_data_out(7+(8*bank) downto 0+(8*bank)),
        full => open,
        empty => s_fifo_miso_empty(bank)
      );
  end generate fifos;
  
empty_flag : process(s_fifo_miso_empty,s_fifo_mosi_empty)
variable v_fifo_miso_empty: std_logic;
variable v_fifo_mosi_empty: std_logic;
 begin
   v_fifo_miso_empty := '0';
   v_fifo_mosi_empty := '0';
  for i in 0 to n_banks-1 loop
  v_fifo_miso_empty := v_fifo_miso_empty or s_fifo_miso_empty(i);
  v_fifo_mosi_empty := v_fifo_mosi_empty or s_fifo_mosi_empty(i);
  end loop;
  fifo_miso_empty <= v_fifo_miso_empty;
  fifo_mosi_empty <= v_fifo_mosi_empty;
 end process; 
  -- data register clock domain logic (including FIFO handling)         
  driver_sync : process(clk_driver)
  begin
    if clk_driver'event and clk_driver = '1' then
        if reset_n = '0' then
            driver_state_q <= S_Reset;
            reg_data_q <= (others => '0');
            i_nb_clk_wait_q <= 0;
        else
            driver_state_q <= driver_state_d;
            reg_data_q <= reg_data_d;
            i_nb_clk_wait_q <= i_nb_clk_wait_d;
        end if;
    end if;
  end process;
  
  driver_comb : process(driver_state_q, reg_data_q, fifo_mosi_data_out, fifo_mosi_empty, i_nb_clk_wait_q)
  begin
  
    reg_data_out <= reg_data_q;
  
    driver_state_d <= driver_state_q;
    reg_data_d <= reg_data_q;
    i_nb_clk_wait_d <= i_nb_clk_wait_q;
    
    fifo_mosi_re <= '0';
    fifo_miso_we <= '0';
    fifo_miso_data_in <= (others => '0');
  
    case driver_state_q is
        when S_Reset =>
            
            i_nb_clk_wait_d <= i_nb_clk_wait_q +1;
            
            if i_nb_clk_wait_q = 5 then             -- RAM FIFOs cannot be updated directly after a reset.
                driver_state_d <= S_Startup;
            end if;
            
        when S_Startup =>
            fifo_miso_we <= '1';     
            fifo_miso_data_in <= reg_data_q;           
            driver_state_d <= S_Idle;
            
        when S_Idle =>
            if fifo_mosi_empty = '0' then           -- A data has been received from the master.
                fifo_mosi_re <= '1';                -- Hence, we request a FIFO read first.
                driver_state_d <= S_UpdateReg;
            end if;
        
        when S_UpdateReg =>
            reg_data_d <= fifo_mosi_data_out;       -- FIFO read has been previously requested.
            --fifo_miso_we <= '1';                  -- Now we update our output register with the
            driver_state_d <= S_UpdateMISO;         -- brand new received data.
            
        when S_UpdateMISO =>
            fifo_miso_we <= '1';                    -- If SPI requests a read,
            fifo_miso_data_in <= reg_data_q;        -- it will get the new value.
            driver_state_d <= S_Idle;
    end case;
    
  end process;
  
  -- SPI clock domain logic.
  spi_sclk_sync : process(sclk, ss_n)
  begin
    if sclk'event and sclk = '1' then
         if reset_n = '0' then
            spi_mosi_q <= (others => '0');
            spi_miso_q <= (others => '0');
            spi_command_q <= (others => '0');
            spi_i_nb_bits_q <= 0;
            miso_read_req_q <= false;
            spi_state_q <= S_Idle;                
        else
            spi_i_nb_bits_q <= spi_i_nb_bits_d;
            spi_mosi_q <= spi_mosi_d;
            spi_miso_q <= spi_miso_d;
            spi_command_q <= spi_command_d;
            miso_read_req_q <= miso_read_req_d;
            spi_state_q <= spi_state_d;
        end if;
    end if;
    
    if ss_n = '1' then
        spi_state_q <= S_Idle;
        spi_mosi_q <= (others => '0');
        spi_command_q <= (others => '0');
        spi_i_nb_bits_q <= 0;
        miso_read_req_q <= false;
    end if;
  end process;

    -- This synchronous process relies on the CPOL and CPOH values (giving the clk output clock).
    -- As the MISO line may be updated at half SCLK period, according to the previously cited attributes, we update this register in function of clk.
  spi_sync : process(clk, ss_n)
  begin
    if clk'event and clk = '1' then
        if reset_n = '0' then
            spi_miso_buffer_q <= (others => '0');
        else
            spi_miso_buffer_q <= spi_miso_buffer_d;
        end if;
    end if;
    
    -- N.B.: spi_miso register shall NOT be reset when ss_n is enabled.
  end process;
  
  spi_comb : process(spi_state_q, spi_i_nb_bits_q, spi_command_q, spi_mosi_q, spi_miso_q,
                    fifo_miso_empty, fifo_mosi_data_in, fifo_miso_data_out, miso_read_req_q,
                    ss_n, mosi, spi_miso_buffer_q)
                    
        variable command_val : std_logic_vector(7 downto 0);
  begin
  
    -- MISO idle state
    miso <= 'Z';
  
    -- avoiding latches
    spi_i_nb_bits_d <= spi_i_nb_bits_q;
    spi_state_d <= spi_state_q;
    spi_command_d <= spi_command_q;
    spi_mosi_d <= spi_mosi_q;
    spi_miso_d <= spi_miso_q;
    spi_miso_buffer_d <= spi_miso_buffer_q;
    miso_read_req_d <= miso_read_req_q;
    
    -- Default signals commanding FIFOs
    fifo_miso_re <= '0';
    fifo_mosi_we <= '0';
    fifo_mosi_data_in <= (others => '0');
    
    -- A LITTLE EXPLAINATION:
    -- This state machine is only updated in the SPI clock-domain. Any clock front edge relies
    -- to one bit propagated on the SPI bus. This is why some transitions relies to a previous
    -- stored bit vector and a current state of the MOSI line. According to the CPOH and CPOL
    -- parameters, MOSI line is generally update at half period of the SCLK clock.
    
    case spi_state_q is
        when S_Idle =>
            if ss_n = '0' then                                      -- The master unit has selected this slave. However, ss_n goes low out of any SPI clock edge context.
                spi_i_nb_bits_d <= 0;                               -- Also, the next SPI clock rising edge implies sampling the current MOSI data.
                spi_state_d <= S_GetCommand;                           -- This is why the transition is in charge of updating the spi_command register.
                spi_command_d <= spi_command_q(6 downto 0)&mosi;
            end if;
        
        when S_GetCommand =>
            
            if fifo_miso_empty = '0' and spi_i_nb_bits_q < 6 then                           -- If the MISO fifo has something new to send, here we read it.
                fifo_miso_re <= '1';                                                        -- The only condition to request a FIFO read is the data must be propagated BEFORE the parse of the command byte.
                miso_read_req_d <= true;                                                    -- (allowing the one clock cycle delay between the read request and the output data propagation).
            end if;
            
            spi_command_d <= spi_command_q(6 downto 0)&mosi;
            spi_i_nb_bits_d <= spi_i_nb_bits_q + 1;
            
            if miso_read_req_q then

                spi_miso_d <= fifo_miso_data_out;
                spi_miso_buffer_d <= fifo_miso_data_out;
                miso_read_req_d <= false;
            else
                spi_miso_buffer_d <= spi_miso_q;
            end if;
            
            if spi_i_nb_bits_q = 6 then                                         -- The 7th bit is received. Next rising edge will give the requested command.
                spi_state_d <= S_ParseCommand;                                  -- A different state is needed because we do not want to handle a new data register value at the same time we are supposed
            end if;                                                             -- to propagate it.
            
            if ss_n = '1' then
                spi_state_d <= S_Idle;
            end if;
            
        when S_ParseCommand =>                                                  -- spi_command_q contains the whole command.
                                                                                -- however, this state implies that the data must be ready on the MISO line if a read is requested,
            case spi_command_q is                                               -- or the data must be sampled into the mosi register at the next clock rising edge in the case of a write.
                when write_command =>
                    spi_state_d <= S_Write;
                    spi_mosi_d <= spi_mosi_q(d_width - 2 downto 0) & mosi;
                when read_command =>
                    spi_state_d <= S_Read;
                    miso <= spi_miso_q(d_width-1);
                    
                when others => spi_state_d <= S_WaitEndOfCS;
            end case;
            
            spi_i_nb_bits_d <= spi_i_nb_bits_q + 1;

            
            if ss_n = '1' then
                spi_state_d <= S_Idle;
            end if;
            
        when S_Write =>
            spi_mosi_d <= spi_mosi_q(d_width - 2 downto 0) & mosi;
            spi_i_nb_bits_d <= spi_i_nb_bits_q + 1;
            
            if spi_i_nb_bits_q = (8 + d_width - 2) then                             -- The signals for data recording are set one clock cycle before as the next clock rising edge is the last for the
                spi_state_d <= S_Idle;                                              -- current packet. This last rising edge will write the spi_mosi_q value into the FIFO.
                fifo_mosi_we <= '1';
                fifo_mosi_data_in <= spi_mosi_q(d_width - 2 downto 0)&mosi;
            end if;
            
            if ss_n = '1' then
                spi_state_d <= S_Idle;
            end if;
            
            
        when S_Read =>
            miso <= spi_miso_buffer_q(d_width-1);
            spi_miso_buffer_d <= spi_miso_buffer_q(d_width-2 downto 0)&'0';
            spi_i_nb_bits_d <= spi_i_nb_bits_q + 1;
            
            if spi_i_nb_bits_q = (8 + d_width) then
                spi_state_d <= S_Idle;
            end if;
            
            if ss_n = '1' then
                spi_state_d <= S_Idle;
            end if;
            
        when S_WaitEndOfCS =>   -- This state should be only enabled
                                -- if a wrong command has been transmitted.
            
            if ss_n = '1' then
                spi_state_d <= S_Idle;
            end if;
            
            
    end case;
            
  end process;
END logic;
