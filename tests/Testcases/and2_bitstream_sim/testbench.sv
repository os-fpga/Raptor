module tb_and2;
  logic a,b;
  wire c;
 
  and2 U1(a,b,c);
   
  initial
    begin
      a = 1'b0;
      b = 1'b0;

      #5
      a = 1'b0; 
      b = 1'b1;

      #5
      a = 1'b1;
      b = 1'b0;

      #5
      a = 1'b1;
      b = 1'b1;
       
      #5
      a = 1'b0;
      b = 1'b0;
    end // initial begin

    initial begin
      $dumpvars(0, tb_and2);
    end
   
endmodule   
