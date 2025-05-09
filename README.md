# SwRDMA: Enabling Software-Programmable Congestion Control in High-Speed NIC RDMA Engines [ATC2025]





## Required hardware and software

- Xilinx Alevo U280 FPGA
- ConnextX-5 NIC
- Required lib: cmake, RISCV-GUN-toolchain
- g++ >= 11.3.0
- MLNX_OFED 


## Install Dependencies and Build
See [INSTALL.md](./doc/INSTALL.md) for install dependencies and build RPCNIC on a single machine.

## Connect and Deploy Bitstream
See [DEPLOY.md](./doc/DEPLOY.md) for connecting to our artifact machine and  deploying FPGA bitstream on Xilinx U280.

## Run Test
If Check if the configuration is correct in Run Experiments of [EXP.md](./doc/EXP.md) passes, then everything will be fine. Please refer to exp.md for more details.


##



### Directory Structure:

~~~
.
├── bitstream (U280 Bitstream)
├── Baseline (chisel compiled intermediate products)
├── include
│   └── bench_header (HyperProtoBench and DeathStarBench header file)
├── src (main CPU control code)
│   └── chisel (partial used chisel code)
├── test_suite (source code of experiments)
│   ├── dsa_offload (used for DSA offload experiment)
│   ├── e2e_compression (used for e2e compression experiment)
│   └── hardware_tb (used for micro benchmark)
├── third_party (software used third party libs)
│   ├── asio
│   ├── atomic_queue
│   ├── eRPC
│   └── fmt
└── utils
~~~


