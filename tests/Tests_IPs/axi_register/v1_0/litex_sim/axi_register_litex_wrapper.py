
# This file is part of RapidSilicon's IP_Catalog.
#
# This file is Copyright (c) 2022 RapidSilicon.
# SPDX-License-Identifier: TBD.

# LiteX wrapper around RapidSilicon axi_register.v

import os
import logging

from migen import *

from litex.soc.interconnect.axi import *

logging.basicConfig(level=logging.INFO)

# AXI DP RAM ---------------------------------------------------------------------------------------
class AXIREGISTER(Module):
    def __init__(self, platform, s_axi, m_axi, size=1024,
        aw_reg_type         = 1,
        w_reg_type          = 2,
        b_reg_type          = 1,
        ar_reg_type         = 1,
        r_reg_type          = 2
    ):

        # Get Parameters.
        # ---------------------
        self.logger = logging.getLogger("AXI_REGISTER")

        self.logger.propagate = False

        
        # Clock Domain.
        self.logger.info(f"Clock Domain   : {s_axi.clock_domain}")

        # Address width.
        address_width = len(s_axi.aw.addr)
        self.logger.info(f"Address Width  : {address_width}")

        # Data width.
        data_width = len(s_axi.w.data)
        self.logger.info(f"Data Width     : {data_width}")
        
        # Size.
        self.logger.info(f"Size           : {size}bytes")

        # ID width.
        id_width = len(s_axi.aw.id)
        self.logger.info(f"ID Width       : {id_width}")

        # AXI Channels User Width
        aw_user_width = len(s_axi.aw.user)
        self.logger.info(f"AWUSER_WIDTH   : {aw_user_width}")
        
        w_user_width = len(s_axi.w.user)
        self.logger.info(f"WUSER_WIDTH    : {w_user_width}")
        
        b_user_width = len(s_axi.b.user)
        self.logger.info(f"BUSER_WIDTH    : {b_user_width}")
        
        ar_user_width = len(s_axi.ar.user)
        self.logger.info(f"ARUSER_WIDTH   : {ar_user_width}")
        
        r_user_width = len(s_axi.r.user)
        self.logger.info(f"RUSER_WIDTH    : {r_user_width}")
        
        # Channel Reg Type
        self.logger.info(f"AW_REG_TYPE    : {aw_reg_type}")
        self.logger.info(f"W_REG_TYPE     : {w_reg_type}")
        self.logger.info(f"B_REG_TYPE     : {b_reg_type}")
        self.logger.info(f"AR_REG_TYPE    : {ar_reg_type}")
        self.logger.info(f"R_REG_TYPE     : {r_reg_type}")

        # Module instance.
        # ----------------
        self.specials += Instance("axi_register",
            # Parameters.
            # -----------
            # Global.
            p_DATA_WIDTH        = data_width,
            p_ADDR_WIDTH        = address_width,
            p_ID_WIDTH          = id_width,

            # AXI Channels User Width
            p_AWUSER_WIDTH      = aw_user_width,
            p_WUSER_WIDTH       = w_user_width,
            p_BUSER_WIDTH       = b_user_width,
            p_ARUSER_WIDTH      = ar_user_width,
            p_RUSER_WIDTH       = r_user_width,
            
            # Channel Reg Type
            p_AW_REG_TYPE      = aw_reg_type,
            p_W_REG_TYPE       = w_reg_type,
            p_B_REG_TYPE       = b_reg_type,
            p_AR_REG_TYPE      = ar_reg_type,
            p_R_REG_TYPE       = r_reg_type,

            # Clk / Rst.
            # ----------
            i_clk            = ClockSignal(),
            i_rst            = ResetSignal(),

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
            i_s_axi_awqos    = s_axi.aw.qos,
            i_s_axi_awregion = s_axi.aw.region,
            i_s_axi_awuser   = s_axi.aw.user,
            i_s_axi_awvalid  = s_axi.aw.valid,
            o_s_axi_awready  = s_axi.aw.ready,

            # W.
            i_s_axi_wdata    = s_axi.w.data,
            i_s_axi_wstrb    = s_axi.w.strb,
            i_s_axi_wlast    = s_axi.w.last,
            i_s_axi_wuser    = s_axi.w.user,
            i_s_axi_wvalid   = s_axi.w.valid,
            o_s_axi_wready   = s_axi.w.ready,

            # B.
            o_s_axi_bid      = s_axi.b.id,
            o_s_axi_bresp    = s_axi.b.resp,
            o_s_axi_buser    = s_axi.b.user,
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
            i_s_axi_arqos    = s_axi.ar.qos,
            i_s_axi_arregion = s_axi.ar.region,
            i_s_axi_aruser   = s_axi.ar.user,
            i_s_axi_arvalid  = s_axi.ar.valid,
            o_s_axi_arready  = s_axi.ar.ready,

            # R.
            o_s_axi_rid      = s_axi.r.id,
            o_s_axi_rdata    = s_axi.r.data,
            o_s_axi_rresp    = s_axi.r.resp,
            o_s_axi_rlast    = s_axi.r.last,
            o_s_axi_ruser    = s_axi.r.user,
            o_s_axi_rvalid   = s_axi.r.valid,
            i_s_axi_rready   = s_axi.r.ready,
            
            
            # AXI Master Interface.
            # --------------------
            # AW.
            o_m_axi_awid     = m_axi.aw.id,
            o_m_axi_awaddr   = m_axi.aw.addr,
            o_m_axi_awlen    = m_axi.aw.len,
            o_m_axi_awsize   = m_axi.aw.size,
            o_m_axi_awburst  = m_axi.aw.burst,
            o_m_axi_awlock   = m_axi.aw.lock,
            o_m_axi_awcache  = m_axi.aw.cache,
            o_m_axi_awprot   = m_axi.aw.prot,
            o_m_axi_awqos    = m_axi.aw.qos,
            o_m_axi_awregion = m_axi.aw.region,
            o_m_axi_awuser   = m_axi.aw.user,
            o_m_axi_awvalid  = m_axi.aw.valid,
            i_m_axi_awready  = m_axi.aw.ready,

            # W.
            o_m_axi_wdata    = m_axi.w.data,
            o_m_axi_wstrb    = m_axi.w.strb,
            o_m_axi_wlast    = m_axi.w.last,
            o_m_axi_wuser    = m_axi.w.user,
            o_m_axi_wvalid   = m_axi.w.valid,
            i_m_axi_wready   = m_axi.w.ready,

            # B.
            i_m_axi_bid      = m_axi.b.id,
            i_m_axi_bresp    = m_axi.b.resp,
            i_m_axi_buser    = m_axi.b.user,
            i_m_axi_bvalid   = m_axi.b.valid,
            o_m_axi_bready   = m_axi.b.ready,

            # AR.
            o_m_axi_arid     = m_axi.ar.id,
            o_m_axi_araddr   = m_axi.ar.addr,
            o_m_axi_arlen    = m_axi.ar.len,
            o_m_axi_arsize   = m_axi.ar.size,
            o_m_axi_arburst  = m_axi.ar.burst,
            o_m_axi_arlock   = m_axi.ar.lock,
            o_m_axi_arcache  = m_axi.ar.cache,
            o_m_axi_arprot   = m_axi.ar.prot,
            o_m_axi_arqos    = m_axi.ar.qos,
            o_m_axi_arregion = m_axi.ar.region,
            o_m_axi_aruser   = m_axi.ar.user,
            o_m_axi_arvalid  = m_axi.ar.valid,
            i_m_axi_arready  = m_axi.ar.ready,

            # R.
            i_m_axi_rid      = m_axi.r.id,
            i_m_axi_rdata    = m_axi.r.data,
            i_m_axi_rresp    = m_axi.r.resp,
            i_m_axi_rlast    = m_axi.r.last,
            i_m_axi_ruser    = m_axi.r.user,
            i_m_axi_rvalid   = m_axi.r.valid,
            o_m_axi_rready   = m_axi.r.ready,

        )

        # Add Sources.
        # ------------
        self.add_sources(platform)

    @staticmethod
    def add_sources(platform):
        rtl_dir = os.path.join(os.path.dirname(__file__), "../src")
        platform.add_source(os.path.join(rtl_dir, "axi_register.v"))
