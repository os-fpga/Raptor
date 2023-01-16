set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/std_cells/tsmc/7.5t/tcbn16ffcllbwp7d5t16p96cpd_100i/tcbn16ffcllbwp7d5t16p96cpd_100d_ccs/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn16ffcllbwp7d5t16p96cpd_100d ]
set search_path [concat $search_path /cadlib/gemini/TSMC16NMFFC/library/std_cells/tsmc/7.5t/tcbn16ffcllbwp7d5t16p96cpdlvt_100i/tcbn16ffcllbwp7d5t16p96cpdlvt_100d_ccs/TSMCHOME/digital/Front_End/timing_power_noise/CCS/tcbn16ffcllbwp7d5t16p96cpdlvt_100d ]

set link_library   [concat * \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
   tcbn16ffcllbwp7d5t16p96cpdlvtssgnp0p72vm40c_ccs.db \
]

# slow/slow, -40C (temperature inversion WC)
set target_library   [concat [list] \
   tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs.db \
   tcbn16ffcllbwp7d5t16p96cpdlvtssgnp0p72vm40c_ccs.db \
]

set_dont_use [get_lib_cell */*D0*]
