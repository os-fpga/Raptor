// Dirty wrapper necessary to decrease the number of pins to fit gemini
module wrapper (input	[0:127]	ct,
	input	ct_vld,
	output	ct_rdy,
	
	input	rkey_vld,
	output	next_rkey,
	
	output	[0:127]	pt,
	output	pt_vld,
	
	input	[0:1]	klen_sel,	
	
	input	clk,
	input	rst);


 decrypt U1 (
	ct,
	ct_vld,
	ct_rdy,
	
        ct ,
	rkey_vld,
	next_rkey,
	
	pt,
	pt_vld,
	
	klen_sel,	
	
	clk,
	rst
	);

endmodule
