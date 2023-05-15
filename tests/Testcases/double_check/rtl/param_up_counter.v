`define size 5

module param_up_counter (input logic clock0, input logic rst_counter, output logic [`size-1:0] q_counter);

    always @ (posedge clock0)
    begin
        if(!rst_counter)
		q_counter <= 'b00000000;
	else
		q_counter <= q_counter + 1;
    end

endmodule 
