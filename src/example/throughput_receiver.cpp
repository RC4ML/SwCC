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

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: sudo " << argv[0] << " PacketSize" << std::endl;
        return 1;
    }
    int length = std::atoi(argv[1]); // 将字符串转换为整数

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




    int qpn = 2;
    int remote_qpn = 1;
    int remote_ip = 0x08bda8c0;
    int remote_mac = 0x089d;
    int ip = 0x03bda8c0;
    int credit = 0;
    int rate = 11500;
    int devide_rate = 356;
    int cpu_start = 1;
    // int length = 2048;
    int qpn_num = 1;
    int msg_num_per_qpn = 10000;
    int tx_delay = 11;
    int alfa = 0x10000;
    uint64_t remote_vaddr = 0;
    int x;


    for(int i=0;i<1024*1024*512/4;i++){
      p[i] = 0;
    }

    fpga_ctl->writeReg(0, 1);
    sleep(1);
    fpga_ctl->writeReg(0, 0);


    fpga_ctl->writeReg(110, qpn);
    fpga_ctl->writeReg(111, remote_qpn);
    fpga_ctl->writeReg(112, remote_ip);
    fpga_ctl->writeReg(113, credit);
    fpga_ctl->writeReg(114, rate);
    fpga_ctl->writeReg(115, devide_rate);
    fpga_ctl->writeReg(116, remote_ip);
    fpga_ctl->writeReg(117, 0x02350a00);
    fpga_ctl->writeReg(118, remote_mac);    
    fpga_ctl->writeReg(120, ip);
    fpga_ctl->writeReg(121, cpu_start);
    fpga_ctl->writeReg(122, tx_delay);
    fpga_ctl->writeReg(123, (uint32_t) ((uint64_t) p >> 32));
    fpga_ctl->writeReg(124, (uint32_t) ((uint64_t) p));
    fpga_ctl->writeReg(125, qpn_num*msg_num_per_qpn);    

    fpga_ctl->writeReg(160, rate); 
    fpga_ctl->writeReg(161, 0); 
    fpga_ctl->writeReg(162, devide_rate); 
    fpga_ctl->writeReg(163, 0); 
    fpga_ctl->writeReg(164, 0); 
    fpga_ctl->writeReg(165, 0);
    fpga_ctl->writeReg(166, 0);
    fpga_ctl->writeReg(168, rate);
    fpga_ctl->writeReg(169, alfa);

    cpu_start = 0;
    std::cout << "After both the sender and receiver programs are started, enter any value to launch the receiver: " << std::endl;



    std::cin >> x;

    

    fpga_ctl->writeReg(121, cpu_start);

    fpga_ctl->writeReg(101, 0);
    sleep(1);
    fpga_ctl->writeReg(101, 1);
    sleep(1);
    fpga_ctl->writeReg(101, 0);

    // qpn = 2;
    // remote_ip = 0x08bda8c0;
    // remote_mac = 0x089d;
    // fpga_ctl->writeReg(110, qpn);
    // fpga_ctl->writeReg(111, remote_qpn);
    // fpga_ctl->writeReg(112, remote_ip);
    // fpga_ctl->writeReg(113, credit);
    // fpga_ctl->writeReg(114, rate);
    // fpga_ctl->writeReg(115, devide_rate);
    // fpga_ctl->writeReg(116, remote_ip);
    // fpga_ctl->writeReg(117, 0x02350a00);
    // fpga_ctl->writeReg(118, remote_mac); 

    // sleep(1);
    // fpga_ctl->writeReg(101, 1);
    // sleep(1);
    // fpga_ctl->writeReg(101, 0);

    fpga_ctl->writeReg(131, qpn_num);
    fpga_ctl->writeReg(132, 1);//write
    fpga_ctl->writeReg(137, length);//write
    fpga_ctl->writeReg(138, msg_num_per_qpn);//write
    fpga_ctl->writeReg(133, (uint32_t) ((uint64_t) p >> 32));
    fpga_ctl->writeReg(134, (uint32_t) ((uint64_t) p));
    fpga_ctl->writeReg(135, (uint32_t) ((uint64_t) remote_vaddr >> 32));
    fpga_ctl->writeReg(136, (uint32_t) ((uint64_t) remote_vaddr));
    fpga_ctl->writeReg(140, length*qpn_num*msg_num_per_qpn/64);



    // sleep(1);
    // fpga_ctl->writeReg(130, 0);
    // sleep(1);
    // fpga_ctl->writeReg(130, 1);
    // sleep(1);
    // fpga_ctl->writeReg(130, 0);

    while(1);




    // fmt::println("data_cnt                                   : {}", fpga_ctl->readReg(512+320));
    // fmt::println("timer_en                                   : {}", fpga_ctl->readReg(512+321));
    // fmt::println("timer_cnt                                  : {}", fpga_ctl->readReg(512+322));
    // fmt::println("timer_cnt                                  : {}", fpga_ctl->readReg(512+322));

    // std::cout<< "speed: " << qpn_num*msg_num_per_qpn*length/(fpga_ctl->readReg(512+322)*4.444444444)<< std::endl; 

    // fmt::println("timer                                  : {}", (fpga_ctl->readReg(512+322))*4.4444444444444/1000);



// //Report width 32:
// fmt::println("tlb.io.tlb_miss_count                                       : {}", fpga_ctl->readReg(512+400));
// fmt::println("io.c2h_cmd.[fire]                                           : {}", fpga_ctl->readReg(512+401));
// fmt::println("io.h2c_cmd.[fire]                                           : {}", fpga_ctl->readReg(512+402));
// fmt::println("io.c2h_data.[fire]                                          : {}", fpga_ctl->readReg(512+403));
// fmt::println("io.h2c_data.[fire]                                          : {}", fpga_ctl->readReg(512+404));
// fmt::println("fifo_c2h_cmd.io.out.[fire]                                  : {}", fpga_ctl->readReg(512+405));
// fmt::println("fifo_h2c_cmd.io.out.[fire]                                  : {}", fpga_ctl->readReg(512+406));
// fmt::println("fifo_c2h_data.io.out.[fire]                                 : {}", fpga_ctl->readReg(512+407));
// fmt::println("fifo_h2c_data.io.in.[fire]                                  : {}", fpga_ctl->readReg(512+408));
// fmt::println("io.s_tx_meta.[fire]                                         : {}", fpga_ctl->readReg(512+409));
// fmt::println("io.m_mem_read_cmd.[fire]                                    : {}", fpga_ctl->readReg(512+410));
// fmt::println("io.s_mem_read_data.[fire]                                   : {}", fpga_ctl->readReg(512+411));
// fmt::println("io.m_mem_write_cmd.[fire]                                   : {}", fpga_ctl->readReg(512+412));
// fmt::println("io.m_mem_write_data.[fire]                                  : {}", fpga_ctl->readReg(512+413));
// fmt::println("io.m_net_tx_data.[fire]                                     : {}", fpga_ctl->readReg(512+414));
// fmt::println("io.s_net_rx_data.[fire]                                     : {}", fpga_ctl->readReg(512+415));
// fmt::println("cus_head_proc.io.rx_data_in.[fire]                          : {}", fpga_ctl->readReg(512+416));
// fmt::println("cus_head_proc.io.meta_in.[fire]                             : {}", fpga_ctl->readReg(512+417));
// fmt::println("cus_head_proc.io.rx_data_out.[fire]                         : {}", fpga_ctl->readReg(512+418));
// fmt::println("cus_head_proc.io.meta_out.[fire]                            : {}", fpga_ctl->readReg(512+419));
// fmt::println("rx_dispatch.io.meta_in.[fire]                               : {}", fpga_ctl->readReg(512+420));
// fmt::println("rx_dispatch.io.conn_req.[fire]                              : {}", fpga_ctl->readReg(512+421));
// fmt::println("rx_dispatch.io.conn_rsp.[fire]                              : {}", fpga_ctl->readReg(512+422));
// fmt::println("rx_dispatch.io.cc_meta_out.[fire]                           : {}", fpga_ctl->readReg(512+423));
// fmt::println("rx_dispatch.io.event_meta_out.[fire]                        : {}", fpga_ctl->readReg(512+424));
// fmt::println("rx_dispatch.state_reg                                       : {}", fpga_ctl->readReg(512+425));
// fmt::println("rx_cc.io.cc_meta_in.[fire]                                  : {}", fpga_ctl->readReg(512+426));
// fmt::println("rx_cc.io.cc_state_in.[fire]                                 : {}", fpga_ctl->readReg(512+427));
// fmt::println("rx_cc.io.cc_req.[fire]                                      : {}", fpga_ctl->readReg(512+428));
// fmt::println("rx_cc.io.cc_meta_out.[fire]                                 : {}", fpga_ctl->readReg(512+429));
// fmt::println("rx_cc.io.cc_meta_in.[fire]                                  : {}", fpga_ctl->readReg(512+430));
// fmt::println("handle_tx.io.app_meta_in.[fire]                             : {}", fpga_ctl->readReg(512+431));
// fmt::println("handle_tx.io.pkg_meta_in.[fire]                             : {}", fpga_ctl->readReg(512+432));
// fmt::println("handle_tx.io.cc_meta_in.[fire]                              : {}", fpga_ctl->readReg(512+433));
// fmt::println("handle_tx.io.priori_meta_out.[fire]                         : {}", fpga_ctl->readReg(512+434));
// fmt::println("handle_tx.io.event_meta_out.[fire]                          : {}", fpga_ctl->readReg(512+435));
// fmt::println("tx_cc.io.cc_meta_in.[fire]                                  : {}", fpga_ctl->readReg(512+436));
// fmt::println("tx_cc.io.cc_state_in.[fire]                                 : {}", fpga_ctl->readReg(512+437));
// fmt::println("tx_cc.io.cc_req.[fire]                                      : {}", fpga_ctl->readReg(512+438));
// fmt::println("tx_cc.io.cc_meta_out.[fire]                                 : {}", fpga_ctl->readReg(512+439));
// fmt::println("tx_cc.state_reg                                             : {}", fpga_ctl->readReg(512+440));
// fmt::println("tx_dispatch.io.meta_in.[fire]                               : {}", fpga_ctl->readReg(512+441));
// fmt::println("tx_dispatch.io.conn_req.[fire]                              : {}", fpga_ctl->readReg(512+442));
// fmt::println("tx_dispatch.io.event_meta_out.[fire]                        : {}", fpga_ctl->readReg(512+443));
// fmt::println("user_add.io.meta_in.[fire]                                  : {}", fpga_ctl->readReg(512+444));
// fmt::println("user_add.io.data_in.[fire]                                  : {}", fpga_ctl->readReg(512+445));
// fmt::println("user_add.io.meta_out.[fire]                                 : {}", fpga_ctl->readReg(512+446));
// fmt::println("user_add.io.reth_data_out.[fire]                            : {}", fpga_ctl->readReg(512+447));
// fmt::println("user_add.io.aeth_data_out.[fire]                            : {}", fpga_ctl->readReg(512+448));
// fmt::println("user_add.io.raw_data_out.[fire]                             : {}", fpga_ctl->readReg(512+449));
// fmt::println("head_add.io.meta_in.[fire]                                  : {}", fpga_ctl->readReg(512+450));
// fmt::println("head_add.io.reth_data_in.[fire]                             : {}", fpga_ctl->readReg(512+451));
// fmt::println("head_add.io.aeth_data_in.[fire]                             : {}", fpga_ctl->readReg(512+452));
// fmt::println("head_add.io.raw_data_in.[fire]                              : {}", fpga_ctl->readReg(512+453));
// fmt::println("head_add.io.tx_data_out.[fire]                              : {}", fpga_ctl->readReg(512+454));
// fmt::println("head_add.io.conn_state.[fire]                               : {}", fpga_ctl->readReg(512+455));
// fmt::println("conn_table.io.rx2conn_req.[fire]                            : {}", fpga_ctl->readReg(512+456));
// fmt::println("conn_table.io.tx2conn_req.[fire]                            : {}", fpga_ctl->readReg(512+457));
// fmt::println("conn_table.io.conn_init.[fire]                              : {}", fpga_ctl->readReg(512+458));
// fmt::println("conn_table.io.conn2tx_rsp.[fire]                            : {}", fpga_ctl->readReg(512+459));
// fmt::println("conn_table.io.conn2rx_rsp.[fire]                            : {}", fpga_ctl->readReg(512+460));
// fmt::println("cc_table.io.rx2cc_req.[fire]                                : {}", fpga_ctl->readReg(512+461));
// fmt::println("cc_table.io.tx2cc_req.[fire]                                : {}", fpga_ctl->readReg(512+462));
// fmt::println("cc_table.io.cc_init.[fire]                                  : {}", fpga_ctl->readReg(512+463));
// fmt::println("cc_table.io.cc2tx_rsp.[fire]                                : {}", fpga_ctl->readReg(512+464));
// fmt::println("cc_table.io.cc2rx_rsp.[fire]                                : {}", fpga_ctl->readReg(512+465));
// fmt::println("io.m_mem_write_cmd.[fire]                                   : {}", fpga_ctl->readReg(512+466));
// fmt::println("io.m_mem_write_data.[fire]                                  : {}", fpga_ctl->readReg(512+467));
// fmt::println("io.cpuReq.[fire]                                            : {}", fpga_ctl->readReg(512+468));
// fmt::println("io.memData.[fire]                                           : {}", fpga_ctl->readReg(512+469));
// fmt::println("io.m_mem_read_cmd.[fire]                                    : {}", fpga_ctl->readReg(512+470));
// fmt::println("io.s_mem_read_data.[fire]                                   : {}", fpga_ctl->readReg(512+471));
// fmt::println("io.s_mac_rx.[fire]                                          : {}", fpga_ctl->readReg(512+472));
// fmt::println("io.m_mac_tx.[fire]                                          : {}", fpga_ctl->readReg(512+473));
// fmt::println("io.s_ip_tx.[fire]                                           : {}", fpga_ctl->readReg(512+474));
// fmt::println("io.m_roce_rx.[fire]                                         : {}", fpga_ctl->readReg(512+475));
// fmt::println("arp.arp_table.io.arpinsert.[fire]                           : {}", fpga_ctl->readReg(512+476));
// fmt::println("arp.arp_table.io.arpinsert1.[fire]                          : {}", fpga_ctl->readReg(512+477));
// fmt::println("arp.arp_table.io.arp_req1.[fire]                            : {}", fpga_ctl->readReg(512+478));
// fmt::println("arp.arp_table.io.arp_rsp1.[fire]                            : {}", fpga_ctl->readReg(512+479));
// fmt::println("arp.arp_table.io.arp_req2.[fire]                            : {}", fpga_ctl->readReg(512+480));
// fmt::println("arp.arp_table.io.arp_rsp2.[fire]                            : {}", fpga_ctl->readReg(512+481));
// fmt::println("io.bucketValue<REG_BUCKET_VALUE>                            : {}", fpga_ctl->readReg(512+482));

// //Report width 1:
// fmt::println("fifo_c2h_cmd.io.out.valid                                   : {}", (fpga_ctl->readReg(512+483) >> 0) & 1);
// fmt::println("fifo_c2h_cmd.io.out.ready                                   : {}", (fpga_ctl->readReg(512+483) >> 1) & 1);
// fmt::println("fifo_h2c_cmd.io.out.valid                                   : {}", (fpga_ctl->readReg(512+483) >> 2) & 1);
// fmt::println("fifo_h2c_cmd.io.out.ready                                   : {}", (fpga_ctl->readReg(512+483) >> 3) & 1);
// fmt::println("fifo_c2h_data.io.out.valid                                  : {}", (fpga_ctl->readReg(512+483) >> 4) & 1);
// fmt::println("fifo_c2h_data.io.out.ready                                  : {}", (fpga_ctl->readReg(512+483) >> 5) & 1);
// fmt::println("fifo_h2c_data.io.in.valid                                   : {}", (fpga_ctl->readReg(512+483) >> 6) & 1);
// fmt::println("fifo_h2c_data.io.in.ready                                   : {}", (fpga_ctl->readReg(512+483) >> 7) & 1);
// fmt::println("rx_dispatch.io.drop_meta_out.ready                          : {}", (fpga_ctl->readReg(512+483) >> 8) & 1);
// fmt::println("rx_dispatch.io.cc_meta_out.ready                            : {}", (fpga_ctl->readReg(512+483) >> 9) & 1);
// fmt::println("rx_dispatch.io.dma_meta_out.ready                           : {}", (fpga_ctl->readReg(512+483) >> 10) & 1);
// fmt::println("rx_dispatch.io.event_meta_out.ready                         : {}", (fpga_ctl->readReg(512+483) >> 11) & 1);
// fmt::println("ip_handler.route_by_eth1.<route_by_eth_overflow>[trigger]   : {}", (fpga_ctl->readReg(512+483) >> 12) & 1);
// fmt::println(".<iptest_rx_buffer_almostfull>[trigger]                     : {}", (fpga_ctl->readReg(512+483) >> 13) & 1);

// fmt::println("tx_latency                     : {}", fpga_ctl->readReg(512+300));
// fmt::println("tx_start_cnt                     : {}", fpga_ctl->readReg(512+301));
// fmt::println("tx_end_cnt                     : {}", fpga_ctl->readReg(512+302));
// fmt::println("net_latency                     : {}", fpga_ctl->readReg(512+303));
// fmt::println("net_start_cnt                     : {}", fpga_ctl->readReg(512+304));
// fmt::println("net_end_cnt                     : {}", fpga_ctl->readReg(512+305));
// fmt::println("rx_latency                     : {}", fpga_ctl->readReg(512+306));
// fmt::println("rx_start_cnt                     : {}", fpga_ctl->readReg(512+307));
// fmt::println("rx_end_cnt                     : {}", fpga_ctl->readReg(512+308));

//     std::cout << "################################ p_h2c data BEGIN ################################" << std::endl;
    
//     float timestamp,final_ts,base_ts=0,pre_timestamp=0;
//     int queue_length,queue_length1;
    
//     // t0 = swap_endian(p[1])/1000.0;

//     for(int j=0;j<qpn_num*msg_num_per_qpn;j++){
//         queue_length = swap_endian(p[j*16+2]);
//         timestamp = swap_endian(p[j*16+1])/1000.0;
//         if(timestamp <pre_timestamp){
//             base_ts = base_ts + 262.144;
//             final_ts = timestamp + base_ts;
//         }else{
//             final_ts = timestamp + base_ts;
//         }
//         std::cout << std::dec << "queue_length: " << queue_length << " timestamp: " << final_ts << std::endl; 

//         pre_timestamp = timestamp;
//         // if(queue_length == 0){
//         //     std::cout << "queue drain time" << (timestamp - t0) << std::endl; 
//         //     break;
//         // }

//         // for(int i=15;i>=0;i--){
//         //     std::cout << std::hex << std::setw(8) << std::setfill('0') << p[i+j*16];
//         // }    
//         // std::cout << std::endl;        
//     }

}
