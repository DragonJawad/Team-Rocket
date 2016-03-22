open_project -project {C:\Users\Ben\Desktop\xBeeTransmit\designer\impl1\xBeeTransmit_fp\xBeeTransmit.pro}
set_programming_file -no_file
set_device_type -type {A2F200M3F}
set_device_package -package {484 FBGA}
update_programming_file \
    -feature {prog_fpga:on} \
    -fdb_source {fdb} \
    -fdb_file {C:\Users\Ben\Desktop\xBeeTransmit\designer\impl1\xBeeTransmit.fdb} \
    -feature {prog_from:off} \
    -feature {prog_nvm:on} \
    -efm_content {location:0;source:efc} \
    -efm_block {location:0;config_file:{C:\Users\Ben\Desktop\xBeeTransmit\component\work\xBeeTransmit_MSS\MSS_ENVM_0\MSS_ENVM_0.efc}} \
    -pdb_file {C:\Users\Ben\Desktop\xBeeTransmit\designer\impl1\xBeeTransmit_fp\xBeeTransmit.pdb}
set_programming_action -action {PROGRAM}
catch {run_selected_actions} return_val
save_project
close_project
if { $return_val != 0 } {
  exit 1 }
