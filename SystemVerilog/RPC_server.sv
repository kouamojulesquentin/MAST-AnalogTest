
`timescale 1ns/1ns
`define N_COMMANDS 2
`define SDR_command `N_COMMANDS'b01
`define SIR_command `N_COMMANDS'b10
`define HALF_CLKPERIOD 5
`define CLKPERIOD (`HALF_CLKPERIOD*2)
//MAST_MSB_VECTOR : leftmost bit is first to be scanned
//`define MAST_MSB_VECTOR 

`ifndef MAST_MSB_VECTOR 
//MAST_LSB_VECTOR : rightmost bit is first to be scanned (default in MAST)
`define MAST_LSB_VECTOR 
`endif


  import "DPI-C" context function void init_server_cpp();
  import "DPI-C" context task do_runtest();
  import "DPI-C" context function int get_scanbits();
  import "DPI-C" context task get_data_to_SUT(inout byte data_to_SUT[]);
  import "DPI-C" context task send_data_from_SUT(int nbits, inout byte data_from_SUT[]);

module top();
  
  
  integer remaining_cycles=2; //non-zero value to start simulation
   reg clk=0; 
   reg Resetn = 0;
     typedef enum bit {FALSE,TRUE} boolean;
  boolean  backend_active;      // To use boolean as variable-type, use typedef.
  
 bit [`N_COMMANDS-1:0] cmd_queue[$] = {};
 event handshake,cmd_end;
  bit [`N_COMMANDS-1:0] current_command;

 bit start_scan;
 bit [`N_COMMANDS-1:0] tap_command;
 bit   scan_finished ;
 wire  TDI;
 wire   TMS;
 wire TRSTN;
 wire TDO;
 wire  TCK;
 integer  next_vector_length;
 integer current_scanbit;
 bit	 data_to_scan ;
 bit	 data_from_scan;
 bit is_shifting,is_shifting_delay;
 reg [0:7] cur_byte;
 
 byte  SV_data_to_SUT [];
 byte  SV_data_from_SUT [];

 integer scanbits;
 integer scanbytes;
    int byte_n;
    int cur_bit;
 
    int from_SUT_byte,next_from_SUT_byte;
    int from_SUT_bit,next_from_SUT_bit;
 
  // Clock generator
  always
   begin
   //DECOMMENT IF to stop time between DPI calls
    if ((remaining_cycles>0 ) || (backend_active == TRUE))
     begin
      #5 clk = 1;
      #5 clk = 0;
     end
    end
   
   always
    begin
     @(negedge clk)
      remaining_cycles--;
    end

   //delay to wait for TDO
   always
    begin
     @(posedge clk)
       from_SUT_bit = next_from_SUT_bit;
       from_SUT_byte = next_from_SUT_byte;
       is_shifting_delay = is_shifting;
    end

   always
    begin
      @(negedge TCK)
       if (is_shifting_delay == 1)
        begin 
	$display("%d, %d, %d",from_SUT_bit,from_SUT_byte,TDO);
        SV_data_from_SUT[from_SUT_byte][from_SUT_bit]=TDO;
	end
    end
    
   always
    begin
     @(backend_active or current_scanbit or is_shifting)
    if (backend_active==TRUE)
     begin
`ifdef MAST_MSB_VECTOR 
    NOT IMPLEMENTED YET
`else
     if (current_scanbit<scanbits%8)
      begin
       cur_bit= 7-current_scanbit;
       next_from_SUT_bit = 8-scanbits%8+current_scanbit;
       byte_n=0;
      end
     else
      begin
        cur_bit= 7-(current_scanbit-scanbits%8)%8; 
       next_from_SUT_bit = 7-cur_bit;
       byte_n=(scanbits%8==0)?(current_scanbit-scanbits%8)/8:(current_scanbit-scanbits%8)/8+1;
      end	
     
     cur_byte = SV_data_to_SUT[scanbytes-byte_n-1];
     data_to_scan = cur_byte[cur_bit];
     
       next_from_SUT_byte = scanbytes-byte_n-1;          
`endif
     end
    end
   
initial
begin
 while (1)
 begin
   $display("SV : Handshake listener"); 
      @(handshake)
      begin
       current_command=cmd_queue.pop_back();
       case (current_command)
	  `SDR_command:$display("START SDR"); 
	  `SIR_command:$display("START SIR"); 
       endcase 
    tap_command = current_command;
    start_scan=1;
    backend_active = TRUE;
    #`CLKPERIOD start_scan = 0;
    $display("SV : wait for scan"); 
    @(negedge scan_finished);
    $display("SV finished"); 
    backend_active = FALSE;
   
    $display("from_SUT: %d bits over %d bytes",scanbits,scanbytes); 
   for (int i = 0; i<scanbytes  ; i++) 
    $display("Vlog: %d %x",i, SV_data_from_SUT[i] ); 
   send_data_from_SUT(scanbits,SV_data_from_SUT);
      end 
    end
 end
   
  export "DPI-C" task runtest;
  task runtest(int runcycles);
   remaining_cycles = runcycles;
     #(runcycles*10);
  endtask
  
  export "DPI-C" task do_SDR;
  task do_SDR();
     $display("START SDR"); 
   start_scan = 0;
    backend_active = FALSE;
     $display("SDR : prepare data"); 
    scanbits= get_scanbits();
    scanbytes=(scanbits%8==0)?(scanbits/8):(scanbits/8+1);
    SV_data_to_SUT = new[scanbytes];
    get_data_to_SUT(SV_data_to_SUT);
    //Communicate with Master TAP
    $display("SDR %d bits over %d bytes",scanbits,scanbytes); 
   for (int i = 0; i<scanbytes  ; i++) 
    $display("Vlog: %d %x",i, SV_data_to_SUT[i] ); 
    
    SV_data_from_SUT = new[scanbits];
    next_vector_length=scanbits;
    tap_command = `SDR_command;
    start_scan=1;
    backend_active = TRUE;
    #`CLKPERIOD start_scan = 0;
    $display("SDR : wait for scan"); 
    @(negedge scan_finished);
    $display("SDR finished"); 
    backend_active = FALSE;
   
    $display("from_SUT: %d bits over %d bytes",scanbits,scanbytes); 
   for (int i = 0; i<scanbytes  ; i++) 
    $display("Vlog: %d %x",i, SV_data_from_SUT[i] ); 
   send_data_from_SUT(scanbits,SV_data_from_SUT);
    
  endtask
  
  export "DPI-C" task do_SIR;
  task do_SIR(int bitsCount);

     $display("START SIR"); 
    start_scan = 0;
    backend_active = FALSE;
     $display("SIR : prepare data"); 
    scanbits= bitsCount; //get_scanbits();
    scanbytes=(scanbits%8==0)?(scanbits/8):(scanbits/8+1);
     $display("SIR : Scanbits = %d, scanbytes = %d",scanbits,scanbytes); 
    SV_data_to_SUT = new[scanbytes];
    
 
    //Communicate with Master TAP
    
      $display("SIR start TAP"); 
   SV_data_from_SUT = new[scanbytes];
    next_vector_length=scanbits;
    tap_command = `SIR_command;
    start_scan=1;
    backend_active = TRUE;
      $display("SIR ..Using queue..."); 
    cmd_queue.push_front(tap_command);
    ->handshake;
//    @(cmd_end);
    
//      $display("SIR ..1..."); 
//    #`CLKPERIOD start_scan = 0;
//    $display("SIR : wait for scan"); 
//    @(negedge scan_finished);
//    $display("SIR finished"); 
//    backend_active = FALSE;
   
//   send_data_from_SUT(scanbits,SV_data_from_SUT);
    
  endtask

  initial 
  begin
   Resetn = 0;
   #100 Resetn =1;
   end

  initial 
  begin
   init_server_cpp();
   run();
  end
  
  task run();
    logic a = 1'bx;

    scanbits=0;
    $display ("-----------------------------------");
    $display ("Running XML Server");
    $display ("--------- time is %0t ",$time);
    $display ("-----------------------------------");
//    do_runtest();
    $display("FINISHED :: time is %0t",$time);
    
  endtask


//Master TAPFSM controlled by DPI calls

master_tapfsm  Master_TAP 
     (  .Clk(clk),
       .Resetn(Resetn),
        .start_scan(start_scan),
	.tap_command(tap_command),
	.next_vector_length(next_vector_length),
	.scan_finished(scan_finished),
	.current_bit(current_scanbit),
	.data_to_scan(data_to_scan),
	.data_from_scan(data_from_scan),
	.is_shifting(is_shifting),

	
       	.TDI	(TDI),
	.TCK   (TCK),
	.TMS   (TMS),
        .TRSTN (TRSTN),
	.TDO(TDO)
	   );       
// ------------------------------SYSTEM UNDER TEST -----------------------
//Slave TAP
SUT SUT_Top   
     ( //Simulation signals
      .Clk(clk),
          //SUT Signals
	.TCK (TCK),
        .TMS (TMS),
        .TRSTN(TRSTN),
        .TDI (TDI), //NB!
        .TDO (TDO) //NB!
	   );       


// ----------------------------END OF SYSTEM UNDER TEST -------------------

endmodule
