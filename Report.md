# Report

### GitHub repo
https://github.com/YU-LIN-LIN/ESL_mid_project.git

### General description or introduction of the problem and your solution
	This project implements selection sort with HLS and TLM platform.
  
### Implementation details
	In TLM part, I just modify the content of lab4, TLM with bus. Changing the testbench and using sorter to replace Sobel_filter.
  	In HLS part, I also modify the content of lab6, stratus HLS. Changing files as above and rewrite part of the project.tcl.
  	Here is the table from HLS : 
![image](https://user-images.githubusercontent.com/61815140/163719941-ec2aacc4-779a-48b5-aad7-f4148bd14ab5.png)
  
### Additional features of your design and models
	I made a sorter that can sort each element in a sequence in order from minimum to maximum. 
	It supports different input sequence size. The hardware of this sorter can sort 9 elements at once. 
	If the sequence length is shorter than 9, testbench will only need to send the whole sequence once 
	and can get a sorted sequence. 
	However, if the sequence length is longer than 9, the input sequence would need to be 
	partitioned and sent to the sorter more than once. Take a 15 elements sequence as an example, 
	it will be partitioned before being sent. At first, index [0] to [8] will be sent into the sorter, 
	then [9] to [14], since the input size of the sorter is 9 elements [15] to [17] will be 
	set as 255 (since each element in the sorter is 8 bits) so that it won't affect the sorting result. 
	And then sent [4] to [12] to the sorter again, now we finish 1 group. 
	I use worst case to try how many input groups we need to get a sorted sequence. 
	By induction, I get the following conclusion showed in the table below.
	Take a sequence with 30 elements input as an example, the input group that will be send into the sorter
	will be {[0~8], [9~17], [18~26], [27~35], [4~12], [13~21], [22~30]}, expressed by index. As mentioed above, 
	the value of the element whose index is larger than 29 will be set as 255. We need input this input group 5 
	times to get a sorted sequence.
![image](https://user-images.githubusercontent.com/61815140/165094493-8926eb28-e4b4-4a27-9c68-45f09bff8107.png)
![1650893104384](https://user-images.githubusercontent.com/61815140/165098182-40fe6414-c54d-4956-acb0-e1094059f2f0.jpg)
 
  	The result of latency and area are the same with BASIC and PIPELINE. I think the reason is that sorting 
	needs to input the whole sequence at first in my coding, so it still needs to wait all system input data 
	enter the hardware then it could start to do calculation.
	

### Experimental results
![image](https://user-images.githubusercontent.com/61815140/163719955-c58926f5-6779-4504-98e0-b412f3de25d0.png)

### Discussions and conclusions
  	In this project, I know how to construct TLM model adequately by using TLM bus API and do high level synthesis with stratus tools.
  	Adding HLS configuration to optimize the synthesis result.
