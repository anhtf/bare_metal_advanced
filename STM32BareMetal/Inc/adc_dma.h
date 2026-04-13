#ifndef __ADC__DMA_H__
#define __ADC__DMA_H__

#include "stdint.h"
#include "stm32f4xx.h"

#define NUMBER_OF_SAMPLES 10

#define LISR_TCIFO   (1U<<5)
#define LIFCR_CTCIFO (1U<<5)

#define LISR_TEIFO  (1U<<3)
#define LISR_CTEIFO (1U<<3)
void adc_dma_init(void);
void adc_tim_dma_init(void);

#endif //__ADC__DMA_H__