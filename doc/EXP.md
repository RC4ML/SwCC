# Evaluation

**Important:** Please see [INSTALL.md](./INSTALL.md) for install dependencies and build RPCNIC on a single machine. 
**Important:** Please see [DEPLOY.md](./DEPLOY.md) for connecting to our artifact machine and deploying FPGA bitstream on Xilinx U280.

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

Entering any value on the receiver. Then，entering any value on the sender. The output will be like this:

~~~
5th percentile: 3096 ns
25th percentile: 3168 ns
50th percentile (median): 3168 ns
75th percentile: 3168 ns
95th percentile: 3168 ns
~~~

### 1.2 Run the RoCE


### 1.3  Run the Soft-RoCE



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

Entering any value on the receiver. Then，entering any value on the sender. The output will be like this:

~~~
data_cnt                                   : 10001
timer_en                                   : 1
timer_cnt                                  : 189988
speed: 6.06354 Gbps
~~~

### 2.2 Run the RoCE-1 & RoCE-8


### 2.3 Run the Soft-RoCE-1 & Soft-RoCE-24



## 3. Average Time to Access QP Context


This is the evalution for fig.11(a) in the paper.


### 3.1 How to run ?
```
cd cache/simulator
bash test.sh

```

user can assign cache_size， block_size, and cache associativity.
```
./JSE --cache_size=32768 --block_size=64 --associativity=1
```