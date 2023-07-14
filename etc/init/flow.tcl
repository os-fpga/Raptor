#
# Raptor Design Suite compilation strategies
#

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

proc congestion_flow { {uniform_congestion "uniform"} } {
    synthesize delay
    packing_options -clb_packing timing_driven
    if {$uniform_congestion == "uniform"} {
        # Uniform congestion mitigation
        pnr_options --target_ext_pin_util clb:0.4 dsp:1.0,1.0 bram:1.0,1.0 0.8
    } elseif {$uniform_congestion == "hotspot"} { {
        # Hotspot congestion mitigation
        pnr_options --place_algorithm congestion_aware --congest_tradeoff 160 --alpha_clustering 0.5
    } else {
        error "ERROR: Not a valid congestion mitigation option"
    }
    packing
    place
    route
    sta
}
