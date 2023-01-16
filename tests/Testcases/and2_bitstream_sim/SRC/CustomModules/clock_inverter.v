module clock_inverter (a, scan_mode, z);
input a;
input scan_mode;
output z;
wire z;
//assign z = ~a;
CKND4BWP7D5T16P96CPD U0 (.I(a), .ZN(z));
endmodule
