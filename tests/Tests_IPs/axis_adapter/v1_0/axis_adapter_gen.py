#!/usr/bin/env python3

# This file is Copyright (c) 2022 RapidSilicon.
# SPDX-License-Identifier: TBD.

import os
import json
import argparse
import shutil
import logging

from litex_sim.axis_adapter_litex_wrapper import AXISADAPTER

from migen import *

from litex.build.generic_platform import *

from litex.build.osfpga import OSFPGAPlatform

from litex.soc.interconnect.axi import AXIStreamInterface

# IOs/Interfaces -----------------------------------------------------------------------------------
def get_clkin_ios():
    return [
        ("clk",  0, Pins(1)),
        ("rst",  0, Pins(1)),
    ]

# AXIS_ADAPTER Wrapper ----------------------------------------------------------------------------------
class AXISADAPTERWrapper(Module):
    def __init__(self, platform, s_data_width, m_data_width, id_en, id_width, 
                dest_en, dest_width, user_en, user_width):
        
        # Clocking ---------------------------------------------------------------------------------
        platform.add_extension(get_clkin_ios())
        self.clock_domains.cd_sys  = ClockDomain()
        self.comb += self.cd_sys.clk.eq(platform.request("clk"))
        self.comb += self.cd_sys.rst.eq(platform.request("rst"))
        
        # AXI STREAM SLAVE -------------------------------------------------------------------------------
        s_axis = AXIStreamInterface(
            data_width = s_data_width,
            user_width = user_width,
            dest_width = dest_width,
            id_width   = id_width
        )
        
        # AXI STREAM MASTER -------------------------------------------------------------------------------
        m_axis = AXIStreamInterface(
            data_width = m_data_width,
            user_width = user_width,
            dest_width = dest_width,
            id_width   = id_width
        )
        # Input AXI
        platform.add_extension(s_axis.get_ios("s_axis"))
        self.comb += s_axis.connect_to_pads(platform.request("s_axis"), mode="slave")
        
        # Output AXI
        platform.add_extension(m_axis.get_ios("m_axis"))
        self.comb += m_axis.connect_to_pads(platform.request("m_axis"), mode="master")
        
        # AXIS-ADAPTER ----------------------------------------------------------------------------------
        self.submodules += AXISADAPTER(platform,
            m_axis          = m_axis,
            s_axis          = s_axis,
            id_en           = id_en,
            dest_en         = dest_en,
            user_en         = user_en
            )
        
# Build --------------------------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description="AXIS ADAPTER CORE")
    parser.formatter_class = lambda prog: argparse.ArgumentDefaultsHelpFormatter(prog,
        max_help_position = 10,
        width             = 120
    )

    # Core Parameters.
    core_group = parser.add_argument_group(title="Core parameters")
    core_group.add_argument("--s_data_width",   default=8,        type=int,         help="Slave Data Width from 1 to 4096")
    core_group.add_argument("--m_data_width",   default=8,        type=int,         help="Master Data Width from 1 to 4096")
    core_group.add_argument("--id_en",          default=0,        type=int,         help="ID Enable 0 or 1")
    core_group.add_argument("--id_width",       default=8,        type=int,         help="ID Width from 1 to 32")
    core_group.add_argument("--dest_en",        default=0,        type=int,         help="Destination Enable 0 or 1")
    core_group.add_argument("--dest_width",     default=8,        type=int,         help="Destination Width from 1 to 32")
    core_group.add_argument("--user_en",        default=1,        type=int,         help="User Enable 0 or 1")
    core_group.add_argument("--user_width",     default=1,        type=int,         help="User Width from 1 to 4096")

    # Build Parameters.
    build_group = parser.add_argument_group(title="Build parameters")
    build_group.add_argument("--build",         action="store_true",                help="Build Core")
    build_group.add_argument("--build-dir",     default="./",                       help="Build Directory")
    build_group.add_argument("--build-name",    default="axis_adapter_wrapper",     help="Build Folder Name, Build RTL File Name and Module Name")

    # JSON Import/Template
    json_group = parser.add_argument_group(title="JSON Parameters")
    json_group.add_argument("--json",                                           help="Generate Core from JSON File")
    json_group.add_argument("--json-template",  action="store_true",            help="Generate JSON Template")

    args = parser.parse_args()

    # Parameter Check -------------------------------------------------------------------------------
    logger = logging.getLogger("Invalid Parameter Value")

    # Data_Width
    data_width_range=range(1,4097)
    if args.s_data_width not in data_width_range:
        logger.error("\nEnter a valid 's_data_width' from 1 to 4096")
        exit()
    if args.m_data_width not in data_width_range:
        logger.error("\nEnter a valid 'm_data_width' from 1 to 4096")
        exit()

    # ID Enable
    id_en_range=range(2)
    if args.id_en not in id_en_range:
        logger.error("\nEnter a valid 'id_en' 0 or 1")
        exit()

    # ID Width
    id_width_range=range(1,33)
    if args.id_width not in id_width_range:
        logger.error("\nEnter a valid 'id_width' from 1 to 32")
        exit()

    # Destination Enable
    dest_en_range=range(2)
    if args.dest_en not in dest_en_range:
        logger.error("\nEnter a valid 'dest_en' 0 or 1")
        exit()
        
    # Destination Width
    dest_width_range=range(1,33)
    if args.dest_width not in dest_width_range:
        logger.error("\nEnter a valid 'dest_width' from 1 to 32")
        exit()
        
    # User Enable
    user_en_range=range(2)
    if args.user_en not in user_en_range:
        logger.error("\nEnter a valid 'user_en' 0 or 1")
        exit()
        
    # User Width
    user_width_range=range(1,4097)
    if args.user_width not in user_width_range:
        logger.error("\nEnter a valid 'user_width' from 1 to 4096")
        exit()


    # Import JSON (Optional) -----------------------------------------------------------------------
    if args.json:
        with open(args.json, 'rt') as f:
            t_args = argparse.Namespace()
            t_args.__dict__.update(json.load(f))
            args = parser.parse_args(namespace=t_args)

    # Export JSON Template (Optional) --------------------------------------------------------------
    if args.json_template:
        print(json.dumps(vars(args), indent=4))

    # Remove build extension when specified.
    args.build_name = os.path.splitext(args.build_name)[0]

    # Build Project Directory ----------------------------------------------------------------------
    if args.build:
        # Build Path
        build_path = os.path.join(args.build_dir, 'rapidsilicon/ip/axis_adapter/v1_0/' + (args.build_name))
        gen_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "axis_adapter_gen.py"))
        if not os.path.exists(build_path):
            os.makedirs(build_path)
            shutil.copy(gen_path, build_path)

        # Litex_sim Path
        litex_sim_path = os.path.join(build_path, "litex_sim")
        if not os.path.exists(litex_sim_path):    
            os.makedirs(litex_sim_path)

        # Simulation Path
        sim_path = os.path.join(build_path, "sim")
        if not os.path.exists(sim_path):    
            os.makedirs(sim_path)

        # Source Path
        src_path = os.path.join(build_path, "src")
        if not os.path.exists(src_path):    
            os.makedirs(src_path) 

        # Synthesis Path
        synth_path = os.path.join(build_path, "synth")
        if not os.path.exists(synth_path):    
            os.makedirs(synth_path) 
        # Design Path
        design_path = os.path.join("../src", (args.build_name + ".v")) 

        # Copy RTL from Source to Destination 
        rtl_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "src")       
        rtl_files = os.listdir(rtl_path)
        for file_name in rtl_files:
            full_file_path = os.path.join(rtl_path, file_name)
            if os.path.isfile(full_file_path):
                shutil.copy(full_file_path, src_path)

        # Copy litex_sim Data from Source to Destination
        litex_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "litex_sim")        
        litex_files = os.listdir(litex_path)
        for file_name in litex_files:
            full_file_path = os.path.join(litex_path, file_name)
            if os.path.isfile(full_file_path):
                shutil.copy(full_file_path, litex_sim_path)
                

        # TCL File Content        
        tcl = []
        # Create Design.
        tcl.append(f"create_design {args.build_name}")
        # Set Device.
        tcl.append(f"target_device {'GEMINI'}")
        # Add Include Path.
        tcl.append(f"add_library_path {'../src'}")
        # Add Sources.
#        for f, typ, lib in file_name:
        tcl.append(f"add_design_file {design_path}")
        # Set Top Module.
        tcl.append(f"set_top_module {args.build_name}")
        # Add Timings Constraints.
#        tcl.append(f"add_constraint_file {args.build_name}.sdc")
        # Run.
        tcl.append("synthesize")


        # Generate .tcl file
        tcl_path = os.path.join(synth_path, "raptor.tcl")
        with open(tcl_path, "w") as f:
            f.write("\n".join(tcl))
        f.close()
        
    # Create LiteX Core ----------------------------------------------------------------------------
    platform   = OSFPGAPlatform( io=[], device="gemini", toolchain="raptor")
    module     = AXISADAPTERWrapper(platform,
        s_data_width    = args.s_data_width,
        m_data_width    = args.m_data_width,
        id_en           = args.id_en,
        id_width        = args.id_width,
        dest_en         = args.dest_en,
        dest_width      = args.dest_width,
        user_en         = args.user_en,
        user_width      = args.user_width
    )

    # Build
    if args.build:
        platform.build(module,
            build_dir    = "litex_build",
            build_name   = args.build_name,
            run          = False,
            regular_comb = False
        )
        shutil.copy(f"litex_build/{args.build_name}.v", src_path)
        shutil.rmtree("litex_build")
        
        # TimeScale Addition to Wrapper
        wrapper = os.path.join(src_path, f'{args.build_name}.v')
        f = open(wrapper, "r")
        content = f.readlines()
        content.insert(14, '`timescale 1ns / 1ps\n')
        f = open(wrapper, "w")
        content = "".join(content)
        f.write(str(content))
        f.close()

if __name__ == "__main__":
    main()
