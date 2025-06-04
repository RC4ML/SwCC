# SwCC: Software-Programmable and Per-Packet Congestion Control in RDMA Engine [ATC2025]


A RDMA Engine that enables oftware-Programmable Congestion Control in High-Speed
NIC. This is the source code for our ATC'25 paper.


## Required hardware and software

- Xilinx Alevo U280 FPGA
- ConnextX-5 NIC
- Required lib: cmake, RISCV-GUN-toolchain
- g++ >= 11.3.0
- MLNX_OFED 


## Install Dependencies and Build
See [INSTALL.md](./doc/INSTALL.md) for install dependencies and build SwCC on a single machine.

## Connect and Deploy Bitstream
See [DEPLOY.md](./doc/DEPLOY.md) for connecting to our artifact machine and  deploying FPGA bitstream on Xilinx U280.

## Run Test
If Check if the configuration is correct in Run Experiments of [EXP.md](./doc/EXP.md) passes, then everything will be fine. Please refer to exp.md for more details.


## Implementation and Compilation of Various Congestion Control Algorithms

See [riscv/README.md](./riscv/README.md) for details on how we implement various congestion control algorithms (CCAs) and compile them into MEM(.mem) files.

After generating the MEM(.mem) file, we load it into the RISC-V instruction memory (on-chip RAM of the FPGA) and then generate the FPGA bitstream. In the ./bitstream directory, we provide the bitstreams for the DCQCN algorithm.

### Directory Structure:

~~~
.
├── Baseline (the source code for RoCE and Soft-RoCE experiments)
│   └── libr (used for RoCE and Soft-RoCE experiments)
│   └── rex (the source code of rdma_rex for Soft-RoCE)
├── bitstream (U280 Bitstream)
├── doc (User Manual)
├── riscv (the CCA code running on RISC-V)
├── SwCC (the source code for SwCC experiments)
~~~
