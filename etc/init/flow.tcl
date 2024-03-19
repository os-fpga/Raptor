# ------------------------------------------
#
# Raptor Design Suite compilation strategies
#
# ------------------------------------------


# ------------------------------------------
# Area driven compilation flow
proc area_flow { } {
    analyze
    synthesize area
    packing
    place
    route
    sta
    power
}

# ------------------------------------------
# Congestion driven compilation flow
# Two options: "uniform" for uniformely congested high utilization designs 
#              "hotspot" for hotspots of congestion in moderately utilized designs
proc congestion_flow { {congestion_type "uniform"} } {
    analyze
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
    power
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
    analyze
    synthesize delay 
    packing_options -clb_packing timing_driven
    if {$initial_placer_type == "analytic"} {
        pnr_options --enable_cascade_placer true
    }
    packing
    place
    route
    sta
    power
}


# Routability driven compilation flow. This proc accepts three optional arguments: 
#   - number_of_molecules_in_partition: Average number of molecules in each cluster (default is 200).
#   - congestion: medium, high
#   - initial VPR placement constraints file
proc routability_flow { {number_of_molecules_in_partition 200} {congestion "medium"} {vpr_constraints_file ""} }  {
    analyze
    synthesize delay
    set options ""
    append options "--use_partitioning_in_pack on --number_of_molecules_in_partition $number_of_molecules_in_partition "
    if [file exists $vpr_constraints_file] {
        append options "--read_vpr_constraints $vpr_constraints_file "
    }
    if {$congestion == "high"} { 
        append options "--target_ext_pin_util clb:0.6 dsp:1.0,1.0 bram:1.0,1.0 0.8 "
    }
    pnr_options $options
    packing
    place
    route
    sta
    power
}
