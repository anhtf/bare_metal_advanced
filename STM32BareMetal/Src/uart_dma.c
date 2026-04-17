#include "uart_dma.h"

#define CLK 16000000U

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
    return (uint16_t)((periph_clk + (baudrate / 2U)) / baudrate);
}

static void usart1_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
    USART1->BRR = compute_uart_bd(periph_clk, baudrate);
}

static void dma2_usart1_tx_str7_channel4_config(void)
{
    // Disable stream first
    DMA2_Stream7->CR &= ~(1U << 0);
    while (DMA2_Stream7->CR & (1U << 0)) {}

    // Clear all pending flags for Stream 7
    DMA2->HIFCR |= (1U << 27); // CTCIF7
    DMA2->HIFCR |= (1U << 25); // CHTIF7
    DMA2->HIFCR |= (1U << 24); // CTEIF7
    DMA2->HIFCR |= (1U << 26); // CDMEIF7
    DMA2->HIFCR |= (1U << 22); // CFEIF7

    // Peripheral address = USART1->DR
    DMA2_Stream7->PAR = (uint32_t)&USART1->DR;

    // Clear CR
    DMA2_Stream7->CR = 0;

    // Channel 4
    DMA2_Stream7->CR |= (4U << 25);

    // Memory-to-peripheral
    DMA2_Stream7->CR |= (1U << 6);
    DMA2_Stream7->CR &= ~(1U << 7);

    // Memory increment enable
    DMA2_Stream7->CR |= (1U << 10);

    // Peripheral increment disable
    DMA2_Stream7->CR &= ~(1U << 9);

    // Peripheral size = 8-bit
    DMA2_Stream7->CR &= ~(1U << 11);
    DMA2_Stream7->CR &= ~(1U << 12);

    // Memory size = 8-bit
    DMA2_Stream7->CR &= ~(1U << 13);
    DMA2_Stream7->CR &= ~(1U << 14);

    // Normal mode
    DMA2_Stream7->CR &= ~(1U << 8);

    // Transfer complete interrupt enable
    DMA2_Stream7->CR |= (1U << 4);

    // Optional: direct mode
    DMA2_Stream7->FCR = 0;
}

static void dma2_usart1_rx_str5_channel4_config(void)
{
    // Disable stream first
    DMA2_Stream5->CR &= ~(1U << 0);
    while (DMA2_Stream5->CR & (1U << 0)) {}

    // Clear all pending flags for Stream 5
    DMA2->HIFCR |= (1U << 11); // CTCIF5
    DMA2->HIFCR |= (1U << 9);  // CHTIF5
    DMA2->HIFCR |= (1U << 8);  // CTEIF5
    DMA2->HIFCR |= (1U << 10); // CDMEIF5
    DMA2->HIFCR |= (1U << 6);  // CFEIF5

    // Peripheral address = USART1->DR
    DMA2_Stream5->PAR = (uint32_t)&USART1->DR;

    // Clear CR
    DMA2_Stream5->CR = 0;

    // Channel 4
    DMA2_Stream5->CR |= (4U << 25);

    // Peripheral-to-memory
    DMA2_Stream5->CR &= ~(1U << 6);
    DMA2_Stream5->CR &= ~(1U << 7);

    // Memory increment enable
    DMA2_Stream5->CR |= (1U << 10);

    // Peripheral increment disable
    DMA2_Stream5->CR &= ~(1U << 9);

    // Peripheral size = 8-bit
    DMA2_Stream5->CR &= ~(1U << 11);
    DMA2_Stream5->CR &= ~(1U << 12);

    // Memory size = 8-bit
    DMA2_Stream5->CR &= ~(1U << 13);
    DMA2_Stream5->CR &= ~(1U << 14);

    // Normal mode
    DMA2_Stream5->CR &= ~(1U << 8);

    // Transfer complete interrupt enable
    DMA2_Stream5->CR |= (1U << 4);

    // Optional: direct mode
    DMA2_Stream5->FCR = 0;
}

void usart1_rx_tx_init(uint32_t baudrate)
{
    /* Configure UART GPIO pin */

    // Enable clock access GPIOA
    RCC->AHB1ENR |= (1U << 0);

    // PA9 -> AF mode
    GPIOA->MODER &= ~(3U << 18);
    GPIOA->MODER |=  (2U << 18);

    // PA10 -> AF mode
    GPIOA->MODER &= ~(3U << 20);
    GPIOA->MODER |=  (2U << 20);

    // PA9 AF7 = USART1_TX
    GPIOA->AFR[1] &= ~(0xFU << 4);
    GPIOA->AFR[1] |=  (7U << 4);

    // PA10 AF7 = USART1_RX
    GPIOA->AFR[1] &= ~(0xFU << 8);
    GPIOA->AFR[1] |=  (7U << 8);

    /* Configure UART module */

    // Enable clock access to USART1
    RCC->APB2ENR |= (1U << 4);

    // Set baudrate
    usart1_set_baudrate(CLK, baudrate);

    // Enable DMA for RX and TX
    USART1->CR3 |= (1U << 6); // DMAR
    USART1->CR3 |= (1U << 7); // DMAT

    // Enable TX and RX
    USART1->CR1 |= (1U << 3);
    USART1->CR1 |= (1U << 2);

    // Clear TC
    USART1->SR &= ~(1U << 6);

    // Optional: USART TC interrupt
    USART1->CR1 |= (1U << 6);

    // Enable USART
    USART1->CR1 |= (1U << 13);
}

// DMA2 Stream 7, Channel 4 -> USART1_TX
// DMA2 Stream 5, Channel 4 -> USART1_RX
void dma2_usart1_init(void)
{
    // DMA2 clock enable
    RCC->AHB1ENR |= (1U << 22);

    dma2_usart1_tx_str7_channel4_config();
    dma2_usart1_rx_str5_channel4_config();

    // Enable NVIC IRQ
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    NVIC_EnableIRQ(DMA2_Stream5_IRQn);
}

void usart1_dma_tx_start(uint8_t *src, uint16_t len)
{
    // Disable stream
    DMA2_Stream7->CR &= ~(1U << 0);
    while (DMA2_Stream7->CR & (1U << 0)) {}

    // Clear flags
    DMA2->HIFCR |= (1U << 27);
    DMA2->HIFCR |= (1U << 25);
    DMA2->HIFCR |= (1U << 24);
    DMA2->HIFCR |= (1U << 26);
    DMA2->HIFCR |= (1U << 22);

    DMA2_Stream7->M0AR = (uint32_t)src;
    DMA2_Stream7->NDTR = len;

    // Enable stream
    DMA2_Stream7->CR |= (1U << 0);
}

void usart1_dma_rx_start(uint8_t *dst, uint16_t len)
{
    // Disable stream
    DMA2_Stream5->CR &= ~(1U << 0);
    while (DMA2_Stream5->CR & (1U << 0)) {}

    // Clear flags
    DMA2->HIFCR |= (1U << 11);
    DMA2->HIFCR |= (1U << 9);
    DMA2->HIFCR |= (1U << 8);
    DMA2->HIFCR |= (1U << 10);
    DMA2->HIFCR |= (1U << 6);

    DMA2_Stream5->M0AR = (uint32_t)dst;
    DMA2_Stream5->NDTR = len;

    // Enable stream
    DMA2_Stream5->CR |= (1U << 0);
}

void DMA2_Stream7_IRQHandler(void)
{
    if (DMA2->HISR & (1U << 27))
    {
        // Clear TCIF7
        DMA2->HIFCR |= (1U << 27);

        // TX complete callback
        uart_dma_tx_done_callback();
    }
}

void DMA2_Stream5_IRQHandler(void)
{
    if (DMA2->HISR & (1U << 11))
    {
        // Clear TCIF5
        DMA2->HIFCR |= (1U << 11);

        // RX complete callback
        uart_dma_rx_done_callback();
    }
}