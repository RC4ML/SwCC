# Evaluation

**Important:** Please see [INSTALL.md](./INSTALL.md) for install dependencies and build SwCC and Baseline on sender and reciever machine. 
**Important:** Please see [DEPLOY.md](./DEPLOY.md) for connecting to our artifact machine and deploying FPGA bitstream on Xilinx U280s.

**Important: The RoCE experiments require MLNX_OFED to be installed, whereas the Soft-RoCE experiments require it to be uninstalled. Therefore, please conduct the SwCC and Soft-RoCE experiments on sender0 and receiver0, and perform the RoCE experiments on sender1 and receiver1.**

## 1. Comparison of Control Loop Delay

This is the evalution for fig.7 in the paper.

### 1.1 Run the SwCC 

Program `bistream/SwCC-1.bit` to the FPGA on both the sender0 and receiver0, and after reboot the machines, run the following command to start the experiment:


Receiver0(atc@r3):
~~~bash
sudo SwCC/build/example/latency_receiver
~~~

Sender0(atc@r4):
~~~bash
sudo SwCC/build/example/latency_sender
~~~

After the receiver0 displays the message:
~~~bash
After both the sender and receiver programs are started, enter any value to launch the receiver: 
~~~
and the sender0 displays the message:
~~~bash
After the receiver is launched, enter any value to begin the latency test.
~~~

Entering any value on the receiver0. Then，entering any value on the sender0. 

The output will be like this:

~~~bash
5th percentile: 3096 ns
25th percentile: 3168 ns
50th percentile (median): 3168 ns
75th percentile: 3168 ns
95th percentile: 3168 ns
~~~

### 1.2 Run the RoCE

To begin the experiment, run the following commands in sequence.

Receiver1(atc@r1):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.1 -nodeId 0  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

Sender1(atc@r2):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.1 -nodeId 1  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

`drdma_bench` program accepts nine arguments: The `threads` argument means the number of QPNs (Queue Pair Numbers) to run during the test. The `packSize` argument means the packet size in bytes. The `outstanding` argument means the number of outstanding RDMA requests.



On the sender1, the output will be like this:

~~~bash
[Mean    =     3231.062, StdDeviation   =      548.675]
[Max     =   241663.955, Total count    =       512000]
[Buckets =            7, SubBuckets     =         2048]
~~~


### 1.3  Run the Soft-RoCE


Run the following commands in sequence.

**Note: If the machine has been rebooted, please reconfigure the Soft-RoCE environment according to Step 4 in [INSTALL.md](./INSTALL.md).**


Receiver0(atc@r3):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.3 -nodeId 0  -deviceName sr3 -gidIndex 1 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

Sender0(atc@r4):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.3 -nodeId 1  -deviceName sr4 -gidIndex 1 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

On the sender0, the output will be like this:

~~~bash
[Mean    =    24071.424, StdDeviation   =     4684.146]
[Max     =   318184.682, Total count    =       512000]
[Buckets =            7, SubBuckets     =         2048]
~~~


## 2. Effect of TX/RX Separated Multi-Core

This is the evalution for fig.10 in the paper.

### 2.1 Run the SwCC-1 & SwCC-8

#### 2.1.1 Run the SwCC-1
Program `bistream/SwCC-1.bit` to the FPGA on both the sender0 and receiver0, and after reboot the machines, run the following command to start the experiment:

Receiver0(atc@r3):
~~~bash
sudo SwCC/build/example/throughput_receiver 64
~~~

Sender0(atc@r4):
~~~bash
sudo SwCC/build/example/throughput_sender 64
~~~


`throughput_receiver` and `throughput_sender` program accepts one arguments: the packet size, range from 64-2048.


After the sender0 displays the message:
~~~bash
After both the sender and receiver programs are started, enter any value to launch the receiver: 
~~~
and the receiver0 displays the message:
~~~bash
After the receiver is launched, enter any value to begin the latency test.
~~~

Entering any value on the receiver0. Then，entering any value on the sender. 

On the sender0, the output will be like this:

~~~bash
data_cnt                                   : 10001
timer_en                                   : 1
timer_cnt                                  : 189988
speed: 6.06354 Gbps
~~~

**Note:** After testing the throughput for one packet size, the receiver needs to press Ctrl+C to terminate the receiver program before initiating the next test.

#### 2.1.2 Run the SwCC-8
Program `bistream/SwCC-8.bit` to the FPGA on both the sender0 and receiver0. After rebooting the machines, conduct the SwCC-8 experiment following the same procedure used for SwCC-1.


### 2.2 Run the RoCE-1 & RoCE-8

To begin the experiment, run the following commands in sequence.

Receiver1(atc@r1):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.1 -nodeId 0  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~


Sender(atc@r2):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.1 -nodeId 1  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~~

`drdma_bench` program accepts nine arguments: The `threads` argument means the number of QPNs (Queue Pair Numbers) to run during the test. The `packSize` argument means the packet size in bytes. 


To test different configurations, adjust the `threads` argument to change the number of QPNs (1 or 8), and the `packSize` argument to change the packet size (64-2048 in bytes).


On the sender0 and receiver1, the output will be like this:

~~~bash
[INFO][15:07:15.973][rdma_bench.cpp:113][sub_task_server]                                           Data verification success, thread [0], duration [0.188152]s, throughput [1.393258] Gpbs
~~~


### 2.3 Run the Soft-RoCE-1 & Soft-RoCE-24



Run the following commands in sequence.

**Note: If the machine has been rebooted, please reconfigure the Soft-RoCE environment according to Step 4 in [INSTALL.md](./INSTALL.md).**

Receiver0(atc@r3):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 0  -deviceName sr3 -gidIndex 1 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~


Sender0(atc@r4):
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 1  -deviceName sr4 -gidIndex 1 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~~

To test different configurations, adjust the `threads` argument to change the number of QPNs(1 or 24), and the `packSize` argument to change the packet size (64-2048 in bytes).


On the sender0 and receiver0, the output will be like this:

~~~bash
[INFO][18:03:04.057][rdma_bench.cpp:113][sub_task_server]                                           Data verification success, thread [0], duration [2.575283]s, throughput [0.101792] Gpbs
~~~

## 3. Flexible User-Defined CC Framework

This is the evalution for Table.3 in the paper.

See [riscv/README.md](./riscv/README.md) for details on how we implement various congestion control algorithms (CCAs) and compile them into MEM(.mem) files.

**Note: We only install 'riscv-gnu-toolchain' dependencies on 'vivado machines'.**

After generating the MEM(.mem) file, we load it into the RISC-V instruction memory (on-chip RAM of the FPGA) and then generate the FPGA bitstream. In the ./bitstream directory, we provide the bitstreams for the DCQCN algorithm.



## 4. Full FPGA Code
Due to confidentiality requirements, we are currently unable to provide the hardware code for bitstream generation. However, we provide pre-generated bitstream files for experimental use.