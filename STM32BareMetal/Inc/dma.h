#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include "stm32f4xx.h"

void dma2_mem2mem_config(void);
void dma2_transfer_start(uint32_t src_address, uint32_t dst_address, uint16_t len);

#endif /* DMA_H */