# Evaluation

# Evaluation

**Important:** Please see [INSTALL.md](./INSTALL.md) for install dependencies and build SwRDMA and Baseline on sender and reciever machine. 
**Important:** Please see [DEPLOY.md](./DEPLOY.md) for connecting to our artifact machine and deploying FPGA bitstream on Xilinx U280s.

## 1. Comparison of Control Loop Delay

This is the evalution for fig.7 in the paper.

### 1.1 Run the SwRDMA 

Program `bistream/SwRDMA-1.bit` to the FPGA on both the sender and receiver, and after reboot the machines, run the following command to start the experiment:


Receiver:
~~~bash
cd src/build
sudo example/latency_receiver
~~~

Sender:
~~~bash
sudo ./example/latency_sender
~~~

After the sender displays the message:
~~~
After both the sender and receiver programs are started, enter any value to launch the receiver: 
~~~
and the receiver displays the message:
~~~
After the receiver is launched, enter any value to begin the latency test.
~~~

Entering any value on the receiver. Then，entering any value on the sender. 

The output will be like this:

~~~
5th percentile: 3096 ns
25th percentile: 3168 ns
50th percentile (median): 3168 ns
75th percentile: 3168 ns
95th percentile: 3168 ns
~~~

### 1.2 Run the RoCE

To begin the experiment, run the following commands in sequence.

Receiver:
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 0  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

Sender:
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 1  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 1024 -outstanding 1
~~~

`drdma_bench` program accepts nine arguments: The `threads` argument means the number of QPNs (Queue Pair Numbers) to run during the test. The `packSize` argument means the packet size in bytes. The `outstanding` argument means the number of outstanding RDMA requests.



On the sender, the output will be like this:

~~~
#[Mean    =     3231.062, StdDeviation   =      548.675]
#[Max     =   241663.955, Total count    =       512000]
#[Buckets =            7, SubBuckets     =         2048]
~~~


### 1.3  Run the Soft-RoCE




~~~
#[Mean    =    24071.424, StdDeviation   =     4684.146]
#[Max     =   318184.682, Total count    =       512000]
#[Buckets =            7, SubBuckets     =         2048]
~~~


## 2. Effect of TX/RX Separated Multi-Core

This is the evalution for fig.10 in the paper.

### 2.1 Run the SwRDMA-1
Program `bistream/SwRDMA-1.bit` to the FPGA on both the sender and receiver, and after reboot the machines, run the following command to start the experiment:

Receiver:
~~~bash
cd src/build
sudo example/throughput_receiver 64
~~~

Sender:
~~~bash
sudo ./example/throughput_sender 64
~~~


`throughput_receiver` and `throughput_sender` program accepts one arguments: the packet size, range from 64-2048.


After the sender displays the message:
~~~
After both the sender and receiver programs are started, enter any value to launch the receiver: 
~~~
and the receiver displays the message:
~~~
After the receiver is launched, enter any value to begin the latency test.
~~~

Entering any value on the receiver. Then，entering any value on the sender. 

On the sender, the output will be like this:

~~~
data_cnt                                   : 10001
timer_en                                   : 1
timer_cnt                                  : 189988
speed: 6.06354 Gbps
~~~

### 2.2 Run the RoCE-1 & RoCE-8

To begin the experiment, run the following commands in sequence.

Receiver:
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 0  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~


Sender:
~~~bash
cd Baseline/libr/build
sudo ./rdma_bench  -serverIp 10.0.0.4 -nodeId 1  -deviceName mlx5_0 -gidIndex 3 -bufSize 104857600 -iterations 500 -threads 1 -packSize 64 -outstanding 48
~~~

`drdma_bench` program accepts nine arguments: The `threads` argument means the number of QPNs (Queue Pair Numbers) to run during the test. The `packSize` argument means the packet size in bytes. 


To test different configurations, adjust the `threads` argument to change the number of QPNs, and the `packSize` argument to change the packet size (in bytes).


On the sender and receiver, the output will be like this:

~~~
[INFO][15:07:15.973][rdma_bench.cpp:113][sub_task_server]                                           Data verification success, thread [0], duration [0.188152]s, throughput [1.393258] Gpbs
~~~


### 2.3 Run the Soft-RoCE-1 & Soft-RoCE-24




~~~
[INFO][18:03:04.057][rdma_bench.cpp:113][sub_task_server]                                           Data verification success, thread [0], duration [2.575283]s, throughput [0.101792] Gpbs
~~~


## 3. Average Time to Access QP Context


This is the evalution for fig.11(a) in the paper.


### 3.1 How to run ?
```bash
cd cache/simulator
bash run_cache.sh

```

user can assign cache_size， block_size, and cache associativity.
```bash
./JSE --cache_size=32768 --block_size=64 --associativity=1
```

### 3.2 without cache
```bash
bash run_no_cache.sh 


```

output should like this
```
Total time: 105993565
```

### 3.2 with cache
```bash
bash run_cache.sh 
```

output should like below
```
Associativity: 1

total time cost: 47669820
Associativity: 2

total time cost: 47667180
Associativity: 4

total time cost: 47678444
Associativity: 8

total time cost: 47672526
Associativity: 16

total time cost: 47681414
Associativity: 32

total time cost: 47672790
Associativity: 64

total time cost: 47674682
Associativity: 128

total time cost: 47671404
```

users can edit cache time parameters in `Cache.cpp` for `hit_time_cost`, `miss_time_cost1`, `miss_time_cost2`, `writeback_time`.
we use these two set ups:

```cpp
//* without hint:
static int hit_time_cost = 264;
static int miss_time_cost1 = 994;
static int miss_time_cost2 = 1324;
static int writeback_time = 264;

//* with hint:
static int hit_time_cost = 264;
static int miss_time_cost1 = 264;
static int miss_time_cost2 = 484;
static int writeback_time = 0;
```