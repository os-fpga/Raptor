#include <iostream>

#include "Vsyn_tb.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "verilated_fst_c.h"

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
  bool fstWaveType = true;
  if (waveformFile.find(".vcd") != std::string::npos) {
    fstWaveType = false;
  }
  Verilated::traceEverOn(true);
  Verilated::assertOn(true);
  VerilatedFstC* tfpFst = nullptr;
  //VerilatedVcdC* tfpVcd = nullptr;
  if (fstWaveType) {
    tfpFst = new VerilatedFstC;
    top->trace(tfpFst, 99);
    tfpFst->open(waveformFile.c_str());
  } else {
    //tfpVcd = new VerilatedVcdC;
    // top->trace(tfpVcd, 99);
    //tfpVcd->open(waveformFile.c_str());
  }
 
  top->rstn = 0;
  top->clk = 0;
  
  // run simulation for 20 clock periods
  // Only generate the clock in C,
  // the testbench is in Verilog
  for(int i = 0; i < 20; i++) {
    top->rstn = (i >= 2);
    for(int clk = 0; clk < 2; ++clk) {
      top->eval();
      if (fstWaveType) {
        tfpFst->dump((2 * i) + clk);
      } else {
        //tfpVcd->dump((2 * i) + clk);
      }
      top->clk = !top->clk;
    }
  }
  if (fstWaveType) {
    tfpFst->close();
  } else {
    //tfpVcd->close();
  }
  delete top;
  exit(0);
}
