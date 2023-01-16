`include "bram.svh"
// ---- Default Nettype ----
`default_nettype wire
module TDP18K_FIFO (
     input wire 	 SCAN_MODE, 
     input wire 	 RESET_ni, 
     input logic [2:0] 	 RMODE_A_i, RMODE_B_i,
     input logic [2:0] 	 WMODE_A_i, WMODE_B_i,
		 
     input logic 	 WEN_A_i, WEN_B_i,
     input logic 	 REN_A_i, REN_B_i,
     input logic 	 CLK_A_i, CLK_B_i,
     input logic [1:0] 	 BE_A_i, BE_B_i,
     input logic [13:0]  ADDR_A_i, ADDR_B_i,
     input logic [17:0]  WDATA_A_i, WDATA_B_i,
     output logic [17:0] RDATA_A_o, RDATA_B_o,
		    
		    
     output logic 	 EMPTY_o,EPO_o, EWM_o, UNDERRUN_o,
     output logic 	 FULL_o, FMO_o, FWM_o, OVERRUN_o,
     input logic 	 FLUSH_ni,
     input logic 	 FMODE_i,
     input logic 	 SYNC_FIFO_i, 
     input logic 	 POWERDN_i, 
     input logic 	 SLEEP_i,
     input logic 	 PROTECT_i,
     input logic [10:0]  UPAF_i,UPAE_i,
		    //Preload Bus
     input 		 PL_INIT_i, PL_ENA_i, PL_WEN_i, PL_REN_i, 
     input 		 PL_CLK_i, PL_CLK_ni,
     input [31:0] 	 PL_ADDR_i,
     input [17:0] 	 PL_DATA_IN_i,
     output logic [17:0] PL_DATA_OUT_o,
     input [ 19:0] 	 RAM_ID_i,
     input [2:0] rwm
);
   
   
   
   
   logic [17:0] 	       wmsk_a, wmsk_b;
   logic [ 8:0] 	       addr_a, addr_b;
   logic [ 4:0] 	       addr_a_d, addr_b_d;
   
   logic [17:0] 	       ram_rdata_a, ram_rdata_b;
   logic [17:0]                ram_rdata_a_int,ram_rdata_b_int;
   logic [17:0] 	       aligned_wdata_a, aligned_wdata_b;
      
   wire 		       ren_o;
   logic [10:0] 	       ff_raddr, ff_waddr;
   logic [13:0] 	       ram_addr_a, ram_addr_b;
   logic [3:0]                 ram_waddr_a, ram_waddr_b;
   wire 		       preload;
   logic 		       my_id;
   logic                       broadcast;
   
   wire 		       smux_rclk, smux_wclk;
   wire 		       real_fmode;
   wire [3:0] 		       raw_fflags;

   logic [1:0]		       fifo_rmode, fifo_wmode;
   logic 		       smux_clk_a;
   logic 		       smux_clk_b;
   logic 		       ram_ren_a;
   logic 		       ram_ren_b;
   logic 		       ram_wen_a;
   logic 		       ram_wen_b;
   logic 		       cen_a;
   logic 		       cen_b;
   logic                       cen_a_n, cen_b_n;
   logic                       ram_wen_a_n, ram_wen_b_n;


   logic [17:0]                bist_status;
   logic [17:0]                PL_COMP;
   logic                       PL_REN_d;
   
   always @(posedge PL_CLK_i or negedge RESET_ni) begin
      if (RESET_ni == 1'b0) begin
         PL_COMP <= 18'h0;
         PL_REN_d <= 1'b0;
      end else begin
         PL_COMP <= PL_DATA_IN_i;
         PL_REN_d <= PL_REN_i;
      end
   end
   

   always @ (posedge PL_CLK_ni or negedge RESET_ni) begin
      if (RESET_ni == 1'b0)
        bist_status <= 18'b0;
      else begin
         if (PL_REN_d == 1'b1)
           bist_status <= (broadcast & PL_INIT_i) ? (PL_COMP ^ ram_rdata_a_int) | bist_status : bist_status;
         else if (PL_WEN_i == 1'b1)
           bist_status <= broadcast ? bist_status : PL_DATA_IN_i;
      end
   end
            
      
   always_comb begin
      fifo_rmode = (RMODE_B_i == MODE_9) ? 2'b10 : 2'b01;
      fifo_wmode = (WMODE_A_i == MODE_9) ? 2'b10 : 2'b01;
   end

   always_comb begin
      my_id = 1'b0;
      broadcast = 1'b0;
      if ((RAM_ID_i[19:10] == PL_ADDR_i[31:22]) || (PL_ADDR_i[31:22] == 10'b0))
        if ((RAM_ID_i[9:0] == PL_ADDR_i[21:12]) || (PL_ADDR_i[21:12] == 10'b0))
          my_id = PL_ENA_i;
      if ((PL_ADDR_i[31:12] == 20'b0))
        broadcast = PL_ENA_i;
   end
   

		   
   assign preload    = PROTECT_i ? 1'b0 : (my_id & PL_ENA_i & PL_WEN_i)  | ( PL_ENA_i & PL_REN_i & (~broadcast | my_id));   
   assign smux_clk_a =  CLK_A_i; // preload ? PL_CLK : CLK_A;   //Preload only on Port A
   assign smux_clk_b =  CLK_B_i;  //FMODE_i ? (SYNC_FIFO_i ? CLK_A_i : CLK_B_i) : CLK_B_i;  // Preload deactivates Port B 
   assign real_fmode = preload ? 1'b0     : FMODE_i;

   assign ram_ren_a      = preload ? PL_REN_i : FMODE_i ? 0 :REN_A_i ;
   assign ram_wen_a      = preload ? PL_WEN_i   : (WEN_A_i); 

   assign ram_ren_b      = preload ? 1'b0  : ( real_fmode ? ren_o : REN_B_i);
   assign ram_wen_b      = preload ? 1'b0 : FMODE_i ? 1'b0 : WEN_B_i;

   assign cen_b = (ram_ren_b | ram_wen_b);
   assign cen_a = (ram_ren_a | ram_wen_a);

   
   

   
   assign ram_waddr_b = preload ? 4'b0 : real_fmode ? {ff_raddr[0],3'b000} : ADDR_B_i[3:0];
   assign ram_waddr_a = preload ? 4'b0 : real_fmode ? {ff_waddr[0],3'b000} : ADDR_A_i[3:0];   
   
   assign ram_addr_b = preload ? {PL_ADDR_i[9:0],4'h0} : 
		       (real_fmode ? {ff_raddr[10:0],3'h0} : {ADDR_B_i[13:4],addr_b_d[3:0]});
   assign ram_addr_a = preload ? {PL_ADDR_i[9:0],4'h0} : 
		       (real_fmode ? {ff_waddr[10:0],3'h0} : {ADDR_A_i[13:4], addr_a_d[3:0]});

   always_ff @ (posedge CLK_A_i or negedge RESET_ni) begin
      if (RESET_ni == 1'b0)
	addr_a_d[3:0] <= 4'h0;
      else
	addr_a_d[3:0] <= ADDR_A_i[3:0];
   end
   always_ff @ (posedge CLK_B_i or negedge RESET_ni) begin
      if (RESET_ni == 1'b0)
	addr_b_d[3:0] <= 4'h0;
      else
	addr_b_d[3:0] <= ADDR_B_i[3:0];
   end

   assign cen_a_n = ~cen_a;
   assign ram_wen_a_n = ~ram_wen_a;
   assign cen_b_n = ~cen_b;
   assign ram_wen_b_n = ~ram_wen_b;



// assign  ram_rdata_a = SCAN_MODE_i ? aligned_wdata_a&~{ram_addr_a[13:4],cen_a_n,ram_wen_a_n,6'b0}+wmsk_a : ram_rdata_a_int;
// assign  ram_rdata_b = SCAN_MODE_i ? aligned_wdata_b&~{ram_addr_b[13:4],cen_b_n,ram_wen_b_n,6'b0}+wmsk_b : ram_rdata_b_int;

wire int_a, int_b;
assign int_a = (cen_a_n ^ ram_wen_a_n) ^ (^wmsk_a) ^ (^ram_addr_a[13:4]);
assign int_b = (cen_b_n ^ ram_wen_b_n) ^ (^wmsk_b) ^ (^ram_addr_b[13:4]);

reg obs_a, obs_b; // dont_touch
always @(posedge smux_clk_a) obs_a <= int_a;
always @(posedge smux_clk_b) obs_b <= int_b;

assign  ram_rdata_a = SCAN_MODE ? {aligned_wdata_a[17:1],int_a ^ aligned_wdata_a[0]} : ram_rdata_a_int;
assign  ram_rdata_b = SCAN_MODE ? {aligned_wdata_b[17:1],int_b ^ aligned_wdata_b[0]} : ram_rdata_b_int;

`ifndef behavioral
dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc uram
  (
   .DO_A(ram_rdata_a_int[17:0]),
   .DO_B(ram_rdata_b_int[17:0]),

   .A_A(ram_addr_a[13:4]),
   .A_B(ram_addr_b[13:4]),
   .T_A_A(ram_addr_a[13:4]),
   .T_A_B(ram_addr_b[13:4]),
   .DI_A(aligned_wdata_a),
   .DI_B(aligned_wdata_b),
   .T_DI_A(aligned_wdata_a),
   .T_DI_B(aligned_wdata_b),
   .T_BE_N(1'b1),
   .CE_N_A(cen_a_n),
   .CE_N_B(cen_b_n), 
   .T_CE_N_A(cen_a_n),
   .T_CE_N_B(cen_b_n),
   .GWE_N_A(ram_wen_a_n),
   .GWE_N_B(ram_wen_b_n),
   .T_GWE_N_A(ram_wen_a_n),
   .T_GWE_N_B(ram_wen_b_n),
   .BWE_N_A(wmsk_a),
   .BWE_N_B(wmsk_b),
   .T_BWE_N_A(wmsk_a),
   .T_BWE_N_B(wmsk_b),
   .T_RWM_A(rwm), 
   .T_RWM_B(rwm),  
   .CLK_A(smux_clk_a),
   .CLK_B(smux_clk_b)
 );
`else 
   sram1024x18 uram(
		   .clk_a(smux_clk_a),
		   .cen_a(cen_a_n), 
		   .wen_a(ram_wen_a_n),
		   .addr_a(ram_addr_a[13:4]),
		   .wmsk_a(wmsk_a),
		   .wdata_a(aligned_wdata_a),
		   .rdata_a(ram_rdata_a_int),
		   
		   .clk_b(smux_clk_b),
		   .cen_b(cen_b_n), 
		   .wen_b(ram_wen_b_n),
		   .addr_b(ram_addr_b[13:4]),
		   .wmsk_b(wmsk_b),
		   .wdata_b(aligned_wdata_b),
		   .rdata_b(ram_rdata_b_int) );
`endif
   fifo_ctl # (.ADDR_WIDTH(11),
		.FIFO_WIDTH(2),
               .DEPTH(6))
   fifo_ctl (
			.rclk   (smux_clk_b), 
			.rst_R_n(FLUSH_ni),
			.wclk   (smux_clk_a), 
			.rst_W_n(FLUSH_ni),
			.ren    (REN_B_i), 
			.wen    (ram_wen_a), 
	                .sync(SYNC_FIFO_i),
		        .rmode  (fifo_rmode), 
			.wmode  (fifo_wmode), 
			.ren_o  (ren_o), 
			.fflags ({FULL_o,FMO_o,FWM_o,OVERRUN_o,EMPTY_o,EPO_o,EWM_o,UNDERRUN_o}),
			.raddr  (ff_raddr), 
			.waddr  (ff_waddr),
			.upaf   (UPAF_i),
			.upae   (UPAE_i)
			);

   always_comb begin: PRELOAD_DATA
     if (my_id & ram_ren_a)
       PL_DATA_OUT_o =  PL_INIT_i ?  (broadcast ? PL_DATA_IN_i : bist_status) : 
                        (broadcast ? (PL_DATA_IN_i | bist_status) : ram_rdata_a_int);
      else
	PL_DATA_OUT_o = PL_DATA_IN_i;
   end
   
   
   /*------------------------------*/
   /*       WRITE_DATA_MUX         */
   /*------------------------------*/
   
   always_comb begin: WDATA_MODE_SEL
      aligned_wdata_a = 18'h0;
      aligned_wdata_b = 18'h0;      
      if (ram_wen_a == 1) begin
	 if (preload) begin
	    aligned_wdata_a    = PL_DATA_IN_i;
	    wmsk_a = 18'h0;
	 end else begin
	    case (WMODE_A_i)
	      MODE_18: begin
		 aligned_wdata_a = WDATA_A_i;
		 {wmsk_a[17],wmsk_a[15: 8]} = FMODE_i ? 9'h0 : BE_A_i[1] ? 9'h000 : 9'h1ff;
		 {wmsk_a[16],wmsk_a[ 7: 0]} = FMODE_i ? 9'h0 : BE_A_i[0] ? 9'h000 : 9'h1ff;
	      end
	      MODE_9: begin
      	         aligned_wdata_a = {{2{WDATA_A_i[16]}},{2{WDATA_A_i[7:0]}}};
		 {wmsk_a[17],wmsk_a[15: 8]} = ram_waddr_a[3] ? 9'h000 : 9'h1ff ;
		 {wmsk_a[16],wmsk_a[ 7: 0]} = ram_waddr_a[3] ? 9'h1ff : 9'h000 ;
	      end
	      MODE_4: begin
	      	 aligned_wdata_a = {2'b00,{4{WDATA_A_i[3:0]}}};
		 wmsk_a[17:16] = 2'b00;
		 wmsk_a[15:12] = (ram_waddr_a[3:2] == 2'b11) ? 4'h0 : 4'hf ;
		 wmsk_a[11:8] = (ram_waddr_a[3:2] == 2'b10) ? 4'h0 : 4'hf ;
		 wmsk_a[7:4] = (ram_waddr_a[3:2] == 2'b01) ? 4'h0 : 4'hf ;
		 wmsk_a[3:0] = (ram_waddr_a[3:2] == 2'b00) ? 4'h0 : 4'hf ;
	      end
	      MODE_2: begin
		 aligned_wdata_a = {2'b00,{8{WDATA_A_i[1:0]}}};
		 wmsk_a[17:16] = 2'b00;
		 wmsk_a[15:14] = (ram_waddr_a[3:1] == 3'b111) ? 2'h0 : 2'h3 ;
		 wmsk_a[13:12] = (ram_waddr_a[3:1] == 3'b110) ? 2'h0 : 2'h3 ;
		 wmsk_a[11:10] = (ram_waddr_a[3:1] == 3'b101) ? 2'h0 : 2'h3 ;
		 wmsk_a[ 9: 8] = (ram_waddr_a[3:1] == 3'b100) ? 2'h0 : 2'h3 ;
		 wmsk_a[ 7: 6] = (ram_waddr_a[3:1] == 3'b011) ? 2'h0 : 2'h3 ;
		 wmsk_a[ 5: 4] = (ram_waddr_a[3:1] == 3'b010) ? 2'h0 : 2'h3 ;
		 wmsk_a[ 3: 2] = (ram_waddr_a[3:1] == 3'b001) ? 2'h0 : 2'h3 ;
		 wmsk_a[ 1: 0] = (ram_waddr_a[3:1] == 3'b000) ? 2'h0 : 2'h3 ;
	      end // case: MODE_2
	      MODE_1: begin
		 aligned_wdata_a = {2'b00,{16{WDATA_A_i[0]}}};
		 wmsk_a = 18'h0_ffff;
		 wmsk_a[{1'b0,ram_waddr_a[3:0]}] = 0;
	      end
              default:
                wmsk_a = 18'h3_ffff;
	    endcase	
	 end // else: !if(preload)
      end // if (ram_wen_a == 1)
      else begin
	 aligned_wdata_a = 18'h0;
	 wmsk_a = 18'h3ffff;
      end // else: !if(ram_wen_a == 1)
      
      if (ram_wen_b == 1) begin
         
	 case (WMODE_B_i)
	   MODE_18: begin
	      aligned_wdata_b = WDATA_B_i;
	      {wmsk_b[17],wmsk_b[15: 8]} = BE_B_i[1] ? 9'h000 : 9'h1ff;
	      {wmsk_b[16],wmsk_b[ 7: 0]} = BE_B_i[0] ? 9'h000 : 9'h1ff;
	   end
	   MODE_9: begin
      	      aligned_wdata_b = {{2{WDATA_B_i[16]}},{2{WDATA_B_i[7:0]}}};
	      {wmsk_b[17],wmsk_b[15: 8]} = ram_waddr_b[3] ?  9'h000 : 9'h1ff ;
	      {wmsk_b[16],wmsk_b[ 7: 0]} = ram_waddr_b[3] ?  9'h1ff : 9'h000 ;
	   end
	   MODE_4: begin
	      aligned_wdata_b = {2'b00,{4{WDATA_B_i[3:0]}}};
	      wmsk_b[17:16] = 2'b00;
	      wmsk_b[15:12] = (ram_waddr_b[3:2] == 2'b11) ? 4'h0 : 4'hf ;
	      wmsk_b[11:8] = (ram_waddr_b[3:2] == 2'b10) ? 4'h0 : 4'hf ;
	      wmsk_b[7:4] = (ram_waddr_b[3:2] == 2'b01) ? 4'h0 : 4'hf ;
	      wmsk_b[3:0] = (ram_waddr_b[3:2] == 2'b00) ? 4'h0 : 4'hf ;
	   end
	   MODE_2: begin
	      aligned_wdata_b = {2'b00,{8{WDATA_B_i[1:0]}}};
	      wmsk_b[17:16] = 2'b00;
	      wmsk_b[15:14] = (ram_waddr_b[3:1] == 3'b111) ? 2'h0 : 2'h3 ;
	      wmsk_b[13:12] = (ram_waddr_b[3:1] == 3'b110) ? 2'h0 : 2'h3 ;
	      wmsk_b[11:10] = (ram_waddr_b[3:1] == 3'b101) ? 2'h0 : 2'h3 ;
	      wmsk_b[ 9: 8] = (ram_waddr_b[3:1] == 3'b100) ? 2'h0 : 2'h3 ;
	      wmsk_b[ 7: 6] = (ram_waddr_b[3:1] == 3'b011) ? 2'h0 : 2'h3 ;
	      wmsk_b[ 5: 4] = (ram_waddr_b[3:1] == 3'b010) ? 2'h0 : 2'h3 ;
	      wmsk_b[ 3: 2] = (ram_waddr_b[3:1] == 3'b001) ? 2'h0 : 2'h3 ;
	      wmsk_b[ 1: 0] = (ram_waddr_b[3:1] == 3'b000) ? 2'h0 : 2'h3 ;
	   end // case: MODE_2
	   MODE_1: begin
	      aligned_wdata_b = {2'b00,{16{WDATA_B_i[0]}}};
	      wmsk_b = 18'h0_ffff;
	      wmsk_b[{1'b0,ram_waddr_b[3:0]}] = 0;
	   end
           default:
             wmsk_b = 18'h3_ffff;
           
	 endcase			
      end // if (ram_wen_b == 1)
      else begin
	 aligned_wdata_b = 18'b0;
	 wmsk_b = 18'h3ffff;
      end // else: !if(ram_wen_b == 1)
   end  // always WDATA_MODE_SEL		 
		   
/*------------------------------*/
/*       READ_DATA_MUX          */
/*------------------------------*/
  
always_comb begin: RDATA_A_MODE_SEL
   RDATA_A_o = 18'h0;   
   case (RMODE_A_i)
     default: RDATA_A_o = 18'h0;
     MODE_18: begin
	RDATA_A_o = ram_rdata_a;
     end
     MODE_9: begin
	{RDATA_A_o[17],RDATA_A_o[15:8]} = 9'h0;
	
	{RDATA_A_o[16],RDATA_A_o[7:0]} = ram_addr_a[3] ? {ram_rdata_a[17],ram_rdata_a[15:8]} :
		       {ram_rdata_a[16],ram_rdata_a[7:0]} ;
     end
     MODE_4: begin
	RDATA_A_o[17:4] = 14'h0;
	
       case (ram_addr_a[3:2])
	 3: RDATA_A_o[3:0] = ram_rdata_a[15:12];
	 2: RDATA_A_o[3:0] = ram_rdata_a[11: 8];
	 1: RDATA_A_o[3:0] = ram_rdata_a[ 7: 4];
	 0: RDATA_A_o[3:0] = ram_rdata_a[ 3: 0];
       endcase // case (ram_addr_a[4:2])
     end // case: MODE_4
     MODE_2: begin
	RDATA_A_o[17:2] = 16'h0;
	case (ram_addr_a[3:1])
	  7: RDATA_A_o[1:0] = ram_rdata_a[15:14];
	  6: RDATA_A_o[1:0] = ram_rdata_a[13:12];
	  5: RDATA_A_o[1:0] = ram_rdata_a[11:10];
	  4: RDATA_A_o[1:0] = ram_rdata_a[ 9: 8];
	  3: RDATA_A_o[1:0] = ram_rdata_a[ 7: 6];
	  2: RDATA_A_o[1:0] = ram_rdata_a[ 5: 4];
	  1: RDATA_A_o[1:0] = ram_rdata_a[ 3: 2];
	  0: RDATA_A_o[1:0] = ram_rdata_a[ 1: 0];
       endcase // case (ram_addr_a[4:1])
     end // case: MODE_2
     MODE_1: begin
	RDATA_A_o[17:1] = 17'h0;
        RDATA_A_o[0] = ram_rdata_a[{1'b0,ram_addr_a[3:0]}];
     end
     
   endcase // case RMODE_A
end // block: RDATA_MODE_SEL
   
always_comb begin
   RDATA_B_o = 18'h15566;   
   case (RMODE_B_i)
     default: begin
	RDATA_B_o = 18'h15566;
     end
     MODE_18: begin
	RDATA_B_o = ram_rdata_b;
     end
     MODE_9: begin
	{RDATA_B_o[17],RDATA_B_o[15:8]} = 9'b0;
	{RDATA_B_o[16],RDATA_B_o[7:0]} = ram_addr_b[3] ? {ram_rdata_b[17],ram_rdata_b[15:8]} :
		       {ram_rdata_b[16],ram_rdata_b[7:0]} ;
     end
     MODE_4: begin
       case (ram_addr_b[3:2])
	 3: RDATA_B_o[3:0] = ram_rdata_b[15:12];
	 2: RDATA_B_o[3:0] = ram_rdata_b[11: 8];
	 1: RDATA_B_o[3:0] = ram_rdata_b[ 7: 4];
	 0: RDATA_B_o[3:0] = ram_rdata_b[ 3: 0];
       endcase // case (ram_addr_b[4:2])
     end // case: MODE_4
     MODE_2: begin
       case (ram_addr_b[3:1])
	  7: RDATA_B_o[1:0] = ram_rdata_b[15:14];
	  6: RDATA_B_o[1:0] = ram_rdata_b[13:12];
	  5: RDATA_B_o[1:0] = ram_rdata_b[11:10];
	  4: RDATA_B_o[1:0] = ram_rdata_b[ 9: 8];
	  3: RDATA_B_o[1:0] = ram_rdata_b[ 7: 6];
	  2: RDATA_B_o[1:0] = ram_rdata_b[ 5: 4];
	  1: RDATA_B_o[1:0] = ram_rdata_b[ 3: 2];
	  0: RDATA_B_o[1:0] = ram_rdata_b[ 1: 0];
       endcase // case (ram_addr_b[4:1])
     end // case: MODE_2
     MODE_1:
       RDATA_B_o[0] = ram_rdata_b[{1'b0,ram_addr_b[3:0]}];
   endcase // case RMODE_B
  end // end RDATA_MODE_SEL
   
endmodule
`default_nettype none
  
