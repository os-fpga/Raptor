set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/std_cells/tsmc/7.5t/tcbn16ffcllbwp7d5t16p96cpd_100i/tcbn16ffcllbwp7d5t16p96cpd_100d_ccs/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn16ffcllbwp7d5t16p96cpd_100d ]

set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/memory/dti/memories/rev_111022/101122_tsmc16ffc_DP_RAPIDSILICON_GEMINI_rev1p0p3_BE/dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc ]

set link_library   [concat * \
   dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc_ssgn40c.db \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
]

# slow/slow, -40C (temperature inversion WC)
set target_library   [concat [list] \
   dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc_ssgn40c.db \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
]

set_dont_use [get_lib_cell */*D0*]
