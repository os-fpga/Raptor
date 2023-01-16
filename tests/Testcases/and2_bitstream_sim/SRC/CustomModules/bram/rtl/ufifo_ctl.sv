`default_nettype wire
`ifndef CK2Q
  `define CK2Q 0.1  
`endif

module fifo_ctl # 
(
 parameter ADDR_WIDTH = 11, // Default 2K bytes
 parameter FIFO_WIDTH = 3'd2,
parameter DEPTH=6  // Native byte width of RAM
 )
(  
   output logic [ADDR_WIDTH-1:0] raddr, 
   output logic [ADDR_WIDTH-1:0] waddr,
   output [7:0]                  fflags,
   output logic                  ren_o,
   input logic                   sync,
   input logic [1:0]             rmode,
   input logic [1:0]             wmode, 
   input logic                   rclk,
   input logic                   rst_R_n,
   input logic                   wclk, 
   input logic                   rst_W_n,
   input logic                   ren, 
   input logic                   wen, 
   input logic [ADDR_WIDTH-1:0]  upaf,
   input logic [ADDR_WIDTH-1:0]  upae
   );
   
   localparam ADDR_PLUS_ONE = ADDR_WIDTH + 1;
   // Synchroniztion flops for crossing clock domains 
   reg [ADDR_WIDTH:0]            pushtopop1;
   reg [ADDR_WIDTH:0]            pushtopop2;
   reg [ADDR_WIDTH:0]            poptopush1;
   reg [ADDR_WIDTH:0]            poptopush2;
   
   wire [ADDR_WIDTH:0]           pushtopop0;
   wire [ADDR_WIDTH:0]           poptopush0;
   wire [ADDR_WIDTH:0]           smux_poptopush;
   wire [ADDR_WIDTH:0]           smux_pushtopop;
   
   
   assign	 smux_poptopush = sync ? poptopush0 : poptopush2 ;
   assign	 smux_pushtopop = sync ? pushtopop0 : pushtopop2 ;
   
   

   always@(posedge rclk or negedge rst_R_n ) begin
      if (~rst_R_n) begin
         pushtopop1 <= #`CK2Q 'h0;
         pushtopop2 <= #`CK2Q 'h0;
      end else begin
         pushtopop1 <= #`CK2Q pushtopop0;
         pushtopop2 <= #`CK2Q pushtopop1;
      end
   end 
   always@(posedge wclk or negedge rst_W_n ) begin
      if (~rst_W_n) begin
         poptopush1 <= #`CK2Q 'h0;
         poptopush2 <= #`CK2Q 'h0;
      end else begin
         poptopush1 <= #`CK2Q poptopush0;
         poptopush2 <= #`CK2Q poptopush1;
      end
   end 
   
   fifo_push  # (.ADDR_WIDTH(ADDR_WIDTH),
                 .DEPTH(DEPTH))
   u_fifo_push
     (
      .wclk(wclk), 
      .wen(wen), 
      .rst_n(rst_W_n),
      .rmode(rmode),
      .wmode(wmode),
      .gcout(pushtopop0),
      .gcin(smux_poptopush),
      .ff_waddr(waddr),
      .pushflags(fflags[7:4]),
      .upaf(upaf)
      );
   
   fifo_pop # (
	       .ADDR_WIDTH(ADDR_WIDTH),
	       .FIFO_WIDTH(FIFO_WIDTH),
               .DEPTH(DEPTH))
   u_fifo_pop
     (
      .rclk(rclk), 
      .ren_in(ren), 
      .rst_n(rst_R_n),
      .rmode(rmode),
      .wmode(wmode),
      .ren_o(ren_o),
      .gcout(poptopush0),
      .gcin(smux_pushtopop),
      .out_raddr(raddr),
      .popflags(fflags[3:0]),
      .upae(upae)
);
   
endmodule // fifo_ctl


module fifo_push # (
                    parameter ADDR_WIDTH = 11,
                    parameter DEPTH = 6
                    )
   (
    output [ 3:0]           pushflags,
    output [ADDR_WIDTH:0]   gcout,
    output [ADDR_WIDTH-1:0] ff_waddr,
    input                   rst_n,
    input                   wclk,
    input                   wen,
    input [ 1:0]            rmode,
    input [ 1:0]            wmode,
    input [ADDR_WIDTH:0]    gcin,
    input [ADDR_WIDTH-1:0]  upaf
    );
   
   localparam ADDR_PLUS_ONE = ADDR_WIDTH + 1;
   
   reg                      full_next;
   reg                      full;
   reg                      paf_next;
   reg                      paf;
   reg                      fmo; // full minus one
   reg                      fmo_next;
   reg                      overflow;
   reg                      p1, p2, f1,f2, q1, q2;
   reg [ 1:0]               gmode;
   reg [ADDR_WIDTH:0]       waddr; 
   reg [ADDR_WIDTH:0]       raddr; 
   reg [ADDR_WIDTH:0]       gcout_reg;
   reg [ADDR_WIDTH:0]       gcout_next;
   reg [ADDR_WIDTH:0]       raddr_next;
   reg [ADDR_WIDTH-1:0]     paf_thresh;
      
   wire                     overflow_next;
   wire [ADDR_WIDTH:0]      waddr_next;
   wire [ADDR_WIDTH:0]      gc8out_next;
   wire [ADDR_WIDTH-1:0]    gc16out_next;
   wire [ADDR_WIDTH-2:0]    gc32out_next;
   wire [ADDR_WIDTH:0]      tmp;
   
   wire [ADDR_WIDTH:0]      next_count, count;
   reg [ADDR_WIDTH:0]       fbytes;
   genvar                   i;

   // count is number of bytes free in the FIFO
   // next count is number that will free if the current clock is a pushflags
   // caclulation the size of the FIFO - (write pointer - read pointer)
   assign next_count = fbytes - ((waddr_next >= raddr_next) ? (waddr_next - raddr_next) : (~raddr_next + waddr_next +1));
   assign count      = fbytes - ((waddr >= raddr) ? (waddr - raddr) : (~raddr + waddr + 1));

   always@(*) begin
      fbytes = 1<<(DEPTH+5);
      //paf_thresh = wmode[1] ? (wmode[0] ? upaf << 1 : upaf) : upaf << 2 ;
      paf_thresh = wmode[1] ? upaf : (wmode[0] ? upaf << 1 : upaf << 2) ;
   end

   always@(*) begin
      case (wmode) 
        2'h0,2'h1,2'h2: begin
           full_next = wen ? f1 : f2;
           fmo_next  = wen ? p1 : p2;
           paf_next  = wen ? q1 : q2;
        end  
        default : begin  
           full_next = 1'b0;
           fmo_next  = 1'b0;
           paf_next  = 1'b0;
        end  
      endcase  
   end
   
   always@(*) begin : PUSH_FULL_FLAGS
      f1 = 1'b0;
      f2 = 1'b0;
      p1 = 1'b0;
      p2 = 1'b0;
      q1 = next_count < {1'b0,paf_thresh};
      q2 = count < {1'b0,paf_thresh};  
      case (wmode)  // Width of Write Port
        2'h0: begin  // 32-bit writes
           case (DEPTH)
             3'h6: begin  // 2k byte depth 
                f1 = ({~waddr_next[11],waddr_next[10:2]} == raddr_next[11:2]);
                f2 = ({~waddr[11],waddr[10:2]} == raddr_next[11:2]);
                p1 = (((waddr_next[10:2]+1) & 9'h1ff) == raddr_next[10:2]);
                p2 = (((waddr[10:2]+1) & 9'h1ff) == raddr_next[10:2]);
             end
             3'h5: begin  // 1k byte depth
                f1 = ({~waddr_next[10],waddr_next[9:2]} == raddr_next[10:2]);
                f2 = ({~waddr[10],waddr[9:2]} == raddr_next[10:2]);
                p1 = (((waddr_next[9:2]+1) & 8'hff) == raddr_next[9:2]);
                p2 = (((waddr[9:2]+1) & 8'hff) == raddr_next[9:2]);
             end
             3'h4: begin  // 512 byte depth
                f1 = ({~waddr_next[9],waddr_next[8:2]} == raddr_next[9:2]);
                f2 = ({~waddr[9],waddr[8:2]} == raddr_next[9:2]);
                p1 = (((waddr_next[8:2]+1) & 7'h7f) == raddr_next[8:2]);
                p2 = (((waddr[8:2]+1) & 7'h7f) == raddr_next[8:2]);
             end
             3'h3: begin  // 256 byte depth
                f1 = ({~waddr_next[8],waddr_next[7:2]} == raddr_next[8:2]);
                f2 = ({~waddr[8],waddr[7:2]} == raddr_next[8:2]);
                p1 = (((waddr_next[7:2]+1) & 6'h3f) == raddr_next[7:2]);
                p2 = (((waddr[7:2]+1) & 6'h3f) == raddr_next[7:2]);
             end
             3'h2: begin  // 128 byte depth
                f1 = ({~waddr_next[7],waddr_next[6:2]} == raddr_next[7:2]);
                f2 = ({~waddr[7],waddr[6:2]} == raddr_next[7:2]);
                p1 = (((waddr_next[6:2]+1) & 5'h1f) == raddr_next[6:2]);
                p2 = (((waddr[6:2]+1) & 5'h1f) == raddr_next[6:2]);
             end
             3'h1: begin  // 64 byte depth
                f1 = ({~waddr_next[6],waddr_next[5:2]} == raddr_next[6:2]);
                f2 = ({~waddr[6],waddr[5:2]} == raddr_next[6:2]);
                p1 = (((waddr_next[5:2]+1) & 4'hf) == raddr_next[5:2]);
                p2 = (((waddr[5:2]+1) & 4'hf) == raddr_next[5:2]);
             end
             3'h0: begin  // 32 byte depth
                f1 = ({~waddr_next[5],waddr_next[4:2]} == raddr_next[5:2]);
                f2 = ({~waddr[5],waddr[4:2]} == raddr_next[5:2]);
                p1 = (((waddr_next[4:2]+1) & 3'h7) == raddr_next[4:2]);
                p2 = (((waddr[4:2]+1) & 3'h7) == raddr_next[4:2]);
             end
             3'h7: begin  // 4k byte depth 
                f1 = ({~waddr_next[ADDR_WIDTH],waddr_next[ADDR_WIDTH-1:2]} == raddr_next[ADDR_WIDTH:2]);
                f2 = ({~waddr[ADDR_WIDTH],waddr[ADDR_WIDTH-1:2]} == raddr_next[ADDR_WIDTH:2]);
                p1 = (((waddr_next[ADDR_WIDTH-1:2]+1) & {ADDR_WIDTH-2{1'b1}}) == raddr_next[ADDR_WIDTH-1:2]);
                p2 = (((waddr[ADDR_WIDTH-1:2]+1) & {ADDR_WIDTH-2{1'b1}}) == raddr_next[ADDR_WIDTH-1:2]);
             end
           endcase // case depth
        end
        2'h1: begin  // 16-bit write mode
           case (DEPTH)
             3'h6: begin  // 2k byte depth
                f1 = ({~waddr_next[11],waddr_next[10:1]} == raddr_next[11:1]);
                f2 = ({~waddr[11],waddr[10:1]} == raddr_next[11:1]);
                p1 = (((waddr_next[10:1]+1) & 10'h3ff) == raddr_next[10:1]);
                p2 = (((waddr[10:1]+1) & 10'h3ff) == raddr_next[10:1]);
             end
             3'h5: begin  // 1k byte depth
                f1 = ({~waddr_next[10],waddr_next[9:1]} == raddr_next[10:1]);
                f2 = ({~waddr[10],waddr[9:1]} == raddr_next[10:1]);
                p1 = (((waddr_next[9:1]+1) & 9'h1ff) == raddr_next[9:1]);
                p2 = (((waddr[9:1]+1) & 9'h1ff) == raddr_next[9:1]);
             end
             3'h4: begin  // 512 byte depth
                f1 = ({~waddr_next[9],waddr_next[8:1]} == raddr_next[9:1]);
                f2 = ({~waddr[9],waddr[8:1]} == raddr_next[9:1]);
                p1 = (((waddr_next[8:1]+1) & 8'hff) == raddr_next[8:1]);
                p2 = (((waddr[8:1]+1) & 8'hff) == raddr_next[8:1]);
             end
             3'h3: begin  // 256 byte depth
                f1 = ({~waddr_next[8],waddr_next[7:1]} == raddr_next[8:1]);
                f2 = ({~waddr[8],waddr[7:1]} == raddr_next[8:1]);
                p1 = (((waddr_next[7:1]+1) & 7'h7f) == raddr_next[7:1]);
                p2 = (((waddr[7:1]+1) & 7'h7f) == raddr_next[7:1]);
             end
             3'h2: begin  // 128 byte depth
                f1 = ({~waddr_next[7],waddr_next[6:1]} == raddr_next[7:1]);
                f2 = ({~waddr[7],waddr[6:1]} == raddr_next[7:1]);
                p1 = (((waddr_next[6:1]+1) & 6'h3f) == raddr_next[6:1]);
                p2 = (((waddr[6:1]+1) & 6'h3f) == raddr_next[6:1]);
             end
             3'h1: begin  // 64 byte depth
                f1 = ({~waddr_next[6],waddr_next[5:1]} == raddr_next[6:1]);
                f2 = ({~waddr[6],waddr[5:1]} == raddr_next[6:1]);
                p1 = (((waddr_next[5:1]+1) & 5'h1f) == raddr_next[5:1]);
                p2 = (((waddr[5:1]+1) & 5'h1f) == raddr_next[5:1]);
             end
             3'h0: begin  // 32 byte depth
                f1 = ({~waddr_next[5],waddr_next[4:1]} == raddr_next[5:1]);
                f2 = ({~waddr[5],waddr[4:1]} == raddr_next[5:1]);
                p1 = (((waddr_next[4:1]+1) & 4'hf) == raddr_next[4:1]);
                p2 = (((waddr[4:1]+1) & 4'hf) == raddr_next[4:1]);
             end
             3'h7: begin  // 4k byte depth
                f1 = ({~waddr_next[ADDR_WIDTH],waddr_next[ADDR_WIDTH-1:1]} == raddr_next[ADDR_WIDTH:1]);
                f2 = ({~waddr[ADDR_WIDTH],waddr[ADDR_WIDTH-1:1]} == raddr_next[ADDR_WIDTH:1]);
                p1 = (((waddr_next[ADDR_WIDTH-1:1]+1) & {ADDR_WIDTH-1{1'b1}}) == raddr_next[ADDR_WIDTH-1:1]);
                p2 = (((waddr[ADDR_WIDTH-1:1]+1) & {ADDR_WIDTH-1{1'b1}}) == raddr_next[ADDR_WIDTH-1:1]);
             end
           endcase // case depth
        end
        2'h2: begin  // 8-bit write mode
           case (DEPTH) 
            3'h6: begin  // 2k byte depth
                f1 = ({~waddr_next[11],waddr_next[10:0]} == raddr_next[11:0]);
                f2 = ({~waddr[11],waddr[10:0]} == raddr_next[11:0]);
                p1 = (((waddr_next[10:0]+1) & 11'h7ff) == raddr_next[10:0]);
                p2 = (((waddr[10:0]+1) & 11'h7ff) == raddr_next[10:0]);
             end
             3'h5: begin  // 1k byte depth
                f1 = ({~waddr_next[10],waddr_next[9:0]} == raddr_next[10:0]);
                f2 = ({~waddr[10],waddr[9:0]} == raddr_next[10:0]);
                p1 = (((waddr_next[9:0]+1) & 10'h3ff) == raddr_next[9:0]);
                p2 = (((waddr[9:0]+1) & 10'h3ff) == raddr_next[9:0]);
             end
             3'h4: begin  // 512 byte depth
                f1 = ({~waddr_next[9],waddr_next[8:0]} == raddr_next[9:0]);
                f2 = ({~waddr[9],waddr[8:0]} == raddr_next[9:0]);
                p1 = (((waddr_next[8:0]+1) & 9'h1ff) == raddr_next[8:0]);
                p2 = (((waddr[8:0]+1) & 9'h1ff) == raddr_next[8:0]);
             end
             3'h3: begin  // 256 byte depth
                f1 = ({~waddr_next[8],waddr_next[7:0]} == raddr_next[8:0]);
                f2 = ({~waddr[8],waddr[7:0]} == raddr_next[8:0]);
                p1 = (((waddr_next[7:0]+1) & 8'hff) == raddr_next[7:0]);
                p2 = (((waddr[7:0]+1) & 8'hff) == raddr_next[7:0]);
             end
             3'h2: begin  // 128 byte depth
                f1 = ({~waddr_next[7],waddr_next[6:0]} == raddr_next[7:0]);
                f2 = ({~waddr[7],waddr[6:0]} == raddr_next[7:0]);
                p1 = (((waddr_next[6:0]+1) & 7'h7f) == raddr_next[6:0]);
                p2 = (((waddr[6:0]+1) & 7'h7f) == raddr_next[6:0]);
             end
             3'h1: begin  // 64 byte depth
                f1 = ({~waddr_next[6],waddr_next[5:0]} == raddr_next[6:0]);
                f2 = ({~waddr[6],waddr[5:0]} == raddr_next[6:0]);
                p1 = (((waddr_next[5:0]+1) & 6'h3f) == raddr_next[5:0]);
                p2 = (((waddr[5:0]+1) & 6'h3f) == raddr_next[5:0]);
             end
             3'h0: begin  // 32 byte depth
                f1 = ({~waddr_next[5],waddr_next[4:0]} == raddr_next[5:0]);
                f2 = ({~waddr[5],waddr[4:0]} == raddr_next[5:0]);
                p1 = (((waddr_next[4:0]+1) & 5'h1f) == raddr_next[4:0]);
                p2 = (((waddr[4:0]+1) & 5'h1f) == raddr_next[4:0]);
             end
             3'h7: begin  // 4k byte depth
                f1 = ({~waddr_next[ADDR_WIDTH],waddr_next[ADDR_WIDTH-1:0]} == raddr_next[ADDR_WIDTH:0]);
                f2 = ({~waddr[ADDR_WIDTH],waddr[ADDR_WIDTH-1:0]} == raddr_next[ADDR_WIDTH:0]);
                p1 = (((waddr_next[ADDR_WIDTH-1:0]+1) & {ADDR_WIDTH{1'b1}}) == raddr_next[ADDR_WIDTH-1:0]);
                p2 = (((waddr[ADDR_WIDTH-1:0]+1) & {ADDR_WIDTH{1'b1}}) == raddr_next[ADDR_WIDTH-1:0]);
             end
           endcase // case depth
        end
        2'h3: begin 
           f1 = 1'b0;
           f2 = 1'b0;
           p1 = 1'b0;
           p2 = 1'b0;
        end
      endcase // case wmode 
   end // always@ begin full flags
   
   always@(*) begin  // calculate grey code transfer size
      case (wmode) 
        2'h0: gmode = 2'h0;
        2'h1: gmode = (rmode == 2'h0) ? 2'h0 : 2'h1;
        2'h2: gmode = (rmode == 2'h2) ? 2'h2 : rmode;
        2'h3: gmode = 2'h3;
      endcase // case (wmode)
   end // always@ begin
   
   
   assign gc8out_next  = ((waddr_next) >> 1) ^ (waddr_next);
   /* verilator lint_off WIDTH */
   assign gc16out_next = ((waddr_next) >> 2) ^ ((waddr_next)>>1);
   assign gc32out_next = ((waddr_next) >> 3) ^ ((waddr_next)>>2);
   /* verilator lint_on WIDTH */
   always@(*) begin
      if (wen) begin
         case (gmode)
           2'h2:  gcout_next = gc8out_next;         // byte mode
           2'h1:  gcout_next = {1'b0,gc16out_next}; // word mode
           /* verilator lint_off WIDTH */
           2'h0:  gcout_next = {2'b0,gc32out_next}; // dword mode
           default: gcout_next = {ADDR_PLUS_ONE{{1'b0}}};
           /* verilator lint_on WIDTH */
         endcase
      end else begin
         /* verilator lint_off WIDTH */
         gcout_next = {ADDR_PLUS_ONE{{1'b0}}};
         /* verilator lint_on WIDTH */
      end  
end
   
   
   always@ (posedge wclk or negedge rst_n) begin
      if (~rst_n) begin
         full  <= #`CK2Q 1'b0;
         fmo   <= #`CK2Q 1'b0;
         paf   <= #`CK2Q 1'b0; 
         /* verilator lint_off WIDTH */
         raddr <= #`CK2Q {ADDR_PLUS_ONE{{1'b0}}};
         /* verilator lint_on WIDTH */
      end else begin
         full  <= #`CK2Q full_next;
         fmo   <= #`CK2Q fmo_next;
         paf   <= #`CK2Q paf_next; // moved to its own always section with no flush condition
         case (gmode)
           0: raddr <= #`CK2Q raddr_next & {{ADDR_WIDTH-1{1'b1}},2'b00};
           /* verilator lint_off WIDTH */
           1: raddr <= #`CK2Q raddr_next & {{ADDR_WIDTH{1'b1}},1'b0};
           /* verilator lint_on WIDTH */
           2: raddr <= #`CK2Q raddr_next & {ADDR_WIDTH+1{1'b1}};
           /* verilator lint_off WIDTH */
           3: raddr <= #`CK2Q 12'h0;
           /* verilator lint_on WIDTH */
	 endcase // case (gmode)
      end
   end
   
   assign overflow_next = full & wen;
   
   
   always@ (posedge wclk or negedge rst_n) begin
      if (~rst_n) begin
         overflow <= #`CK2Q 1'b0;
      end else if (wen == 1'b1) begin
         overflow <= #`CK2Q overflow_next;  // set until fflush clears
      end
   end
   
   
   always@ (posedge wclk or negedge rst_n) begin
      if (~rst_n) begin
         waddr     <= #`CK2Q {ADDR_WIDTH+1{1'b0}};
         gcout_reg <= #`CK2Q {ADDR_WIDTH+1{1'b0}};
      end else if (wen == 1'b1) begin 
         waddr     <= #`CK2Q waddr_next;
         gcout_reg <= #`CK2Q gcout_next;
      end
   end  
   
   assign gcout = gcout_reg;
   
   generate
      for (i = 0; i < (ADDR_WIDTH+1); i= i+1)
        assign tmp[i] = ^(gcin >> i);
   endgenerate
   
   always@(*) begin
      case (gmode)
        2'h0: raddr_next = {tmp[ ADDR_WIDTH-2:0],2'b0} & {{ADDR_WIDTH-1{1'b1}},2'b00};
        2'h1: raddr_next = {tmp[ADDR_WIDTH-1:0],1'b0} & {{ADDR_WIDTH{1'b1}},1'b0};
        2'h2: raddr_next = {tmp[ADDR_WIDTH:0]}      & {ADDR_WIDTH+1{1'b1}};
        default : raddr_next = {ADDR_WIDTH+1{1'b0}};
      endcase // case (gmode)
   end 
   
   assign ff_waddr   = waddr[ADDR_WIDTH-1:0];
   
  //vincentassign pushflags = rst_n ? {full,fmo,paf,overflow} : 4'b1111;
   assign pushflags = {full,fmo,paf,overflow};
   assign waddr_next = waddr + ((wmode == 2'h0) ? 'h4 : ((wmode == 2'h1) ? 'h2 : 'h1));
   
endmodule //end of module fifo_push


module fifo_pop #
  (
   parameter ADDR_WIDTH = 11,
   parameter FIFO_WIDTH = 3'd2,// Native byte width of RAM
   parameter DEPTH=6
   
   )
   (
    output                        ren_o,
    output [ 3:0]                 popflags,
    output logic [ADDR_WIDTH-1:0] out_raddr,
    output [ADDR_WIDTH:0]         gcout,
    input                         rst_n,
    input                         rclk,
    input                         ren_in,
    input [ 1:0]                  rmode,
    input [ 1:0]                  wmode,
    input [ADDR_WIDTH:0]          gcin,
    input [ADDR_WIDTH-1:0]        upae
    );
   localparam ADDR_PLUS_ONE = ADDR_WIDTH + 1;
   
   
   reg                            empty;
   reg                            epo; //empty Plus one
   reg                            pae; 
   reg                            underflow;
   reg                            e1,e2,o1,o2,q1,q2;
   reg [ 1:0]                     bwl_sel;   // byte/word/line select -- lines low address with latched address in ram.
   reg [ 1:0]                     gmode;
   reg [ADDR_WIDTH-1:0]           ff_raddr;  // pre fetch address
   reg [ADDR_WIDTH:0]             waddr;
   reg [ADDR_WIDTH:0]             raddr;
   reg [ADDR_WIDTH:0]             gcout_reg;
   reg [ADDR_WIDTH:0]             gcout_next;
   reg [ADDR_WIDTH:0]             waddr_next; 
   reg [ADDR_WIDTH-1:0]           pae_thresh;
   
   wire                           ren_out;
   wire                           empty_next;
   wire                           pae_next;
   wire                           epo_next;
   wire [ADDR_WIDTH-2:0]          gc32out_next;
   wire [ADDR_WIDTH-1:0]          gc16out_next;
   wire [ADDR_WIDTH:0]            gc8out_next;
   wire [ADDR_WIDTH:0]            raddr_next; 
   wire [ADDR_WIDTH-1:0]          ff_raddr_next;
   wire [ADDR_WIDTH:0]            tmp;
   wire [ADDR_PLUS_ONE:0]            next_count, count;
   reg [ADDR_PLUS_ONE:0]             fbytes;
   genvar                         i;
   
   
   //Count is number of bytes currenly in the FIFO
   //next count is the number of bytes that will be in the FIFO if the current clock is a POP
   // these equations need to be verified for adress wrap conditions.
   assign next_count = (waddr - raddr_next);
   assign count      = (waddr - raddr);
   
   
   always@(*) begin
      fbytes = 1 << (DEPTH+5);
   end 
   
   always@(*) begin
      //pae_thresh = rmode ? (rmode[0] ? upae << 1 : upae) : upae << 2 ;
      pae_thresh = rmode[1] ? upae : (rmode[0] ? upae << 1 : upae << 2);
   end
   
   assign ren_out = empty ? 1'b1 : ren_in;
   
   always@(*) begin
      case (rmode) 
        2'h0: gmode = 2'h0;
        2'h1: gmode = (wmode == 2'h0) ? 2'h0 : 2'h1;
        2'h2: gmode = (wmode == 2'h2) ? 2'h2 : wmode;
        2'h3: gmode = 2'h3;
      endcase // case (rmode)
   end // always@ begin
   
   always@(*) begin // Empty Flags
      e1 = 1'b0;
      e2 = 1'b0;
      o1 = 1'b0;
      o2 = 1'b0;
      /* verilator lint_off WIDTH */
      q1 = next_count < {1'b0,pae_thresh};
      q2 = count < {1'b0,pae_thresh};
      /* verilator lint_on WIDTH */
      case (rmode)  // Width of read port
        2'h0: begin  // 32-bit read
           e1 = raddr_next[ADDR_WIDTH:2] == waddr_next[ADDR_WIDTH:2];
           e2 = raddr[ADDR_WIDTH:2] == waddr_next[ADDR_WIDTH:2];
           o1 = (raddr_next[ADDR_WIDTH:2] + 1) == waddr_next[ADDR_WIDTH:2];
           o2 = (raddr[ADDR_WIDTH:2]+1) == (waddr_next[ADDR_WIDTH:2]);
        end
        2'h1: begin // 16-bit read
           e1 = (raddr_next[ADDR_WIDTH:1] == waddr_next[ADDR_WIDTH:1]);
           e2 = (raddr[ADDR_WIDTH:1] == waddr_next[ADDR_WIDTH:1]);
           o1 = ((raddr_next[ADDR_WIDTH:1] + 1) == waddr_next[ADDR_WIDTH:1]);
           o2 = ((raddr[ADDR_WIDTH:1]+1) == waddr_next[ADDR_WIDTH:1]);
        end
        2'h2: begin // 8-bit read
           e1 = (raddr_next[ADDR_WIDTH:0] ==  waddr_next[ADDR_WIDTH:0]);
           e2 = (raddr[ADDR_WIDTH:0] == waddr_next[ADDR_WIDTH:0]);
           o1 = ((raddr_next[ADDR_WIDTH:0] + 1) ==  waddr_next[ADDR_WIDTH:0]);
           /* verilator lint_off WIDTH */
           o2 = ((raddr[ADDR_WIDTH:0]+1) == waddr_next[11:0]);
           /* verilator lint_on WIDTH */
        end
        2'h3: begin 
           e1 = 1'b0;
           e2 = 1'b0;
           o1 = 1'b0;
           o2 = 1'b0;
        end
      endcase // case gmode
   end // always@ begin
   
   assign empty_next = (ren_in & !empty) ? e1 : e2;     
   assign epo_next = (ren_in & !empty) ? o1 : o2;
   assign pae_next = (ren_in & !empty) ? q1 : q2; 
   
   
   always@ (posedge rclk or negedge rst_n) begin
      if (~rst_n) begin
         empty <= #`CK2Q 1'b0;
         pae   <= #`CK2Q 1'b0;
         epo   <= #`CK2Q 1'b0;
      end else begin
         empty <= #`CK2Q empty_next;
         pae   <= #`CK2Q pae_next;
         epo   <= #`CK2Q epo_next;
      end  
   end  
   
   /* verilator lint_off WIDTH */
   assign gc8out_next  = ((raddr_next) >> 1) ^ (raddr_next);
   assign gc16out_next = ((raddr_next) >> 2) ^ ((raddr_next)>>1);
   assign gc32out_next = ((raddr_next) >> 3) ^ ((raddr_next)>>2);
   /* verilator lint_on WIDTH */
   
   always@(*) begin
      if (ren_in) begin
         case (gmode)
           2'h2:  gcout_next = gc8out_next;         // byte mode
           2'h1:  gcout_next = {1'b0,gc16out_next}; // word mode
           2'h0:  gcout_next = {2'b0,gc32out_next}; // dword mode
           default: gcout_next = 'h0;
         endcase
      end else begin 
         gcout_next = 'h0;
      end
   end  
   
   
   always@ (posedge rclk or negedge rst_n) begin
      if (~rst_n) begin
         /* verilator lint_off WIDTH */
         waddr <= #`CK2Q 12'h0;
         /* verilator lint_on WIDTH */
      end else begin
         waddr <= #`CK2Q waddr_next;
      end
   end
   
   
   always@ (posedge rclk or negedge rst_n) begin
      if (~rst_n) begin
         underflow <= #`CK2Q 1'b0;
         bwl_sel   <= #`CK2Q 2'h0;
         /* verilator lint_off WIDTH */
         gcout_reg <= #`CK2Q 12'h0;
         /* verilator lint_on WIDTH */
      end else if (ren_in) begin
         underflow <= #`CK2Q empty ;          // pop while empty
         if (!empty) begin
	    bwl_sel   <= #`CK2Q raddr_next[1:0]; // byte/word/line select FF 
	    gcout_reg <= #`CK2Q gcout_next;
         end
      end
   end
   
   generate
      for (i = 0; i < ADDR_WIDTH+1; i= i +1)
        assign tmp[i] = ^(gcin >> i);
   endgenerate
   
   always@(*) begin
      case (gmode)
        2'h0: waddr_next = {tmp[ADDR_WIDTH-2:0],2'b0} & {{{ADDR_WIDTH-1{1'b1}}},2'b00};
        2'h1: waddr_next = {tmp[ADDR_WIDTH-1:0],1'b0} & {{{ADDR_WIDTH{1'b1}}},1'b0};  
        2'h2: waddr_next = {tmp[ADDR_WIDTH:0]     } & {ADDR_PLUS_ONE{1'b1}};
        
        default: waddr_next = {ADDR_PLUS_ONE{1'b0}};
      endcase  
   end
   
   assign ff_raddr_next = ff_raddr + ((rmode == 2'h0) ? 'h4 : ((rmode == 2'h1) ? 'h2 : 'h1));
   assign raddr_next    = raddr    + ((rmode == 2'h0) ? 'h4 : ((rmode == 2'h1) ? 'h2 : 'h1));
   
   always@ (posedge rclk or negedge rst_n) begin //? Vincent: Need to check this blcok funtion
      if (~rst_n)
        ff_raddr  <= #`CK2Q '0;
      else if (empty & ~empty_next)  // prefetch address increment when new data available
        ff_raddr  <= #`CK2Q raddr_next[ADDR_WIDTH-1:0]; //HERE!!
      else if ((ren_in & !empty) & ~empty_next)
        ff_raddr  <= #`CK2Q ff_raddr_next;
   end

always@ (posedge rclk or negedge rst_n) begin
  if (~rst_n)
  /* verilator lint_off WIDTH */
    raddr <= #`CK2Q 12'h0;
  /* verilator lint_on WIDTH */
  else if ((ren_in & !empty))
    raddr <= #`CK2Q raddr_next;
end  
   
   always_comb begin
      // out_raddr = {ff_raddr[ADDR_WIDTH-1:2],bwl_sel}; // Since the ram is 4 bytes wide
      case (FIFO_WIDTH)
        3'h2: out_raddr = {ff_raddr[ADDR_WIDTH-1:1],bwl_sel[0]}; // SInce the ram is 2 bytes wide
        3'h4: out_raddr = {ff_raddr[ADDR_WIDTH-1:2],bwl_sel}; // Since the ram is 4 bytes wide
        default: out_raddr = ff_raddr[ADDR_WIDTH-1:0];     
      endcase // case (FIFO_WIDTH)
   end
   
   assign ren_o     = ren_out;
   assign gcout     = gcout_reg;
   assign popflags = {empty,epo,pae,underflow};
   
endmodule // fifo_pop
`default_nettype none
