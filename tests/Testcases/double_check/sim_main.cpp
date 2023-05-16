#include <iostream>

#include "Vparam_up_counter.h"
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
  Vparam_up_counter* top = new Vparam_up_counter;
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
  top->rst_counter = 0;
  top->clock0 = 0;
  
  for(int i = 0; i < 50; i++) {
    top->rst_counter = (i >= 2);
    for(int clk = 0; clk < 2; ++clk) {
      top->eval();
      tfp->dump((2 * i) + clk);
      top->clock0 = !top->clock0;
    }
  }
  tfp->close();
  delete top;
  exit(0);
}
