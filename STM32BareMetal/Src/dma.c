#include "dma.h"

/* RCC AHB1 peripheral clock enable */
#define DMA2EN             (1U << 22)

/* DMA stream control bits */
#define DMA_SxCR_EN        (1U << 0)
#define DMA_SxCR_TEIE      (1U << 2)
#define DMA_SxCR_TCIE      (1U << 4)
#define DMA_SxCR_DIR_0     (1U << 6)
#define DMA_SxCR_DIR_1     (1U << 7)
#define DMA_SxCR_PINC      (1U << 9)
#define DMA_SxCR_MINC      (1U << 10)
#define DMA_SxCR_PSIZE_0   (1U << 11)
#define DMA_SxCR_PSIZE_1   (1U << 12)
#define DMA_SxCR_MSIZE_0   (1U << 13)
#define DMA_SxCR_MSIZE_1   (1U << 14)

/* DMA FIFO control bits */
#define DMA_SxFCR_DMDIS    (1U << 2)
#define DMA_SxFCR_FTH_0    (1U << 0)
#define DMA_SxFCR_FTH_1    (1U << 1)

/* DMA2 Stream0 flags in LISR/LIFCR */
#define DMA_LISR_TCIF0     (1U << 5)
#define DMA_LISR_TEIF0     (1U << 3)
#define DMA_LIFCR_CTCIF0   (1U << 5)
#define DMA_LIFCR_CTEIF0   (1U << 3)
#define DMA_LIFCR_CDMEIF0  (1U << 2)
#define DMA_LIFCR_CFEIF0   (1U << 0)

static void dma2_stream0_disable(void)
{
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream0->CR & DMA_SxCR_EN)
    {
        /* wait until hardware clears EN */
    }
}

static void dma2_stream0_clear_flags(void)
{
    DMA2->LIFCR = DMA_LIFCR_CTCIF0 |
                  DMA_LIFCR_CTEIF0 |
                  DMA_LIFCR_CDMEIF0 |
                  DMA_LIFCR_CFEIF0;
}

void dma2_mem2mem_config(void)
{
    RCC->AHB1ENR |= DMA2EN;
    dma2_stream0_disable();

    DMA2_Stream0->CR = 0;
    DMA2_Stream0->FCR = 0;

    /* 1. Set Direction: Memory-to-Memory (10) */
    DMA2_Stream0->CR |= (1U << 7);  /* DIR[1] = 1 */
    DMA2_Stream0->CR &= ~(1U << 6); /* DIR[0] = 0 */

    /* 2. Set Data Size: 16-bit (01) cho cả Source và Destination */
    DMA2_Stream0->CR |= (1U << 11); /* PSIZE = 01 */
    DMA2_Stream0->CR |= (1U << 13); /* MSIZE = 01 */

    /* 3. Increment both Source and Destination */
    DMA2_Stream0->CR |= DMA_SxCR_PINC | DMA_SxCR_MINC;

    /* 4. Enable Interrupts */
    DMA2_Stream0->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;

    /* 5. Quan trọng: M2M yêu cầu bật FIFO */
    DMA2_Stream0->FCR |= (1U << 2); /* DMDIS = 1 (Direct mode disabled) */
    // Có thể để FTH = 00 (1/4 full) để dữ liệu đẩy đi nhanh hơn
    DMA2_Stream0->FCR &= ~(3U << 0); 

    dma2_stream0_clear_flags();
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void dma2_transfer_start(uint32_t src_address, uint32_t dst_address, uint16_t len)
{
    /* Stream must be disabled before reloading addresses/length */
    dma2_stream0_disable();

    /* Clear old flags before new transfer */
    dma2_stream0_clear_flags();

    /* Source and destination */
    DMA2_Stream0->PAR  = src_address;   /* source */
    DMA2_Stream0->M0AR = dst_address;   /* destination */

    /* Number of half-words to transfer */
    DMA2_Stream0->NDTR = len;

    /* Start DMA */
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}