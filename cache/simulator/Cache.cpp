#include"Cache.hpp"
#include<math.h>

static int hit_time_cost = 264;
static int miss_time_cost1 = 264;
static int miss_time_cost2 = 484;
static int writeback_time = 0;
// static int hit_time_cost = 264;
// static int miss_time_cost1 = 994;
// static int miss_time_cost2 = 1324;
// static int writeback_time = 264;
static u_int64_t last_addr;
static u_int64_t time_cost_x10;

Set::Set(int block_num,int block_offset,int index_len,Replacement* replacer) {
    size_t size = 1<<block_offset;
   
    blocks = new Block*[block_num];
    for(int i = 0; i < block_num; i++) {
        blocks[i] = new Block(size,block_offset,index_len);
    }
    this->block_num = block_num;
    this->block_offset = block_offset;
    this->index_len = index_len;
    this->replacer = replacer; 
}

Set::~Set() {
    for(int i = 0; i < block_num; i++) {
        delete blocks[i];
    }
    delete[] blocks;
}

void* Set::read(u_int64_t addr) {
    u_int64_t tag = addr >> (index_len + block_offset);
    for(int i = 0; i < block_num; i++) {
        if(blocks[i]->is_hit(tag)) {
            record(READ_HIT);
            replacer->update(1,i,blocks);
            time_cost_x10 += hit_time_cost;
            return blocks[i]->get_data();
        }
    }
    record(READ_MISS);
    int victim = replacer->get_victim(blocks);
    blocks[victim]->fetch(addr);
    replacer->update(1,victim,blocks);
    if(std::max(last_addr,addr) - std::min(last_addr,addr) >= 100){
        time_cost_x10 += miss_time_cost2;
    }else{
        time_cost_x10 += miss_time_cost1;
    }
    return blocks[victim]->get_data();
}

void Set::write(u_int64_t addr, void* data) {
    u_int64_t tag = addr >> (index_len + block_offset);
    for(int i = 0; i < block_num; i++) {
        if(blocks[i]->is_hit(tag)) {
            record(WRITE_HIT);
            replacer->update(1,i,blocks);
            blocks[i]->write(data);
            return;
        }
    }
    record(WRITE_MISS);
    int victim = replacer->get_victim(blocks);
    if(blocks[victim]->is_valid()) {
        time_cost_x10 += writeback_time;
    }
    blocks[victim]->fetch(addr);
    blocks[victim]->write(data);
    replacer->update(1,victim,blocks);
}

void Set::static_update() {
    replacer->update(0,0,blocks);
}

void Set::print_snapshot() {
    std::cout << "*************************************"<<std::endl;
    std::cout << "           |valid | drity| tag | lru | data |"<<std::endl;
    for(int i = 0; i < block_num; i++) {
        std::cout << "block " << i << ":  ";
        blocks[i]->print_snapshot();
    }
}

void Set::record(OP_STATE state) {
    switch(state) {
        case READ_HIT:
            read_count++;
            read_hit_count++;
            break;
        case WRITE_HIT:
            write_count++;
            write_hit_count++;
            break;
        case READ_MISS:
            read_count++;
            read_miss_count++;
            break;
        case WRITE_MISS:
            write_count++;
            write_miss_count++;
            break;
    }
}

void Set::print_stats() {
    std::cout << "read_count: " << read_count << std::endl;
    std::cout << "write_count: " << write_count << std::endl;
    std::cout << "read_hit_count: " << read_hit_count << std::endl;
    std::cout << "write_hit_count: " << write_hit_count << std::endl;
    std::cout << "read_miss_count: " << read_miss_count << std::endl;
    std::cout << "write_miss_count: " << write_miss_count << std::endl;
}

Cache::Cache(int size, int block_size, int associativity, int write_policy, int write_miss_policy, int hit_latency, int miss_latency) {
    Replacement* replacer ;
    //if(replace_strategy == "LRU") {
        replacer = new LRU(associativity);
    // }else{
    //     replacer = new LRU(associativity);
    // }
    int set_num = size / (block_size * associativity);
    last_addr =0;
    time_cost_x10 = 0;
    int block_num = associativity;
    int block_offset = std::log2(block_size);
    //std::cout << "block_offset: " << block_offset << std::endl;
    //std::cout << "block_size: " << block_size << std::endl;

    int index_len = std::log2(set_num);
    //std::cout << "index_len: " << index_len << std::endl;
    sets = new Set*[set_num];
    for(int i = 0; i < set_num; i++) {
        sets[i] = new Set(block_num,block_offset,index_len,replacer);
    }
    this->size = size;
    this->set_num = set_num;
    this->block_size = block_size;
    this->associativity = associativity;
    this->write_policy = write_policy;  //write through or write back
    this->write_miss_policy = write_miss_policy; //write allocate or write no allocate
    this->hit_latency = hit_latency;
    this->miss_latency = miss_latency;
}

Cache::~Cache() {
    for(int i = 0; i < set_num; i++) {
        delete sets[i];
    }
    delete[] sets;
}

void* Cache::read(u_int64_t addr) {
    u_int64_t index = (addr >> (int)(std::log2(block_size))) & (set_num - 1);
    void* data;
    for(int i = 0; i < set_num; i++) {
        if(i == index) {
            data = sets[i]->read(addr);
        }
        else{
            sets[i]->static_update();
        }
    }
    last_addr = addr;
    return data;
}

void Cache::write(u_int64_t addr, void* data) {
    u_int64_t index = (addr >> (int)(std::log2(block_size))) & (set_num - 1);
    for(int i = 0; i < set_num; i++) {
        if(i == index) {
            sets[i]->write(addr, data);
            return;
        }
        else{
            sets[i]->static_update();
        }
    }
}

void Cache::print_snapshot() {
    for(int i = 0; i < set_num; i++) {
        std::cout << "*************************************"<<std::endl;
        std::cout << "Set " << i << ":"<<std::endl;
        
        sets[i]->print_snapshot();
    }
}

void Cache::print_stats() {
    u_int64_t total_read_count = 0;
    u_int64_t total_write_count = 0;
    u_int64_t total_read_hit_count = 0;
    u_int64_t total_write_hit_count = 0;
    u_int64_t total_read_miss_count = 0;
    u_int64_t total_write_miss_count = 0;
    
    for(int i = 0; i < set_num; i++) {
        //std::cout << "set "<<i<<":"<<std::endl;
        //sets[i]->print_stats();
        total_read_count += sets[i]->read_count;
        total_write_count += sets[i]->write_count;
        total_read_hit_count += sets[i]->read_hit_count;
        total_write_hit_count += sets[i]->write_hit_count;
        total_read_miss_count += sets[i]->read_miss_count;
        total_write_miss_count += sets[i]->write_miss_count;
    }
    //std::cout << "total_read_count: " << total_read_count << std::endl;
    //std::cout << "total_write_count: " << total_write_count << std::endl;
    //std::cout << "total_read_hit_count: " << total_read_hit_count << std::endl;
    //std::cout << "total_write_hit_count: " << total_write_hit_count << std::endl;
    //std::cout << "total_read_miss_count: " << total_read_miss_count << std::endl;
    //std::cout << "total_write_miss_count: " << total_write_miss_count << std::endl;
    std::cout << "hit rate: " << (double)(total_read_hit_count + total_write_hit_count) / (total_read_count + total_write_count) << std::endl;
    std::cout << "total time cost: " << time_cost_x10/10 << std::endl;
    //std::cout << "miss rate: " << (double)(total_read_miss_count + total_write_miss_count) / (total_read_count + total_write_count) << std::endl;
    //std::cout << "read hit rate: " << (double)total_read_hit_count / total_read_count << std::endl;
    //std::cout << "write hit rate: " << (double)total_write_hit_count / total_write_count << std::endl;
    //std::cout << "read miss rate: " << (double)total_read_miss_count / total_read_count << std::endl;
    //std::cout << "write miss rate: " << (double)total_write_miss_count / total_write_count << std::endl;
}
