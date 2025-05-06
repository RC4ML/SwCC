# Evaluation

**Important:** Please see [INSTALL.md](./INSTALL.md) for install dependencies and build RPCNIC on a single machine. 
**Important:** Please see [DEPLOY.md](./DEPLOY.md) for connecting to our artifact machine and deploying FPGA bitstream on Xilinx U280.

## 1. Comparison of Control Loop Delay

This is the evalution for fig.7 in the paper.

### 1.1 Run the SwRDMA 

Program `bistream/deser_one-shot_DMA.bit` to the FPGA, and after reboot the machine, run the following command to start the experiment:

~~~bash
sudo ../bin_host/deserialize_hw 0 8
~~~

`deserialize_hw` program accepts two arguments: the first argument is the number of messages, range from [0-9], and the second argument is the number of outstanding req, set it to `8` is enough.

To run differenct BENCH, please edit `/src/deserialize_hw.cpp`, change `#define BENCH0` to `#define BENCHX` and recompile the program.

The otput will be like this:
~~~
total size: 91323
data_cnt                                   : 142701027
timer_en                                   : 1
timer_cnt                                  : 149914182
timer_cnt                                  : 149914182
speed: 92.9517
timer                                  : 599656
~~~

### 1.2 Run the field-by-field DMA deserializer
Program `bitstream/deser_field_by_field.bit` to the FPGA, and after reboot the machine, run the following command like above to start the experiment:

~~~bash
sudo ../bin_host/deserialize_hw 0 8
~~~

The output will be like this:

~~~bash
total size: 91323
data_cnt                                   : 142701027
timer_en                                   : 1
timer_cnt                                  : 149914182
timer_cnt                                  : 149914182
speed: 92.9517
timer                                  : 599656
~~~

### 1.3 Calculate the speedup
We can calculate the speedup by dividing the speed of one-shot DMA deserializer by the speed of field-by-field DMA deserializer.


## 2. Effect of TX/RX Separated Multi-Core

This is the evalution for fig.10 in the paper.

### 2.1 Run the SwRDMA-1
Program `bistream/ser_memory-affinity.bit` to the FPGA, and after reboot the machine, run the following command to start the experiment:

~~~bash
sudo ../bin_host/serialize_sw 0
~~~

The output will be like this:
~~~bash
1. Hyperprotobench.bench0.sw.Serialize() timeing: 54.575 us
1. Hyperprotobench.bench0.sw + hw Total() timeing: 66.497 us
host_serialized_len = 91537
p_c2h_buffer_size = 91776
~~~

### 2.2 Run the RoCE-1 & RoCE-8
Program `bistream/ser_memory-affinity.bit` to the FPGA, and after reboot the machine, run the following command to start the experiment:

~~~bash
sudo ../bin_host/serialize_co 0
~~~
The output will be like this:
~~~bash
1. Hyperprotobench.bench0.sw.Serialize() timeing: 54.575 us
1. Hyperprotobench.bench0.sw + hw Total() timeing: 66.497 us
host_serialized_len = 91537
p_c2h_buffer_size = 91776
~~~

### 2.3 Run the Soft-RoCE-1 & Soft-RoCE-24
Program `bistream/ser_protoACC-PCIe.bit` to the FPGA, and after reboot the machine, run the following command to start the experiment:
~~~bash
sudo ../bin_host/serialize_hw 0
~~~
The output will be like this:
~~~bash
1. Hyperprotobench.bench0.Serialize() timeing: 27.404 us
host_serialized_len = 1f6
p_c2h_buffer_size = 16680
~~~


## 3. Average Time to Access QP Context


This is the evalution for fig.7 in the paper.

### 1.1 Run the SwRDMA 

Program `bistream/deser_one-shot_DMA.bit` to the FPGA, and after reboot the machine, run the following command to start the experiment:

~~~bash
sudo ../bin_host/deserialize_hw 0 8
~~~

`deserialize_hw` program accepts two arguments: the first argument is the number of messages, range from [0-9], and the second argument is the number of outstanding req, set it to `8` is enough.

To run differenct BENCH, please edit `/src/deserialize_hw.cpp`, change `#define BENCH0` to `#define BENCHX` and recompile the program.

The otput will be like this:
~~~
total size: 91323
data_cnt                                   : 142701027
timer_en                                   : 1
timer_cnt                                  : 149914182
timer_cnt                                  : 149914182
speed: 92.9517
timer                                  : 599656
~~~

### 1.2 Run the field-by-field DMA deserializer
Program `bitstream/deser_field_by_field.bit` to the FPGA, and after reboot the machine, run the following command like above to start the experiment:

~~~bash
sudo ../bin_host/deserialize_hw 0 8
~~~

The output will be like this:

~~~bash
total size: 91323
data_cnt                                   : 142701027
timer_en                                   : 1
timer_cnt                                  : 149914182
timer_cnt                                  : 149914182
speed: 92.9517
timer                                  : 599656
~~~


## 4. Full FPGA Code
Due to some confidential requirements regarding FPGA-specific modules such as DMA, and networking (which is not related to the core idea of the paper), we are currently unable to provide full chisel code to generate each bitstream. However, we have open-sourced the core Chisel source code files in `/src/chisel` and generated bitstream to reproduce the core results.


## 5. Extra Attention
- You can change `#define BENCH0` to `#define BENCHX` in source to run different hyperprotobench benchmarks.
- Sometimes the kernel may panic. It will occur if the configuration is not correct. Reboot is need when encounter the kernel panic.

And if you have any questions, please contact us.
