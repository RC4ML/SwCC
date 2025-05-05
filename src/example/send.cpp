#include <QDMAController.hpp>

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/os.h>
#include <fmt/args.h>
#include <fmt/ostream.h>
#include <fmt/std.h>
#include <fmt/color.h>

#include <unistd.h>
#include <cmath>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>


uint32_t swap_endian(uint32_t val) {
    return (val << 24) | ((val << 8) & 0x00FF0000) | ((val >> 8) & 0x0000FF00) | (val >> 24);
}

int main() {

    int pci_bus = 0x1a;
    FPGACtl::explictInit(pci_bus, 4 * 1024 * 1024);
    auto fpga_ctl = FPGACtl::getInstance(pci_bus);

    auto cpu_mem_ctl = CPUMemCtl::getInstance(1UL * 1024 * 1024 * 1024);

	size_t size = 1L*1024*1024*1024;

    cpu_mem_ctl->writeTLB([=](uint32_t page_index, uint32_t page_size, uint64_t vaddr, uint64_t paddr) {
        fpga_ctl->writeReg(8, (uint32_t) (vaddr));
        fpga_ctl->writeReg(9, (uint32_t) ((vaddr) >> 32));
        fpga_ctl->writeReg(10, (uint32_t) (paddr));
        fpga_ctl->writeReg(11, (uint32_t) ((paddr) >> 32));
        fpga_ctl->writeReg(12, (page_index == 0));
        fpga_ctl->writeReg(13, 1);
        fpga_ctl->writeReg(13, 0);
    });


    //----------init set
    auto dma_buff = cpu_mem_ctl->alloc(size);
    auto p = (uint32_t *) dma_buff;




    int length = 1024*1024;
    int pkg_num = 8000;
    int base_rtt = 600;
    int max_rate = 22;  //idle_cycle

    int queue_len = 30;
    int init_idle_cycle = 22;


    for(int i=0;i<length/4;i++){
      p[i] = 0;
    }

    fpga_ctl->writeReg(0, 1);
    sleep(1);
    fpga_ctl->writeReg(0, 0);
    sleep(1);


    fpga_ctl->writeReg(210, length);
    fpga_ctl->writeReg(215, queue_len);
    fpga_ctl->writeReg(216, init_idle_cycle);
    fpga_ctl->writeReg(217, 0);
    fpga_ctl->writeReg(218, pkg_num);
    fpga_ctl->writeReg(219, base_rtt);
    fpga_ctl->writeReg(220, max_rate);
    fpga_ctl->writeReg(212, (uint32_t) ((uint64_t) p >> 32));
    fpga_ctl->writeReg(211, (uint32_t) ((uint64_t) p));

	sleep(1);
    fpga_ctl->writeReg(213, 1);
    sleep(1);
    fpga_ctl->writeReg(214, 1);
    sleep(2);
    fpga_ctl->writeReg(213, 0);
    fpga_ctl->writeReg(214, 0);
    
    sleep(5);

fmt::println("tlb.io.tlb_miss_count                                       : {}", fpga_ctl->readReg(512+400));
fmt::println("io.c2h_cmd.[fire]                                           : {}", fpga_ctl->readReg(512+401));
fmt::println("io.h2c_cmd.[fire]                                           : {}", fpga_ctl->readReg(512+402));
fmt::println("io.c2h_data.[fire]                                          : {}", fpga_ctl->readReg(512+403));
fmt::println("io.h2c_data.[fire]                                          : {}", fpga_ctl->readReg(512+404));
fmt::println("fifo_c2h_cmd.io.out.[fire]                                  : {}", fpga_ctl->readReg(512+405));
fmt::println("fifo_h2c_cmd.io.out.[fire]                                  : {}", fpga_ctl->readReg(512+406));
fmt::println("fifo_c2h_data.io.out.[fire]                                 : {}", fpga_ctl->readReg(512+407));
fmt::println("fifo_h2c_data.io.in.[fire]                                  : {}", fpga_ctl->readReg(512+408));
fmt::println("io.data_in.[fire]                                           : {}", fpga_ctl->readReg(512+409));
fmt::println("io.q_time_out.[fire]                                        : {}", fpga_ctl->readReg(512+410));
fmt::println("io.data_out.[fire]                                          : {}", fpga_ctl->readReg(512+411));
fmt::println("io.data_out.[fireLast]                                      : {}", fpga_ctl->readReg(512+412));
fmt::println("pkg_cnt                                                     : {}", fpga_ctl->readReg(512+413));
fmt::println("pkg_num                                                     : {}", fpga_ctl->readReg(512+414));
fmt::println("state_reg                                                   : {}", fpga_ctl->readReg(512+415));
fmt::println("io.data_out.[fire]                                          : {}", fpga_ctl->readReg(512+416));
fmt::println("io.data_out.[fireLast]                                      : {}", fpga_ctl->readReg(512+417));
fmt::println("pkg_cnt                                                     : {}", fpga_ctl->readReg(512+418));
fmt::println("pkg_num                                                     : {}", fpga_ctl->readReg(512+419));
fmt::println("state_reg                                                   : {}", fpga_ctl->readReg(512+420));

//Report width 1:
fmt::println("fifo_c2h_cmd.io.out.valid                                   : {}", (fpga_ctl->readReg(512+421) >> 0) & 1);
fmt::println("fifo_c2h_cmd.io.out.ready                                   : {}", (fpga_ctl->readReg(512+421) >> 1) & 1);
fmt::println("fifo_h2c_cmd.io.out.valid                                   : {}", (fpga_ctl->readReg(512+421) >> 2) & 1);
fmt::println("fifo_h2c_cmd.io.out.ready                                   : {}", (fpga_ctl->readReg(512+421) >> 3) & 1);
fmt::println("fifo_c2h_data.io.out.valid                                  : {}", (fpga_ctl->readReg(512+421) >> 4) & 1);
fmt::println("fifo_c2h_data.io.out.ready                                  : {}", (fpga_ctl->readReg(512+421) >> 5) & 1);
fmt::println("fifo_h2c_data.io.in.valid                                   : {}", (fpga_ctl->readReg(512+421) >> 6) & 1);
fmt::println("fifo_h2c_data.io.in.ready                                   : {}", (fpga_ctl->readReg(512+421) >> 7) & 1);


    std::cout << "################################ p_h2c data BEGIN ################################" << std::endl;
    
    float timestamp,t0;
    int queue_length,queue_length1;
    
    t0 = swap_endian(p[2])/1000.0;

    for(int j=0;j<(length+63)/64;j++){
        queue_length = swap_endian(p[j*16+3]);
        timestamp = swap_endian(p[j*16+2])/1000.0;
        std::cout << "queue_length: " << queue_length << " timestamp: " << timestamp << std::endl; 

        // if(queue_length == 0){
        //     std::cout << "queue drain time" << (timestamp - t0) << std::endl; 
        //     break;
        // }

        // for(int i=15;i>=0;i--){
        //     std::cout << std::hex << std::setw(8) << std::setfill('0') << p[i+j*16];
        // }    
        // std::cout << std::endl;        
    }

}
