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
  reg a     = 1'b0;
  reg b     = 1'b0;
  reg clk   = 1'b0;
  reg reset = 1'b1;

  // DUT output
  wire c;
  
  // Check signals
  reg reset_delay = 1'b0;
  reg [1:0] a_delay = 2'b00, b_delay = 2'b00;
  wire a_delay_by_2 = a_delay[0];
  wire b_delay_by_2 = b_delay[0];
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
      #1 a = ~a;
      @(posedge clk);
      #1 b = ~b;
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
	  a_delay = {a, a_delay[1]};
	  b_delay = {b, b_delay[1]};
	  #(period-2);
	  if (reset_delay && (c != 1'b0)) begin
	    $display("\nError @ %t: Design in reset but output is not zero.", $realtime);
	    error_cnt = error_cnt + 1;
      end else if (!reset_delay && (c != (a_delay[0] && b_delay[0]))) begin
	    $display("\nError @ %t: c=%b where a=%b and b=%b",$realtime, c, a_delay[0], b_delay[0]);
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

