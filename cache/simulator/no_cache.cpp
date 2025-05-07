#include <iostream>
#include <gflags/gflags.h>
#include<random>

u_int64_t QPnumber;
uint64_t read_num;



DEFINE_uint64(QPnumber, 1000000, "QPnumber");
DEFINE_uint64(read_num, 1000000, "read_num");


int main() {
    std::random_device rd;
    QPnumber = FLAGS_QPnumber;
    read_num = FLAGS_read_num;
    u_int64_t last_random =0;
    u_int64_t totaltime = 0;
    u_int64_t random1;
        for(int i = 0; i < read_num; i++) {
            random1 = (rd()%QPnumber);
            if(std::max(random1,last_random)-std::min(random1,last_random)>=200){
                totaltime = totaltime + 106;
            }else{
                totaltime = totaltime + 73;
            }
                
        }
    std::cout<<"Total time: "<<totaltime<<std::endl;
    return 0;
}