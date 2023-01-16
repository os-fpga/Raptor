set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/std_cells/tsmc/7.5t/tcbn16ffcllbwp7d5t16p96cpdlvt_100i/tcbn16ffcllbwp7d5t16p96cpdlvt_100d_ccs/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn16ffcllbwp7d5t16p96cpdlvt_100d ]
set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/std_cells/tsmc/7.5t/tcbn16ffcllbwp7d5t16p96cpd_100i/tcbn16ffcllbwp7d5t16p96cpd_100d_ccs/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn16ffcllbwp7d5t16p96cpd_100d ]
set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/memory/dti/memories/rev_111022/101122_tsmc16ffc_DP_RAPIDSILICON_GEMINI_rev1p0p3_BE/dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc ]

set link_library   [concat * \
   tcbn16ffcllbwp7d5t16p96cpdlvtssgnp0p72vm40c_ccs.db \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
   dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc_ssgnp125c.db \
   dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc_typ.db \
]
#set target_library [concat [list]  \
   tcbn16ffcllbwp7d5t16p96cpdlvtssgnp0p72vm40c_ccs.db \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
]
set symbol_library 	""

read_file /projects/castor/dparre/Ulka_PD/synthesis/Release/pass_20220927/RS_BRAM/ddc/RS_BRAM_dc.ddc
read_file /projects/castor/dparre/Ulka_PD/synthesis_no_timestamps/BRAM_tsmc7p5t/technology/tsmc16/ddc/TDP36K_top_dc.ddc
