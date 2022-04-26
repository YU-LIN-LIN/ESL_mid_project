# Report

### GitHub repo
https://github.com/YU-LIN-LIN/ESL_mid_project.git

### General description or introduction of the problem and your solution
	This project implements selection sort with HLS and TLM platform.
  
### Implementation details
	In TLM part, I just modify the content of lab4, TLM with bus. Changing the testbench and using sorter to replace Sobel_filter.
  	In HLS part, I also modify the content of lab6, stratus HLS. Changing files as above and rewrite part of the project.tcl.
  
### Additional features of your design and models
	I made a sorter that can sort each element in a sequence in order from minimum to maximum. 
	It supports different input sequence size. The hardware of this sorter can sort 9 elements at once. 
	If the sequence length is shorter than 9, testbench will only need to send the whole sequence once 
	and can get a sorted sequence. 
	However, if the sequence length is longer than 9, the input sequence would need to be 
	partitioned and sent to the sorter more than once. Take a 15 elements sequence as an example, 
	it will be partitioned before being sent. At first, index [0] to [8] will be sent into the sorter, 
	then [9] to [14]. Since the input size of the sorter is 9 elements,  index [15] to [17] will be 
	set to 255 (because each element in the sorter is 8 bits) so the sorting result won't be affected.
	And then send [4] to [12] to the sorter again. Now we finish 1 group. 
	I used the worst case to try how many input groups we need to get a sorted sequence. 
	By induction, I get the conclusion showed in the table below.
	As an example, consider a 30-element input. The input group that will be send into the sorter
	will be {[0~8], [9~17], [18~26], [27~35], [4~12], [13~21], [22~30]}, expressed by index. As mentioed above, 
	the value of the element whose index is larger than 29 will be set as 255. We need to input this input group 5 
	times to get a sorted sequence.
![image](https://user-images.githubusercontent.com/61815140/165207121-4424399b-738a-41b5-900e-fa618a99e791.png)
![1650893104384](https://user-images.githubusercontent.com/61815140/165098182-40fe6414-c54d-4956-acb0-e1094059f2f0.jpg)
 
  	Here is the block diagram of the hardware :
![1650939676931](https://user-images.githubusercontent.com/61815140/165206720-9612f3c1-37de-49f5-a305-cd11f91ae1ec.jpg)


### Experimental results
	The table below shows latency in different input sequence size :
![image](https://user-images.githubusercontent.com/61815140/165208133-1cf4f276-c1d1-431b-86ab-88e8ae79656a.png)

	The result of latency and area are the same with BASIC and PIPELINE. I think the reason is that sorting 
	needs to input the whole sequence at first in my coding, so it still needs to wait all system input data 
	enter the hardware then it could start to do calculation.
	DPAUA means "data path optimization" + "unroll all loops", which has the same result with DPA.
	I thought that the unroll cannot have a much better latency, due to data dependancy in the loop.
	Thus, I use another version that unroll the inner loop directly in code (using HLS command : make sim_V_UNROLL), which is "unroll by 2" in the above table, 
	and run HLS again.
	
![1650940874969](https://user-images.githubusercontent.com/61815140/165209156-a95c75da-bc24-456d-ad52-c00af45cb658.jpg)
![1650940891984](https://user-images.githubusercontent.com/61815140/165209165-0cff8284-3699-4b08-850b-c5653c7abd6b.jpg)

	The above two figure shows the 2 version of the loop I designed, the above is origin without unrolling and the 
	below is unrolling by 2. However, the result is almost the same in these 2 version. 
	The reason is that the latency constraint of unrolling by 2 needs to be set twice among the origin.
	
	The following table is the area report. Because the hardware is the same, the area of different input sequence length 
	wil not have significant difference. I just compare original area and the area after unrolling by 2.
	
![image](https://user-images.githubusercontent.com/61815140/165226858-3c770111-4ce6-4e0e-a268-d8fb8fc15307.png)
![image](https://user-images.githubusercontent.com/61815140/165226884-aea922c9-f757-47b0-af61-fd002346666c.png)
	
### Discussions and conclusions
  	In this project, I know how to construct TLM model adequately by using TLM bus API and do high level synthesis with stratus tools.
  	Adding HLS configuration to optimize the synthesis result.
