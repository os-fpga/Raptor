#
# This file is part of RapidSilicon's IP_Catalog.
#
# This file is Copyright (c) 2022 RapidSilicon.
# SPDX-License-Identifier: TBD.

# LiteX wrapper around Alex Forencich Verilog-AXI's axi_ram.v.

import os
import logging

from migen import *

from litex.soc.interconnect.axi import *

logging.basicConfig(level=logging.INFO)

# Helpers ------------------------------------------------------------------------------------------

class Open(Signal): pass

def colorer(s, color="bright"):
    header  = {
        "bright": "\x1b[1m",
        "green":  "\x1b[32m",
        "cyan":   "\x1b[36m",
        "red":    "\x1b[31m",
        "yellow": "\x1b[33m",
        "underline": "\x1b[4m"}[color]
    trailer = "\x1b[0m"
    return header + str(s) + trailer

# AXI RAM ------------------------------------------------------------------------------------------

class AXIRAM(Module):
    def __init__(self, platform, s_axi, size=1024, pipeline_output=False):

        # Get/Check Parameters.
        # ---------------------
        self.logger = logging.getLogger("AXI_RAM")

        self.logger.propagate = False

        # Clock Domain.
        clock_domain = s_axi.clock_domain
        self.logger.info(f"Clock Domain     : {colorer(clock_domain)}")

        # Address width.
        address_width = len(s_axi.aw.addr)
        self.logger.info(f"Address Width    : {colorer(address_width)}")

        # Data width.
        data_width = len(s_axi.w.data)
        self.logger.info(f"Data Width       : {colorer(data_width)}")

        # Size.
        self.logger.info(f"Size             : {colorer(size)}bytes")

        # ID width.
        id_width = len(s_axi.aw.id)
        self.logger.info(f"ID Width         : {colorer(id_width)}")
        
        # Pipeline Output
        self.logger.info(f"PIPELINE_OUTPUT  : {colorer(pipeline_output)}")

        # Module instance.
        # ----------------

        self.specials += Instance("axi_ram",
            # Parameters.
            # -----------
            p_DATA_WIDTH      = data_width,
            p_ADDR_WIDTH      = address_width,
            p_ID_WIDTH        = id_width,
            p_PIPELINE_OUTPUT = pipeline_output,

            # Clk / Rst.
            # ----------
            i_clk = ClockSignal(clock_domain),
            i_rst = ResetSignal(clock_domain),

            # AXI Slave Interface.
            # --------------------
            # AW.
            i_s_axi_awid     = s_axi.aw.id,
            i_s_axi_awaddr   = s_axi.aw.addr,
            i_s_axi_awlen    = s_axi.aw.len,
            i_s_axi_awsize   = s_axi.aw.size,
            i_s_axi_awburst  = s_axi.aw.burst,
            i_s_axi_awlock   = s_axi.aw.lock,
            i_s_axi_awcache  = s_axi.aw.cache,
            i_s_axi_awprot   = s_axi.aw.prot,
            i_s_axi_awvalid  = s_axi.aw.valid,
            o_s_axi_awready  = s_axi.aw.ready,

            # W.
            i_s_axi_wdata    = s_axi.w.data,
            i_s_axi_wstrb    = s_axi.w.strb,
            i_s_axi_wlast    = s_axi.w.last,
            i_s_axi_wvalid   = s_axi.w.valid,
            o_s_axi_wready   = s_axi.w.ready,

            # B.
            o_s_axi_bid      = s_axi.b.id,
            o_s_axi_bresp    = s_axi.b.resp,
            o_s_axi_bvalid   = s_axi.b.valid,
            i_s_axi_bready   = s_axi.b.ready,

            # AR.
            i_s_axi_arid     = s_axi.ar.id,
            i_s_axi_araddr   = s_axi.ar.addr,
            i_s_axi_arlen    = s_axi.ar.len,
            i_s_axi_arsize   = s_axi.ar.size,
            i_s_axi_arburst  = s_axi.ar.burst,
            i_s_axi_arlock   = s_axi.ar.lock,
            i_s_axi_arcache  = s_axi.ar.cache,
            i_s_axi_arprot   = s_axi.ar.prot,
            i_s_axi_arvalid  = s_axi.ar.valid,
            o_s_axi_arready  = s_axi.ar.ready,

            # R.
            o_s_axi_rid      = s_axi.r.id,
            o_s_axi_rdata    = s_axi.r.data,
            o_s_axi_rresp    = s_axi.r.resp,
            o_s_axi_rlast    = s_axi.r.last,
            o_s_axi_rvalid   = s_axi.r.valid,
            i_s_axi_rready   = s_axi.r.ready,
        )

        # Add Sources.
        # ------------
        self.add_sources(platform)

    @staticmethod
    def add_sources(platform):
        rtl_dir = os.path.join(os.path.dirname(__file__), "../src")
        platform.add_source(os.path.join(rtl_dir, "axi_ram.v"))
