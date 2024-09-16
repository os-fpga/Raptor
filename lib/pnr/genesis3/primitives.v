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
    input  in0,
    input  in1,
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
          .A({in0, in1}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 3 for IVerilog SDF annotated simulation
module LUT_K3 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in0,
    input  in1,
    input  in2,
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
          .A({in0, in1, in2}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 4 for IVerilog SDF annotated simulation
module LUT_K4 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in0,
    input  in1,
    input  in2,
    input  in3,
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
          .A({in0, in1, in2, in3}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 5 for IVerilog SDF annotated simulation
module LUT_K5 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in0,
    input  in1,
    input  in2,
    input  in3,
    input  in4,
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
          .A({in0, in1, in2, in3, in4}),
          .Y(out)
        );
endmodule

// Bit blasted LUT_K with K == 6 for IVerilog SDF annotated simulation
module LUT_K6 #(
    parameter K = 1, 
    parameter LUT_MASK={2**K{1'b0}} 
) (
    input  in0,
    input  in1,
    input  in2,
    input  in3,
    input  in4,
    input  in5,
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
          .A({in0, in1, in2, in3, in4, in5}),
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
          // TODO: 1 to many:
         (posedge CLK_A1 => (RDATA_A1_0+:WDATA_A1_0)) = (0, 0);
         (posedge CLK_A2 => (RDATA_A2_0+:WDATA_A2_0)) = (0, 0);
         (posedge CLK_B1 => (RDATA_B1_0+:WDATA_B1_0)) = (0, 0);
         (posedge CLK_B2 => (RDATA_B2_0+:WDATA_B2_0)) = (0, 0);
    endspecify
 `endif  
`endif   
   
endmodule

                          
