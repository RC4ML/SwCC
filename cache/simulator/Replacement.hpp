#include"Block.hpp"

class Replacement {
    public:
    virtual int get_victim(Block** block_array) = 0;
    virtual ~Replacement() {}
    virtual void update(bool if_lat_op,int last_index,Block** block_array) = 0;


};

class LRU : public Replacement {
    public:
    LRU(int block_num) {
        this->block_num = block_num;
        lru = new u_int32_t[block_num];
        for(int i = 0; i < block_num; i++) {
            lru[i] = i;
        }
    }
    ~LRU() {
        delete[] lru;
    }
    int get_victim(Block** block_array) {
        //find the max lru number
        int max = 0;
        u_int32_t index = 0;
        for(int i = 0; i < block_num; i++) {
            if(block_array[i]->get_lru() > max) {
                max = block_array[i]->get_lru();
                index = i;
            }
        }
        return index;
    }

    void update(bool if_lat_op,int last_index,Block** block_array) {
        if(if_lat_op) {
            for(int i = 0; i < block_num; i++) {
                //std::cout <<"block_num"<< block_num << std::endl;
                //if(block_array[i]->get_lru() < block_array[last_index]->get_lru()) {
                block_array[i]->set_lru(block_array[i]->get_lru() + 1);
                //}
            }
            block_array[last_index]->set_lru(0);
        }
        else {
            for(int i = 0; i < block_num; i++) {
                block_array[i]->set_lru(block_array[i]->get_lru() + 1);
            }
        }
    }
    private:
    int block_num;
    u_int32_t* lru;
};