#include<iostream>
#include<string.h>
#include "Cache.hpp"
#include<fstream>
#include<random>
#include<ctime>
#include <gflags/gflags.h>
using namespace std;


int cache_size;
int block_size;
int associativity;
uint64_t read_num;
DEFINE_int32(cache_size, 32768, "cache_size");
DEFINE_int32(block_size, 64, "block_size");
DEFINE_int32(associativity, 1, "associativity");
DEFINE_uint64(read_num, 1000000, "read_num");


int main() {
    cache_size = FLAGS_cache_size;
    block_size = FLAGS_block_size;
    associativity = FLAGS_associativity;
    read_num = FLAGS_read_num;
    int a=0;
    
    std::string str = "LRU";
    
    //cache->read((u_int64_t)0);
    //cache->print_snapshot();
    char ch[64] ;
    ch[0] = 0x1a;
    ch[1] = 0x2b;
    // cache->write((u_int64_t)0,(void*)ch);
    // cache->write((u_int64_t)0x4000,(void*)ch);
    // cache->write((u_int64_t)0x8000,(void*)ch);
    // cache->write((u_int64_t)0xc000,(void*)ch);
    // cache->write((u_int64_t)0x10000,(void*)ch);
    // cache->write((u_int64_t)0x8000,(void*)ch);
    // cache->read((u_int64_t)0x10000);
    // cache->read((u_int64_t)0x10000);
    //read  from trace.txt and write/read to cache
    //FILE *fp;
 


   std::random_device rd;
    u_int64_t addr;

    //random read number interations

    u_int64_t * addr_array = new u_int64_t[1000];
    int random1;
    int last_random;
    for(int j=1 ; j<=128 ; j=j*2){
        std::cout<<"Associativity: "<<j<<std::endl;
        Cache* cache = new Cache(cache_size, block_size, j, 0, 0, 1, 10);
        for(int i = 0; i < read_num; i++) {
            random1 = (rd()%1000000);
            //std::cout<<"random number :"<<random1<<std::endl;
            cache->read(random1);
                
        }
        std::cout<<std::endl;
        cache->print_stats();
        delete cache;
    }
    
    
    
    //std::cout<<"Data: "<<*(int*)cache->read((u_int64_t)12345678)<<std::endl;
    return 0;
}

