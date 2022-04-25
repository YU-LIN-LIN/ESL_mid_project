# Report

### GitHub repo
https://github.com/YU-LIN-LIN/ESL_mid_project.git

### General description or introduction of the problem and your solution
	This project implements heap sort with HLS and TLM platform.
  
### Implementation details
	In TLM part, I just modify the content of lab4, TLM with bus. Changing the testbench and using sorter to replace Sobel_filter.
  	In HLS part, I also modify the content of lab6, stratus HLS. Changing files as above and rewrite part of the project.tcl.
  Here is the table from HLS : 
![image](https://user-images.githubusercontent.com/61815140/163719941-ec2aacc4-779a-48b5-aad7-f4148bd14ab5.png)
  
### Additional features of your design and models
  	The result of latency and area are the same with BASIC and PIPELINE. I think the reason is that sorting needs to input the whole sequence at first in my coding, 
  so it still needs to wait all system input data enter the hardware then it could start to do calculation.
  	Since the kernel is composed of 2-layer loops, so loop unrolling could make the latency lower however make the area larger.
  	With DPA, it would optimize latency and area, so the area in HLS with data path optimization and loop unrolling would get 27(um^2) lower area compared with HLS   with just loop unrolling.

### Experimental results
![image](https://user-images.githubusercontent.com/61815140/163719955-c58926f5-6779-4504-98e0-b412f3de25d0.png)

### Discussions and conclusions
  	In this project, I know how to construct TLM model adequately by using TLM bus API and do high level synthesis with stratus tools.
  Adding HLS configuration to optimize the synthesis result.
