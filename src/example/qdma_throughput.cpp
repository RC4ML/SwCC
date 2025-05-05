#include "dma.hpp"

int main() {
    h2c_benchmark(0x43);
    c2h_benchmark(0x43);
    // benchmark_bridge_write();
    return 0;
}