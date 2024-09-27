`timescale 1ns/1ps
//Overivew
//========
//This file contains the verilog primitives produced by VPR's
//post-synthesis netlist writer.
//
//If you wish to do back-annotated timing simulation you will need
//to link with this file during simulation.
//
//To ensure currect result when performing back-annoatation with 
//Modelsim see the notes at the end of this comment.
//
//Specifying Timing Edges
//=======================
//To perform timing back-annotation the simulator must know the delay 
//dependancies (timing edges) between the ports on each primitive.
//
//During back-annotation the simulator will attempt to annotate SDF delay
//values onto the timing edges.  It should give a warning if was unable
//to find a matching edge.
//
//
//In Verilog timing edges are specified using a specify block (delimited by the
//'specify' and 'endspecify' keywords.
//
//Inside the specify block a set of specify statements are used to describe
//the timing edges.  For example consider:
//
//  input [1:0] in;
//  output [1:0] out;
//  specify
//      (in[0] => out[0]) = "";
//      (in[1] => out[1]) = "";
//  endspecify
//
//This states that there are the following timing edges (dependancies):
//  * from in[0] to out[0]
//  * from in[1] to out[1]
//
//We could (according to the Verilog standard) equivalently have used:
//
//  input [1:0] in;
//  output [1:0] out;
//  specify
//      (in => out) = "";
//  endspecify
//
//However NOT ALL SIMULATORS TREAT MULTIBIT SPECIFY STATEMENTS CORRECTLY,
//at least by default (in particular ModelSim, see notes below).
//
//The previous examples use the 'parrallel connection' operator '=>', which
//creates parallel edges between the two operands (i.e. bit 0 to bit 0, bit
//1 to bit 1 etc.).  Note that both operands must have the same bit-width. 
//
//Verilog also supports the 'full connection' operator '*>' which will create
//a fully connected set of edges (e.g. from all-to-all). It does not require
//both operands to have the same bit-width. For example:
//
//  input [1:0] in;
//  output [2:0] out;
//  specify
//      (in *> out) = "";
//  endspecify
//
//states that there are the following timing edges (dependancies):
//  * from in[0] to out[0]
//  * from in[0] to out[1]
//  * from in[0] to out[2]
//  * from in[1] to out[0]
//  * from in[1] to out[1]
//  * from in[1] to out[2]
//
//For more details on specify blocks see Section 14 "Specify Blocks" of the
//Verilog standard (IEEE 1364-2005).
//
//Back-annotation with Modelsim
//=============================
//
//Ensuring Multi-bit Specifies are Handled Correctly: Bit-blasting
//----------------------------------------------------------------
//
//ModelSim (tested on Modelsim SE 10.4c) ignores multi-bit specify statements
//by default.
//
//This causes SDF annotation errors such as:
//
//  vsim-SDF-3261: Failed to find matching specify module path
//
//To force Modelsim to correctly interpret multi-bit specify statements you
//should provide the '+bitblast' option to the vsim executable.
//This forces it to apply specify statements using multi-bit operands to
//each bit of the operand (i.e. according to the Verilog standard).
//
//Confirming back-annotation is occuring correctly
//------------------------------------------------
//
//Another useful option is '+sdf_verbose' which produces extra output about
//SDF annotation, which can be used to verify annotation occured correctly.
//
//For example:
//
//      Summary of Verilog design objects annotated: 
//      
//           Module path delays =          5
//      
//       ******************************************************************************
//      
//       Summary of constructs read: 
//      
//                 IOPATH =          5
//
//shows that all 5 IOPATH constructs in the SDF were annotated to the verilog
//design.
//
//Example vsim Command Line
//--------------------------
//The following is an example command-line to vsim (where 'tb' is the name of your
//testbench):
//
//  vsim -t 1ps -L rtl_work -L work -voptargs="+acc" +sdf_verbose +bitblast tb


//K-input Look-Up Table
module LUT_K #(
    //The Look-up Table size (number of inputs)
    parameter K = 1, 

    //The lut mask.  
    //Left-most (MSB) bit corresponds to all inputs logic one. 
    //Defaults to always false.
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input [K-1:0] in,
    output out
);

`ifndef SYNTHESIS  
  `ifdef TIMED_SIM
   `ifndef IVERILOG   
      specify
        (in *> out) = "";
      endspecify
    `else
      specparam T1 = 0;
      specparam T2 = 0;
      specify
        (in *> out) = (T1, T2);
      endspecify
   `endif
 `endif
`endif
   
    generate
        if (K == 1) begin
            LUT1 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_1 (
                .A(in),
                .Y(out)
            );
        end
        else if (K == 2) begin
            LUT2 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_2 (
                .A(in),
                .Y(out)
            );
        end
        else if (K == 3) begin
            LUT3 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_3 (
                .A(in),
                .Y(out)
            );
        end
        else if (K == 4) begin
            LUT4 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_4 (
                .A(in),
                .Y(out)
            );
        end
        else if (K == 5) begin
            LUT5 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_5 (
                .A(in),
                .Y(out)
            );
        end
        else if (K == 6) begin
            LUT6 #(
                    .INIT_VALUE(LUT_MASK)
                )lut_6 (
                .A(in),
                .Y(out)
            );
        end
        else  begin
            $error("Error in LUT_K: Parameter K is out of bounds. Valid range: 1 to 6.");
        end
    endgenerate

endmodule

// Bit blasted LUT_K with K == 1 for IVerilog SDF annotated simulation
module LUT_K1 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in0,
    output out
);
`ifndef SYNTHESIS  
 `ifdef TIMED_SIM
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT1 #(
     .INIT_VALUE(LUT_MASK)
        ) lut_1 (
          .A({in0}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 2 for IVerilog SDF annotated simulation
module LUT_K2 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in1,
    input  in0,
    output out
);
   
`ifndef SYNTHESIS  
 `ifdef TIMED_SIM   
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
        (in1 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT2 #(
     .INIT_VALUE(LUT_MASK)
        ) lut_2 (
          .A({in1, in0}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 3 for IVerilog SDF annotated simulation
module LUT_K3 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in2,
    input  in1,
    input  in0,
    output out
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM      
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
        (in1 => out) = (T1, T2);
        (in2 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT3 #(
     .INIT_VALUE(LUT_MASK)
        ) lut_3 (
          .A({in2, in1, in0}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 4 for IVerilog SDF annotated simulation
module LUT_K4 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in3,
    input  in2,
    input  in1,
    input  in0,
    output out
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM   
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
        (in1 => out) = (T1, T2);
        (in2 => out) = (T1, T2);
        (in3 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT4 #(
     .INIT_VALUE(LUT_MASK)
        ) lut_4 (
          .A({in3, in2, in1, in0}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 5 for IVerilog SDF annotated simulation
module LUT_K5 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in4,
    input  in3,
    input  in2,
    input  in1,
    input  in0,
    output out
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM   
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
        (in1 => out) = (T1, T2);
        (in2 => out) = (T1, T2);
        (in3 => out) = (T1, T2);
        (in4 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT5 #(
     .INIT_VALUE(LUT_MASK)
        )lut_5(
          .A({in4, in3, in2, in1, in0}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 6 for IVerilog SDF annotated simulation
module LUT_K6 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in5,
    input  in4,
    input  in3,
    input  in2,
    input  in1,
    input  in0,
    output out
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM     
   specparam T1 = 0;
   specparam T2 = 0;
   specify
        (in0 => out) = (T1, T2);
        (in1 => out) = (T1, T2);
        (in2 => out) = (T1, T2);
        (in3 => out) = (T1, T2);
        (in4 => out) = (T1, T2);
        (in5 => out) = (T1, T2);
   endspecify
 `endif
`endif
   
   LUT6 #(
     .INIT_VALUE(LUT_MASK)
        )lut_6(
          .A({in5, in4, in3, in2, in1, in0}),
          .Y(out)
        );
endmodule

//------------------------------------------------------------------------------
// Rising-edge D-flip-flop with
// active-Low asynchronous reset and
// active-high enable
//------------------------------------------------------------------------------
module dffre(
    input D,
    input R,
    input E,
    (* clkbuf_sink *)
    input C,
    output reg Q
);
`ifndef VCS_MODE
parameter INIT_VALUE = 1'b0;
initial begin
    Q = INIT_VALUE;
end
`endif
    always @(posedge C or negedge R)
        if (R == 1'b0)
            Q <= 1'b0;
        else if (E == 1'b1)
            Q <= D;
endmodule

//------------------------------------------------------------------------------
// Falling-edge D-flip-flop with
// active-Low asynchronous reset and
// active-high enable
//------------------------------------------------------------------------------
module dffnre(
    input D,
    input R,
    input E,
    (* clkbuf_sink *)
    input C,
    output reg Q
);
`ifndef VCS_MODE
parameter INIT_VALUE = 1'b0;
initial begin
    Q = INIT_VALUE;
end
`endif
    always @(negedge C or negedge R)
        if (R == 1'b0)
            Q <= 1'b0;
        else if (E == 1'b1)
            Q <= D;
endmodule

//D-FlipFlop module
module DFF #(
    parameter INITIAL_VALUE=1'b0    
) (
    input clk,
    input D,
    output reg Q
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM      
  `ifndef IVERILOG   
    specify
        (clk => Q) = "";
        $setup(D, posedge clk, "");
        $hold(posedge clk, D, "");
    endspecify
  `else
   specparam T1 = 3,
             T2 = 2;
   specparam tSetup = 1;
   specparam tHold = 1;
   specify
        (clk => Q) = (T1, T2);
        $setup(D, posedge clk,  tSetup);
        $hold(posedge clk, D, tHold);
   endspecify
    `endif
  `endif 
`endif

    initial begin
        Q <= INITIAL_VALUE;
    end

    always@(posedge clk) begin
        Q <= D;
    end
endmodule

//Routing fpga_interconnect module
module fpga_interconnect(
    input datain,
    output dataout
);

`ifndef SYNTHESIS  
 `ifdef TIMED_SIM       
  `ifndef IVERILOG   
    specify
       (datain=>dataout)="";
    endspecify
  `else
   specparam T1 = 0,
             T2 = 0;
    specify
       (datain=>dataout) = (T1, T2);
    endspecify
  `endif
  `endif 
`endif

    assign dataout = datain;

endmodule


//2-to-1 mux module
module mux(
    input select,
    input x,
    input y,
    output z
);

    assign z = (x & ~select) | (y & select);

endmodule

//n-bit adder
module adder #(
    parameter WIDTH = 1   
) (
    input [WIDTH-1:0] a, 
    input [WIDTH-1:0] b, 
    input cin, 
    output cout, 
    output [WIDTH-1:0] sumout);

`ifndef IVERILOG   
   specify
      (a*>sumout)="";
      (b*>sumout)="";
      (cin*>sumout)="";
      (a*>cout)="";
      (b*>cout)="";
      (cin=>cout)="";
   endspecify
`endif
   
   assign {cout, sumout} = a + b + cin;
   
endmodule
   
//nxn multiplier module
module multiply #(
    //The width of input signals
    parameter WIDTH = 1
) (
    input [WIDTH-1:0] a,
    input [WIDTH-1:0] b,
    output [2*WIDTH-1:0] out
);

`ifndef IVERILOG   
    specify
        (a *> out) = "";
        (b *> out) = "";
    endspecify
`endif
   
    assign out = a * b;

endmodule // mult

//single_port_ram module
(* keep_hierarchy *)
module single_port_ram #(
    parameter ADDR_WIDTH = 1,
    parameter DATA_WIDTH = 1
) (
    input clk,
    input [ADDR_WIDTH-1:0] addr,
    input [DATA_WIDTH-1:0] data,
    input we,
    output reg [DATA_WIDTH-1:0] out
);

    localparam MEM_DEPTH = 2 ** ADDR_WIDTH;

    reg [DATA_WIDTH-1:0] Mem[MEM_DEPTH-1:0];

`ifndef IVERILOG   
    specify
        (clk*>out)="";
        $setup(addr, posedge clk, "");
        $setup(data, posedge clk, "");
        $setup(we, posedge clk, "");
        $hold(posedge clk, addr, "");
        $hold(posedge clk, data, "");
        $hold(posedge clk, we, "");
    endspecify
`endif
   
    always@(posedge clk) begin
        if(we) begin
            Mem[addr] = data;
        end
    	out = Mem[addr]; //New data read-during write behaviour (blocking assignments)
    end
   
endmodule // single_port_RAM

module DFFSRE(
    output reg Q,
    input D,
    (* clkbuf_sink *)
    input C,
    input E,
    input R,
    input S
);
 `ifndef VCS_MODE
    parameter [0:0] INIT = 1'b0;
    initial Q = INIT;
`endif

    always @(posedge C or negedge S or negedge R)
        if (!R)
            Q <= 1'b0;
        else if (!S)
            Q <= 1'b1;
        else if (E)
            Q <= D;
        
endmodule

module RS_DSP_MULT_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Unchanged inputs
    input  wire unsigned_a,
    input  wire unsigned_b
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [37:0] z_internal; // Internal signal to hold the output of the original module

    RS_DSP_MULT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
        (a0 => z0) = (0, 0);
        (a1 => z0) = (0, 0);
        (a2 => z0) = (0, 0);
        (a3 => z0) = (0, 0);
        (a4 => z0) = (0, 0);
        (a5 => z0) = (0, 0);
        (a6 => z0) = (0, 0);
        (a7 => z0) = (0, 0);
        (a8 => z0) = (0, 0);
        (a9 => z0) = (0, 0);
        (a10 => z0) = (0, 0);
        (a11 => z0) = (0, 0);
        (a12 => z0) = (0, 0);
        (a13 => z0) = (0, 0);
        (a14 => z0) = (0, 0);
        (a15 => z0) = (0, 0);
        (a16 => z0) = (0, 0);
        (a17 => z0) = (0, 0);
        (a18 => z0) = (0, 0);
        (a19 => z0) = (0, 0);
        (b0 => z0) = (0, 0);
        (b1 => z0) = (0, 0);
        (b2 => z0) = (0, 0);
        (b3 => z0) = (0, 0);
        (b4 => z0) = (0, 0);
        (b5 => z0) = (0, 0);
        (b6 => z0) = (0, 0);
        (b7 => z0) = (0, 0);
        (b8 => z0) = (0, 0);
        (b9 => z0) = (0, 0);
        (b10 => z0) = (0, 0);
        (b11 => z0) = (0, 0);
        (b12 => z0) = (0, 0);
        (b13 => z0) = (0, 0);
        (b14 => z0) = (0, 0);
        (b15 => z0) = (0, 0);
        (b16 => z0) = (0, 0);
        (b17 => z0) = (0, 0);
        (a0 => z1) = (0, 0);
        (a1 => z1) = (0, 0);
        (a2 => z1) = (0, 0);
        (a3 => z1) = (0, 0);
        (a4 => z1) = (0, 0);
        (a5 => z1) = (0, 0);
        (a6 => z1) = (0, 0);
        (a7 => z1) = (0, 0);
        (a8 => z1) = (0, 0);
        (a9 => z1) = (0, 0);
        (a10 => z1) = (0, 0);
        (a11 => z1) = (0, 0);
        (a12 => z1) = (0, 0);
        (a13 => z1) = (0, 0);
        (a14 => z1) = (0, 0);
        (a15 => z1) = (0, 0);
        (a16 => z1) = (0, 0);
        (a17 => z1) = (0, 0);
        (a18 => z1) = (0, 0);
        (a19 => z1) = (0, 0);
        (b0 => z1) = (0, 0);
        (b1 => z1) = (0, 0);
        (b2 => z1) = (0, 0);
        (b3 => z1) = (0, 0);
        (b4 => z1) = (0, 0);
        (b5 => z1) = (0, 0);
        (b6 => z1) = (0, 0);
        (b7 => z1) = (0, 0);
        (b8 => z1) = (0, 0);
        (b9 => z1) = (0, 0);
        (b10 => z1) = (0, 0);
        (b11 => z1) = (0, 0);
        (b12 => z1) = (0, 0);
        (b13 => z1) = (0, 0);
        (b14 => z1) = (0, 0);
        (b15 => z1) = (0, 0);
        (b16 => z1) = (0, 0);
        (b17 => z1) = (0, 0);
        (a0 => z2) = (0, 0);
        (a1 => z2) = (0, 0);
        (a2 => z2) = (0, 0);
        (a3 => z2) = (0, 0);
        (a4 => z2) = (0, 0);
        (a5 => z2) = (0, 0);
        (a6 => z2) = (0, 0);
        (a7 => z2) = (0, 0);
        (a8 => z2) = (0, 0);
        (a9 => z2) = (0, 0);
        (a10 => z2) = (0, 0);
        (a11 => z2) = (0, 0);
        (a12 => z2) = (0, 0);
        (a13 => z2) = (0, 0);
        (a14 => z2) = (0, 0);
        (a15 => z2) = (0, 0);
        (a16 => z2) = (0, 0);
        (a17 => z2) = (0, 0);
        (a18 => z2) = (0, 0);
        (a19 => z2) = (0, 0);
        (b0 => z2) = (0, 0);
        (b1 => z2) = (0, 0);
        (b2 => z2) = (0, 0);
        (b3 => z2) = (0, 0);
        (b4 => z2) = (0, 0);
        (b5 => z2) = (0, 0);
        (b6 => z2) = (0, 0);
        (b7 => z2) = (0, 0);
        (b8 => z2) = (0, 0);
        (b9 => z2) = (0, 0);
        (b10 => z2) = (0, 0);
        (b11 => z2) = (0, 0);
        (b12 => z2) = (0, 0);
        (b13 => z2) = (0, 0);
        (b14 => z2) = (0, 0);
        (b15 => z2) = (0, 0);
        (b16 => z2) = (0, 0);
        (b17 => z2) = (0, 0);
        (a0 => z3) = (0, 0);
        (a1 => z3) = (0, 0);
        (a2 => z3) = (0, 0);
        (a3 => z3) = (0, 0);
        (a4 => z3) = (0, 0);
        (a5 => z3) = (0, 0);
        (a6 => z3) = (0, 0);
        (a7 => z3) = (0, 0);
        (a8 => z3) = (0, 0);
        (a9 => z3) = (0, 0);
        (a10 => z3) = (0, 0);
        (a11 => z3) = (0, 0);
        (a12 => z3) = (0, 0);
        (a13 => z3) = (0, 0);
        (a14 => z3) = (0, 0);
        (a15 => z3) = (0, 0);
        (a16 => z3) = (0, 0);
        (a17 => z3) = (0, 0);
        (a18 => z3) = (0, 0);
        (a19 => z3) = (0, 0);
        (b0 => z3) = (0, 0);
        (b1 => z3) = (0, 0);
        (b2 => z3) = (0, 0);
        (b3 => z3) = (0, 0);
        (b4 => z3) = (0, 0);
        (b5 => z3) = (0, 0);
        (b6 => z3) = (0, 0);
        (b7 => z3) = (0, 0);
        (b8 => z3) = (0, 0);
        (b9 => z3) = (0, 0);
        (b10 => z3) = (0, 0);
        (b11 => z3) = (0, 0);
        (b12 => z3) = (0, 0);
        (b13 => z3) = (0, 0);
        (b14 => z3) = (0, 0);
        (b15 => z3) = (0, 0);
        (b16 => z3) = (0, 0);
        (b17 => z3) = (0, 0);
        (a0 => z4) = (0, 0);
        (a1 => z4) = (0, 0);
        (a2 => z4) = (0, 0);
        (a3 => z4) = (0, 0);
        (a4 => z4) = (0, 0);
        (a5 => z4) = (0, 0);
        (a6 => z4) = (0, 0);
        (a7 => z4) = (0, 0);
        (a8 => z4) = (0, 0);
        (a9 => z4) = (0, 0);
        (a10 => z4) = (0, 0);
        (a11 => z4) = (0, 0);
        (a12 => z4) = (0, 0);
        (a13 => z4) = (0, 0);
        (a14 => z4) = (0, 0);
        (a15 => z4) = (0, 0);
        (a16 => z4) = (0, 0);
        (a17 => z4) = (0, 0);
        (a18 => z4) = (0, 0);
        (a19 => z4) = (0, 0);
        (b0 => z4) = (0, 0);
        (b1 => z4) = (0, 0);
        (b2 => z4) = (0, 0);
        (b3 => z4) = (0, 0);
        (b4 => z4) = (0, 0);
        (b5 => z4) = (0, 0);
        (b6 => z4) = (0, 0);
        (b7 => z4) = (0, 0);
        (b8 => z4) = (0, 0);
        (b9 => z4) = (0, 0);
        (b10 => z4) = (0, 0);
        (b11 => z4) = (0, 0);
        (b12 => z4) = (0, 0);
        (b13 => z4) = (0, 0);
        (b14 => z4) = (0, 0);
        (b15 => z4) = (0, 0);
        (b16 => z4) = (0, 0);
        (b17 => z4) = (0, 0);
        (a0 => z5) = (0, 0);
        (a1 => z5) = (0, 0);
        (a2 => z5) = (0, 0);
        (a3 => z5) = (0, 0);
        (a4 => z5) = (0, 0);
        (a5 => z5) = (0, 0);
        (a6 => z5) = (0, 0);
        (a7 => z5) = (0, 0);
        (a8 => z5) = (0, 0);
        (a9 => z5) = (0, 0);
        (a10 => z5) = (0, 0);
        (a11 => z5) = (0, 0);
        (a12 => z5) = (0, 0);
        (a13 => z5) = (0, 0);
        (a14 => z5) = (0, 0);
        (a15 => z5) = (0, 0);
        (a16 => z5) = (0, 0);
        (a17 => z5) = (0, 0);
        (a18 => z5) = (0, 0);
        (a19 => z5) = (0, 0);
        (b0 => z5) = (0, 0);
        (b1 => z5) = (0, 0);
        (b2 => z5) = (0, 0);
        (b3 => z5) = (0, 0);
        (b4 => z5) = (0, 0);
        (b5 => z5) = (0, 0);
        (b6 => z5) = (0, 0);
        (b7 => z5) = (0, 0);
        (b8 => z5) = (0, 0);
        (b9 => z5) = (0, 0);
        (b10 => z5) = (0, 0);
        (b11 => z5) = (0, 0);
        (b12 => z5) = (0, 0);
        (b13 => z5) = (0, 0);
        (b14 => z5) = (0, 0);
        (b15 => z5) = (0, 0);
        (b16 => z5) = (0, 0);
        (b17 => z5) = (0, 0);
        (a0 => z6) = (0, 0);
        (a1 => z6) = (0, 0);
        (a2 => z6) = (0, 0);
        (a3 => z6) = (0, 0);
        (a4 => z6) = (0, 0);
        (a5 => z6) = (0, 0);
        (a6 => z6) = (0, 0);
        (a7 => z6) = (0, 0);
        (a8 => z6) = (0, 0);
        (a9 => z6) = (0, 0);
        (a10 => z6) = (0, 0);
        (a11 => z6) = (0, 0);
        (a12 => z6) = (0, 0);
        (a13 => z6) = (0, 0);
        (a14 => z6) = (0, 0);
        (a15 => z6) = (0, 0);
        (a16 => z6) = (0, 0);
        (a17 => z6) = (0, 0);
        (a18 => z6) = (0, 0);
        (a19 => z6) = (0, 0);
        (b0 => z6) = (0, 0);
        (b1 => z6) = (0, 0);
        (b2 => z6) = (0, 0);
        (b3 => z6) = (0, 0);
        (b4 => z6) = (0, 0);
        (b5 => z6) = (0, 0);
        (b6 => z6) = (0, 0);
        (b7 => z6) = (0, 0);
        (b8 => z6) = (0, 0);
        (b9 => z6) = (0, 0);
        (b10 => z6) = (0, 0);
        (b11 => z6) = (0, 0);
        (b12 => z6) = (0, 0);
        (b13 => z6) = (0, 0);
        (b14 => z6) = (0, 0);
        (b15 => z6) = (0, 0);
        (b16 => z6) = (0, 0);
        (b17 => z6) = (0, 0);
        (a0 => z7) = (0, 0);
        (a1 => z7) = (0, 0);
        (a2 => z7) = (0, 0);
        (a3 => z7) = (0, 0);
        (a4 => z7) = (0, 0);
        (a5 => z7) = (0, 0);
        (a6 => z7) = (0, 0);
        (a7 => z7) = (0, 0);
        (a8 => z7) = (0, 0);
        (a9 => z7) = (0, 0);
        (a10 => z7) = (0, 0);
        (a11 => z7) = (0, 0);
        (a12 => z7) = (0, 0);
        (a13 => z7) = (0, 0);
        (a14 => z7) = (0, 0);
        (a15 => z7) = (0, 0);
        (a16 => z7) = (0, 0);
        (a17 => z7) = (0, 0);
        (a18 => z7) = (0, 0);
        (a19 => z7) = (0, 0);
        (b0 => z7) = (0, 0);
        (b1 => z7) = (0, 0);
        (b2 => z7) = (0, 0);
        (b3 => z7) = (0, 0);
        (b4 => z7) = (0, 0);
        (b5 => z7) = (0, 0);
        (b6 => z7) = (0, 0);
        (b7 => z7) = (0, 0);
        (b8 => z7) = (0, 0);
        (b9 => z7) = (0, 0);
        (b10 => z7) = (0, 0);
        (b11 => z7) = (0, 0);
        (b12 => z7) = (0, 0);
        (b13 => z7) = (0, 0);
        (b14 => z7) = (0, 0);
        (b15 => z7) = (0, 0);
        (b16 => z7) = (0, 0);
        (b17 => z7) = (0, 0);
        (a0 => z8) = (0, 0);
        (a1 => z8) = (0, 0);
        (a2 => z8) = (0, 0);
        (a3 => z8) = (0, 0);
        (a4 => z8) = (0, 0);
        (a5 => z8) = (0, 0);
        (a6 => z8) = (0, 0);
        (a7 => z8) = (0, 0);
        (a8 => z8) = (0, 0);
        (a9 => z8) = (0, 0);
        (a10 => z8) = (0, 0);
        (a11 => z8) = (0, 0);
        (a12 => z8) = (0, 0);
        (a13 => z8) = (0, 0);
        (a14 => z8) = (0, 0);
        (a15 => z8) = (0, 0);
        (a16 => z8) = (0, 0);
        (a17 => z8) = (0, 0);
        (a18 => z8) = (0, 0);
        (a19 => z8) = (0, 0);
        (b0 => z8) = (0, 0);
        (b1 => z8) = (0, 0);
        (b2 => z8) = (0, 0);
        (b3 => z8) = (0, 0);
        (b4 => z8) = (0, 0);
        (b5 => z8) = (0, 0);
        (b6 => z8) = (0, 0);
        (b7 => z8) = (0, 0);
        (b8 => z8) = (0, 0);
        (b9 => z8) = (0, 0);
        (b10 => z8) = (0, 0);
        (b11 => z8) = (0, 0);
        (b12 => z8) = (0, 0);
        (b13 => z8) = (0, 0);
        (b14 => z8) = (0, 0);
        (b15 => z8) = (0, 0);
        (b16 => z8) = (0, 0);
        (b17 => z8) = (0, 0);
        (a0 => z9) = (0, 0);
        (a1 => z9) = (0, 0);
        (a2 => z9) = (0, 0);
        (a3 => z9) = (0, 0);
        (a4 => z9) = (0, 0);
        (a5 => z9) = (0, 0);
        (a6 => z9) = (0, 0);
        (a7 => z9) = (0, 0);
        (a8 => z9) = (0, 0);
        (a9 => z9) = (0, 0);
        (a10 => z9) = (0, 0);
        (a11 => z9) = (0, 0);
        (a12 => z9) = (0, 0);
        (a13 => z9) = (0, 0);
        (a14 => z9) = (0, 0);
        (a15 => z9) = (0, 0);
        (a16 => z9) = (0, 0);
        (a17 => z9) = (0, 0);
        (a18 => z9) = (0, 0);
        (a19 => z9) = (0, 0);
        (b0 => z9) = (0, 0);
        (b1 => z9) = (0, 0);
        (b2 => z9) = (0, 0);
        (b3 => z9) = (0, 0);
        (b4 => z9) = (0, 0);
        (b5 => z9) = (0, 0);
        (b6 => z9) = (0, 0);
        (b7 => z9) = (0, 0);
        (b8 => z9) = (0, 0);
        (b9 => z9) = (0, 0);
        (b10 => z9) = (0, 0);
        (b11 => z9) = (0, 0);
        (b12 => z9) = (0, 0);
        (b13 => z9) = (0, 0);
        (b14 => z9) = (0, 0);
        (b15 => z9) = (0, 0);
        (b16 => z9) = (0, 0);
        (b17 => z9) = (0, 0);
        (a0 => z10) = (0, 0);
        (a1 => z10) = (0, 0);
        (a2 => z10) = (0, 0);
        (a3 => z10) = (0, 0);
        (a4 => z10) = (0, 0);
        (a5 => z10) = (0, 0);
        (a6 => z10) = (0, 0);
        (a7 => z10) = (0, 0);
        (a8 => z10) = (0, 0);
        (a9 => z10) = (0, 0);
        (a10 => z10) = (0, 0);
        (a11 => z10) = (0, 0);
        (a12 => z10) = (0, 0);
        (a13 => z10) = (0, 0);
        (a14 => z10) = (0, 0);
        (a15 => z10) = (0, 0);
        (a16 => z10) = (0, 0);
        (a17 => z10) = (0, 0);
        (a18 => z10) = (0, 0);
        (a19 => z10) = (0, 0);
        (b0 => z10) = (0, 0);
        (b1 => z10) = (0, 0);
        (b2 => z10) = (0, 0);
        (b3 => z10) = (0, 0);
        (b4 => z10) = (0, 0);
        (b5 => z10) = (0, 0);
        (b6 => z10) = (0, 0);
        (b7 => z10) = (0, 0);
        (b8 => z10) = (0, 0);
        (b9 => z10) = (0, 0);
        (b10 => z10) = (0, 0);
        (b11 => z10) = (0, 0);
        (b12 => z10) = (0, 0);
        (b13 => z10) = (0, 0);
        (b14 => z10) = (0, 0);
        (b15 => z10) = (0, 0);
        (b16 => z10) = (0, 0);
        (b17 => z10) = (0, 0);
        (a0 => z11) = (0, 0);
        (a1 => z11) = (0, 0);
        (a2 => z11) = (0, 0);
        (a3 => z11) = (0, 0);
        (a4 => z11) = (0, 0);
        (a5 => z11) = (0, 0);
        (a6 => z11) = (0, 0);
        (a7 => z11) = (0, 0);
        (a8 => z11) = (0, 0);
        (a9 => z11) = (0, 0);
        (a10 => z11) = (0, 0);
        (a11 => z11) = (0, 0);
        (a12 => z11) = (0, 0);
        (a13 => z11) = (0, 0);
        (a14 => z11) = (0, 0);
        (a15 => z11) = (0, 0);
        (a16 => z11) = (0, 0);
        (a17 => z11) = (0, 0);
        (a18 => z11) = (0, 0);
        (a19 => z11) = (0, 0);
        (b0 => z11) = (0, 0);
        (b1 => z11) = (0, 0);
        (b2 => z11) = (0, 0);
        (b3 => z11) = (0, 0);
        (b4 => z11) = (0, 0);
        (b5 => z11) = (0, 0);
        (b6 => z11) = (0, 0);
        (b7 => z11) = (0, 0);
        (b8 => z11) = (0, 0);
        (b9 => z11) = (0, 0);
        (b10 => z11) = (0, 0);
        (b11 => z11) = (0, 0);
        (b12 => z11) = (0, 0);
        (b13 => z11) = (0, 0);
        (b14 => z11) = (0, 0);
        (b15 => z11) = (0, 0);
        (b16 => z11) = (0, 0);
        (b17 => z11) = (0, 0);
        (a0 => z12) = (0, 0);
        (a1 => z12) = (0, 0);
        (a2 => z12) = (0, 0);
        (a3 => z12) = (0, 0);
        (a4 => z12) = (0, 0);
        (a5 => z12) = (0, 0);
        (a6 => z12) = (0, 0);
        (a7 => z12) = (0, 0);
        (a8 => z12) = (0, 0);
        (a9 => z12) = (0, 0);
        (a10 => z12) = (0, 0);
        (a11 => z12) = (0, 0);
        (a12 => z12) = (0, 0);
        (a13 => z12) = (0, 0);
        (a14 => z12) = (0, 0);
        (a15 => z12) = (0, 0);
        (a16 => z12) = (0, 0);
        (a17 => z12) = (0, 0);
        (a18 => z12) = (0, 0);
        (a19 => z12) = (0, 0);
        (b0 => z12) = (0, 0);
        (b1 => z12) = (0, 0);
        (b2 => z12) = (0, 0);
        (b3 => z12) = (0, 0);
        (b4 => z12) = (0, 0);
        (b5 => z12) = (0, 0);
        (b6 => z12) = (0, 0);
        (b7 => z12) = (0, 0);
        (b8 => z12) = (0, 0);
        (b9 => z12) = (0, 0);
        (b10 => z12) = (0, 0);
        (b11 => z12) = (0, 0);
        (b12 => z12) = (0, 0);
        (b13 => z12) = (0, 0);
        (b14 => z12) = (0, 0);
        (b15 => z12) = (0, 0);
        (b16 => z12) = (0, 0);
        (b17 => z12) = (0, 0);
        (a0 => z13) = (0, 0);
        (a1 => z13) = (0, 0);
        (a2 => z13) = (0, 0);
        (a3 => z13) = (0, 0);
        (a4 => z13) = (0, 0);
        (a5 => z13) = (0, 0);
        (a6 => z13) = (0, 0);
        (a7 => z13) = (0, 0);
        (a8 => z13) = (0, 0);
        (a9 => z13) = (0, 0);
        (a10 => z13) = (0, 0);
        (a11 => z13) = (0, 0);
        (a12 => z13) = (0, 0);
        (a13 => z13) = (0, 0);
        (a14 => z13) = (0, 0);
        (a15 => z13) = (0, 0);
        (a16 => z13) = (0, 0);
        (a17 => z13) = (0, 0);
        (a18 => z13) = (0, 0);
        (a19 => z13) = (0, 0);
        (b0 => z13) = (0, 0);
        (b1 => z13) = (0, 0);
        (b2 => z13) = (0, 0);
        (b3 => z13) = (0, 0);
        (b4 => z13) = (0, 0);
        (b5 => z13) = (0, 0);
        (b6 => z13) = (0, 0);
        (b7 => z13) = (0, 0);
        (b8 => z13) = (0, 0);
        (b9 => z13) = (0, 0);
        (b10 => z13) = (0, 0);
        (b11 => z13) = (0, 0);
        (b12 => z13) = (0, 0);
        (b13 => z13) = (0, 0);
        (b14 => z13) = (0, 0);
        (b15 => z13) = (0, 0);
        (b16 => z13) = (0, 0);
        (b17 => z13) = (0, 0);
        (a0 => z14) = (0, 0);
        (a1 => z14) = (0, 0);
        (a2 => z14) = (0, 0);
        (a3 => z14) = (0, 0);
        (a4 => z14) = (0, 0);
        (a5 => z14) = (0, 0);
        (a6 => z14) = (0, 0);
        (a7 => z14) = (0, 0);
        (a8 => z14) = (0, 0);
        (a9 => z14) = (0, 0);
        (a10 => z14) = (0, 0);
        (a11 => z14) = (0, 0);
        (a12 => z14) = (0, 0);
        (a13 => z14) = (0, 0);
        (a14 => z14) = (0, 0);
        (a15 => z14) = (0, 0);
        (a16 => z14) = (0, 0);
        (a17 => z14) = (0, 0);
        (a18 => z14) = (0, 0);
        (a19 => z14) = (0, 0);
        (b0 => z14) = (0, 0);
        (b1 => z14) = (0, 0);
        (b2 => z14) = (0, 0);
        (b3 => z14) = (0, 0);
        (b4 => z14) = (0, 0);
        (b5 => z14) = (0, 0);
        (b6 => z14) = (0, 0);
        (b7 => z14) = (0, 0);
        (b8 => z14) = (0, 0);
        (b9 => z14) = (0, 0);
        (b10 => z14) = (0, 0);
        (b11 => z14) = (0, 0);
        (b12 => z14) = (0, 0);
        (b13 => z14) = (0, 0);
        (b14 => z14) = (0, 0);
        (b15 => z14) = (0, 0);
        (b16 => z14) = (0, 0);
        (b17 => z14) = (0, 0);
        (a0 => z15) = (0, 0);
        (a1 => z15) = (0, 0);
        (a2 => z15) = (0, 0);
        (a3 => z15) = (0, 0);
        (a4 => z15) = (0, 0);
        (a5 => z15) = (0, 0);
        (a6 => z15) = (0, 0);
        (a7 => z15) = (0, 0);
        (a8 => z15) = (0, 0);
        (a9 => z15) = (0, 0);
        (a10 => z15) = (0, 0);
        (a11 => z15) = (0, 0);
        (a12 => z15) = (0, 0);
        (a13 => z15) = (0, 0);
        (a14 => z15) = (0, 0);
        (a15 => z15) = (0, 0);
        (a16 => z15) = (0, 0);
        (a17 => z15) = (0, 0);
        (a18 => z15) = (0, 0);
        (a19 => z15) = (0, 0);
        (b0 => z15) = (0, 0);
        (b1 => z15) = (0, 0);
        (b2 => z15) = (0, 0);
        (b3 => z15) = (0, 0);
        (b4 => z15) = (0, 0);
        (b5 => z15) = (0, 0);
        (b6 => z15) = (0, 0);
        (b7 => z15) = (0, 0);
        (b8 => z15) = (0, 0);
        (b9 => z15) = (0, 0);
        (b10 => z15) = (0, 0);
        (b11 => z15) = (0, 0);
        (b12 => z15) = (0, 0);
        (b13 => z15) = (0, 0);
        (b14 => z15) = (0, 0);
        (b15 => z15) = (0, 0);
        (b16 => z15) = (0, 0);
        (b17 => z15) = (0, 0);
        (a0 => z16) = (0, 0);
        (a1 => z16) = (0, 0);
        (a2 => z16) = (0, 0);
        (a3 => z16) = (0, 0);
        (a4 => z16) = (0, 0);
        (a5 => z16) = (0, 0);
        (a6 => z16) = (0, 0);
        (a7 => z16) = (0, 0);
        (a8 => z16) = (0, 0);
        (a9 => z16) = (0, 0);
        (a10 => z16) = (0, 0);
        (a11 => z16) = (0, 0);
        (a12 => z16) = (0, 0);
        (a13 => z16) = (0, 0);
        (a14 => z16) = (0, 0);
        (a15 => z16) = (0, 0);
        (a16 => z16) = (0, 0);
        (a17 => z16) = (0, 0);
        (a18 => z16) = (0, 0);
        (a19 => z16) = (0, 0);
        (b0 => z16) = (0, 0);
        (b1 => z16) = (0, 0);
        (b2 => z16) = (0, 0);
        (b3 => z16) = (0, 0);
        (b4 => z16) = (0, 0);
        (b5 => z16) = (0, 0);
        (b6 => z16) = (0, 0);
        (b7 => z16) = (0, 0);
        (b8 => z16) = (0, 0);
        (b9 => z16) = (0, 0);
        (b10 => z16) = (0, 0);
        (b11 => z16) = (0, 0);
        (b12 => z16) = (0, 0);
        (b13 => z16) = (0, 0);
        (b14 => z16) = (0, 0);
        (b15 => z16) = (0, 0);
        (b16 => z16) = (0, 0);
        (b17 => z16) = (0, 0);
        (a0 => z17) = (0, 0);
        (a1 => z17) = (0, 0);
        (a2 => z17) = (0, 0);
        (a3 => z17) = (0, 0);
        (a4 => z17) = (0, 0);
        (a5 => z17) = (0, 0);
        (a6 => z17) = (0, 0);
        (a7 => z17) = (0, 0);
        (a8 => z17) = (0, 0);
        (a9 => z17) = (0, 0);
        (a10 => z17) = (0, 0);
        (a11 => z17) = (0, 0);
        (a12 => z17) = (0, 0);
        (a13 => z17) = (0, 0);
        (a14 => z17) = (0, 0);
        (a15 => z17) = (0, 0);
        (a16 => z17) = (0, 0);
        (a17 => z17) = (0, 0);
        (a18 => z17) = (0, 0);
        (a19 => z17) = (0, 0);
        (b0 => z17) = (0, 0);
        (b1 => z17) = (0, 0);
        (b2 => z17) = (0, 0);
        (b3 => z17) = (0, 0);
        (b4 => z17) = (0, 0);
        (b5 => z17) = (0, 0);
        (b6 => z17) = (0, 0);
        (b7 => z17) = (0, 0);
        (b8 => z17) = (0, 0);
        (b9 => z17) = (0, 0);
        (b10 => z17) = (0, 0);
        (b11 => z17) = (0, 0);
        (b12 => z17) = (0, 0);
        (b13 => z17) = (0, 0);
        (b14 => z17) = (0, 0);
        (b15 => z17) = (0, 0);
        (b16 => z17) = (0, 0);
        (b17 => z17) = (0, 0);
        (a0 => z18) = (0, 0);
        (a1 => z18) = (0, 0);
        (a2 => z18) = (0, 0);
        (a3 => z18) = (0, 0);
        (a4 => z18) = (0, 0);
        (a5 => z18) = (0, 0);
        (a6 => z18) = (0, 0);
        (a7 => z18) = (0, 0);
        (a8 => z18) = (0, 0);
        (a9 => z18) = (0, 0);
        (a10 => z18) = (0, 0);
        (a11 => z18) = (0, 0);
        (a12 => z18) = (0, 0);
        (a13 => z18) = (0, 0);
        (a14 => z18) = (0, 0);
        (a15 => z18) = (0, 0);
        (a16 => z18) = (0, 0);
        (a17 => z18) = (0, 0);
        (a18 => z18) = (0, 0);
        (a19 => z18) = (0, 0);
        (b0 => z18) = (0, 0);
        (b1 => z18) = (0, 0);
        (b2 => z18) = (0, 0);
        (b3 => z18) = (0, 0);
        (b4 => z18) = (0, 0);
        (b5 => z18) = (0, 0);
        (b6 => z18) = (0, 0);
        (b7 => z18) = (0, 0);
        (b8 => z18) = (0, 0);
        (b9 => z18) = (0, 0);
        (b10 => z18) = (0, 0);
        (b11 => z18) = (0, 0);
        (b12 => z18) = (0, 0);
        (b13 => z18) = (0, 0);
        (b14 => z18) = (0, 0);
        (b15 => z18) = (0, 0);
        (b16 => z18) = (0, 0);
        (b17 => z18) = (0, 0);
        (a0 => z19) = (0, 0);
        (a1 => z19) = (0, 0);
        (a2 => z19) = (0, 0);
        (a3 => z19) = (0, 0);
        (a4 => z19) = (0, 0);
        (a5 => z19) = (0, 0);
        (a6 => z19) = (0, 0);
        (a7 => z19) = (0, 0);
        (a8 => z19) = (0, 0);
        (a9 => z19) = (0, 0);
        (a10 => z19) = (0, 0);
        (a11 => z19) = (0, 0);
        (a12 => z19) = (0, 0);
        (a13 => z19) = (0, 0);
        (a14 => z19) = (0, 0);
        (a15 => z19) = (0, 0);
        (a16 => z19) = (0, 0);
        (a17 => z19) = (0, 0);
        (a18 => z19) = (0, 0);
        (a19 => z19) = (0, 0);
        (b0 => z19) = (0, 0);
        (b1 => z19) = (0, 0);
        (b2 => z19) = (0, 0);
        (b3 => z19) = (0, 0);
        (b4 => z19) = (0, 0);
        (b5 => z19) = (0, 0);
        (b6 => z19) = (0, 0);
        (b7 => z19) = (0, 0);
        (b8 => z19) = (0, 0);
        (b9 => z19) = (0, 0);
        (b10 => z19) = (0, 0);
        (b11 => z19) = (0, 0);
        (b12 => z19) = (0, 0);
        (b13 => z19) = (0, 0);
        (b14 => z19) = (0, 0);
        (b15 => z19) = (0, 0);
        (b16 => z19) = (0, 0);
        (b17 => z19) = (0, 0);
        (a0 => z20) = (0, 0);
        (a1 => z20) = (0, 0);
        (a2 => z20) = (0, 0);
        (a3 => z20) = (0, 0);
        (a4 => z20) = (0, 0);
        (a5 => z20) = (0, 0);
        (a6 => z20) = (0, 0);
        (a7 => z20) = (0, 0);
        (a8 => z20) = (0, 0);
        (a9 => z20) = (0, 0);
        (a10 => z20) = (0, 0);
        (a11 => z20) = (0, 0);
        (a12 => z20) = (0, 0);
        (a13 => z20) = (0, 0);
        (a14 => z20) = (0, 0);
        (a15 => z20) = (0, 0);
        (a16 => z20) = (0, 0);
        (a17 => z20) = (0, 0);
        (a18 => z20) = (0, 0);
        (a19 => z20) = (0, 0);
        (b0 => z20) = (0, 0);
        (b1 => z20) = (0, 0);
        (b2 => z20) = (0, 0);
        (b3 => z20) = (0, 0);
        (b4 => z20) = (0, 0);
        (b5 => z20) = (0, 0);
        (b6 => z20) = (0, 0);
        (b7 => z20) = (0, 0);
        (b8 => z20) = (0, 0);
        (b9 => z20) = (0, 0);
        (b10 => z20) = (0, 0);
        (b11 => z20) = (0, 0);
        (b12 => z20) = (0, 0);
        (b13 => z20) = (0, 0);
        (b14 => z20) = (0, 0);
        (b15 => z20) = (0, 0);
        (b16 => z20) = (0, 0);
        (b17 => z20) = (0, 0);
        (a0 => z21) = (0, 0);
        (a1 => z21) = (0, 0);
        (a2 => z21) = (0, 0);
        (a3 => z21) = (0, 0);
        (a4 => z21) = (0, 0);
        (a5 => z21) = (0, 0);
        (a6 => z21) = (0, 0);
        (a7 => z21) = (0, 0);
        (a8 => z21) = (0, 0);
        (a9 => z21) = (0, 0);
        (a10 => z21) = (0, 0);
        (a11 => z21) = (0, 0);
        (a12 => z21) = (0, 0);
        (a13 => z21) = (0, 0);
        (a14 => z21) = (0, 0);
        (a15 => z21) = (0, 0);
        (a16 => z21) = (0, 0);
        (a17 => z21) = (0, 0);
        (a18 => z21) = (0, 0);
        (a19 => z21) = (0, 0);
        (b0 => z21) = (0, 0);
        (b1 => z21) = (0, 0);
        (b2 => z21) = (0, 0);
        (b3 => z21) = (0, 0);
        (b4 => z21) = (0, 0);
        (b5 => z21) = (0, 0);
        (b6 => z21) = (0, 0);
        (b7 => z21) = (0, 0);
        (b8 => z21) = (0, 0);
        (b9 => z21) = (0, 0);
        (b10 => z21) = (0, 0);
        (b11 => z21) = (0, 0);
        (b12 => z21) = (0, 0);
        (b13 => z21) = (0, 0);
        (b14 => z21) = (0, 0);
        (b15 => z21) = (0, 0);
        (b16 => z21) = (0, 0);
        (b17 => z21) = (0, 0);
        (a0 => z22) = (0, 0);
        (a1 => z22) = (0, 0);
        (a2 => z22) = (0, 0);
        (a3 => z22) = (0, 0);
        (a4 => z22) = (0, 0);
        (a5 => z22) = (0, 0);
        (a6 => z22) = (0, 0);
        (a7 => z22) = (0, 0);
        (a8 => z22) = (0, 0);
        (a9 => z22) = (0, 0);
        (a10 => z22) = (0, 0);
        (a11 => z22) = (0, 0);
        (a12 => z22) = (0, 0);
        (a13 => z22) = (0, 0);
        (a14 => z22) = (0, 0);
        (a15 => z22) = (0, 0);
        (a16 => z22) = (0, 0);
        (a17 => z22) = (0, 0);
        (a18 => z22) = (0, 0);
        (a19 => z22) = (0, 0);
        (b0 => z22) = (0, 0);
        (b1 => z22) = (0, 0);
        (b2 => z22) = (0, 0);
        (b3 => z22) = (0, 0);
        (b4 => z22) = (0, 0);
        (b5 => z22) = (0, 0);
        (b6 => z22) = (0, 0);
        (b7 => z22) = (0, 0);
        (b8 => z22) = (0, 0);
        (b9 => z22) = (0, 0);
        (b10 => z22) = (0, 0);
        (b11 => z22) = (0, 0);
        (b12 => z22) = (0, 0);
        (b13 => z22) = (0, 0);
        (b14 => z22) = (0, 0);
        (b15 => z22) = (0, 0);
        (b16 => z22) = (0, 0);
        (b17 => z22) = (0, 0);
        (a0 => z23) = (0, 0);
        (a1 => z23) = (0, 0);
        (a2 => z23) = (0, 0);
        (a3 => z23) = (0, 0);
        (a4 => z23) = (0, 0);
        (a5 => z23) = (0, 0);
        (a6 => z23) = (0, 0);
        (a7 => z23) = (0, 0);
        (a8 => z23) = (0, 0);
        (a9 => z23) = (0, 0);
        (a10 => z23) = (0, 0);
        (a11 => z23) = (0, 0);
        (a12 => z23) = (0, 0);
        (a13 => z23) = (0, 0);
        (a14 => z23) = (0, 0);
        (a15 => z23) = (0, 0);
        (a16 => z23) = (0, 0);
        (a17 => z23) = (0, 0);
        (a18 => z23) = (0, 0);
        (a19 => z23) = (0, 0);
        (b0 => z23) = (0, 0);
        (b1 => z23) = (0, 0);
        (b2 => z23) = (0, 0);
        (b3 => z23) = (0, 0);
        (b4 => z23) = (0, 0);
        (b5 => z23) = (0, 0);
        (b6 => z23) = (0, 0);
        (b7 => z23) = (0, 0);
        (b8 => z23) = (0, 0);
        (b9 => z23) = (0, 0);
        (b10 => z23) = (0, 0);
        (b11 => z23) = (0, 0);
        (b12 => z23) = (0, 0);
        (b13 => z23) = (0, 0);
        (b14 => z23) = (0, 0);
        (b15 => z23) = (0, 0);
        (b16 => z23) = (0, 0);
        (b17 => z23) = (0, 0);
        (a0 => z24) = (0, 0);
        (a1 => z24) = (0, 0);
        (a2 => z24) = (0, 0);
        (a3 => z24) = (0, 0);
        (a4 => z24) = (0, 0);
        (a5 => z24) = (0, 0);
        (a6 => z24) = (0, 0);
        (a7 => z24) = (0, 0);
        (a8 => z24) = (0, 0);
        (a9 => z24) = (0, 0);
        (a10 => z24) = (0, 0);
        (a11 => z24) = (0, 0);
        (a12 => z24) = (0, 0);
        (a13 => z24) = (0, 0);
        (a14 => z24) = (0, 0);
        (a15 => z24) = (0, 0);
        (a16 => z24) = (0, 0);
        (a17 => z24) = (0, 0);
        (a18 => z24) = (0, 0);
        (a19 => z24) = (0, 0);
        (b0 => z24) = (0, 0);
        (b1 => z24) = (0, 0);
        (b2 => z24) = (0, 0);
        (b3 => z24) = (0, 0);
        (b4 => z24) = (0, 0);
        (b5 => z24) = (0, 0);
        (b6 => z24) = (0, 0);
        (b7 => z24) = (0, 0);
        (b8 => z24) = (0, 0);
        (b9 => z24) = (0, 0);
        (b10 => z24) = (0, 0);
        (b11 => z24) = (0, 0);
        (b12 => z24) = (0, 0);
        (b13 => z24) = (0, 0);
        (b14 => z24) = (0, 0);
        (b15 => z24) = (0, 0);
        (b16 => z24) = (0, 0);
        (b17 => z24) = (0, 0);
        (a0 => z25) = (0, 0);
        (a1 => z25) = (0, 0);
        (a2 => z25) = (0, 0);
        (a3 => z25) = (0, 0);
        (a4 => z25) = (0, 0);
        (a5 => z25) = (0, 0);
        (a6 => z25) = (0, 0);
        (a7 => z25) = (0, 0);
        (a8 => z25) = (0, 0);
        (a9 => z25) = (0, 0);
        (a10 => z25) = (0, 0);
        (a11 => z25) = (0, 0);
        (a12 => z25) = (0, 0);
        (a13 => z25) = (0, 0);
        (a14 => z25) = (0, 0);
        (a15 => z25) = (0, 0);
        (a16 => z25) = (0, 0);
        (a17 => z25) = (0, 0);
        (a18 => z25) = (0, 0);
        (a19 => z25) = (0, 0);
        (b0 => z25) = (0, 0);
        (b1 => z25) = (0, 0);
        (b2 => z25) = (0, 0);
        (b3 => z25) = (0, 0);
        (b4 => z25) = (0, 0);
        (b5 => z25) = (0, 0);
        (b6 => z25) = (0, 0);
        (b7 => z25) = (0, 0);
        (b8 => z25) = (0, 0);
        (b9 => z25) = (0, 0);
        (b10 => z25) = (0, 0);
        (b11 => z25) = (0, 0);
        (b12 => z25) = (0, 0);
        (b13 => z25) = (0, 0);
        (b14 => z25) = (0, 0);
        (b15 => z25) = (0, 0);
        (b16 => z25) = (0, 0);
        (b17 => z25) = (0, 0);
        (a0 => z26) = (0, 0);
        (a1 => z26) = (0, 0);
        (a2 => z26) = (0, 0);
        (a3 => z26) = (0, 0);
        (a4 => z26) = (0, 0);
        (a5 => z26) = (0, 0);
        (a6 => z26) = (0, 0);
        (a7 => z26) = (0, 0);
        (a8 => z26) = (0, 0);
        (a9 => z26) = (0, 0);
        (a10 => z26) = (0, 0);
        (a11 => z26) = (0, 0);
        (a12 => z26) = (0, 0);
        (a13 => z26) = (0, 0);
        (a14 => z26) = (0, 0);
        (a15 => z26) = (0, 0);
        (a16 => z26) = (0, 0);
        (a17 => z26) = (0, 0);
        (a18 => z26) = (0, 0);
        (a19 => z26) = (0, 0);
        (b0 => z26) = (0, 0);
        (b1 => z26) = (0, 0);
        (b2 => z26) = (0, 0);
        (b3 => z26) = (0, 0);
        (b4 => z26) = (0, 0);
        (b5 => z26) = (0, 0);
        (b6 => z26) = (0, 0);
        (b7 => z26) = (0, 0);
        (b8 => z26) = (0, 0);
        (b9 => z26) = (0, 0);
        (b10 => z26) = (0, 0);
        (b11 => z26) = (0, 0);
        (b12 => z26) = (0, 0);
        (b13 => z26) = (0, 0);
        (b14 => z26) = (0, 0);
        (b15 => z26) = (0, 0);
        (b16 => z26) = (0, 0);
        (b17 => z26) = (0, 0);
        (a0 => z27) = (0, 0);
        (a1 => z27) = (0, 0);
        (a2 => z27) = (0, 0);
        (a3 => z27) = (0, 0);
        (a4 => z27) = (0, 0);
        (a5 => z27) = (0, 0);
        (a6 => z27) = (0, 0);
        (a7 => z27) = (0, 0);
        (a8 => z27) = (0, 0);
        (a9 => z27) = (0, 0);
        (a10 => z27) = (0, 0);
        (a11 => z27) = (0, 0);
        (a12 => z27) = (0, 0);
        (a13 => z27) = (0, 0);
        (a14 => z27) = (0, 0);
        (a15 => z27) = (0, 0);
        (a16 => z27) = (0, 0);
        (a17 => z27) = (0, 0);
        (a18 => z27) = (0, 0);
        (a19 => z27) = (0, 0);
        (b0 => z27) = (0, 0);
        (b1 => z27) = (0, 0);
        (b2 => z27) = (0, 0);
        (b3 => z27) = (0, 0);
        (b4 => z27) = (0, 0);
        (b5 => z27) = (0, 0);
        (b6 => z27) = (0, 0);
        (b7 => z27) = (0, 0);
        (b8 => z27) = (0, 0);
        (b9 => z27) = (0, 0);
        (b10 => z27) = (0, 0);
        (b11 => z27) = (0, 0);
        (b12 => z27) = (0, 0);
        (b13 => z27) = (0, 0);
        (b14 => z27) = (0, 0);
        (b15 => z27) = (0, 0);
        (b16 => z27) = (0, 0);
        (b17 => z27) = (0, 0);
        (a0 => z28) = (0, 0);
        (a1 => z28) = (0, 0);
        (a2 => z28) = (0, 0);
        (a3 => z28) = (0, 0);
        (a4 => z28) = (0, 0);
        (a5 => z28) = (0, 0);
        (a6 => z28) = (0, 0);
        (a7 => z28) = (0, 0);
        (a8 => z28) = (0, 0);
        (a9 => z28) = (0, 0);
        (a10 => z28) = (0, 0);
        (a11 => z28) = (0, 0);
        (a12 => z28) = (0, 0);
        (a13 => z28) = (0, 0);
        (a14 => z28) = (0, 0);
        (a15 => z28) = (0, 0);
        (a16 => z28) = (0, 0);
        (a17 => z28) = (0, 0);
        (a18 => z28) = (0, 0);
        (a19 => z28) = (0, 0);
        (b0 => z28) = (0, 0);
        (b1 => z28) = (0, 0);
        (b2 => z28) = (0, 0);
        (b3 => z28) = (0, 0);
        (b4 => z28) = (0, 0);
        (b5 => z28) = (0, 0);
        (b6 => z28) = (0, 0);
        (b7 => z28) = (0, 0);
        (b8 => z28) = (0, 0);
        (b9 => z28) = (0, 0);
        (b10 => z28) = (0, 0);
        (b11 => z28) = (0, 0);
        (b12 => z28) = (0, 0);
        (b13 => z28) = (0, 0);
        (b14 => z28) = (0, 0);
        (b15 => z28) = (0, 0);
        (b16 => z28) = (0, 0);
        (b17 => z28) = (0, 0);
        (a0 => z29) = (0, 0);
        (a1 => z29) = (0, 0);
        (a2 => z29) = (0, 0);
        (a3 => z29) = (0, 0);
        (a4 => z29) = (0, 0);
        (a5 => z29) = (0, 0);
        (a6 => z29) = (0, 0);
        (a7 => z29) = (0, 0);
        (a8 => z29) = (0, 0);
        (a9 => z29) = (0, 0);
        (a10 => z29) = (0, 0);
        (a11 => z29) = (0, 0);
        (a12 => z29) = (0, 0);
        (a13 => z29) = (0, 0);
        (a14 => z29) = (0, 0);
        (a15 => z29) = (0, 0);
        (a16 => z29) = (0, 0);
        (a17 => z29) = (0, 0);
        (a18 => z29) = (0, 0);
        (a19 => z29) = (0, 0);
        (b0 => z29) = (0, 0);
        (b1 => z29) = (0, 0);
        (b2 => z29) = (0, 0);
        (b3 => z29) = (0, 0);
        (b4 => z29) = (0, 0);
        (b5 => z29) = (0, 0);
        (b6 => z29) = (0, 0);
        (b7 => z29) = (0, 0);
        (b8 => z29) = (0, 0);
        (b9 => z29) = (0, 0);
        (b10 => z29) = (0, 0);
        (b11 => z29) = (0, 0);
        (b12 => z29) = (0, 0);
        (b13 => z29) = (0, 0);
        (b14 => z29) = (0, 0);
        (b15 => z29) = (0, 0);
        (b16 => z29) = (0, 0);
        (b17 => z29) = (0, 0);
        (a0 => z30) = (0, 0);
        (a1 => z30) = (0, 0);
        (a2 => z30) = (0, 0);
        (a3 => z30) = (0, 0);
        (a4 => z30) = (0, 0);
        (a5 => z30) = (0, 0);
        (a6 => z30) = (0, 0);
        (a7 => z30) = (0, 0);
        (a8 => z30) = (0, 0);
        (a9 => z30) = (0, 0);
        (a10 => z30) = (0, 0);
        (a11 => z30) = (0, 0);
        (a12 => z30) = (0, 0);
        (a13 => z30) = (0, 0);
        (a14 => z30) = (0, 0);
        (a15 => z30) = (0, 0);
        (a16 => z30) = (0, 0);
        (a17 => z30) = (0, 0);
        (a18 => z30) = (0, 0);
        (a19 => z30) = (0, 0);
        (b0 => z30) = (0, 0);
        (b1 => z30) = (0, 0);
        (b2 => z30) = (0, 0);
        (b3 => z30) = (0, 0);
        (b4 => z30) = (0, 0);
        (b5 => z30) = (0, 0);
        (b6 => z30) = (0, 0);
        (b7 => z30) = (0, 0);
        (b8 => z30) = (0, 0);
        (b9 => z30) = (0, 0);
        (b10 => z30) = (0, 0);
        (b11 => z30) = (0, 0);
        (b12 => z30) = (0, 0);
        (b13 => z30) = (0, 0);
        (b14 => z30) = (0, 0);
        (b15 => z30) = (0, 0);
        (b16 => z30) = (0, 0);
        (b17 => z30) = (0, 0);
        (a0 => z31) = (0, 0);
        (a1 => z31) = (0, 0);
        (a2 => z31) = (0, 0);
        (a3 => z31) = (0, 0);
        (a4 => z31) = (0, 0);
        (a5 => z31) = (0, 0);
        (a6 => z31) = (0, 0);
        (a7 => z31) = (0, 0);
        (a8 => z31) = (0, 0);
        (a9 => z31) = (0, 0);
        (a10 => z31) = (0, 0);
        (a11 => z31) = (0, 0);
        (a12 => z31) = (0, 0);
        (a13 => z31) = (0, 0);
        (a14 => z31) = (0, 0);
        (a15 => z31) = (0, 0);
        (a16 => z31) = (0, 0);
        (a17 => z31) = (0, 0);
        (a18 => z31) = (0, 0);
        (a19 => z31) = (0, 0);
        (b0 => z31) = (0, 0);
        (b1 => z31) = (0, 0);
        (b2 => z31) = (0, 0);
        (b3 => z31) = (0, 0);
        (b4 => z31) = (0, 0);
        (b5 => z31) = (0, 0);
        (b6 => z31) = (0, 0);
        (b7 => z31) = (0, 0);
        (b8 => z31) = (0, 0);
        (b9 => z31) = (0, 0);
        (b10 => z31) = (0, 0);
        (b11 => z31) = (0, 0);
        (b12 => z31) = (0, 0);
        (b13 => z31) = (0, 0);
        (b14 => z31) = (0, 0);
        (b15 => z31) = (0, 0);
        (b16 => z31) = (0, 0);
        (b17 => z31) = (0, 0);
        (a0 => z32) = (0, 0);
        (a1 => z32) = (0, 0);
        (a2 => z32) = (0, 0);
        (a3 => z32) = (0, 0);
        (a4 => z32) = (0, 0);
        (a5 => z32) = (0, 0);
        (a6 => z32) = (0, 0);
        (a7 => z32) = (0, 0);
        (a8 => z32) = (0, 0);
        (a9 => z32) = (0, 0);
        (a10 => z32) = (0, 0);
        (a11 => z32) = (0, 0);
        (a12 => z32) = (0, 0);
        (a13 => z32) = (0, 0);
        (a14 => z32) = (0, 0);
        (a15 => z32) = (0, 0);
        (a16 => z32) = (0, 0);
        (a17 => z32) = (0, 0);
        (a18 => z32) = (0, 0);
        (a19 => z32) = (0, 0);
        (b0 => z32) = (0, 0);
        (b1 => z32) = (0, 0);
        (b2 => z32) = (0, 0);
        (b3 => z32) = (0, 0);
        (b4 => z32) = (0, 0);
        (b5 => z32) = (0, 0);
        (b6 => z32) = (0, 0);
        (b7 => z32) = (0, 0);
        (b8 => z32) = (0, 0);
        (b9 => z32) = (0, 0);
        (b10 => z32) = (0, 0);
        (b11 => z32) = (0, 0);
        (b12 => z32) = (0, 0);
        (b13 => z32) = (0, 0);
        (b14 => z32) = (0, 0);
        (b15 => z32) = (0, 0);
        (b16 => z32) = (0, 0);
        (b17 => z32) = (0, 0);
        (a0 => z33) = (0, 0);
        (a1 => z33) = (0, 0);
        (a2 => z33) = (0, 0);
        (a3 => z33) = (0, 0);
        (a4 => z33) = (0, 0);
        (a5 => z33) = (0, 0);
        (a6 => z33) = (0, 0);
        (a7 => z33) = (0, 0);
        (a8 => z33) = (0, 0);
        (a9 => z33) = (0, 0);
        (a10 => z33) = (0, 0);
        (a11 => z33) = (0, 0);
        (a12 => z33) = (0, 0);
        (a13 => z33) = (0, 0);
        (a14 => z33) = (0, 0);
        (a15 => z33) = (0, 0);
        (a16 => z33) = (0, 0);
        (a17 => z33) = (0, 0);
        (a18 => z33) = (0, 0);
        (a19 => z33) = (0, 0);
        (b0 => z33) = (0, 0);
        (b1 => z33) = (0, 0);
        (b2 => z33) = (0, 0);
        (b3 => z33) = (0, 0);
        (b4 => z33) = (0, 0);
        (b5 => z33) = (0, 0);
        (b6 => z33) = (0, 0);
        (b7 => z33) = (0, 0);
        (b8 => z33) = (0, 0);
        (b9 => z33) = (0, 0);
        (b10 => z33) = (0, 0);
        (b11 => z33) = (0, 0);
        (b12 => z33) = (0, 0);
        (b13 => z33) = (0, 0);
        (b14 => z33) = (0, 0);
        (b15 => z33) = (0, 0);
        (b16 => z33) = (0, 0);
        (b17 => z33) = (0, 0);
        (a0 => z34) = (0, 0);
        (a1 => z34) = (0, 0);
        (a2 => z34) = (0, 0);
        (a3 => z34) = (0, 0);
        (a4 => z34) = (0, 0);
        (a5 => z34) = (0, 0);
        (a6 => z34) = (0, 0);
        (a7 => z34) = (0, 0);
        (a8 => z34) = (0, 0);
        (a9 => z34) = (0, 0);
        (a10 => z34) = (0, 0);
        (a11 => z34) = (0, 0);
        (a12 => z34) = (0, 0);
        (a13 => z34) = (0, 0);
        (a14 => z34) = (0, 0);
        (a15 => z34) = (0, 0);
        (a16 => z34) = (0, 0);
        (a17 => z34) = (0, 0);
        (a18 => z34) = (0, 0);
        (a19 => z34) = (0, 0);
        (b0 => z34) = (0, 0);
        (b1 => z34) = (0, 0);
        (b2 => z34) = (0, 0);
        (b3 => z34) = (0, 0);
        (b4 => z34) = (0, 0);
        (b5 => z34) = (0, 0);
        (b6 => z34) = (0, 0);
        (b7 => z34) = (0, 0);
        (b8 => z34) = (0, 0);
        (b9 => z34) = (0, 0);
        (b10 => z34) = (0, 0);
        (b11 => z34) = (0, 0);
        (b12 => z34) = (0, 0);
        (b13 => z34) = (0, 0);
        (b14 => z34) = (0, 0);
        (b15 => z34) = (0, 0);
        (b16 => z34) = (0, 0);
        (b17 => z34) = (0, 0);
        (a0 => z35) = (0, 0);
        (a1 => z35) = (0, 0);
        (a2 => z35) = (0, 0);
        (a3 => z35) = (0, 0);
        (a4 => z35) = (0, 0);
        (a5 => z35) = (0, 0);
        (a6 => z35) = (0, 0);
        (a7 => z35) = (0, 0);
        (a8 => z35) = (0, 0);
        (a9 => z35) = (0, 0);
        (a10 => z35) = (0, 0);
        (a11 => z35) = (0, 0);
        (a12 => z35) = (0, 0);
        (a13 => z35) = (0, 0);
        (a14 => z35) = (0, 0);
        (a15 => z35) = (0, 0);
        (a16 => z35) = (0, 0);
        (a17 => z35) = (0, 0);
        (a18 => z35) = (0, 0);
        (a19 => z35) = (0, 0);
        (b0 => z35) = (0, 0);
        (b1 => z35) = (0, 0);
        (b2 => z35) = (0, 0);
        (b3 => z35) = (0, 0);
        (b4 => z35) = (0, 0);
        (b5 => z35) = (0, 0);
        (b6 => z35) = (0, 0);
        (b7 => z35) = (0, 0);
        (b8 => z35) = (0, 0);
        (b9 => z35) = (0, 0);
        (b10 => z35) = (0, 0);
        (b11 => z35) = (0, 0);
        (b12 => z35) = (0, 0);
        (b13 => z35) = (0, 0);
        (b14 => z35) = (0, 0);
        (b15 => z35) = (0, 0);
        (b16 => z35) = (0, 0);
        (b17 => z35) = (0, 0);
        (a0 => z36) = (0, 0);
        (a1 => z36) = (0, 0);
        (a2 => z36) = (0, 0);
        (a3 => z36) = (0, 0);
        (a4 => z36) = (0, 0);
        (a5 => z36) = (0, 0);
        (a6 => z36) = (0, 0);
        (a7 => z36) = (0, 0);
        (a8 => z36) = (0, 0);
        (a9 => z36) = (0, 0);
        (a10 => z36) = (0, 0);
        (a11 => z36) = (0, 0);
        (a12 => z36) = (0, 0);
        (a13 => z36) = (0, 0);
        (a14 => z36) = (0, 0);
        (a15 => z36) = (0, 0);
        (a16 => z36) = (0, 0);
        (a17 => z36) = (0, 0);
        (a18 => z36) = (0, 0);
        (a19 => z36) = (0, 0);
        (b0 => z36) = (0, 0);
        (b1 => z36) = (0, 0);
        (b2 => z36) = (0, 0);
        (b3 => z36) = (0, 0);
        (b4 => z36) = (0, 0);
        (b5 => z36) = (0, 0);
        (b6 => z36) = (0, 0);
        (b7 => z36) = (0, 0);
        (b8 => z36) = (0, 0);
        (b9 => z36) = (0, 0);
        (b10 => z36) = (0, 0);
        (b11 => z36) = (0, 0);
        (b12 => z36) = (0, 0);
        (b13 => z36) = (0, 0);
        (b14 => z36) = (0, 0);
        (b15 => z36) = (0, 0);
        (b16 => z36) = (0, 0);
        (b17 => z36) = (0, 0);
        (a0 => z37) = (0, 0);
        (a1 => z37) = (0, 0);
        (a2 => z37) = (0, 0);
        (a3 => z37) = (0, 0);
        (a4 => z37) = (0, 0);
        (a5 => z37) = (0, 0);
        (a6 => z37) = (0, 0);
        (a7 => z37) = (0, 0);
        (a8 => z37) = (0, 0);
        (a9 => z37) = (0, 0);
        (a10 => z37) = (0, 0);
        (a11 => z37) = (0, 0);
        (a12 => z37) = (0, 0);
        (a13 => z37) = (0, 0);
        (a14 => z37) = (0, 0);
        (a15 => z37) = (0, 0);
        (a16 => z37) = (0, 0);
        (a17 => z37) = (0, 0);
        (a18 => z37) = (0, 0);
        (a19 => z37) = (0, 0);
        (b0 => z37) = (0, 0);
        (b1 => z37) = (0, 0);
        (b2 => z37) = (0, 0);
        (b3 => z37) = (0, 0);
        (b4 => z37) = (0, 0);
        (b5 => z37) = (0, 0);
        (b6 => z37) = (0, 0);
        (b7 => z37) = (0, 0);
        (b8 => z37) = (0, 0);
        (b9 => z37) = (0, 0);
        (b10 => z37) = (0, 0);
        (b11 => z37) = (0, 0);
        (b12 => z37) = (0, 0);
        (b13 => z37) = (0, 0);
        (b14 => z37) = (0, 0);
        (b15 => z37) = (0, 0);
        (b16 => z37) = (0, 0);
        (b17 => z37) = (0, 0);        
     endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULT_BLASTED

module RS_DSP_MULT_REGIN_BLASTED ( 
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs
    input  wire unsigned_a,
    input  wire unsigned_b
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULT_REGIN #(.MODE_BITS(MODE_BITS)) multiplier (
        // Connect re-assembled inputs and outputs
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULT_REGIN_BLASTED

module RS_DSP_MULT_REGOUT_BLASTED ( 
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs
    input  wire unsigned_a,
    input  wire unsigned_b
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULT_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULT_REGOUT_BLASTED

module RS_DSP_MULT_REGIN_REGOUT_BLASTED ( 
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs
    input  wire unsigned_a,
    input  wire unsigned_b
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULT_REGIN_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULT_REGIN_REGOUT_BLASTED

module RS_DSP_MULTADD_BLASTED ( 
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted inputs 'feedback' and 'acc_fir'
    input wire feedback2, feedback1, feedback0,
    input wire acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs 
    input  wire load_acc,
    input  wire unsigned_a,
    input  wire unsigned_b,
    input  wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Unchanged inputs 
    input  wire round,
    input  wire subtract,

    // Bit-blasted output 'dly_b'
    output wire dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
    output wire dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] acc_fir_reassembled = {acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output
    wire [17:0] dly_b_internal;

    RS_DSP_MULTADD #(.MODE_BITS(MODE_BITS)) multiplier (
        // Connect re-assembled inputs and outputs
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .acc_fir(acc_fir_reassembled),
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract),
        .dly_b(dly_b_internal)
    );

   assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

    // Connect the bit-blasted dly_b ports to the internal output
    assign {dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
            dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0} = dly_b_internal; 

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (dly_b0+:b0)) = (0, 0);
         (posedge clk => (dly_b1+:b1)) = (0, 0);
         (posedge clk => (dly_b2+:b2)) = (0, 0);
         (posedge clk => (dly_b3+:b3)) = (0, 0);
         (posedge clk => (dly_b4+:b4)) = (0, 0);
         (posedge clk => (dly_b5+:b5)) = (0, 0);
         (posedge clk => (dly_b6+:b6)) = (0, 0);
         (posedge clk => (dly_b7+:b7)) = (0, 0);
         (posedge clk => (dly_b8+:b8)) = (0, 0);
         (posedge clk => (dly_b9+:b9)) = (0, 0);
         (posedge clk => (dly_b10+:b10)) = (0, 0);
         (posedge clk => (dly_b11+:b11)) = (0, 0);
         (posedge clk => (dly_b12+:b12)) = (0, 0);
         (posedge clk => (dly_b13+:b13)) = (0, 0);
         (posedge clk => (dly_b14+:b14)) = (0, 0);
         (posedge clk => (dly_b15+:b15)) = (0, 0);
         (posedge clk => (dly_b16+:b16)) = (0, 0);
         (posedge clk => (dly_b17+:b17)) = (0, 0);
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule


module RS_DSP_MULTADD_REGIN_BLASTED ( 
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted inputs 'feedback' and 'acc_fir'
    input wire feedback2, feedback1, feedback0,
    input wire acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs 
    input  wire load_acc,
    input  wire unsigned_a,
    input  wire unsigned_b,
    input  wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Unchanged inputs 
    input  wire round,
    input  wire subtract,

    // Bit-blasted output 'dly_b'
    output wire dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
    output wire dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] acc_fir_reassembled = {acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output
    wire [17:0] dly_b_internal;

    RS_DSP_MULTADD_REGIN #(.MODE_BITS(MODE_BITS)) multiplier (
        // Connect re-assembled inputs and outputs
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .acc_fir(acc_fir_reassembled),
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract),
        .dly_b(dly_b_internal)
    );

      // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

    // Connect the bit-blasted dly_b ports to the internal output
    assign {dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
            dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0} = dly_b_internal; 

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (dly_b0+:b0)) = (0, 0);
         (posedge clk => (dly_b1+:b1)) = (0, 0);
         (posedge clk => (dly_b2+:b2)) = (0, 0);
         (posedge clk => (dly_b3+:b3)) = (0, 0);
         (posedge clk => (dly_b4+:b4)) = (0, 0);
         (posedge clk => (dly_b5+:b5)) = (0, 0);
         (posedge clk => (dly_b6+:b6)) = (0, 0);
         (posedge clk => (dly_b7+:b7)) = (0, 0);
         (posedge clk => (dly_b8+:b8)) = (0, 0);
         (posedge clk => (dly_b9+:b9)) = (0, 0);
         (posedge clk => (dly_b10+:b10)) = (0, 0);
         (posedge clk => (dly_b11+:b11)) = (0, 0);
         (posedge clk => (dly_b12+:b12)) = (0, 0);
         (posedge clk => (dly_b13+:b13)) = (0, 0);
         (posedge clk => (dly_b14+:b14)) = (0, 0);
         (posedge clk => (dly_b15+:b15)) = (0, 0);
         (posedge clk => (dly_b16+:b16)) = (0, 0);
         (posedge clk => (dly_b17+:b17)) = (0, 0);
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULTADD_REGIN_BLASTED

module RS_DSP_MULTADD_REGOUT_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted inputs 'feedback' and 'acc_fir'
    input wire feedback2, feedback1, feedback0,
    input wire acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Unchanged inputs 
    input  wire load_acc,
    input  wire unsigned_a,
    input  wire unsigned_b,
    input  wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Unchanged inputs 
    input  wire round,
    input  wire subtract,

    // Bit-blasted output 'dly_b'
    output wire dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
    output wire dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] acc_fir_reassembled = {acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output
    wire [17:0] dly_b_internal;

    RS_DSP_MULTADD_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        // Connect re-assembled inputs and outputs
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .acc_fir(acc_fir_reassembled),
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract),
        .dly_b(dly_b_internal)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

    // Connect the bit-blasted dly_b ports to the internal output
    assign {dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
            dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0} = dly_b_internal; 

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (dly_b0+:b0)) = (0, 0);
         (posedge clk => (dly_b1+:b1)) = (0, 0);
         (posedge clk => (dly_b2+:b2)) = (0, 0);
         (posedge clk => (dly_b3+:b3)) = (0, 0);
         (posedge clk => (dly_b4+:b4)) = (0, 0);
         (posedge clk => (dly_b5+:b5)) = (0, 0);
         (posedge clk => (dly_b6+:b6)) = (0, 0);
         (posedge clk => (dly_b7+:b7)) = (0, 0);
         (posedge clk => (dly_b8+:b8)) = (0, 0);
         (posedge clk => (dly_b9+:b9)) = (0, 0);
         (posedge clk => (dly_b10+:b10)) = (0, 0);
         (posedge clk => (dly_b11+:b11)) = (0, 0);
         (posedge clk => (dly_b12+:b12)) = (0, 0);
         (posedge clk => (dly_b13+:b13)) = (0, 0);
         (posedge clk => (dly_b14+:b14)) = (0, 0);
         (posedge clk => (dly_b15+:b15)) = (0, 0);
         (posedge clk => (dly_b16+:b16)) = (0, 0);
         (posedge clk => (dly_b17+:b17)) = (0, 0);
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULTADD_REGOUT_BLASTED


module RS_DSP_MULTADD_REGIN_REGOUT_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted inputs 'feedback' and 'acc_fir'
    input wire feedback2, feedback1, feedback0,
    input wire acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Bit-blasted input 'load_acc'
    input wire load_acc,

    // Unchanged inputs 
    input  wire unsigned_a,
    input  wire unsigned_b,

    // Bit-blasted input 'saturate_enable'
    input wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Bit-blasted inputs 'round' and 'subtract'
    input wire round,
    input wire subtract,

    // Bit-blasted output 'dly_b'
    output wire dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
    output wire dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] acc_fir_reassembled = {acc_fir5, acc_fir4, acc_fir3, acc_fir2, acc_fir1, acc_fir0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output
    wire [17:0] dly_b_internal;

    RS_DSP_MULTADD_REGIN_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .acc_fir(acc_fir_reassembled),
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract),
        .dly_b(dly_b_internal)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

    // Connect the bit-blasted dly_b ports to the internal output
    assign {dly_b17, dly_b16, dly_b15, dly_b14, dly_b13, dly_b12, dly_b11, dly_b10,
            dly_b9, dly_b8, dly_b7, dly_b6, dly_b5, dly_b4, dly_b3, dly_b2, dly_b1, dly_b0} = dly_b_internal; 

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (dly_b0+:b0)) = (0, 0);
         (posedge clk => (dly_b1+:b1)) = (0, 0);
         (posedge clk => (dly_b2+:b2)) = (0, 0);
         (posedge clk => (dly_b3+:b3)) = (0, 0);
         (posedge clk => (dly_b4+:b4)) = (0, 0);
         (posedge clk => (dly_b5+:b5)) = (0, 0);
         (posedge clk => (dly_b6+:b6)) = (0, 0);
         (posedge clk => (dly_b7+:b7)) = (0, 0);
         (posedge clk => (dly_b8+:b8)) = (0, 0);
         (posedge clk => (dly_b9+:b9)) = (0, 0);
         (posedge clk => (dly_b10+:b10)) = (0, 0);
         (posedge clk => (dly_b11+:b11)) = (0, 0);
         (posedge clk => (dly_b12+:b12)) = (0, 0);
         (posedge clk => (dly_b13+:b13)) = (0, 0);
         (posedge clk => (dly_b14+:b14)) = (0, 0);
         (posedge clk => (dly_b15+:b15)) = (0, 0);
         (posedge clk => (dly_b16+:b16)) = (0, 0);
         (posedge clk => (dly_b17+:b17)) = (0, 0);
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULTADD_REGIN_REGOUT_BLASTED



module RS_DSP_MULTACC_BLASTED ( 
 // Bit-blasted inputs 'a' and 'b'
    input wire  a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input wire  a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input wire  b17, b16, b15, b14, b13, b12, b11, b10, 
    input wire  b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire  feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire  clk,
    input wire  lreset,

    // Bit-blasted input 'load_acc'
    input wire  load_acc,

    // Unchanged inputs 
    input wire  unsigned_a,
    input wire  unsigned_b,

    // Bit-blasted input 'saturate_enable'
    input wire  saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire  shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Bit-blasted inputs 'round' and 'subtract'
    input wire  round,
    input wire  subtract);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;       
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output

    // Instantiate the bit-blasted module 
    RS_DSP_MULTACC #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2,  z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif

   
endmodule // RS_DSP_MULTACC_BLASTED


module RS_DSP_MULTACC_REGIN_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Bit-blasted input 'load_acc'
    input wire load_acc,

    // Unchanged inputs 
    input  wire unsigned_a,
    input  wire unsigned_b,

    // Bit-blasted input 'saturate_enable'
    input wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Bit-blasted inputs 'round' and 'subtract'
    input wire round,
    input wire subtract
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULTACC_REGIN #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULTACC_REGIN_BLASTED

   
module RS_DSP_MULTACC_REGOUT_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Bit-blasted input 'load_acc'
    input wire load_acc,

    // Unchanged inputs 
    input  wire unsigned_a,
    input  wire unsigned_b,

    // Bit-blasted input 'saturate_enable'
    input wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Bit-blasted inputs 'round' and 'subtract'
    input wire round,
    input wire subtract
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    // Internal signals to re-assemble the bit-blasted ports
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULTACC_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule // RS_DSP_MULTACC_REGOUT_BLASTED


module RS_DSP_MULTACC_REGIN_REGOUT_BLASTED (
    // Bit-blasted inputs 'a' and 'b'
    input  wire a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
    input  wire a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, 

    input  wire b17, b16, b15, b14, b13, b12, b11, b10, 
    input  wire b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, 

    // Bit-blasted output 'z'
    output wire z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
    output wire z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0, 

    // Bit-blasted input 'feedback'
    input wire feedback2, feedback1, feedback0,

    // Bit-blasted inputs 'clk' and 'lreset'
    input wire clk,
    input wire lreset,

    // Bit-blasted input 'load_acc'
    input wire load_acc,

    // Unchanged inputs 
    input  wire unsigned_a,
    input  wire unsigned_b,

    // Bit-blasted input 'saturate_enable'
    input wire saturate_enable,

    // Bit-blasted input 'shift_right'
    input wire shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0,

    // Bit-blasted inputs 'round' and 'subtract'
    input wire round,
    input wire subtract
);
    parameter [0:84] MODE_BITS = 85'b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000;
    wire [19:0] a_reassembled = {a19, a18, a17, a16, a15, a14, a13, a12, a11, a10, 
                                 a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    wire [17:0] b_reassembled = {b17, b16, b15, b14, b13, b12, b11, b10, 
                                 b9, b8, b7, b6, b5, b4, b3, b2, b1, b0};
    wire [2:0] feedback_reassembled = {feedback2, feedback1, feedback0};
    wire [5:0] shift_right_reassembled = {shift_right5, shift_right4, shift_right3, shift_right2, shift_right1, shift_right0};
    wire [37:0] z_internal; // Internal signal to hold the output

    RS_DSP_MULTACC_REGIN_REGOUT #(.MODE_BITS(MODE_BITS)) multiplier (
        .a(a_reassembled), 
        .b(b_reassembled), 
        .z(z_internal), 
        .feedback(feedback_reassembled), 
        .clk(clk),
        .lreset(lreset),
        .load_acc(load_acc),
        .unsigned_a(unsigned_a), 
        .unsigned_b(unsigned_b),
        .saturate_enable(saturate_enable),
        .shift_right(shift_right_reassembled),
        .round(round),
        .subtract(subtract)
    );

    // Connect the bit-blasted output ports to the internal output
    assign {z37, z36, z35, z34, z33, z32, z31, z30, z29, z28, z27, z26, z25, z24, z23, z22, z21, z20, 
            z19, z18, z17, z16, z15, z14, z13, z12, z11, z10, z9, z8, z7, z6, z5, z4, z3, z2, z1, z0} = z_internal;

`ifndef SYNTHESIS
  `ifdef TIMED_SIM
     specify
         (posedge clk => (z0+:z0)) = (0, 0);
         (posedge clk => (z1+:z1)) = (0, 0);
         (posedge clk => (z2+:z2)) = (0, 0);
         (posedge clk => (z3+:z3)) = (0, 0);
         (posedge clk => (z4+:z4)) = (0, 0);
         (posedge clk => (z5+:z5)) = (0, 0);
         (posedge clk => (z6+:z6)) = (0, 0);
         (posedge clk => (z7+:z7)) = (0, 0);
         (posedge clk => (z8+:z8)) = (0, 0);
         (posedge clk => (z9+:z9)) = (0, 0);
         (posedge clk => (z10+:z10)) = (0, 0);
         (posedge clk => (z11+:z11)) = (0, 0);
         (posedge clk => (z12+:z12)) = (0, 0);
         (posedge clk => (z13+:z13)) = (0, 0);
         (posedge clk => (z14+:z14)) = (0, 0);
         (posedge clk => (z15+:z15)) = (0, 0);
         (posedge clk => (z16+:z16)) = (0, 0);
         (posedge clk => (z17+:z17)) = (0, 0);
         (posedge clk => (z18+:z18)) = (0, 0);
         (posedge clk => (z19+:z19)) = (0, 0);
         (posedge clk => (z20+:z20)) = (0, 0);
         (posedge clk => (z21+:z21)) = (0, 0);
         (posedge clk => (z22+:z22)) = (0, 0);
         (posedge clk => (z23+:z23)) = (0, 0);
         (posedge clk => (z24+:z24)) = (0, 0);
         (posedge clk => (z25+:z25)) = (0, 0);
         (posedge clk => (z26+:z26)) = (0, 0);
         (posedge clk => (z27+:z27)) = (0, 0);
         (posedge clk => (z28+:z28)) = (0, 0);
         (posedge clk => (z29+:z29)) = (0, 0);
         (posedge clk => (z30+:z30)) = (0, 0);
         (posedge clk => (z31+:z31)) = (0, 0);
         (posedge clk => (z32+:z32)) = (0, 0);
         (posedge clk => (z33+:z33)) = (0, 0);
         (posedge clk => (z34+:z34)) = (0, 0);
         (posedge clk => (z35+:z35)) = (0, 0);
         (posedge clk => (z36+:z36)) = (0, 0);
         (posedge clk => (z37+:z37)) = (0, 0);
   endspecify
  `endif
`endif
   
endmodule   


module RS_TDP36K_BLASTED  #(
    // Mode Bits
    parameter [0:80] MODE_BITS = {81{1'b0}},
    // Memory Initialization
    parameter [36863:0] INIT_i = {36864{1'b0}}
) (
    // Bit-blasted ports for interface 1
    input wire  WEN_A1,
    input wire  WEN_B1,
    input wire  REN_A1,
    input wire  REN_B1,
    input wire  CLK_A1,
    input wire  CLK_B1,
    input wire  BE_A1_1, BE_A1_0, 
    input wire  BE_B1_1, BE_B1_0, 
    input wire  ADDR_A1_14, ADDR_A1_13, ADDR_A1_12, ADDR_A1_11, ADDR_A1_10, ADDR_A1_9, ADDR_A1_8, ADDR_A1_7, ADDR_A1_6, ADDR_A1_5,
    input wire  ADDR_A1_4, ADDR_A1_3, ADDR_A1_2, ADDR_A1_1, ADDR_A1_0,
    input wire  ADDR_B1_14, ADDR_B1_13, ADDR_B1_12, ADDR_B1_11, ADDR_B1_10, ADDR_B1_9, ADDR_B1_8, ADDR_B1_7, ADDR_B1_6, ADDR_B1_5,
    input wire  ADDR_B1_4, ADDR_B1_3, ADDR_B1_2, ADDR_B1_1, ADDR_B1_0,
    input wire  WDATA_A1_17, WDATA_A1_16, WDATA_A1_15, WDATA_A1_14, WDATA_A1_13, WDATA_A1_12, WDATA_A1_11, WDATA_A1_10, WDATA_A1_9, WDATA_A1_8,
    input wire  WDATA_A1_7, WDATA_A1_6, WDATA_A1_5, WDATA_A1_4, WDATA_A1_3, WDATA_A1_2, WDATA_A1_1, WDATA_A1_0,
    input wire  WDATA_B1_17, WDATA_B1_16, WDATA_B1_15, WDATA_B1_14, WDATA_B1_13, WDATA_B1_12, WDATA_B1_11, WDATA_B1_10, WDATA_B1_9, WDATA_B1_8,
    input wire  WDATA_B1_7, WDATA_B1_6, WDATA_B1_5, WDATA_B1_4, WDATA_B1_3, WDATA_B1_2, WDATA_B1_1, WDATA_B1_0,
    output wire RDATA_A1_17, RDATA_A1_16, RDATA_A1_15, RDATA_A1_14, RDATA_A1_13, RDATA_A1_12, RDATA_A1_11, RDATA_A1_10, RDATA_A1_9, RDATA_A1_8,
    output wire RDATA_A1_7, RDATA_A1_6, RDATA_A1_5, RDATA_A1_4, RDATA_A1_3, RDATA_A1_2, RDATA_A1_1, RDATA_A1_0,
    output wire RDATA_B1_17, RDATA_B1_16, RDATA_B1_15, RDATA_B1_14, RDATA_B1_13, RDATA_B1_12, RDATA_B1_11, RDATA_B1_10, RDATA_B1_9, RDATA_B1_8,
    output wire RDATA_B1_7, RDATA_B1_6, RDATA_B1_5, RDATA_B1_4, RDATA_B1_3, RDATA_B1_2, RDATA_B1_1, RDATA_B1_0,
    input wire  FLUSH1,

    // Bit-blasted ports for interface 2
    input wire  WEN_A2,
    input wire  WEN_B2,
    input wire  REN_A2,
    input wire  REN_B2,
    input wire  CLK_A2,
    input wire  CLK_B2,
    input wire  BE_A2_1, BE_A2_0, 
    input wire  BE_B2_1, BE_B2_0, 
    input wire  ADDR_A2_13, ADDR_A2_12, ADDR_A2_11, ADDR_A2_10, ADDR_A2_9, ADDR_A2_8, ADDR_A2_7, ADDR_A2_6, ADDR_A2_5, ADDR_A2_4, 
    input wire  ADDR_A2_3, ADDR_A2_2, ADDR_A2_1, ADDR_A2_0,
    input wire  ADDR_B2_13, ADDR_B2_12, ADDR_B2_11, ADDR_B2_10, ADDR_B2_9, ADDR_B2_8, ADDR_B2_7, ADDR_B2_6, ADDR_B2_5, ADDR_B2_4,
    input wire  ADDR_B2_3, ADDR_B2_2, ADDR_B2_1, ADDR_B2_0,
    input wire  WDATA_A2_17, WDATA_A2_16, WDATA_A2_15, WDATA_A2_14, WDATA_A2_13, WDATA_A2_12, WDATA_A2_11, WDATA_A2_10, WDATA_A2_9, WDATA_A2_8,
    input wire  WDATA_A2_7, WDATA_A2_6, WDATA_A2_5, WDATA_A2_4, WDATA_A2_3, WDATA_A2_2, WDATA_A2_1, WDATA_A2_0,
    input wire  WDATA_B2_17, WDATA_B2_16, WDATA_B2_15, WDATA_B2_14, WDATA_B2_13, WDATA_B2_12, WDATA_B2_11, WDATA_B2_10, WDATA_B2_9, WDATA_B2_8, 
    input wire  WDATA_B2_7, WDATA_B2_6, WDATA_B2_5, WDATA_B2_4, WDATA_B2_3, WDATA_B2_2, WDATA_B2_1, WDATA_B2_0,
    output wire RDATA_A2_17, RDATA_A2_16, RDATA_A2_15, RDATA_A2_14, RDATA_A2_13, RDATA_A2_12, RDATA_A2_11, RDATA_A2_10, RDATA_A2_9, RDATA_A2_8, 
    output wire RDATA_A2_7, RDATA_A2_6, RDATA_A2_5, RDATA_A2_4, RDATA_A2_3, RDATA_A2_2, RDATA_A2_1, RDATA_A2_0,
    output wire RDATA_B2_17, RDATA_B2_16, RDATA_B2_15, RDATA_B2_14, RDATA_B2_13, RDATA_B2_12, RDATA_B2_11, RDATA_B2_10, RDATA_B2_9, RDATA_B2_8,
    output wire RDATA_B2_7, RDATA_B2_6, RDATA_B2_5, RDATA_B2_4, RDATA_B2_3, RDATA_B2_2, RDATA_B2_1, RDATA_B2_0,
    input wire  FLUSH2);


    // Internal signals to re-assemble the bit-blasted ports for interface 1
    wire [1:0] BE_A1_reassembled = {BE_A1_1, BE_A1_0};
    wire [1:0] BE_B1_reassembled = {BE_B1_1, BE_B1_0};
    wire [14:0] ADDR_A1_reassembled = {ADDR_A1_14, ADDR_A1_13, ADDR_A1_12, ADDR_A1_11, ADDR_A1_10, ADDR_A1_9, ADDR_A1_8, ADDR_A1_7, ADDR_A1_6, ADDR_A1_5,
                                       ADDR_A1_4, ADDR_A1_3, ADDR_A1_2, ADDR_A1_1, ADDR_A1_0};
    wire [14:0] ADDR_B1_reassembled = {ADDR_B1_14, ADDR_B1_13, ADDR_B1_12, ADDR_B1_11, ADDR_B1_10, ADDR_B1_9, ADDR_B1_8, ADDR_B1_7, ADDR_B1_6, ADDR_B1_5,
                                       ADDR_B1_4, ADDR_B1_3, ADDR_B1_2, ADDR_B1_1, ADDR_B1_0};
    wire [17:0] WDATA_A1_reassembled = {WDATA_A1_17, WDATA_A1_16, WDATA_A1_15, WDATA_A1_14, WDATA_A1_13, WDATA_A1_12, WDATA_A1_11, WDATA_A1_10, WDATA_A1_9, WDATA_A1_8,
                                        WDATA_A1_7, WDATA_A1_6, WDATA_A1_5, WDATA_A1_4, WDATA_A1_3, WDATA_A1_2, WDATA_A1_1, WDATA_A1_0};
    wire [17:0] WDATA_B1_reassembled = {WDATA_B1_17, WDATA_B1_16, WDATA_B1_15, WDATA_B1_14, WDATA_B1_13, WDATA_B1_12, WDATA_B1_11, WDATA_B1_10, WDATA_B1_9, WDATA_B1_8,
                                        WDATA_B1_7, WDATA_B1_6, WDATA_B1_5, WDATA_B1_4, WDATA_B1_3, WDATA_B1_2, WDATA_B1_1, WDATA_B1_0};

    // Internal signals to re-assemble the bit-blasted ports for interface 2
    wire [1:0] BE_A2_reassembled = {BE_A2_1, BE_A2_0};
    wire [1:0] BE_B2_reassembled = {BE_B2_1, BE_B2_0};
    wire [13:0] ADDR_A2_reassembled = {ADDR_A2_13, ADDR_A2_12, ADDR_A2_11, ADDR_A2_10, ADDR_A2_9, ADDR_A2_8, ADDR_A2_7, ADDR_A2_6, ADDR_A2_5,
                                       ADDR_A2_4, ADDR_A2_3, ADDR_A2_2, ADDR_A2_1, ADDR_A2_0};
    wire [13:0] ADDR_B2_reassembled = {ADDR_B2_13, ADDR_B2_12, ADDR_B2_11, ADDR_B2_10, ADDR_B2_9, ADDR_B2_8, ADDR_B2_7, ADDR_B2_6, ADDR_B2_5,
                                       ADDR_B2_4, ADDR_B2_3, ADDR_B2_2, ADDR_B2_1, ADDR_B2_0};
    wire [17:0] WDATA_A2_reassembled = {WDATA_A2_17, WDATA_A2_16, WDATA_A2_15, WDATA_A2_14, WDATA_A2_13, WDATA_A2_12, WDATA_A2_11, WDATA_A2_10, WDATA_A2_9, WDATA_A2_8,
                                        WDATA_A2_7, WDATA_A2_6, WDATA_A2_5, WDATA_A2_4, WDATA_A2_3, WDATA_A2_2, WDATA_A2_1, WDATA_A2_0};
    wire [17:0] WDATA_B2_reassembled = {WDATA_B2_17, WDATA_B2_16, WDATA_B2_15, WDATA_B2_14, WDATA_B2_13, WDATA_B2_12, WDATA_B2_11, WDATA_B2_10, WDATA_B2_9, WDATA_B2_8,
                                        WDATA_B2_7, WDATA_B2_6, WDATA_B2_5, WDATA_B2_4, WDATA_B2_3, WDATA_B2_2, WDATA_B2_1, WDATA_B2_0};
   
    // Instantiate the bit-blasted module 
    RS_TDP36K #(.MODE_BITS(MODE_BITS), .INIT_i(INIT_i)) bram (
        .WEN_A1(WEN_A1),
        .WEN_B1(WEN_B1),
        .REN_A1(REN_A1),
        .REN_B1(REN_B1),
        .CLK_A1(CLK_A1),
        .CLK_B1(CLK_B1),
        .BE_A1(BE_A1_reassembled), 
        .BE_B1(BE_B1_reassembled), 
        .ADDR_A1(ADDR_A1_reassembled), 
        .ADDR_B1(ADDR_B1_reassembled), 
        .WDATA_A1(WDATA_A1_reassembled), 
        .WDATA_B1(WDATA_B1_reassembled), 
        .RDATA_A1({RDATA_A1_17, RDATA_A1_16, RDATA_A1_15, RDATA_A1_14, RDATA_A1_13, RDATA_A1_12, RDATA_A1_11, RDATA_A1_10, RDATA_A1_9, RDATA_A1_8,
                    RDATA_A1_7, RDATA_A1_6, RDATA_A1_5, RDATA_A1_4, RDATA_A1_3, RDATA_A1_2, RDATA_A1_1, RDATA_A1_0}), 
        .RDATA_B1({RDATA_B1_17, RDATA_B1_16, RDATA_B1_15, RDATA_B1_14, RDATA_B1_13, RDATA_B1_12, RDATA_B1_11, RDATA_B1_10, RDATA_B1_9, RDATA_B1_8,
                    RDATA_B1_7, RDATA_B1_6, RDATA_B1_5, RDATA_B1_4, RDATA_B1_3, RDATA_B1_2, RDATA_B1_1, RDATA_B1_0}), 
        .FLUSH1(FLUSH1),

        .WEN_A2(WEN_A2),
        .WEN_B2(WEN_B2),
        .REN_A2(REN_A2),
        .REN_B2(REN_B2),
        .CLK_A2(CLK_A2),
        .CLK_B2(CLK_B2),
        .BE_A2(BE_A2_reassembled), 
        .BE_B2(BE_B2_reassembled), 
        .ADDR_A2(ADDR_A2_reassembled), 
        .ADDR_B2(ADDR_B2_reassembled), 
        .WDATA_A2(WDATA_A2_reassembled), 
        .WDATA_B2(WDATA_B2_reassembled), 
        .RDATA_A2({RDATA_A2_17, RDATA_A2_16, RDATA_A2_15, RDATA_A2_14, RDATA_A2_13, RDATA_A2_12, RDATA_A2_11, RDATA_A2_10, RDATA_A2_9, RDATA_A2_8,
                    RDATA_A2_7, RDATA_A2_6, RDATA_A2_5, RDATA_A2_4, RDATA_A2_3, RDATA_A2_2, RDATA_A2_1, RDATA_A2_0}), 
        .RDATA_B2({RDATA_B2_17, RDATA_B2_16, RDATA_B2_15, RDATA_B2_14, RDATA_B2_13, RDATA_B2_12, RDATA_B2_11, RDATA_B2_10, RDATA_B2_9, RDATA_B2_8,
                    RDATA_B2_7, RDATA_B2_6, RDATA_B2_5, RDATA_B2_4, RDATA_B2_3, RDATA_B2_2, RDATA_B2_1, RDATA_B2_0}), 
        .FLUSH2(FLUSH2)           
    );


  
`ifndef SYNTHESIS
 `ifdef TIMED_SIM
     specify
         (posedge CLK_A1 => (RDATA_A1_0+:WDATA_A1_0)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_1+:WDATA_A1_1)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_2+:WDATA_A1_2)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_3+:WDATA_A1_3)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_4+:WDATA_A1_4)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_5+:WDATA_A1_5)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_6+:WDATA_A1_6)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_7+:WDATA_A1_7)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_8+:WDATA_A1_8)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_9+:WDATA_A1_9)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_10+:WDATA_A1_10)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_11+:WDATA_A1_11)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_12+:WDATA_A1_12)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_13+:WDATA_A1_13)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_14+:WDATA_A1_14)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_15+:WDATA_A1_15)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_16+:WDATA_A1_16)) = (0, 0);
         (posedge CLK_A1 => (RDATA_A1_17+:WDATA_A1_17)) = (0, 0);

         (posedge CLK_A2 => (RDATA_A2_0+:WDATA_A2_0)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_1+:WDATA_A2_1)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_2+:WDATA_A2_2)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_3+:WDATA_A2_3)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_4+:WDATA_A2_4)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_5+:WDATA_A2_5)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_6+:WDATA_A2_6)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_7+:WDATA_A2_7)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_8+:WDATA_A2_8)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_9+:WDATA_A2_9)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_10+:WDATA_A2_10)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_11+:WDATA_A2_11)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_12+:WDATA_A2_12)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_13+:WDATA_A2_13)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_14+:WDATA_A2_14)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_15+:WDATA_A2_15)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_16+:WDATA_A2_16)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_17+:WDATA_A2_17)) = (0, 0);

         (posedge CLK_B1 => (RDATA_B1_0+:WDATA_B1_0)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_1+:WDATA_B1_1)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_2+:WDATA_B1_2)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_3+:WDATA_B1_3)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_4+:WDATA_B1_4)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_5+:WDATA_B1_5)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_6+:WDATA_B1_6)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_7+:WDATA_B1_7)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_8+:WDATA_B1_8)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_9+:WDATA_B1_9)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_10+:WDATA_B1_10)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_11+:WDATA_B1_11)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_12+:WDATA_B1_12)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_13+:WDATA_B1_13)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_14+:WDATA_B1_14)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_15+:WDATA_B1_15)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_16+:WDATA_B1_16)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_17+:WDATA_B1_17)) = (0, 0);

         (posedge CLK_B2 => (RDATA_B2_0+:WDATA_B2_0)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_1+:WDATA_B2_1)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_2+:WDATA_B2_2)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_3+:WDATA_B2_3)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_4+:WDATA_B2_4)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_5+:WDATA_B2_5)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_6+:WDATA_B2_6)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_7+:WDATA_B2_7)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_8+:WDATA_B2_8)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_9+:WDATA_B2_9)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_10+:WDATA_B2_10)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_11+:WDATA_B2_11)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_12+:WDATA_B2_12)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_13+:WDATA_B2_13)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_14+:WDATA_B2_14)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_15+:WDATA_B2_15)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_16+:WDATA_B2_16)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_17+:WDATA_B2_17)) = (0, 0);
    endspecify
 `endif  
`endif   
   
endmodule

                          
