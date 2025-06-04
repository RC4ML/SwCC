#ifndef _QDMACONTROLLER_H_
#define _QDMACONTROLLER_H_

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <immintrin.h>
#include <rc4ml.h>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void init(uint8_t pci_bus, size_t bridge_bar_size);
void writeConfig(uint32_t index,uint32_t value,uint8_t pci_bus);
uint32_t readConfig(uint32_t index,uint8_t pci_bus);
void writeReg(uint32_t index,uint32_t value,uint8_t pci_bus);
uint32_t readReg(uint32_t index,uint8_t pci_bus);
void writeBridge(uint32_t index, uint64_t *value, uint8_t pci_bus);
void readBridge(uint32_t index, uint64_t *value, uint8_t pci_bus);
void* getBridgeAddr(uint8_t pci_bus);
void* getLiteAddr(uint8_t pci_bus);

void enableDebug();
void disableDebug();

#ifdef __cplusplus
}
#endif

#endif
