`celldefine
module BUFFD1BWP7D5T16P96CPDLVT (I, Z);
    input I;
    output Z;
    buf (Z, I);

  specify
    (I => Z) = (0, 0);
  endspecify
endmodule
`endcelldefine

