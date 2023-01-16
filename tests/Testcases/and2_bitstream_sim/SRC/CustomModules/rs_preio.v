
module RS_PREIO ( SOC_CLK, SOC_IN, SOC_OUT, FPGA_CLK, FPGA_IN, FPGA_OUT);

  output SOC_CLK;
  input SOC_IN;
  output SOC_OUT;
  input FPGA_CLK;
  output FPGA_IN;
  input FPGA_OUT;

  BUFFD1BWP7D5T16P96CPDLVT BUF_CLK(
                .I(FPGA_CLK),
                .Z(SOC_CLK)
        );

  BUFFD1BWP7D5T16P96CPDLVT BUF_F2A(
                .I(FPGA_OUT),
                .Z(SOC_OUT)
        );

  BUFFD1BWP7D5T16P96CPDLVT BUF_A2F(
                .I(SOC_IN),
                .Z(FPGA_IN)
        );

endmodule

