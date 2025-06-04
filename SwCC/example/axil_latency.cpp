#include <QDMAController.h>
#include "load.hpp"
#include <unistd.h>
#include <cstdio>

int main(void) {
    using namespace std;
    init(0x1a);
    void *qdmaBuffer = qdma_alloc(1024L * 1024 * 1024);
    uint32_t *H2CBuffer = (uint32_t * )(qdmaBuffer);
    volatile uint32_t *C2HBuffer = (volatile uint32_t *) (H2CBuffer + 128L * 1024 * 1024);
    volatile uint32_t *axilReg = (volatile uint32_t *) (getLiteAddr());

    if (!startFpgaH2C(H2CBuffer, axilReg) || !startFpgaC2H(C2HBuffer, axilReg)) {
        printCounters();
        resetCounters();
        cerr << "Aborted!" << endl;
        sleep(1);
        return 1;
    }
    printCounters();
    resetCounters();
    sleep(1);

    // AXIL Benchmark
    cerr << "Axil Benchmark Begin!" << endl;

    uint32_t h2cTimeBegin, h2cTimeEnd, c2hTimeBegin, c2hTimeEnd, h2cTimeCount, c2hTimeCount;
    uint32_t h2cCommands, c2hCommands;
    double h2cBandwidth, c2hBandwidth;
    // read delay with no load
    h2cTimeBegin = axilReg[H2C_TIME_COUNT];
    c2hTimeBegin = axilReg[C2H_TIME_COUNT];
    auto readDelayWithNoLoad = axilReadBenchmark(axilReg);
    h2cTimeEnd = axilReg[H2C_TIME_COUNT];
    c2hTimeEnd = axilReg[C2H_TIME_COUNT];
    h2cCommands = axilReg[H2C_COMMANDS_CHECK];
    c2hCommands = axilReg[C2H_COMMANDS_CHECK];
    h2cTimeCount = h2cTimeEnd - h2cTimeBegin;
    c2hTimeCount = c2hTimeEnd - c2hTimeBegin;
    h2cBandwidth = h2cCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * h2cCommands /
                                          (1.0 * h2cTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    c2hBandwidth = c2hCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * c2hCommands /
                                          (1.0 * c2hTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    fprintf(stderr, "Average Read Delay With No Load: %.2lfns ==> [QDMA]: H2C: %.2lfGBps  C2H: %.2lfGBps\n",
            readDelayWithNoLoad / 512.0, h2cBandwidth, c2hBandwidth);
    printCounters();
    resetCounters();
    sleep(1);


    // read with qdma H2C Load
    h2cTimeBegin = axilReg[H2C_TIME_COUNT];
    c2hTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeFpgaH2C(axilReg);
    auto readDelayWithH2CLoad = axilReadBenchmark(axilReg);
    pauseFpgaH2C(axilReg);
    sleep(2);
    h2cTimeEnd = axilReg[H2C_TIME_COUNT];
    c2hTimeEnd = axilReg[C2H_TIME_COUNT];
    h2cCommands = axilReg[H2C_COMMANDS_CHECK];
    c2hCommands = axilReg[C2H_COMMANDS_CHECK];
    h2cTimeCount = h2cTimeEnd - h2cTimeBegin;
    c2hTimeCount = c2hTimeEnd - c2hTimeBegin;
    h2cBandwidth = h2cCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * h2cCommands /
                                          (1.0 * h2cTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    c2hBandwidth = c2hCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * c2hCommands /
                                          (1.0 * c2hTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    fprintf(stderr, "Average Read Delay With H2C Load: %.2lfns ==> [QDMA]: H2C: %.2lfGBps  C2H: %.2lfGBps\n",
            readDelayWithH2CLoad / 512.0, h2cBandwidth, c2hBandwidth);
    printCounters();
    resetCounters();
    sleep(1);




    // read with qdma write load
    h2cTimeBegin = axilReg[H2C_TIME_COUNT];
    c2hTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeFpgaC2H(axilReg);
    auto readDelayWithC2HLoad = axilReadBenchmark(axilReg);
    pauseFpgaC2H(axilReg);
    sleep(2);
    h2cTimeEnd = axilReg[H2C_TIME_COUNT];
    c2hTimeEnd = axilReg[C2H_TIME_COUNT];
    h2cCommands = axilReg[H2C_COMMANDS_CHECK];
    c2hCommands = axilReg[C2H_COMMANDS_CHECK];
    h2cTimeCount = h2cTimeEnd - h2cTimeBegin;
    c2hTimeCount = c2hTimeEnd - c2hTimeBegin;
    h2cBandwidth = h2cCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * h2cCommands /
                                          (1.0 * h2cTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    c2hBandwidth = c2hCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * c2hCommands /
                                          (1.0 * c2hTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    fprintf(stderr, "Average Read Delay With C2H Load: %.2lfns ==> [QDMA]: H2C: %.2lfGBps  C2H: %.2lfGBps\n",
            readDelayWithC2HLoad / 512.0, h2cBandwidth, c2hBandwidth);
    printCounters();
    resetCounters();
    sleep(1);



    // read with qdma read & write load
    h2cTimeBegin = axilReg[H2C_TIME_COUNT];
    c2hTimeBegin = axilReg[C2H_TIME_COUNT];
    resumeFpgaH2C(axilReg);
    resumeFpgaC2H(axilReg);
    auto readDelayWithH2CAndC2HLoad = axilReadBenchmark(axilReg);
    pauseFpgaC2H(axilReg);
    pauseFpgaH2C(axilReg);
    sleep(2);
    h2cTimeEnd = axilReg[H2C_TIME_COUNT];
    c2hTimeEnd = axilReg[C2H_TIME_COUNT];
    h2cCommands = axilReg[H2C_COMMANDS_CHECK];
    c2hCommands = axilReg[C2H_COMMANDS_CHECK];
    h2cTimeCount = h2cTimeEnd - h2cTimeBegin;
    c2hTimeCount = c2hTimeEnd - c2hTimeBegin;
    h2cBandwidth = h2cCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * h2cCommands /
                                          (1.0 * h2cTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    c2hBandwidth = c2hCommands == 0 ? 0 : 1.0 * DEFAULT_DATA_BYTE_LENGTH * c2hCommands /
                                          (1.0 * c2hTimeCount * 4 / 1000 / 1000 / 1000) / 1024 / 1024 / 1024;
    fprintf(stderr, "Average Read Delay With H2C And C2H Load: %.2lfns ==> [QDMA]: H2C: %.2lfGBps  C2H: %.2lfGBps\n",
            readDelayWithH2CAndC2HLoad / 512.0, h2cBandwidth, c2hBandwidth);
    printCounters();
    resetCounters();
    sleep(1);

    cerr << "Axil Benchmark Complete!" << endl;
}