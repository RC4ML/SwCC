#include <QDMAController.h>
#include "dma.hpp"

using namespace std;

int main() {
    h2c_benchmark_random(0x43);
    c2h_benchmark_random(0x43);
    concurrent_random(0x43);
    return 0;
}