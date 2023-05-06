module add__a_to_output (output wire out);
    and_out a1(.a(1'b1),.b(1'b1),.out(out));
endmodule

module and_out (input a,input b,output out);

assign out=a & b;
endmodule