#include <QDMAController.h>
#include "dma.hpp"

using namespace std;

int main() {
    h2c_benchmark_latency(0x43);
    c2h_benchmark_latency(0x43);
    concurrent_latency(0x43);
    return 0;
}