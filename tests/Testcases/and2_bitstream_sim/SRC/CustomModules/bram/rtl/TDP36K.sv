`include "bram.svh"
// ---- Default Nettype ----
`default_nettype wire
  module TDP36K (
                 input               RESET_ni,
                 input               SCAN_MODE_i,
        
// Ports for 1st 18K RAMfifo    59 Inputs - 42 outputs
		 input logic         WEN_A1_i, WEN_B1_i,
		 input logic         REN_A1_i, REN_B1_i,
		 input logic         CLK_A1_i, CLK_B1_i,
		 input logic [1:0]   BE_A1_i, BE_B1_i,
		 input logic [14:0]  ADDR_A1_i, 
		 input logic [14:0]  ADDR_B1_i,
		 input logic [17:0]  WDATA_A1_i, WDATA_B1_i,

		 output logic [17:0] RDATA_A1_o, RDATA_B1_o,
		 input logic         FLUSH1_i,

//CCF Bits specific to RAMFIFO 2  41 inputs		  		  
	         input logic         SYNC_FIFO1_i,
		 input logic [2:0]   RMODE_A1_i, RMODE_B1_i,
		 input logic [2:0]   WMODE_A1_i, WMODE_B1_i,
		 input logic         FMODE1_i, 
		 input logic         POWERDN1_i, 
	 	 input logic         SLEEP1_i,
		 input logic         PROTECT1_i,
		 input logic [11:0]  UPAE1_i,
		 input logic [11:0]  UPAF1_i,
		  
// Ports for 2nd 18K RAMfifo  57 inputs 42 outputs
		 input logic         WEN_A2_i, WEN_B2_i,
		 input logic         REN_A2_i, REN_B2_i,
		 input logic         CLK_A2_i, CLK_B2_i,
		 input logic [1:0]   BE_A2_i, BE_B2_i,
		 input logic [13:0]  ADDR_A2_i, ADDR_B2_i,
		 input logic [17:0]  WDATA_A2_i, WDATA_B2_i,
		 output logic [17:0] RDATA_A2_o, RDATA_B2_o,
		 input logic         FLUSH2_i,
		  
//CCF Bits specific to RAMFIFO 2  41 inputs		  
		 input logic         SYNC_FIFO2_i,
		 input logic [2:0]   RMODE_A2_i, RMODE_B2_i,
		 input logic [2:0]   WMODE_A2_i, WMODE_B2_i,
		 input logic         FMODE2_i, 
		 input logic         POWERDN2_i, 
	 	 input logic         SLEEP2_i,
		 input logic         PROTECT2_i,
		 input logic [10:0]  UPAE2_i,
		 input logic [10:0]  UPAF2_i,

//common CCF bits  10 inputs 
		 input               SPLIT_i,
		 input [19:0]        RAM_ID_i,
		 
//Preload Bus 66 inputs, 66 outputs
		 input logic         PL_INIT_i, PL_ENA_i, PL_REN_i, PL_CLK_i,
	         input logic [1:0]   PL_WEN_i,
		 input logic [31:0]  PL_ADDR_i,
		 input logic [35:0]  PL_DATA_i,
 		 output logic        PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o,
		 output logic [1:0]  PL_WEN_o,
		 output logic [31:0] PL_ADDR_o,
		 output logic [35:0] PL_DATA_o,
         input logic [2:0] rwm
		 );
   logic        EMPTY2, EPO2, EWM2;
   logic 	FULL2, FMO2, FWM2;
   logic 	EMPTY1, EPO1, EWM1;
   logic 	FULL1, FMO1, FWM1;
   logic 	UNDERRUN1, OVERRUN1;
   logic 	UNDERRUN2,OVERRUN2;
   logic        UNDERRUN3, OVERRUN3;
   logic 	EMPTY3, EPO3, EWM3;
   logic 	FULL3, FMO3, FWM3;
   logic        PL_CLK,PL_CLKn;
   

   logic 	ram_fmode1, ram_fmode2;
   
   logic [17:0] ram_rdata_a1, ram_rdata_b1;
   logic [17:0] ram_rdata_a2, ram_rdata_b2;
   logic [17:0] ram_wdata_a1, ram_wdata_b1;
   logic [17:0] ram_wdata_a2, ram_wdata_b2; 
   logic [14:0] laddr_a1;
   logic [14:0] laddr_b1;
   logic [13:0] ram_addr_a1, ram_addr_b1;
   logic [13:0] ram_addr_a2, ram_addr_b2;
   
   logic 	smux_clk_a1, smux_clk_b1;
   logic 	smux_clk_a2, smux_clk_b2;
   logic [1:0] 	ram_be_a1, ram_be_a2;
   logic [1:0] 	ram_be_b1, ram_be_b2;
   logic [2:0] 	ram_rmode_a1, ram_wmode_a1;
   logic [2:0] 	ram_rmode_b1, ram_wmode_b1;
   logic [2:0] 	ram_rmode_a2, ram_wmode_a2;
   logic [2:0] 	ram_rmode_b2, ram_wmode_b2;
   logic 	ram_ren_a1, ram_ren_b1;
   logic 	ram_ren_a2, ram_ren_b2;
   logic 	ram_wen_a1, ram_wen_b1;
   logic 	ram_wen_a2, ram_wen_b2;
   

   
   logic 	ren_o;
   logic [11:0] ff_raddr, ff_waddr;
   
   logic [35:0] fifo_rdata;
   logic [1:0] 	fifo_rmode;
   logic [1:0] 	fifo_wmode;
   logic [1:0] 	bwl;
   

   logic [17:0] pl_dout0;
   logic [17:0] pl_dout1;
   
   logic        sclk_a1, sclk_b1, sclk_a2, sclk_b2;
   logic        sresetn;

   logic        flush1, flush2;
   logic        preload1, preload2, my_id;
   
   // clock muxes
   /*
   assign PL_CLK = SCAN_MODE_i ? CLK_A1_i : PL_CLK_i;
   assign PL_CLKn = SCAN_MODE_i ? CLK_A1_i : ~PL_CLK_i;
   assign smux_clk_a1 =  preload1 ? PL_CLK_i : CLK_A1_i;
   assign smux_clk_b1 =  FMODE1_i ? (SYNC_FIFO1_i ? CLK_A1_i : CLK_B1_i) : CLK_B1_i;
   assign smux_clk_a2 =  preload2 ? PL_CLK_i : SPLIT_i ? CLK_A2_i : CLK_A1_i;
   assign smux_clk_b2 =  SPLIT_i ? (FMODE2_i ? (SYNC_FIFO2_i ? CLK_A2_i: CLK_B2_i) : CLK_B2_i ) : FMODE1_i ? (SYNC_FIFO1_i ? CLK_A1_i : CLK_B1_i) : CLK_B1_i;
   assign sclk_a1 = SCAN_MODE_i ? CLK_A1_i : smux_clk_a1;
   assign sclk_b1 = SCAN_MODE_i ? CLK_B1_i : smux_clk_b1;
   assign sclk_a2 = SCAN_MODE_i ? CLK_A2_i : smux_clk_a2;
   assign sclk_b2 = SCAN_MODE_i ? CLK_B2_i : smux_clk_b2;
   */
   ql_clkmux U_CLKMUX ( .SCAN_MODE_i(SCAN_MODE_i), .CLK_A1_i(CLK_A1_i), .CLK_B1_i(CLK_B1_i), .CLK_A2_i(CLK_A2_i), .CLK_B2_i(CLK_B2_i), .PL_CLK_i(PL_CLK_i), .FMODE1_i(FMODE1_i), .FMODE2_i(FMODE2_i), .SYNC_FIFO1_i(SYNC_FIFO1_i), .SYNC_FIFO2_i(SYNC_FIFO2_i), .SPLIT_i(SPLIT_i), .preload1(preload1), .preload2(preload2), .sclk_a1(sclk_a1), .sclk_b1(sclk_b1), .sclk_a2(sclk_a2), .sclk_b2(sclk_b2), .PL_CLK(PL_CLK), .PL_CLKn(PL_CLKn) );

   assign sresetn = RESET_ni;
   assign flush1 = SCAN_MODE_i ? RESET_ni : (~FLUSH1_i & RESET_ni);
   assign flush2 = SCAN_MODE_i ? RESET_ni : (~FLUSH2_i & RESET_ni);
   
   assign ram_fmode1 = FMODE1_i & SPLIT_i;
   assign ram_fmode2 = FMODE2_i & SPLIT_i;

   assign my_id = (PL_ADDR_i[31:12] == RAM_ID_i) | (PL_INIT_i);
   assign preload1    = PROTECT1_i ? 1'b0 : (my_id & PL_ENA_i) ;
   assign preload2    = PROTECT2_i ? 1'b0 : (my_id & PL_ENA_i) ;   
   
   assign ram_ren_a1 = SPLIT_i ? REN_A1_i : FMODE1_i ? 0 : REN_A1_i;
   assign ram_ren_a2 = SPLIT_i ? REN_A2_i : FMODE1_i ? 0 : REN_A1_i;
   assign ram_ren_b1 = SPLIT_i ? REN_B1_i : FMODE1_i ? ren_o : REN_B1_i;
   assign ram_ren_b2 = SPLIT_i ? REN_B2_i : FMODE1_i ? ren_o : REN_B1_i;
   
   assign ram_wen_a1 = SPLIT_i ? WEN_A1_i : (FMODE1_i ? (WEN_A1_i) : (WMODE_A1_i == MODE_36) ? WEN_A1_i : WEN_A1_i & ~ADDR_A1_i[4]);
   assign ram_wen_a2 = SPLIT_i ? WEN_A2_i : (FMODE1_i ? (WEN_A1_i) : (WMODE_A1_i == MODE_36) ? WEN_A1_i : WEN_A1_i &  ADDR_A1_i[4]);
   assign ram_wen_b1 = SPLIT_i ? WEN_B1_i : (WMODE_B1_i == MODE_36) ? WEN_B1_i : WEN_B1_i & ~ADDR_B1_i[4];
   assign ram_wen_b2 = SPLIT_i ? WEN_B2_i : (WMODE_B1_i == MODE_36) ? WEN_B1_i : WEN_B1_i &  ADDR_B1_i[4];
   
   
   assign ram_addr_a1 = SPLIT_i ? ADDR_A1_i[13:0] : FMODE1_i ? {ff_waddr[11:2],ff_waddr[0],3'b000} : {ADDR_A1_i[14:5],ADDR_A1_i[3:0]};
   assign ram_addr_b1 = SPLIT_i ? ADDR_B1_i[13:0] : FMODE1_i ? {ff_raddr[11:2],ff_raddr[0],3'b000} : {ADDR_B1_i[14:5],ADDR_B1_i[3:0]};
   assign ram_addr_a2 = SPLIT_i ? ADDR_A2_i[13:0] : FMODE1_i ? {ff_waddr[11:2],ff_waddr[0],3'b000} : {ADDR_A1_i[14:5],ADDR_A1_i[3:0]};
   assign ram_addr_b2 = SPLIT_i ? ADDR_B2_i[13:0] : FMODE1_i ? {ff_raddr[11:2],ff_raddr[0],3'b000} : {ADDR_B1_i[14:5],ADDR_B1_i[3:0]};

   assign bwl = SPLIT_i ? ADDR_A1_i[4:3] : FMODE1_i ? ff_waddr[1:0] : ADDR_A1_i[4:3];
   
      
   /*------------------------------*/
   /*       WRITE_DATA_MUX         */
   /*------------------------------*/
   
   always_comb begin: WDATA_SEL
      case (SPLIT_i)
	1: begin
	   ram_wdata_a1 = WDATA_A1_i;
	   ram_wdata_a2 = WDATA_A2_i;
	   ram_wdata_b1 = WDATA_B1_i;
	   ram_wdata_b2 = WDATA_B2_i;
	   ram_be_a2 = BE_A2_i;
	   ram_be_b2 = BE_B2_i;
	   ram_be_a1 = BE_A1_i;
	   ram_be_b1 = BE_B1_i;
	end
	0: begin  // single RAM/FIFO
	   case (WMODE_A1_i)
	     MODE_36: begin
	     	ram_wdata_a1 = WDATA_A1_i;
		ram_wdata_a2 = WDATA_A2_i;
		ram_be_a2 = FMODE1_i ? 2'b11 : BE_A2_i;
		ram_be_a1 = FMODE1_i ? 2'b11 : BE_A1_i;
	     end
	     MODE_18: begin
		ram_wdata_a1 = WDATA_A1_i;
		ram_wdata_a2 = WDATA_A1_i;
		ram_be_a1 = FMODE1_i ? (ff_waddr[1] ? 2'b00 : 2'b11) : BE_A1_i;
		ram_be_a2 = FMODE1_i ? (ff_waddr[1] ? 2'b11 : 2'b00) : BE_A1_i;
	     end
	     MODE_9: begin
		ram_wdata_a1[7:0] = WDATA_A1_i[7:0];
                ram_wdata_a1[16] = WDATA_A1_i[16];
                ram_wdata_a1[15:8] = WDATA_A1_i[7:0];
                ram_wdata_a1[17] = WDATA_A1_i[16];
		ram_wdata_a2[7:0] = WDATA_A1_i[7:0];
                ram_wdata_a2[16] = WDATA_A1_i[16];
                ram_wdata_a2[15:8] = WDATA_A1_i[7:0];
                ram_wdata_a2[17] = WDATA_A1_i[16];                
		case (bwl)
		  0: {ram_be_a2,ram_be_a1} = 4'b0001;
		  1: {ram_be_a2,ram_be_a1} = 4'b0010;
		  2: {ram_be_a2,ram_be_a1} = 4'b0100;
		  3: {ram_be_a2,ram_be_a1} = 4'b1000;
		endcase
	     end // case: MODE_9
             default: begin
	     	ram_wdata_a1 = WDATA_A1_i;
		ram_wdata_a2 = WDATA_A1_i;
		ram_be_a2 = FMODE1_i ? 2'b11 : BE_A1_i;
		ram_be_a1 = FMODE1_i ? 2'b11 : BE_A1_i;
             end                
	   endcase // case (WMODE_A1)
	   case (WMODE_B1_i)
	     MODE_36: begin
		ram_wdata_b1 = FMODE1_i ? 18'b0 : WDATA_B1_i;
		ram_wdata_b2 = FMODE1_i ? 18'b0 : WDATA_B2_i;
		ram_be_b2 = BE_B2_i;
		ram_be_b1 = BE_B1_i;

	     end
	     MODE_18: begin
		ram_wdata_b1 = FMODE1_i ? 18'b0 : WDATA_B1_i;
		ram_wdata_b2 = FMODE1_i ? 18'b0 : WDATA_B1_i;
		ram_be_b1 = BE_B1_i;
		ram_be_b2 = BE_B1_i;
	     end
	     MODE_9: begin
		ram_wdata_b1[7:0] =  WDATA_B1_i[7:0];
		ram_wdata_b1[16] = WDATA_B1_i[16];
		ram_wdata_b1[15:8] =  WDATA_B1_i[7:0];
		ram_wdata_b1[17] = WDATA_B1_i[16];
		ram_wdata_b2[7:0] =  WDATA_B1_i[7:0];
		ram_wdata_b2[16] = WDATA_B1_i[16];
		ram_wdata_b2[15:8] =  WDATA_B1_i[7:0];
		ram_wdata_b2[17] = WDATA_B1_i[16];


		case (ADDR_B1_i[4:3])
		  0: {ram_be_b2,ram_be_b1} = 4'b0001;
		  1: {ram_be_b2,ram_be_b1} = 4'b0010;
		  2: {ram_be_b2,ram_be_b1} = 4'b0100;
		  3: {ram_be_b2,ram_be_b1} = 4'b1000;		     
		endcase // case (ADDR_A1[4:3])
	     end // case: MODE_9
             default: begin
                ram_wdata_b1 = FMODE1_i ? 18'b0 : WDATA_B1_i;
		ram_wdata_b2 = FMODE1_i ? 18'b0 : WDATA_B1_i;
		ram_be_b2 = BE_B1_i;
		ram_be_b1 = BE_B1_i;
             end
	   endcase // case (WMODE_B1)
	end // case: 0
      endcase // case (SPLIT_i)
   end // block: WDATA_SEL

   always_comb begin
      case (SPLIT_i)
	0: begin
      	 ram_rmode_a1 = (RMODE_A1_i == MODE_36) ? MODE_18 : RMODE_A1_i;
	 ram_rmode_a2 = (RMODE_A1_i == MODE_36) ? MODE_18 : RMODE_A1_i;
	 ram_wmode_a1 = (WMODE_A1_i == MODE_36) ? MODE_18 : FMODE1_i ? MODE_18 : WMODE_A1_i;
	 ram_wmode_a2 = (WMODE_A1_i == MODE_36) ? MODE_18 : FMODE1_i ? MODE_18 : WMODE_A1_i;	 
	 ram_rmode_b1 = (RMODE_B1_i == MODE_36) ? MODE_18 : FMODE1_i ? MODE_18 : RMODE_B1_i;
	 ram_rmode_b2 = (RMODE_B1_i == MODE_36) ? MODE_18 : FMODE1_i ? MODE_18 : RMODE_B1_i;
	 ram_wmode_b1 = (WMODE_B1_i == MODE_36) ? MODE_18 : WMODE_B1_i;
	 ram_wmode_b2 = (WMODE_B1_i == MODE_36) ? MODE_18 : WMODE_B1_i;
	end
	1: begin
       	 ram_rmode_a1 = (RMODE_A1_i == MODE_36) ? MODE_18 : RMODE_A1_i;
	 ram_rmode_a2 = (RMODE_A2_i == MODE_36) ? MODE_18 : RMODE_A2_i;
	 ram_wmode_a1 = (WMODE_A1_i == MODE_36) ? MODE_18 : WMODE_A1_i;
	 ram_wmode_a2 = (WMODE_A2_i == MODE_36) ? MODE_18 : WMODE_A2_i;	 
	 ram_rmode_b1 = (RMODE_B1_i == MODE_36) ? MODE_18 : RMODE_B1_i;
	 ram_rmode_b2 = (RMODE_B2_i == MODE_36) ? MODE_18 : RMODE_B2_i;
	 ram_wmode_b1 = (WMODE_B1_i == MODE_36) ? MODE_18 : WMODE_B1_i;
	 ram_wmode_b2 = (WMODE_B2_i == MODE_36) ? MODE_18 : WMODE_B2_i;
	end
      endcase // case (SPLIT_i)
      
   end // always@ (*)
   

   /*------------------------------*/
   /*       READ_DATA_MUX          */
   /*------------------------------*/
   

   always_comb begin: FIFO_READ_SEL // only for none split fifo data
     case (RMODE_B1_i)
       MODE_36: fifo_rdata = {ram_rdata_b2[17:16],ram_rdata_b1[17:16],ram_rdata_b2[15:0],ram_rdata_b1[15:0]};
       MODE_18:
	 fifo_rdata = ff_raddr[1] ? {18'b0,ram_rdata_b2} : {18'b0,ram_rdata_b1};
       MODE_9: begin
	  case (ff_raddr[1:0])
	    0: fifo_rdata = {19'b0,ram_rdata_b1[16],8'b0,ram_rdata_b1[7:0]};
	    1: fifo_rdata = {19'b0,ram_rdata_b1[17],8'b0,ram_rdata_b1[15:8]};
	    2: fifo_rdata = {19'b0,ram_rdata_b2[16],8'b0,ram_rdata_b2[7:0]};
	    3: fifo_rdata = {19'b0,ram_rdata_b2[17],8'b0,ram_rdata_b2[15:8]};
	  endcase // case (ff_raddr[1:0])
       end
       default:
         fifo_rdata = {ram_rdata_b2,ram_rdata_b1};

     endcase // case (RMODE_B1)
   end
   
   always_comb begin: RDATA_SEL
      case (SPLIT_i)
	1: begin
	   RDATA_A1_o = FMODE1_i ? {10'b0,EMPTY1,EPO1,EWM1,UNDERRUN1,FULL1,FMO1,FWM1,OVERRUN1} : 
		      ram_rdata_a1;
	   RDATA_B1_o = ram_rdata_b1;
	   RDATA_A2_o = FMODE2_i ? {10'b0,EMPTY2,EPO2,EWM2,UNDERRUN2,FULL2,FMO2,FWM2,OVERRUN2} : 
		      ram_rdata_a2;
	   RDATA_B2_o = ram_rdata_b2;
        end
	0: begin
	   if (FMODE1_i) begin
	      RDATA_A1_o =  {10'b0,EMPTY3,EPO3,EWM3,UNDERRUN3,FULL3,FMO3,FWM3,OVERRUN3};
	      RDATA_A2_o = 18'b0;
	   end
	   else begin
	      case (RMODE_A1_i)
		MODE_36: begin
		   RDATA_A1_o = {ram_rdata_a1[17:0]};
		   RDATA_A2_o = {ram_rdata_a2[17:0]};
		end
		MODE_18: begin
		   RDATA_A1_o = (laddr_a1[4] ? ram_rdata_a2 : ram_rdata_a1);
		   RDATA_A2_o = 18'b0;
		end
		MODE_9: begin
		   RDATA_A1_o = laddr_a1[4] ? {{2{ram_rdata_a2[16]}},{2{ram_rdata_a2[7:0]}}} :
                                {{2{ram_rdata_a1[16]}},{2{ram_rdata_a1[7:0]}}} ;
		   RDATA_A2_o = 18'b0;
		end
		MODE_4: begin
		   RDATA_A2_o = 18'b0;
		   RDATA_A1_o[17:4] = 14'b0;
		   RDATA_A1_o[3:0] = laddr_a1[4] ? ram_rdata_a2[3:0] : ram_rdata_a1[3:0];
		end // case: MODE_4
		MODE_2: begin
		   RDATA_A2_o = 18'b0;
		   RDATA_A1_o[17:2] = 16'b0;
		   RDATA_A1_o[1:0] = laddr_a1[4] ? ram_rdata_a2[1:0] : ram_rdata_a1[1:0];;
		end // case: MODE_2
		MODE_1: begin
		   RDATA_A2_o = 18'b0;
		   RDATA_A1_o[17:1] = 17'b0;
		   RDATA_A1_o[0] = laddr_a1[4] ? ram_rdata_a2[0] : ram_rdata_a1[0];
		end
		default: begin
		   RDATA_A1_o = {ram_rdata_a2[1:0],ram_rdata_a1[15:0]};
		   RDATA_A2_o =  {ram_rdata_a2[17:16],ram_rdata_a1[17:16],ram_rdata_a2[15:2]};
		end
	      endcase // case (RMODE_A1_i)
	   end // else: !if(FMODE1_i)
	   
	   case (RMODE_B1_i)
	     MODE_36: begin
		RDATA_B1_o = {ram_rdata_b1};
		RDATA_B2_o =  {ram_rdata_b2};
	     end
	     MODE_18: begin
		RDATA_B1_o = FMODE1_i ? fifo_rdata[17:0] : (laddr_b1[4] ? ram_rdata_b2 : ram_rdata_b1);
		RDATA_B2_o = 18'b0;
	     end
	     MODE_9: begin
		RDATA_B1_o = FMODE1_i ? {fifo_rdata[17:0]} :
			   (laddr_b1[4] ? {1'b0,ram_rdata_b2[16],8'b0,ram_rdata_b2[7:0]} :
                             {1'b0,ram_rdata_b1[16],8'b0,ram_rdata_b1[7:0]} );
		RDATA_B2_o = 18'b0;
	     end
	     MODE_4: begin
		RDATA_B2_o = 18'b0;
		RDATA_B1_o[17:4] = 14'b0;
		RDATA_B1_o[3:0] = laddr_b1[4] ? ram_rdata_b2[3:0] : ram_rdata_b1[3:0];;
	     end // case: MODE_4
	     MODE_2: begin
		RDATA_B2_o = 18'b0;
		RDATA_B1_o[17:2] = 16'b0;
		RDATA_B1_o[1:0] = laddr_b1[4] ? ram_rdata_b2[1:0] : ram_rdata_b1[1:0];
	     end // case: MODE_2
	     MODE_1: begin
		RDATA_B2_o = 18'b0;
		RDATA_B1_o[17:1] = 17'b0;
		RDATA_B1_o[0] = laddr_b1[4] ? ram_rdata_b2[0] : ram_rdata_b1[0];
	     end
	     default: begin
		RDATA_B1_o = ram_rdata_b1;
		RDATA_B2_o = ram_rdata_b2;
	     end
	     
	     endcase // case (RMODE_B1)
	end // case: 0
      endcase // case (SPLIT_i)
   end // block: RDATA_SEL
   
   always_ff@(posedge sclk_a1 or negedge sresetn) begin
      if (sresetn == 0)
        laddr_a1 <= '0;
      else
        laddr_a1 <= ADDR_A1_i;
   end
   
   always_ff@(posedge sclk_b1 or negedge sresetn) begin
      if (sresetn == 0)
        laddr_b1 <= '0;
      else
        laddr_b1 <= ADDR_B1_i;
   end

   always_comb begin
      case (WMODE_A1_i)
	default: fifo_wmode = 2'b00; // 32/36 bit wide
	MODE_36: fifo_wmode = 2'b00; // 32/36 bit wide
	MODE_18: fifo_wmode = 2'b01; // 16/18 bit wide
	MODE_9: fifo_wmode = 2'b10; // 16/18 bit wide
      endcase
      case (RMODE_B1_i)
	default: fifo_rmode = 2'b00; // 32/36 bit wide
	MODE_36: fifo_rmode = 2'b00; // 32/36 bit wide
	MODE_18: fifo_rmode = 2'b01; // 16/18 bit wide
	MODE_9: fifo_rmode = 2'b10; // 16/18 bit wide
      endcase
   end	
   
   fifo_ctl # (.ADDR_WIDTH(12),
		.FIFO_WIDTH(3'd4),
               .DEPTH(7))
   fifo36_ctl (
	       .rclk   (sclk_b1), 
	       .rst_R_n(flush1),
	       .wclk   (sclk_a1), 
	       .rst_W_n(flush1),
	       .ren    (REN_B1_i), 
	       .wen    (ram_wen_a1), 
               .sync(SYNC_FIFO1_i),
	       .rmode  (fifo_rmode), 
	       .wmode  (fifo_wmode), 
	       .ren_o  (ren_o), 
	       .fflags ({FULL3,FMO3,FWM3,OVERRUN3,EMPTY3,EPO3,EWM3,UNDERRUN3}),
	       .raddr  (ff_raddr), 
	       .waddr  (ff_waddr),
	       .upaf   (UPAF1_i),
	       .upae   (UPAE1_i)
			);
   
   TDP18K_FIFO u1(
		  .SCAN_MODE(SCAN_MODE_i),
		  .RESET_ni(sresetn),
		  .RMODE_A_i(ram_rmode_a1),
		  .RMODE_B_i(ram_rmode_b1),
		  .WMODE_A_i(ram_wmode_a1),
		  .WMODE_B_i(ram_wmode_b1),
		  .WEN_A_i(ram_wen_a1),
		  .WEN_B_i(ram_wen_b1), 
		  .REN_A_i(ram_ren_a1),
		  .REN_B_i(ram_ren_b1),
		  .CLK_A_i(sclk_a1),
		  .CLK_B_i(sclk_b1),
		  .BE_A_i(ram_be_a1),
		  .BE_B_i(ram_be_b1),
		  .ADDR_A_i(ram_addr_a1),
		  .ADDR_B_i(ram_addr_b1),
		  .WDATA_A_i(ram_wdata_a1),
		  .WDATA_B_i(ram_wdata_b1),
		  .RDATA_A_o(ram_rdata_a1),
		  .RDATA_B_o(ram_rdata_b1),
		  .EMPTY_o(EMPTY1),
		  .EPO_o(EPO1),
		  .EWM_o(EWM1),
                  .UNDERRUN_o(UNDERRUN1),
		  .FULL_o(FULL1),
		  .FMO_o(FMO1),
		  .FWM_o(FWM1),
                  .OVERRUN_o(OVERRUN1),
		  .FLUSH_ni(flush1),
		  .FMODE_i(ram_fmode1),
                  .UPAF_i(UPAF1_i[10:0]),
		  .UPAE_i(UPAE1_i[10:0]),
		  .SYNC_FIFO_i(SYNC_FIFO1_i),
		  .POWERDN_i(POWERDN1_i),   
		  .SLEEP_i(SLEEP1_i),
		  .PROTECT_i(PROTECT1_i),
		  .PL_INIT_i(PL_INIT_i),
		  .PL_ENA_i(PL_ENA_i),
		  .PL_WEN_i(PL_WEN_i[0]),
		  .PL_REN_i(PL_REN_i),
		  .PL_CLK_i(PL_CLK),
  		  .PL_CLK_ni(PL_CLKn),
		  .PL_ADDR_i(PL_ADDR_i),
		  .PL_DATA_IN_i({PL_DATA_i[33:32],PL_DATA_i[15:0]}),
		  .PL_DATA_OUT_o(pl_dout0),
		  .RAM_ID_i({RAM_ID_i}),
          .rwm(rwm)
		  );
   TDP18K_FIFO u2(
		  .SCAN_MODE(SCAN_MODE_i),
		  .RESET_ni(sresetn),
		  .RMODE_A_i(ram_rmode_a2),
		  .RMODE_B_i(ram_rmode_b2),
		  .WMODE_A_i(ram_wmode_a2),
		  .WMODE_B_i(ram_wmode_b2),
		  .WEN_A_i(ram_wen_a2),
		  .WEN_B_i(ram_wen_b2), 
		  .REN_A_i(ram_ren_a2),
		  .REN_B_i(ram_ren_b2),
		  .CLK_A_i(sclk_a2),
		  .CLK_B_i(sclk_b2),
		  .BE_A_i(ram_be_a2),
		  .BE_B_i(ram_be_b2),
		  .ADDR_A_i(ram_addr_a2),
		  .ADDR_B_i(ram_addr_b2),
		  .WDATA_A_i(ram_wdata_a2),
		  .WDATA_B_i(ram_wdata_b2),
		  .RDATA_A_o(ram_rdata_a2),
		  .RDATA_B_o(ram_rdata_b2),
		  .EMPTY_o(EMPTY2),
		  .EPO_o(EPO2),
		  .EWM_o(EWM2),
                  .UNDERRUN_o(UNDERRUN2),
		  .FULL_o(FULL2),
		  .FMO_o(FMO2),
		  .FWM_o(FWM2),
                  .OVERRUN_o(OVERRUN2),
		  .FLUSH_ni(flush2),
		  .FMODE_i(ram_fmode2),		 
		  .UPAF_i(UPAF2_i),
		  .UPAE_i(UPAE2_i),
		  .SYNC_FIFO_i(SYNC_FIFO2_i),
		  .POWERDN_i(POWERDN2_i),   
		  .SLEEP_i(SLEEP2_i),
		  .PROTECT_i(PROTECT2_i),
		  .PL_INIT_i(PL_INIT_i),
		  .PL_ENA_i(PL_ENA_i),
		  .PL_WEN_i(PL_WEN_i[1]),
		  .PL_REN_i(PL_REN_i),
		  .PL_CLK_i(PL_CLK),
  		  .PL_CLK_ni(PL_CLKn),
		  .PL_ADDR_i(PL_ADDR_i),
		  .PL_DATA_IN_i({PL_DATA_i[35:34],PL_DATA_i[31:16]}),
		  .PL_DATA_OUT_o(pl_dout1),
		  .RAM_ID_i(RAM_ID_i),
          .rwm(rwm)
		  );
		  
   always_comb begin
      PL_DATA_o = PL_REN_i ?
                     {pl_dout1[17:16],pl_dout0[17:16],pl_dout1[15:0],pl_dout0[15:0]}: PL_DATA_i;
      PL_ADDR_o = PL_ADDR_i;
      PL_INIT_o = PL_INIT_i;
      PL_ENA_o = PL_ENA_i;
      PL_WEN_o = PL_WEN_i;
      PL_REN_o = PL_REN_i;
      PL_CLK_o = PL_CLK_i;
   end    
   
endmodule
`default_nettype none
  
