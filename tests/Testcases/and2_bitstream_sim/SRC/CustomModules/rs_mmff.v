//-----------------------------------------------------
// Design Name    : Multimode data flipflop
// File Name      : rs_mmff.v
// Function       : Multimode flipflop
//                   - mode[0] Flop (0) / latch (1)
//                   - mode[1] Clock inversion: posedge=0, negedge=1
//                   - mode[2] Enable (0) / No Enable (1) (only for ff)
//                   - mode[3] Reset (0) / No reset (1)
//                   - mode[4] Reset: sync (0) / async (1)
// Author         : Ganesh Gore
//                : [ganesh.gore@rapidsilicon.com]
//-----------------------------------------------------
//              ┌─────────────┐
//         din  │             │
//      ───────►│             │
//     scan_in  │             │
//      ───────►│             │
//       clock  │             │ out
//      ───────►│             ├────►
//       reset  │             │
//      ───────►│   RS_MMFF   │
//      enable  │             │ scan_out
//      ───────►│             ├────►
// scan_enable  │             │
//      ───────►│             │
//   mode[4:0]  │             │
//      ───────►│             │
// global_reset │             │
//      ───────►│             │
//              │             │
//              │             │
//              └─────────────┘
//--------------------------------------------------------------------------
// mode[2]     mode[3]     mode[4] |   ff_mode      |   latch_mode
//--------------------------------------------------------------------------
//   0           0           0     | sdffre/sdffnre |   latchr/latchnr
//   0           0           1     |  dffre/dffnre  |   latchr/latchnr
//   0           1           0     |  sdffe/sdffne  |   latch/latchn
//   0           1           1     |   dffe/dffne   |   latch/latchn
//   1           0           0     |  sdffr/sdffnr  |   latchr/latchnr
//   1           0           1     |   dffr/dffnr   |   latchr/latchnr
//   1           1           0     |   sdff/sdffn   |   latch/latchn
//   1           1           1     |    dff/dffn    |   latch/latchn
//--------------------------------------------------------------------------
//
// read_verilog rs_mmff.v
// create_clock clock
// compile
//

module MMFF ( din, scan_in, clock, reset,
                 enable, scan_enable, scan_mode,
                 mode, global_reset, out,scan_out);

// Input ports
input din;
input scan_in;
input clock;
input reset;
input enable;
input scan_enable;
input scan_mode;
input [4:0] mode;
input global_reset; // asynchronous, unconditional, active high reset
// Output ports
output out;
output scan_out;


wire clock_in; // clock after polarity inversion
wire clock_in_select;
reg dff_d;
wire dff_q;
wire latch_q;
reg async_reset_n;
wire async_reset_n_final;

assign out = ~(~mode[0] | scan_mode) ? latch_q : dff_q;
assign scan_out = out;
assign clock_in_select = ~(~mode[1] | scan_mode);
assign async_reset_n_final = scan_mode ? 1'b1: async_reset_n; // no resets in scan mode

always @(*)
    if (~mode[3] & ~mode[4] & reset) // sync reset
        dff_d = 1'b0;
    else if (~mode[2]) // load enable
        dff_d = enable ? din : dff_q;
    else
        dff_d = din;

// Balanced clock inverter cell using "create_clock clock" before synthesis
CKXOR2D1BWP7D5T16P96CPD clock_edge_selection (
    .A1(clock_in_select),
    .A2(clock),
    .Z(clock_in)
);

always @(*)
    if (global_reset)
        async_reset_n = 1'b0; // active low async reset
    else if (~mode[3] & mode[4] & reset) // async reset
        async_reset_n = 1'b0;
    else
        async_reset_n = 1'b1;

// Active low reset
// Active high enable, 1=transparent 0=hold
// W=1.536um
LHCNQD1BWP7D5T16P96CPD latch (
                .D(din),
                .CDN(async_reset_n_final),
                .E(clock_in),
                .Q(latch_q)
        );

// Positive edge flop
// Active low reset
// Active high scan enable signal
// W=2.496um
SDFCNQD1BWP7D5T16P96CPD dff (
                .D(dff_d),
                .SI(scan_in),
                .SE(scan_enable),
                .CP(clock_in),
                .CDN(async_reset_n_final),
                .Q(dff_q)
        );

endmodule

