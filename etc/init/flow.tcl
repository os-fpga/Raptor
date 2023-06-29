proc timing_flow { } {
  analyze
  synthesize
  packing_options -clb_packing timing_driven
  packing
  place
  route
  sta
  bitstream
}

proc area_flow { } {
  analyze
  synthesize area
  packing
  place
  route
  sta
  bitstream
}
