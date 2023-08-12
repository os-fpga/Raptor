# ------------------------------------------
#
# Raptor Design Suite compilation strategies
#
# ------------------------------------------

# ------------------------------------------
# # Timing driven compilation flow
# proc timing_flow { } {
#     synthesize delay 
#     packing_options -clb_packing timing_driven
#     packing
#     place
#     route
#     sta
# }

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

# Timing driven compilation flow
# This proc performs a series of steps in the design flow to achieve timing-driven placement and routing.
# It accepts an optional argument:
#   - initial_placer_type: Specifies the type of initial placer to be used.
#       - 'analytic': Use timing-driven analytic placement as initial placement.
#                     The result is then passed through the simulated annealing (SA) placer.
#       - 'default': Use VPR's default initial placer (default).
proc timing_flow { {initial_placer_type "default"} } {
    synthesize delay 
    packing_options -clb_packing timing_driven
    if {$initial_placer_type == "analytic"}
        pnr_options --enable_cascade_placer true
    packing
    place
    route
    sta
}

# ------------------------------------------
# Routability driven compilation flow. This proc accepts two optional arguments: 
#   - number_of_molecules_in_partition: Average number of molecules in each cluster (default is 200).
#   - hmetisPath: The path to the hmetis executable (default is "~/bin/hmetis").
proc routability_flow { {number_of_molecules_in_partition 200} {hmetisPath "~/bin/hmetis"} } {  
    if {![file exists $hmetisPath]} {
        puts "The 'hmetis' binary file does not exist at: $hmetisPath"
        exit 1
    }
    synthesize delay
    pnr_options --use_partitioning_in_pack on --number_of_molecules_in_partition $number_of_molecules_in_partition  
    pnr_options --hmetis_path $hmetisPath --target_ext_pin_util clb:0.6 dsp:1.0,1.0 bram:1.0,1.0 0.8
    packing
    place
    route
    sta
}
