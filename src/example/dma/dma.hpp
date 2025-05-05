#ifndef dma_hpp
#define dma_hpp

#include <cstdint>

void h2c_benchmark(uint8_t pci_bus);

void c2h_benchmark(uint8_t pci_bus);

void h2c_benchmark_random(uint8_t pci_bus);

void c2h_benchmark_random(uint8_t pci_bus);

void concurrent_random(uint8_t pci_bus);

void h2c_benchmark_latency(uint8_t pci_bus);

void c2h_benchmark_latency(uint8_t pci_bus);

void gpu_h2c_benchmark(uint8_t pci_bus);

void concurrent_latency(uint8_t pci_bus);

#endif