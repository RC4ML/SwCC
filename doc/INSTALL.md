# Installation

This document shows all of the essential software installation process on test machine. **We already install all dependencies on our artifact machines.**

**Important: The RoCE experiments require MLNX_OFED to be installed, whereas the Soft-RoCE experiments require it to be uninstalled. Therefore, we strongly recommend completing all RoCE experiments before uninstalling MLNX_OFED.**

## 1. Install QDMA Driver:

You should use `lspci | grep Xilinx` to find the correct device id. For example, if the device id is `2a:00.0`, you should replace `2a:00.0` with `1a:00.0` in the following commands.
~~~bash
cd qdma_driver
make
sudo insmod /path/to/qdma_driver/src/qdma-pf.ko
echo '1024' | sudo tee -a /sys/bus/pci/devices/0000:1a:00.0/qdma/qmax
sudo dma-ctl qdma1a000 q add idx 0 mode st dir bi
sudo dma-ctl qdma1a000 q start idx 0 dir bi desc_bypass_en pfetch_bypass_en
~~~


## 2. Install MLNX_OFED:

You can choose any version of MLNX_OFED, but we recommend using the version we used in our artifact evaluation. You can download the version from the following link
~~~bash
wget https://content.mellanox.com/ofed/MLNX_OFED-23.04-1.1.3.0/MLNX_OFED_LINUX-23.04-1.1.3.0-ubuntu18.04-x86_64.tgz
tar -zxvf ./MLNX_OFED_LINUX-23.04-1.1.3.0-ubuntu18.04-x86_64.tgz
cd MLNX_OFED_LINUX-23.04-1.1.3.0-ubuntu18.04-x86_64 && sudo ./ofedinstall
~~~


## 3. Validate if preparations are all done

### 3.1 Check QDMA driver status

We use `sudo dmesg | grep qdma_pf` to check if the QDMA driver is loaded successfully. Here is a successful output:
~~~bash
[  194.566491] qdma_pf:qdma_mod_init: Xilinx QDMA PF Reference Driver v2020.2.1.1.
[  194.568638] qdma_pf:probe_one: 0000:1a:00.0: func 0x0, p/v 0/0,0x0000000000000000.
[  194.568643] qdma_pf:probe_one: Configuring '1a:00:0' as master pf
[  194.568645] qdma_pf:probe_one: Driver is loaded in auto(0) mode
[  194.568647] qdma_pf:qdma_device_open: qdma-pf, 1a:00.00, pdev 0x0000000055a384ed, 0x10ee:0x903f.
[  194.568822] qdma_pf:qdma_device_attributes_get: qdma1a000-p0000:1a:00.0: num_pfs:1, num_qs:512, flr_present:0, st_en:1, mm_en:1, mm_cmpt_en:0, mailbox_en:1, mm_channel_max:1, qid2vec_ctx:0, cmpt_ovf_chk_dis:1, mailbox_intr:1, sw_desc_64b:1, cmpt_desc_64b:1, dynamic_bar:1, legacy_intr:1, cmpt_trig_count_timer:1
[  194.568829] qdma_pf:qdma_device_open: Vivado version = vivado 2020.2
[  194.568835] qdma_pf:intr_setup: current device supports only (8) msix vectors per function. ignoring input for (32) vectors
[  194.576278] qdma_pf:xdev_identify_bars: AXI Master Lite BAR 2.
[  194.576281] qdma_pf:xdev_identify_bars: AXI Bridge Master BAR 4.
[  194.576282] qdma_pf:qdma_device_open: 0000:1a:00.0, 1a000, pdev 0x0000000055a384ed, xdev 0x0000000036f2e3ee, ch 1, q 0, vf 0.
~~~

### 3.2 Build SwRDMA software

Now we create a `SwRDMA/build` directory, and build the SwRDMA software in the `build` directory.
~~~bash
mkdir SwRDMA/build
cd SwRDMA/build
cmake ..
make
~~~

It should report no error. And we will get the output binary in the `SwRDMA/build/example` directory.

### 3.3 Build baseline software

Now we create a `Baseline/libr/build` directory, and build the Baseline software in the `build` directory.
~~~bash
mkdir Baseline/libr/build
cd Baseline/libr/build
cmake ..
make
~~~

It should report no error. And we will get the output binary in the `Baseline/libr/build` directory.


## 4. After completing the RoCE experiments, follow these steps to prepare for the Soft-RoCE experiments.

### 4.1 Uninstall MLNX_OFED

Use the official uninstallation script **uninstall.sh** to uninstall MLNX_OFED, and then reboot the machine.

~~~bash
cd ~/MLNX_OFED_LINUX-23.04-1.1.3.0-ubuntu18.04-x86_64/
sudo ./uninstall.sh
sudo reboot
~~~

### 4.2 Load kernel modules

Ensure that the kernel modules **ib_core**, **ip6_udp_tunnel**, **udp_tunnel**, and **ib_uverbs** are loaded, and update module dependencies.

~~~bash
sudo modprobe ib_core
sudo modprobe ip6_udp_tunnel 
sudo modprobe udp_tunnel 
sudo modprobe ib_uverbs
sudo depmod -a
~~~

### 4.3 Compile and install the RXE module

Compile **rdma_rxe.ko** and install the kernel module

~~~bash
cd ~/SwCC/Baseline/rxe/
make -j
sudo insmod ./rdma_rxe.ko
~~~

### 4.4 Install dependency libraries

~~~bash
sudo apt install librdmacm-dev libibverbs-dev libibumad-dev libpci-dev
~~~

### 4.5 Other environment setup

Use ifconfig to check the network interface name, bind the interface as a Soft-RoCE device, then modify the MTU and disable irqbalance.

atc25@r4:
~~~bash
sudo rdma link add sr4 type rxe netdev enp62s0np0
sudo ifconfig enp62s0np0 mtu 9000 up
sudo service irqbalance stop
~~~

atc25@r3:
~~~bash
sudo rdma link add sr3 type rxe netdev enp28s0np0
sudo ifconfig enp28s0np0 mtu 9000 up
sudo service irqbalance stop
~~~