#include<iostream>
#include<string.h>




class Block {
    public:
    Block(size_t size, int block_offset, int index_len); 
    ~Block();
    void write(void* data); //todo add a function pointer to the replacement policy
    void* get_data();
    bool is_valid();
    bool is_dirty();
    bool is_hit(u_int64_t tag);
    void set_dirty();
    void reset_dirty();
    void set_valid();
    void reset_valid();
    void set_lru(u_int32_t lru);
    u_int32_t get_lru();
    void fetch(u_int64_t addr);
    void print_snapshot();
    private:
    size_t size;
    bool valid;
    bool dirty;
    u_int64_t tag;
    void* data;
    int block_offset;
    int index_len;
    u_int32_t lru;

};

