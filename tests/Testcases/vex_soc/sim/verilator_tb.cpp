#include "../obj_dir/Vtestbench.h"
int sc_main(int argc, char** argv){
    Verilated::traceEverOn(true);
    new Vtestbench("top");
    while (!Verilated::gotFinish()) { sc_start(1, SC_NS); }
    return 0;
}
