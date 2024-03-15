`timescale 1ns / 1ps
/////////////////////////////////////////////
// Rapid Silicon Raptor Example Design     //
// and2_verilog                            //
// testbench_and2.v - Testbench for design //
/////////////////////////////////////////////

module testbench_and2;

  // Simulation parameters
  parameter period = 10;

  // DUT inputs
  reg [1:0] a     = 2'b0;
  reg [1:0] b     = 2'b0;
  reg clk   = 1'b0;
  reg reset = 1'b1;

  // DUT output
  wire [1:0] c;
  
  // Check signals
  reg reset_delay = 1'b0;
  reg [1:0] a_delay_0 = 2'b00, b_delay_0 = 2'b00;
  reg [1:0] a_delay_1 = 2'b00, b_delay_1 = 2'b00;
  integer error_cnt = 0;

  and2 DUT (
    .a(a),
    .b(b),
    .clk(clk),
    .reset(reset),
    .c(c)
  );

  // Clock
  always
    #period clk = ~clk;

  // Stimulus
  initial begin
    // Assert reset for 100 ns
    #100;
    @(posedge clk);
    #1 reset = 1'b0;
    repeat (2) begin
      @(posedge clk);
      #1 a[0] = ~a[0]; a[1] = ~a[1];
      @(posedge clk);
      #1 b[0] = ~b[0]; b[1] = ~b[1];
    end
    repeat (2)
      @(posedge clk);
    $display ("\n\nSimulation completed at simulation time %t with %d error(s).\n", $realtime, error_cnt);
    $finish;
  end
  
  // Self-chekcing
  initial begin
    $display("");
    // Wait 100 ns
	#100;
	forever begin
	  @(posedge clk);
	  #1 reset_delay = reset;
	  a_delay_0 = {a[0], a_delay_0[1]};
	  b_delay_0 = {b[0], b_delay_0[1]};
	  #(period-2);
	  if (reset_delay && (c != 2'b00)) begin
	    $display("\nError @ %t: Design in reset but output is not zero.", $realtime);
	    error_cnt = error_cnt + 1;
      end else if (!reset_delay && (c[0] != (a_delay_0[0] && b_delay_0[0]))) begin
	    $display("\nError @ %t: c=%b where a=%b and b=%b",$realtime, c[0], a_delay_0[0], b_delay_0[0]);
	    error_cnt = error_cnt + 1;
	  end else
	    $write(".");
	end
  end

  initial
    $timeformat(-9,0," ns", 5);

  initial begin
    $dumpfile("and2.vcd");
    $dumpvars(0,testbench_and2);
 end

endmodule

