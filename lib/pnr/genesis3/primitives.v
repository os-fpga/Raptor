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

`ifndef IVERILOG   
    specify
        (in *> out) = "";
    endspecify
`else
   specparam T1 = 1;
    specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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
   specparam T1 = 1;
   specparam         T2 = 1;
   specify
        (in *> out) = (T1, T2);
   endspecify
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

`ifndef IVERILOG   
    specify
       (datain=>dataout)="";
    endspecify
`else
   specparam T1 = 3,
            T2 = 2;
    specify
       (datain=>dataout) = (T1, T2);
    endspecify
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
