# ------------------------------------------
#
# Raptor Design Suite compilation strategies
#
# ------------------------------------------

# ------------------------------------------
# Timing driven compilation flow
proc timing_flow { } {
    synthesize delay 
    packing_options -clb_packing timing_driven
    packing
    place
    route
    sta
}

# ------------------------------------------
# Area driven compilation flow
proc area_flow { } {
    synthesize area
    packing
    place
    route
    sta
}

# ------------------------------------------
# Congestion driven compilation flow
# Two options: "uniform" for uniformely congested high utilization designs 
#              "hotspot" for hotspots of congestion in moderately utilized designs
proc congestion_flow { {congestion_type "uniform"} } {
    synthesize delay
    packing_options -clb_packing timing_driven
    if {$congestion_type == "uniform"} {
        # Uniform congestion mitigation
        pnr_options --target_ext_pin_util clb:0.4 dsp:1.0,1.0 bram:1.0,1.0 0.8
    } elseif {$congestion_type == "hotspot"} {
        # Hotspot congestion mitigation
        pnr_options --place_algorithm congestion_aware --congest_tradeoff 160 --alpha_clustering 0.5
    } else {
        error "ERROR: $congestion_type is not a valid congestion mitigation option, valid options: uniform, hotspot"
    }
    packing
    place
    route
    sta
}

# ------------------------------------------
