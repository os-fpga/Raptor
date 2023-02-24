module counter16 (clock0,reset,op,q);
input clock0;
input reset;
input op;
output [15:0] q;
reg [15:0] q;

always @ (posedge clock0)
begin
    if (reset)
        q <= 'd0;
    else if (op=='d1)
        q <= q[7:0] * q[15:8];
    else
        q <= q+'d1;
end

endmodule 

