#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           5.0
set_attr message_detail         3 
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all 
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
### 1. You may add your own options for C++ compilation here.
set_attr cc_options             "-DCLOCK_PERIOD=5.0 -g"
#enable_waveform_logging -vcd
set_attr end_of_sim_command "make saySimPassed"

#
# Testbench or System Level Modules
#
### 2. Add your testbench source files here.
define_system_module ../main.cpp
define_system_module ../Testbench.cpp
define_system_module ../System.cpp

#
# SC_MODULEs to be synthesized
#
### 3. Add your design source files here (to be high-level synthesized).
define_hls_module HeapSorter ../HeapSorter.cpp

### 4. Define your HLS configuration (arbitrary names, BASIC and DPA in this example). 
define_hls_config HeapSorter BASIC
define_hls_config HeapSorter DPA       --dpopt_auto=op,expr
define_hls_config HeapSorter DPAUA       --dpopt_auto=op,expr  -DUNROLL_ALL
define_hls_config HeapSorter UNROLL       -DUNROLL_ALL
# Add configurations for various intitation intervals
define_hls_config HeapSorter PIPE -DII=5

set IMAGE_DIR           ".."
set IN_FILE_NAME        "${IMAGE_DIR}/data.txt"
set OUT_FILE_NAME				"output.txt"

### 5. Define simulation configuration for each HLS configuration
### 5.1 The behavioral simulation (C++ only).
define_sim_config B -argv "$IN_FILE_NAME $OUT_FILE_NAME"
### 5.2 The Verilog simulation for HLS config "BASIC". 
define_sim_config V_BASIC "HeapSorter RTL_V BASIC" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
### 5.3 The Verilog simulation for HLS config "DPA". 
define_sim_config V_DPA "HeapSorter RTL_V DPA" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
### DPopt + Unroll
define_sim_config V_DPAUA "HeapSorter RTL_V DPAUA" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
### Unroll
define_sim_config V_UNROLL "HeapSorter RTL_V UNROLL" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
### PIPELINE
define_sim_config V_PIPE "HeapSorter RTL_V PIPE" -argv "$IN_FILE_NAME $OUT_FILE_NAME"

