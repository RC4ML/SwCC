#include "Block.hpp"


Block::Block(size_t size, int block_offset, int index_len) {
    
    valid = 0;
    dirty = 0;
    tag = 0;
    data = malloc(size);
    lru = 0;
    this->size = size;
   
    this->block_offset = block_offset;
    this->index_len = index_len;
}

Block::~Block() {
    free(data);
}

void Block::write(void* data) {
    memcpy(this->data, data, size);
    set_dirty();
}

void* Block::get_data() {
    return data;
}

bool Block::is_valid() {
    return valid;
}

bool Block::is_dirty() {
    return dirty;
}

bool Block::is_hit(u_int64_t tag) {
    return this->tag == tag && valid;
}

void Block::set_dirty() {
    dirty = 1;
}

void Block::reset_dirty() {
    dirty = 0;
}

void Block::set_valid() {
    valid = 1;
}

void Block::reset_valid() {
    valid = 0;
}

void Block::set_lru(u_int32_t lru) {
    this->lru = lru;
}   

u_int32_t Block::get_lru() {
    return lru;
}

void Block::fetch(u_int64_t addr) {
    tag = addr >> (index_len + block_offset);
    reset_dirty();
    set_valid();
}

void Block::print_snapshot() {
    
    std::cout << " |  " << valid ;
    std::cout << "   | " << dirty ;
    std::cout << "    | " << tag ;
    std::cout << "   | "<<lru; 
    std::cout << "   | 0x" ;
    //std::cout << " |" << *((u_int8_t*)data) << "|"<<std::endl;
    //output  size  byte length of data int hex
    for(int i = size/2 -1; i >=0 ; i--) {
        std::cout << std::hex <<(uint) *((u_int8_t*)data +i ) ;
    }
    std::cout<< std::dec << " | " << std::endl;

}