 
module ddr_mem_intf (
  input wire clk,
  input wire reset,
  input wire data_in,
  input wire write_enable,
  output wire empty,
  output wire full,  
  output logic data_out

);

  wire [1:0] ddr_in;
  wire empty_in;
  wire full_in;

  reg [1:0] ddr_out;

  wire write_enable_in;

  I_DDR  #(.SLEW_RATE("FAST"), .DELAY(0))  
  iddr_inst(.D(data_in),.R(reset),.C(clk),.Q(ddr_in),.DLY_LD(1),.DLY_ADJ(0),.DLY_INC(1));

  
  O_DDR #(.SLEW_RATE("FAST"), .DELAY(0)) 
  oddr_inst (.D(ddr_out),.R(reset),.C(clk),.Q(data_out),.DLY_ADJ(0),.DLY_LD(1),.DLY_INC(0));
  
  I_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
  ibuf_inst (.I(write_enable),.C(clk),.O(write_enable_in));

  O_BUF#(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
  obuf_inst (.I(empty_in),.C(clk),.O(empty));

  O_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
  obuf_inst1 (.I(full_in),.C(clk),.O(full));

  FIFO #(.DEPTH(256)) 
  fifo (
     .clk(clk),
     .reset(reset),
     .write_enable(write_enable_in),
     .read_enable(!write_enable_in),
     .data_in(ddr_in),
     .data_out(ddr_out),
     .full(full_in),
     .empty(empty_in)
);

endmodule

module FIFO (
  input wire clk,
  input wire reset,
  input wire write_enable,
  input wire read_enable,
  input wire [1:0] data_in,
  output reg [1:0] data_out,
  output wire full,
  output wire empty
);

  parameter DEPTH = 256;  // Depth of the FIFO

  localparam PWIDTH = $clog2(DEPTH);
  reg [1:0] memory [0:DEPTH-1];
  reg [PWIDTH:0] write_pointer;
  reg [PWIDTH:0] read_pointer;
  wire [PWIDTH:0] next_write_pointer;
  wire [PWIDTH:0] next_read_pointer;

  assign next_write_pointer = (write_pointer == DEPTH-1) ? 0 : write_pointer + 1;
  assign next_read_pointer = (read_pointer == DEPTH-1) ? 0 : read_pointer + 1;

  always @(posedge clk or posedge reset) begin
    if (reset) begin
      write_pointer <= 4'b0;
      read_pointer <= 4'b0;
    end else begin
      if (write_enable && !full) begin
        memory[write_pointer] <= data_in;
        write_pointer <= next_write_pointer;
      end
      if (read_enable && !empty) begin
        data_out <= memory[read_pointer];
        read_pointer <= next_read_pointer;
      end
    end
  end

  assign full = (next_write_pointer == read_pointer);
  assign empty = (write_pointer == read_pointer);

endmodule