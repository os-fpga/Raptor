# Block
create_block(name="gbox_root_bank_clkmux")

# Ports
add_port(name="core_clk_in",  dir=DIR_IN,   bit=40)
add_port(name="cdr_clk_in",   dir=DIR_IN,   bit=40)
add_port(name="core_clk",     dir=DIR_OUT,  bit=2)
add_port(name="cdr_clk",      dir=DIR_OUT,  bit=2)

# Config
core_clk_selection_A = {}
core_clk_selection_B = {}
cdr_clk_selection_A = {}
cdr_clk_selection_B = {}
for i in range(20) :
  core_clk_selection_A[i] = "core_clk_in[%d]" % i
  core_clk_selection_B[i] = "core_clk_in[%d]" % (20 + i)
  cdr_clk_selection_A[i] = "cdr_clk_in[%d]" % i
  cdr_clk_selection_B[i] = "cdr_clk_in[%d]" % (20 + i)
add_config_mux(out="core_clk[0]",
                selection=core_clk_selection_A,
                bits=[{"CORE_CLK_ROOT_SEL_A" : 5}])
add_config_mux(out="core_clk[1]",
                selection=core_clk_selection_B,
                bits=[{"CORE_CLK_ROOT_SEL_B" : 5}])
add_config_mux(out="cdr_clk[0]",
                selection=cdr_clk_selection_A,
                bits=[{"CDR_CLK_ROOT_SEL_A" : 5}])
add_config_mux(out="cdr_clk[1]",
                selection=cdr_clk_selection_B,
                bits=[{"CDR_CLK_ROOT_SEL_B" : 5}])