# Report

### GitHub repo
https://github.com/YU-LIN-LIN/ESL_mid_project.git

### General description or introduction of the problem and your solution
	This project implements heap sort with HLS and TLM platform.
  
### Implementation details
	In TLM part, I just modify the content of lab4, TLM with bus. Changing the testbench and using sorter to replace Sobel_filter.
  In HLS part, I also modify the content of lab6, stratus HLS. Changing files as above and rewrite part of the project.tcl.
  Here is the table from HLS :
  HLS	BASIC	DPA	DPAUA	UNROLL	PIPELINE
Simulated time(ns)	2150	2060	2060	2150	2150
Total run time(ns)	2090	2000	2000	2090	2090
Total latency	160	152	152	160	160
Avg, latency	20	19	19	20	20
Area	2880.6	2281.1	2281.1	2880.6	2880.6
![image](https://user-images.githubusercontent.com/61815140/163719273-8d73f453-72e5-4934-88c7-2aec1ed52f79.png)

  
### Additional features of your design and models
  

### Experimental results
	

### Discussions and conclusions
  
