#  -----------------------------------------------------------------------------
#  FILE NAME      : tb_generator.py
#  AUTHOR         : Muhammad Bilal Malik
#  AUTHOR'S EMAIL : bilalmalik12341@gmail.com
#  -----------------------------------------------------------------------------
#  RELEASE HISTORY
#  VERSION          DATE       AUTHOR                        DESCRIPTION
#  1.0              2024       Muhammad Bilal Malik          Initial Version
#  -----------------------------------------------------------------------------
#  -----------------------------------------------------------------------------
#  PURPOSE: Generates LEC based test bench to compare RTL vs NETLIST O/Ps
#  -----------------------------------------------------------------------------
#  REQUIRES: 
        #    Raptor
        #    random
        #    json
        #    sys
#  -----------------------------------------------------------------------------
#  USED BY:  Raptor
#  -----------------------------------------------------------------------------
#  DESCRIPTION: 
        #   Parse .json file created after synthesis to extract and create
        #   test bench for combinational as well as sequential designs
#  -----------------------------------------------------------------------------

import os
import random
import json
import sys

def create_folders_and_file():
    default_iteration   = 100
    design              = sys.argv[1]
    design_path         = sys.argv[2]
    if len(sys.argv) >= 4:
        try:
            iteration_val = sys.argv[3]
        except ValueError:
            print("Invalid value provided for iteration_val. Using default value.")
            iteration_val = default_iteration
    else:
        iteration_val = default_iteration

    # Construct the path to port_info.json using *
    port_info_path = os.path.join(
        design_path,
        design,
        "run_1",
        "synth_1_1",
        "synthesis",
        "netlist_info.json"
    )

    # Define the folder names
    main_folder = os.path.join(design_path, "sim")
    sub_folder = os.path.join(main_folder, "co_sim_tb")
    file_string = "co_sim_"
    rtl_inst = "golden (.*);"
    
    # List to instantiate post_synth
    wire_instances = []
    p_name_compare = []
    p_name_netlist = []
    
    # List to check outputs
    out_instances = []
    
    # List to store memories
    filtered_mem = []

    # Create the main folder "sim"
    try:
        os.mkdir(main_folder)
    except FileExistsError:
        print(f"Folder '{main_folder}' already exists.")

    # Create the sub folder "co_sim_tb" inside "sim"
    sub_folder_path = os.path.join(main_folder, sub_folder)
    try:
        os.mkdir(sub_folder_path)
    except FileExistsError:
        print(f"Folder '{sub_folder}' already exists inside '{main_folder}'.")

    # Read the JSON file
    with open(port_info_path) as json_file:
        # Read the content of the file
        content = json_file.read()

        # Replace all occurrences of backslash with double backslashes
        content = content.replace("\\", "\\\\")

        # Parse the corrected JSON content
        data = json.loads(content)

    # Extract topModule
    top_module = data['top']
    
    # memory info 
    if "memories" in data:
        for port in data["memories"]:
            # Check if the "name" field contains "$" or ":" characters
            if "$" not in port["name"] and ":" not in port["name"]:
                filtered_mem.append(port)
        rtl_mem = filtered_mem
    else:
        rtl_mem = []
        print("No memories were found in this design")

    # Create a file with the topModule name
    filename = file_string + top_module + ".v"
    output_filename = os.path.join(sub_folder_path, filename) 
    
    # Extract port information
    input_ports = []
    output_ports = []
    inout_ports = []
    clk_port = []
    reset_port = []
    wire_instances = []
    p_name_compare = []
    p_name_netlist = []
    bit_width_list = []
    inout_wdth_lst = []
    ports = {}
    
    # Extract iteration value 
    iteration_map = {'1': 1000, '2': 2000}
    iteration = iteration_map.get(iteration_val)
    if iteration is None:
        iteration = default_iteration

    # store port list info 
    for port in data['ports']:
        if "clock" in port and port["direction"] == "input":
            clk_port.append(port["name"])
        elif "sync_reset" in port:
            reset_port.append(port["name"])
            sync_reset_value = port["sync_reset"]
        elif "async_reset" in port:
            reset_port.append(port["name"])
            sync_reset_value = port["async_reset"]
        else:
            port_name = port["name"]
            port_direction = port["direction"]
            if "[" in port_name:
                bus_name, bus_width = port_name.split("[")
                bus_width = "[" + bus_width
                if bus_name not in ports:
                    ports[bus_name] = {"direction": port_direction, "width": bus_width}
                else:
                    existing_width = ports[bus_name]["width"]
                    if existing_width is None:
                        ports[bus_name]["width"] = bus_width
                    else:
                        existing_width_int = int(existing_width.strip("[]"))
                        new_width_int = int(bus_width.strip("[]"))
                        max_width = max(existing_width_int, new_width_int)
                        ports[bus_name]["width"] = f"[{max_width}]"
            else:
                ports[port_name] = {"direction": port_direction, "width": None}
           
    # Separate input, inout and output ports    
    for port, info in ports.items():
        if info["direction"] == "input":
            input_ports.append(port)
            if info["width"] is not None:
                width_str = info["width"].strip("[]")
                width_int = int(width_str) + 1
                bit_width_list.append(width_int)
            else:
                bit_width_list.append(None)
        elif info["direction"] == "inout":
            inout_ports.append(port)
            if info["width"] is not None:
                inout_width_str = info["width"].strip("[]")
                inout_width_int = int(inout_width_str) + 1
                inout_wdth_lst.append(inout_width_int)
            else:
                inout_wdth_lst.append(None)
        elif info["direction"] == "output":
            output_ports.append(port)
    
    print (inout_ports)
    print (inout_wdth_lst) 
    
    # Write module name and port information to a file
    with open(output_filename, "w") as file:
        file.write("`timescale 1ns/1ps\nmodule " + file_string + top_module + ";\n")
        if clk_port:
            file.write("// Clock signals\n")
            for port in clk_port:
                file.write("    reg\t\t\t" + port + ";\n")

        # Check if reset_port list is not empty and write its contents to the file
        if reset_port:
            file.write("// Reset signals\n")
            for port in reset_port:
                file.write("    reg\t\t\t" + port + ";\n")
            file.write("\n")
        for i, (port, info) in enumerate(ports.items()):
            if i != 0:
                file.write(";\n")
            if info["width"] is not None:
                if "]" in info["width"]:
                    info["width"] = info["width"].replace("]", ":0]")
                if info["direction"] == "input":
                    file.write("    reg \t\t" + info["width"] + " \t\t" + port)
                elif info["direction"] == "inout":
                    file.write("    wire \t\t" + info["width"] + " \t\t" + port + ";\n")
                    file.write("    reg  \t\t" + info["width"] + " \t\tinout_drv_" + port + ";\n")
                    file.write("    wire\t\t" + info["width"] + " \t\tinout_rcv_" + port)
                elif info["direction"] == "output":
                    p_name_inst     = port + "_netlist"
                    p_name_with_netlist = port + "\t,\t" + p_name_inst
                    p_name_compare.append(port)
                    p_name_netlist.append(p_name_inst)
                    wire_instances.append(".{}({})".format(port, p_name_inst))
                    out_instances.append("{} !== {}".format(port, p_name_inst))
                    file.write("    wire \t\t" + info["width"] + " \t\t" + p_name_with_netlist)
            else:
                if info["direction"] == "input":
                    file.write("    reg \t\t" + port)
                elif info["direction"] == "inout":
                    file.write("    wire \t\t" + " \t\t" + port + ";\n")
                    file.write("    reg  \t\t" + " \t\tinout_drv_" + port + ";\n")
                    file.write("    wire\t\t" + " \t\tinout_rcv_" + port)
                elif info["direction"] == "output":
                    p_name_inst     = port + "_netlist"
                    p_name_with_netlist = port + "\t,\t" + p_name_inst
                    p_name_compare.append(port)
                    p_name_netlist.append(p_name_inst)
                    wire_instances.append(".{}({})".format(port, p_name_inst))
                    out_instances.append("{} !== {}".format(port, p_name_inst))
                    file.write("    wire \t\t" + p_name_with_netlist)
        
        file.write(";\n\tinteger\t\tmismatch\t=\t0;\n\n")
        file.write(top_module + "\t" + rtl_inst + "\n\n`ifdef PNR\n")
        file.write("\t" + top_module + '_post_route route_net (.*, {} );\n'.format(', '.join(wire_instances)) + '`else\n' )
        file.write("\t" + top_module + '_post_synth synth_net (.*, {} );\n'.format(', '.join(wire_instances)) + "`endif\n\n" )
        
        if inout_ports:
            for inout in inout_ports:
                file.write("assign\t" + inout + " = inout_drv_" + inout + ";\n")
                file.write("assign\tinout_rcv_" + inout  + " = " + inout + ";\n\n")
            inout_ports = ["inout_drv_" + port for port in inout_ports]
            input_ports.extend(inout_ports)
            bit_width_list.extend(inout_wdth_lst)
        if len(clk_port) == 0:
            print("FOUND COMBINATIONAL DESIGN")
            # initialize values to zero
            if len(input_ports) > 1:
              file.write("\t\t// Initialize values to zero \ninitial\tbegin\n\t{")
              input_port_str = ', '.join(input_ports)
              input_port_str += " } <= 'd0;"
              print(input_port_str, file=file) 
            else:
              file.write("// Initialize values to zero \ninitial\tbegin\n\t" + str(input_ports[0]) + " <= 'd0;\n")
            # generate random stimulus
            file.write('\t#10;\n\tcompare();\n// Generating random stimulus \n\tfor (int i = 0; i < ' + str(iteration) + '; i = i + 1) begin\n') 
            random_stimulus_lines = []
            for port in input_ports:
                random_stimulus_lines.append(f'{port} <= $urandom();')    
            for rand_line in random_stimulus_lines:
              file.write('\t\t' + rand_line + '\n')
            file.write('\t\t#10;\n\t\tcompare();\n\tend\n\n')            
            
            # generate corner case stimulus 
            file.write("\t// ----------- Corner Case stimulus generation -----------\n")
            max_values = []
            for bit_width in bit_width_list:
                if bit_width is None:
                    bit_width = 1  # Consider None as 1 for the port
                max_value = 2**bit_width - 1
                max_values.append(max_value)
            # Create stimulus assignments for each input port
            stimulus_lines = []
            for port, max_value in zip(input_ports, max_values):
              stimulus_lines.append(f'{port} <= {max_value};')
            for line in stimulus_lines:
              file.write('\t' + line + '\n')
            file.write('\tcompare();\n\t#10;\n\tif(mismatch == 0)\n\t\t$display("**** All Comparison Matched *** \\n\t\tSimulation Passed\\n");\n\telse\n\t\t')
            file.write('$display("%0d comparison(s) mismatched\\nERROR: SIM: Simulation Failed", mismatch);\n\t#200;\n\t$finish;\nend\n\n') 
        else:
            print ("FOUND SEQUENTIAL DESIGN")
            for clk in clk_port:
                file.write('// clock initialization for ' + clk + '\n')
                file.write('initial begin\n')
                file.write('\t' + clk + " = 1'b0;\n")
                file.write('\tforever #1 ' + clk + ' = ~' + clk + ';\n')
                file.write('end\n')      
            
            # check for reset signal 
            if len(reset_port) == 0:  
                print("No Reset Signal Found")
                # initialize values to zero
                for clk in clk_port:
                    file.write("\n// Initialize values to zero \ninitial\tbegin\n\t")
                    # look for any inout signal 
                    if inout_ports:
                        for inout in inout_ports:
                            file.write("\t" + inout + " = 'bz;\n")  
                    # look for memories and initialize to 0 
                    if len(rtl_mem) > 0:
                        # Iterate over each memory in the "memories" list
                        for memory in rtl_mem:
                            memory_name = memory["name"]
                            memory_width = int(memory["width"])
                            memory_depth = int(memory["depth"])
                            # Write the module declaration to the file
                            file.write(f"\n// Initialization for {memory_name}\n")
                            # Initialize the memory with 0 values for each depth
                            file.write(f"\tfor (integer i = 0; i < {memory_depth}; i++)  begin\n")
                            file.write(f"\t\tgolden.{memory_name}[i] = 'b0;\n")
                            file.write("\tend\n\n")
                    file.write('repeat (2) @ (negedge ' + clk + ');\n')
                    if len(input_ports) > 1:
                        file.write("\t{")
                        input_port_str = ', '.join(input_ports)
                        input_port_str += " } <= 'd0;"
                        print(input_port_str, file=file) 
                        file.write("\t repeat (2) @ (negedge " + clk + "); ")
                    else:
                        if len(input_ports) == 1:
                            file.write("\n\t" + str(input_ports[0]) + 
                                    " <= 'd0;\n\t repeat (2) @ (negedge " + clk + "); ")
                        else:
                            file.write ("\n")
                    # generate random stimulus
                    file.write('\n\tcompare();\n\t//Random stimulus generation\n\trepeat(' + str(iteration) + ') @ (negedge ' + clk + ') begin\n')
                    random_stimulus_lines = []
                    for port in input_ports:
                        random_stimulus_lines.append(f'{port.ljust(20)} <= $urandom();')    
                    for rand_line in random_stimulus_lines:
                        file.write('\t\t' + rand_line + '\n')
                    file.write('\n\t\tcompare();\n\tend\n\n')            
                    
                    # generate corner case stimulus 
                    file.write("\t// ----------- Corner Case stimulus generation -----------\n")
                    max_values = []
                    for bit_width in bit_width_list:
                        if bit_width is None:
                            bit_width = 1  # Consider None as 1 for the port
                        max_value = 2**bit_width - 1
                        max_values.append(max_value)
                    # Create stimulus assignments for each input port
                    stimulus_lines = []
                    for port, max_value in zip(input_ports, max_values):
                        stimulus_lines.append(f'{port.ljust(22)} <= {max_value};')
                    for line in stimulus_lines:
                        file.write('\t' + line + '\n')
                    file.write('\trepeat (2) @ (negedge ' + clk + ');\n\tcompare();\n\tif(mismatch == 0)\n\t\t$display("**** All Comparison Matched *** \\n\t\tSimulation Passed\\n");\n\telse\n\t\t')
                    file.write('$display("%0d comparison(s) mismatched\\nERROR: SIM: Simulation Failed", mismatch);\n\t#200;\n\t$finish;\nend\n\n')  
            else:
                print("Found Reset Signal:")
                # Check sync_reset value and write stimulus generation accordingly
                file.write ("//Reset Stimulus generation\ninitial begin\n")
                # look for any inout signal 
                if inout_ports:
                    for inout in inout_ports:
                        file.write("\t" + inout + " = 'bz;\n")                
                # look for memories and initialize to 0 
                if len(rtl_mem) > 0:
                    # Iterate over each memory in the "memories" list
                    for memory in rtl_mem:
                        memory_name = memory["name"]
                        memory_width = int(memory["width"])
                        memory_depth = int(memory["depth"])
                        # Write the module declaration to the file
                        file.write(f"\n// Initialization for {memory_name}\n")
                        # Initialize the memory with 0 values for each depth
                        file.write(f"\tfor (integer i = 0; i < {memory_depth}; i++)  begin\n")
                        file.write(f"\t\tgolden.{memory_name}[i] = 'b0;\n")
                        file.write("\tend\n\n")
                for clk in clk_port:
                    for rst in reset_port:
                        if not input_ports:
                            if sync_reset_value == "active_high":
                                file.write("\t" + rst + ' <= 1;\n\t@(negedge ' + clk + ');\n' )               
                                file.write('\t' + rst + ' <= 0;\n\t@(negedge ' + clk + ');\n')
                            else:
                                file.write("\t" + rst + ' <= 0;\n\t@(negedge ' + clk + ');\n\t' )          
                                file.write('\t' + rst + ' <= 1;\n\t@(negedge ' + clk + ');\n')                        
                        else:
                            if sync_reset_value == "active_high":
                                file.write("\t" + rst + ' <= 1;\n\t@(negedge ' + clk + ');\n\t{' )
                                input_port_str = ', '.join(input_ports)
                                input_port_str += " } <= 'd0;"
                                print(input_port_str, file=file)                
                                file.write('\t' + rst + ' <= 0;\n\t@(negedge ' + clk + ');\n')
                            else:
                                file.write("\t" + rst + ' <= 0;\n\t@(negedge ' + clk + ');\n\t{' )
                                input_port_str = ', '.join(input_ports)
                                input_port_str += " } <= 'd0;"
                                print(input_port_str, file=file)                
                                file.write('\t' + rst + ' <= 1;\n\t@(negedge ' + clk + ');\n')
                file.write('\t$display ("***Reset Test is applied***");\n\t@(negedge ' + 
                              clk + ');\n\t@(negedge ' + clk + ');\n\tcompare();\n\t$display ("***Reset Test is ended***");\n')
                # Generate random stimulus values for each input port
                file.write('\t//Random stimulus generation\n\trepeat(' + str(iteration) + ') @ (negedge ' + clk + ') begin\n')
                random_stimulus_lines = []
                for port in input_ports:
                    random_stimulus_lines.append(f'{port.ljust(20)} <= $urandom();')    
                for rand_line in random_stimulus_lines:
                    file.write('\t\t' + rand_line + '\n')
                file.write('\t\tcompare();\nend\n\n')

                file.write("\t// ----------- Corner Case stimulus generation -----------\n")
                
                # Generate Corner Case stimulus for remaining INPUTS
                max_values = []
                for bit_width in bit_width_list:
                    if bit_width is None:
                        bit_width = 1  # Consider None as 1 for the port
                    max_value = 2**bit_width - 1
                    max_values.append(max_value)
                # Create stimulus assignments for each input port
                stimulus_lines = []
                for port, max_value in zip(input_ports, max_values):
                    stimulus_lines.append(f'{port.ljust(22)} <= {max_value};')
                for line in stimulus_lines:
                    file.write('\t' + line + '\n')
                file.write('\tcompare();\n\n\tif(mismatch == 0)\n\t\t$display("**** All Comparison Matched *** \\n\t\tSimulation Passed\\n");\n\telse\n\t\t')
                file.write('$display("%0d comparison(s) mismatched\\nERROR: SIM: Simulation Failed", mismatch);\n\trepeat(200) @(posedge ' + clk + ');\n\t$finish;\nend\n\n')
                      
        # compare task
        dec = len(out_instances)
        dec_string = ["%0d"] * dec
        dec_string = ", ".join(dec_string)
        file.write("task compare();\n")
        file.write('\tif ( {} ) begin\n'.format('\t||\t'.join(out_instances) ))
        file.write('\t\t$display("Data Mismatch: Actual output: ' + dec_string + ", Netlist Output " +
        dec_string + ', Time: %0t ", ')
        for ports in p_name_compare:
          file.write("%s, " % ports)
        for net_ports in p_name_netlist:
          file.write("%s, " % net_ports)
        file.write( ' $time);\n\t\tmismatch = mismatch+1;\n\tend\n\telse\n\t\t$display("Data Matched: Actual output: ' + 
        dec_string + ", Netlist Output " + dec_string + ', Time: %0t ", ')
        for ports in p_name_compare:
          file.write("%s, " % ports)
        for net_ports in p_name_netlist:
          file.write("%s, " % net_ports)
        file.write( ' $time);\nendtask\n\n')
        
        file.write('initial begin\n\t$dumpfile("tb.vcd");\n\t$dumpvars;\nend\n\nendmodule\n')

if __name__ == "__main__":
    create_folders_and_file()
