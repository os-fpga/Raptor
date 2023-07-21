 
module bidirec_intf(
    
    input wire clk,
    input wire reset,

    input wire Ta,
    input wire Tb,

    input  wire in_a,
    input  wire in_b,

    inout wire ino_a,
    inout wire ino_b,
    inout wire ino_bn,
    
    output wire outa,
    output wire outb

 
  );


    wire in_a_in;
    wire in_a_in1;
    wire in_b_in;
    wire in_b_in1;

    
    wire outa_in;
    wire outb_in;

    wire Ta_in;
    wire Tb_in;


  reg [3:0] register;

  always @(posedge clk or posedge reset) begin
    if (reset)
      register <= 4'b0000;
    else if (!Ta_in)
      register <= {register[2:0], in_a_in};
  end

  assign in_b_in = !Ta?register[3]:in_b_in1;


  reg [3:0] register1;

  always @(posedge clk or posedge reset) begin
    if (reset)
      register1 <= 4'b0000;
    else if (!Tb_in)
      register1 <= {register1[2:0], in_a_in};
  end

  assign in_a_in = !Tb?register1[3]:in_a_in1;

    I_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    ibuf_inst1 (.I(in_a),.C(clk),.O(in_a_in1));

    I_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    ibuf_inst2 (.I(in_b),.C(clk),.O(in_b_in1));
  
    I_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    ibuf_inst3 (.I(Ta),.C(clk),.O(Ta_in));

    I_BUF #(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    ibuf_inst4 (.I(Tb),.C(clk),.O(Tb_in));

    O_BUF#(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    obuf_inst5 (.I(outa_in),.C(clk),.O(outa));
  
    O_BUF#(.PULL_UP_DOWN("NONE" ), .SLEW_RATE("FAST"), .DELAY(0),.REG_EN("FALSE")) 
    obuf_inst6 (.I(outb_in),.C(clk),.O(outa));
  
  

    IO_BUF iobuf_inst (.I(in_a_in),.IO(ino_a),.T(Ta_in),.O(outa_in));
    IO_BUF_DS iobufds_inst (.I(in_b_in),.IOP(ino_b),.ION(ino_bn),.T(Tb_in),.O(outb_in));

    
    
  
  endmodule