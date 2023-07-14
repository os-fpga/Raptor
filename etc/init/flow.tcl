proc timing_flow { } {
    synthesize delay 
    packing_options -clb_packing timing_driven
    packing
    place
    route
    sta
}

proc area_flow { } {
    synthesize area
    packing
    place
    route
    sta
}

proc congestion_flow { {uniform_congestion "1"} } {
    synthesize delay
    packing_options -clb_packing timing_driven
    if {uniform_congestion} {
        # Uniform congestion
        pnr_options --target_ext_pin_util clb:0.4 dsp:1.0,1.0 bram:1.0,1.0 0.8
    } else {
        # Hotspot congestion
        pnr_options --place_algorithm congestion_aware --congest_tradeoff 160 --alpha_clustering 0.5
    }
    packing
    place
    route
    sta
}
