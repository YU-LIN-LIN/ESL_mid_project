# ESL_mid_project
### HeapSort implementation with TLM and HLS platform

## heapsort.cpp
  Software implementation for heap sort.

## heapsort compile
  This directory implements heap sort with HLS.
  
### Enter the strtus directory.
  cd stratus

### Behavioral simulation
  make sim_B
  (output file will be generated in "stratus" directory)

### Verilog simulation for HLS config "BASIC".
  make sim_V_BASIC

### Verilog simulation for HLS with data path optimization.
  make sim_V_DPA
  
### Verilog simulation for HLS with data path optimization and loop unrolling.
  make sim_V_DPAUA
  
### Verilog simulation for HLS with loop unrolling.
  make sim_V_UNROLL
  
### Verilog simulation for HLS with loop pipeline.
  make sim_V_PIPELINE  


## heapsort_TLM compile
  This directory implements heap sort with TLM.
  
### Construct a new directory.
  mkdir build

### Go into the directory. 
  cd build

### Read CMakeList.txt and visit the whole directory.
  make

### Generate output figure.
  make run
  (output file will be generated in "build" directory)
  
## heapsort_v2 is a newer version of this project
  Support general input sequence size. (sorter_def.h : change the constant 'SEQ_TOT')
  Using selection sort as sorter to parallelize better without dependency issue.
