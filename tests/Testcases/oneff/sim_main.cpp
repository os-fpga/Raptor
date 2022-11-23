#include <iostream>

#include "Vsyn_tb.h"
#include "verilated.h"

#if VM_TRACE_VCD == 1
#include "verilated_vcd_c.h"
#else  
#include "verilated_fst_c.h"
#endif

double sc_time_stamp() {
  return 0;
} 

int main(int argc, char** argv, char** env) {
  Verilated::commandArgs(argc, argv);
  Vsyn_tb* top = new Vsyn_tb;
  // init trace dump
  std::string waveformFile;
  if (argc > 1) {
    waveformFile = argv[1];
    std::cout << "Waveform file: " << waveformFile << std::endl;
  } else {
    waveformFile = "syn_tb.fst";
  }
  Verilated::traceEverOn(true);
  Verilated::assertOn(true);

#if VM_TRACE_FST == 1 
  VerilatedFstC* tfp = new VerilatedFstC;
# else 
  VerilatedVcdC* tfp = new VerilatedVcdC;
#endif
  
  top->trace(tfp, 99);
  tfp->open(waveformFile.c_str());
  top->rstn = 0;
  top->clk = 0;
  
  // run simulation for 20 clock periods
  // Only generate the clock in C,
  // the testbench is in Verilog
  for(int i = 0; i < 20; i++) {
    top->rstn = (i >= 2);
    for(int clk = 0; clk < 2; ++clk) {
      top->eval();
      tfp->dump((2 * i) + clk);
      top->clk = !top->clk;
    }
  }
  tfp->close();
  delete top;
  exit(0);
}
