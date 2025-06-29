# Connect and Deploy Bitstream

We provide five machines for artifact evaluation: a **sender0**(atc@r4), a **receiver0**(atc@r3), a **sender1**(atc@r2), a **receiver1**(atc@r1), and a **Vivado machine**(atc@max). All the **sender** and the **receiver** are equiped with a Xilinx U280 FPGA and a ConnectX-5 NIC machine. The **sender** and the **receiver** are used for the experiment, and the **Vivado machine** is used for deploying bitstream.

**Important: the PASSPORT is: atc25**

## 1. Connect to Artifact Machine
We already create a sudo user `atc25` on three machines, but disable password login for security reasons. You can use the following command to connect to the artifact machine.


1. Download private key `atc25_id_ed25519` from the submit website.
2. Get the jump server domain name (use `jump` refer as following) from the submit website, we provide IPV4 server and IPV6 server(recommend).
3. `ssh atc25@js.v4.rc4ml.org -p xxx -i atc25_id_ed25519`
4. Enjoy for testing
5. If you have any questions, please contact us.

## 2. Deploy Bitstream on FPGA

### 2.1 Open Vivado

We already install vivado21 on **Vivado machine**, you should connect to the Vivado machine **by a terminal support X11 forward(important!!)** and open Vivado by typing `vivado` in the terminal. We recommend using **MobaXterm** to connect to the Vivado machine.


### 2.2 Open Hardware Manager

Click `Open Hardware Manager` to open the hardware manager.
![](images/202411301455.png)


### 2.3 Open Target 

First click `Open Target` to open the target, and then click `Open New Target` to open the new target.
![](images/202411301457.png)

Click `Next` to continue.

![](images/202411301458.png)

Type in `192.168.189.16` and `3121` for `Remote server`, and click `Next`.

![](images/202411301500.png)

Then check the name and the type of hardware target, and click `Next`.

![](images/202411301501.png)

Finally we connect to the FPGA.

![](images/202505092221.png)

### 2.4 Program FPGA

Right click the U280 with the ID **21770205K02VA** and click `Open Target`.
![](images/202505092223.png)

Right click the FPGA name `xcu280_u55c_0` and click `Program Device`.
![](images/202505092224.png)

Then choose the bitstream file `bitstream/xxxxx.bit` and click `Program`. You don't need set `Debug probes file`.
![](images/202505092226.png)

After the bitstream has been successfully programmed.
Right click the U280 with the ID **21760297J03CA** and click `Open Target`.
![](images/202505092227.png)

Right click the FPGA name `xcu280_u55c_0` and click `Program Device`.
![](images/202505092228.png)

Then choose the bitstream file `bitstream/xxxxx.bit` and click `Program`. You don't need set `Debug probes file`.
![](images/202505092226.png)

### 2.5 Restart sender and recerver machine

After programming the FPGA, you should restart the sender and receiver by typing `sudo reboot` in the terminal. And then you can run the SwCC experiment.
