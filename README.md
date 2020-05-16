# TCP Variant Analysis

The project compares the performance between TCP cubic and DCTCP based on two metrics being throughput and flow completion time 

### Network Topology 

We setup a dumbell topology in network simulator (ns3) with the following configuation and run a few experiments on it and compare the results of throughput and flow completion time in each case. 

### Configuration 

There are two sender servers S1 and S2 which are connected to router R1 with 1 Gbps link each. There are two destination servers D1 and D2 which are connected to router R2 with 1 Gbps link each. Both R1 and R2 are connected with 1 Gbps link as well.

1. S1 will send traffic to D1 only
2. will send traffic to D2 only
3. All the servers i.e., S1, S2, D1, and D2 are configured to support TCP Cubic (also known as TCP BIC in ns-3) and DCTCP.

### Measurement Metric 

1. Throughput tells how much data can be transferred from a source to destination at any given time. Its unit is bits per second (bps).
2. Average Flow Completion Time measures the time taken by the sender to send the desired amount of data to the sender. It is measured in seconds.

### Experiments 

Once the topology is set and configurations are done, we run the following experiments and measure throughput and average flow completion time. In each experiment, we use bulk sender application in ns-3 to generate the traffic. we use bulk sender to send 500 MB of data from source to destination in each of the following experiments:

#### Exp-1: 
S1 sends traffic to D1 using TCP Cubic. S2 and D2 are not used in this experiment.

#### Exp-2: 
S1 sends traffic to D1 and S2 sends traffic to D2. Both senders will use TCP Cubic and start sending data to respective destinations simultaneously.

#### Exp-3: 
S1 sends traffic to D1 using DCTCP. S2 and D2 are not used in this experiment.

#### Exp-4: 
S1 sends traffic to D1 and S2 sends traffic to D2. Both senders will use DCTCP and start sending data to respective destinations simultaneously.

#### Exp-5: 
S1 sends traffic to D1 using TCP Cubic whereas S2 sends traffic to D2 using DCTCP. Both senders will start sending data to respective destinations simultaneously.

We run each experiment 3 times and find the average throughput and average flow completion time. 

After running the experiments we tabulate the results in a csv file as shown above. 

### Running Instructions:

1. On an ubuntu system, make sure you have git, if you don't use the following commands to install git. 

``` sudo apt install git-all ```

2. Use git to clone the master branch of ns-3-dev as follows to the home folder of the system: 

``` git clone https://gitlab.com/nsnam/ns-3-dev.git ```

3. ``` cd ns-3-dev ```
4. Run the following commands to build ns-3

```./waf configure --enable-examples``` <br />
```./waf```

5. Now, copy the tcp_srachak.cc file to /home/ns-3-dev/scratch/

6. Navigate to the ns-3-dev folder and run the file using the following commands: 

```./waf --run scratch/tcp_srachak```

7. The run may take upto 4 hours. Notice that a file called tcp_srachak.csv gets generated. 
8. View the file for results: <br/>
``` vi tcp_srachak```
