# Floorplan sgemm to lower SLR
current_instance -quiet
add_cells_to_pblock [get_pblocks pblock_lower] [get_cells -quiet [list xcl_design_i/expanded_region/u_ocl_region/dr_i/sqlpageloop0/inst]]
