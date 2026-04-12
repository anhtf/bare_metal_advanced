#include "adc_dma.h"
#include "stm32f407xx.h"

/*
Need configure GPIO, DMA, ADC
ADC connect to APB2; GPIO connect to AHB1

DMA2 request mapping table show: DMA2-Stream0-Channel0 is ADC1
*/

#define GPIO_A_ENABLE       (1U << 0)
#define ADC_1_ENABLE        (1U<<8)
#define CR1_SCAN            (1U<<8)
#define CR2_DMA             (1U<<8)
#define CR2_DDS             (1U<<9)
#define CR2_CONT            (1U<<1)
#define CR2_ADC_ENABLE      (1U<<0)
#define CR2_SW_START        (1U<<30)

#define DMA2EN              (1U<<22)
#define DMA_SCR_EN          (1U<<0)
#define DMA_SCR_MEM_INCRE   (1U<<10)
#define DMA_SCR_PER_INCRE   (1U<<9)
#define DMA_SCR_TCIE        (1U<<4)
#define DMA_SCR_TEIE        (1U<<2)
#define DMA_SCR_DMDIS       (1U<<2)
#define DMA_SCR_CIRC        (1U<<8)


#define NUMER_OF_CHANNELS 2
uint16_t adc_raw[NUMER_OF_CHANNELS];
void adc_dma_init(void)
{
     /*************GPIO Configuration**********/
     /*Enable clock access to ADC GPIO Pin's Port*/
     RCC->AHB1ENR |= GPIO_A_ENABLE;

     /*Set PA0 and PA1 mode to analog mode*/
     //MODER = 11
     GPIOA->MODER |= (1U << 0);
     GPIOA->MODER |= (1U << 1);

     GPIOA->MODER |= (1U << 2);
     GPIOA->MODER |= (1U << 3);

    /*************ADC Configuration**********/
    /*Enable clock access to ADC*/
    RCC->APB2ENR  |= ADC_1_ENABLE;

    /*Set sequence length*/ //2 conversion
    ADC1->SQR1 |= (1U<<20);
    ADC1->SQR1 &= ~(1U<<21);
    ADC1->SQR1 &= ~(1U<<22);
    ADC1->SQR1 &= ~(1U<<23);
    /*Set sequence */
    ADC1->SQR3  = (0U<<0) | (1U<<5);
    /*Enable scan mode*/
    ADC1->CR1 = CR1_SCAN;
    /*Select to use DMA*/
    ADC1->CR2  |= CR2_CONT | CR2_DMA |CR2_DDS;

    /*************DMA Configuration**********/
    /*Enable clock access to DMA*/
    RCC->AHB1ENR |= DMA2EN;
    /*Disable DMA stream*/
    DMA2_Stream0->CR &= ~DMA_SCR_EN;
    /*Wait till DMA disable*/
    while ((DMA2_Stream0->CR & DMA_SCR_EN)) {
    
    }

    /*Enable Circular mode*/
    DMA2_Stream0->CR |= DMA_SCR_CIRC;
    /*Set mem transfer size*/
    DMA2_Stream0->CR |= (1U<<13);
    DMA2_Stream0->CR &= ~(1U<<14);

    /*Set peripheral transfer size*/
    DMA2_Stream0->CR |= (1U<<11);
    DMA2_Stream0->CR &= ~(1U<<12);

    /*Enable mem increment*/
    DMA2_Stream0->CR |= DMA_SCR_MEM_INCRE;

    /*Set peripheral address*/\
    DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));
    /*Set mem address*/
    DMA2_Stream0->M0AR = (uint32_t)(&adc_raw);
    /*Set number of transfer*/
    DMA2_Stream0->NDTR = (uint16_t)NUMER_OF_CHANNELS;
    /*Enable DMA Stream*/
    DMA2_Stream0->CR |= DMA_SCR_EN;

    /*************ADC Configuration**********/
    /*Enable ADC*/
    ADC1->CR2 |= CR2_ADC_ENABLE;

    /*Start ADC*/
    ADC1->CR2 |= CR2_SW_START;
}