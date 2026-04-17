#ifndef UART_DMA_H
#define UART_DMA_H

#include "stm32f4xx.h"
#include <stdint.h>

void usart1_rx_tx_init(uint32_t baudrate);
void dma2_usart1_init(void);

void usart1_dma_tx_start(uint8_t *src, uint16_t len);
void usart1_dma_rx_start(uint8_t *dst, uint16_t len);

void uart_dma_tx_done_callback(void);
void uart_dma_rx_done_callback(void);

#endif