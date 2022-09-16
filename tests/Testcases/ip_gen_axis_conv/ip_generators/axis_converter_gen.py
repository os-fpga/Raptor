#!/usr/bin/env python3

import os
import json
import argparse

from migen import *

from litex.build.generic_platform import *
from litex.build.osfpga import OSFPGAPlatform

from litex.soc.interconnect import stream
from litex.soc.interconnect.axi import *

# IOs/Interfaces -----------------------------------------------------------------------------------

def get_clkin_ios():
    return [
        ("axis_clk",  0, Pins(1)),
        ("axis_rst",  0, Pins(1)),
    ]

# AXI Stream Converter -----------------------------------------------------------------------------

class AXISConverter(Module):
    def __init__(self, platform, in_width=64, out_width=64, user_width=0, reverse=False):
        # Clocking ---------------------------------------------------------------------------------
        platform.add_extension(get_clkin_ios())
        self.clock_domains.cd_sys  = ClockDomain()
        self.comb += self.cd_sys.clk.eq(platform.request("axis_clk"))
        self.comb += self.cd_sys.rst.eq(platform.request("axis_rst"))

        # Input AXI --------------------------------------------------------------------------------
        axis_in = AXIStreamInterface(data_width=in_width, user_width=user_width)
        platform.add_extension(axis_in.get_ios("axis_in"))
        self.comb += axis_in.connect_to_pads(platform.request("axis_in"), mode="slave")

        # Output AXI -------------------------------------------------------------------------------
        axis_out = AXIStreamInterface(data_width=out_width, user_width=user_width)
        platform.add_extension(axis_out.get_ios("axis_out"))
        self.comb += axis_out.connect_to_pads(platform.request("axis_out"), mode="master")

        # Converter --------------------------------------------------------------------------------
        converter = stream.StrideConverter(axis_in.description, axis_out.description, reverse=reverse)
        self.submodules += converter
        self.comb += axis_in.connect(converter.sink)
        self.comb += converter.source.connect(axis_out)

# Build --------------------------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="AXI Stream Converter core.")
    parser.formatter_class = lambda prog: argparse.ArgumentDefaultsHelpFormatter(prog,
        max_help_position = 10,
        width             = 120
    )
    # Core Parameters.
    core_group = parser.add_argument_group(title="Core parameters")
    core_group.add_argument("--core-in-width",      default=128,         help="AXI-ST Input Data-width.")
    core_group.add_argument("--core-out-width",     default=64,          help="AXI-ST Output Data-width.")
    core_group.add_argument("--core-user-width",    default=0,           help="AXI-ST User width.")
    core_group.add_argument("--core-reverse",       action="store_true", help="Reverse Converter Ordering.")

    # Build Parameters.
    build_group = parser.add_argument_group(title="Build parameters")
    build_group.add_argument("--build",         action="store_true", help="Build core.")
    build_group.add_argument("--build-dir",     default="build",     help="Build directory.")
    build_group.add_argument("--build-name",    default=None,        help="Build name.")

    # JSON Import/Template
    json_group = parser.add_argument_group(title="JSON parameters")
    json_group.add_argument("--json",          help="Generate core from JSON file.")
    json_group.add_argument("--json-template", action="store_true", help="Generate JSON template.")

    args = parser.parse_args()

    # Import JSON (Optional) -----------------------------------------------------------------------
    if args.json:
        with open(args.json, 'rt') as f:
            t_args = argparse.Namespace()
            t_args.__dict__.update(json.load(f))
            args = parser.parse_args(namespace=t_args)

    # Create LiteX Core ----------------------------------------------------------------------------
    core_in_width   = int(args.core_in_width)
    core_out_width  = int(args.core_out_width)
    core_user_width = int(args.core_user_width)
    platform   = OSFPGAPlatform("", io=[], toolchain="raptor")
    module     = AXISConverter(platform,
        in_width   = core_in_width,
        out_width  = core_out_width,
        user_width = core_user_width,
    )

    # Generate Verilog Core ------------------------------------------------------------------------
    # Enforce build name when not specified.
    if args.build_name is None:
        args.build_name = "axis_converter_{}b_to_{}b".format(core_in_width, core_out_width)
    # Remove build extension when specified.
    args.build_name = os.path.splitext(args.build_name)[0]

    # Build
    if args.build:
        platform.build(module,
            build_dir    = args.build_dir,
            build_name   = args.build_name,
            run          = False,
            regular_comb = False
        )

    # Export JSON Template (Optional) --------------------------------------------------------------
    if args.json_template:
        print(json.dumps(vars(args), indent=4))

if __name__ == "__main__":
    main()
