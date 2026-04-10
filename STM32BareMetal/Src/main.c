/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : DMA2 Stream0 memory-to-memory example for STM32F407
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f4xx.h"
#include "dma.h"

uint16_t peripheral[5] = {12345, 23456, 34567, 45678, 56789};
uint16_t tmp[5];

volatile uint8_t g_completed_transfer = 0;
volatile uint8_t g_transfer_error     = 0;

int main(void)
{
    dma2_mem2mem_config();

    while (1)
    {
        g_completed_transfer = 0;
        g_transfer_error = 0;

        for (int i = 0; i < 5; i++) {
            peripheral[i]++;
        }

        dma2_transfer_start((uint32_t)peripheral, (uint32_t)tmp, 5);

        while ((g_completed_transfer == 0) && (g_transfer_error == 0));

        if (g_transfer_error) {
            break; 
        }
        
    }
}

void DMA2_Stream0_IRQHandler(void)
{
    /* Transfer complete */
    if (DMA2->LISR & (1U << 5))   /* TCIF0 */
    {
        g_completed_transfer = 1;
        DMA2->LIFCR |= (1U << 5); /* clear TCIF0 */
    }

    /* Transfer error */
    if (DMA2->LISR & (1U << 3))   /* TEIF0 */
    {
        g_transfer_error = 1;
        DMA2->LIFCR |= (1U << 3); /* clear TEIF0 */
    }
}