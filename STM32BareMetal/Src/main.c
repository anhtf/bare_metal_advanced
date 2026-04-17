#include "uart_dma.h"

uint8_t tx_data[] = "Hello DMA UART\r\n";
uint8_t rx_data[32];

volatile uint8_t g_tx_done = 0;
volatile uint8_t g_rx_done = 0;

void uart_dma_tx_done_callback(void)
{
    g_tx_done = 1;
}

void uart_dma_rx_done_callback(void)
{
    g_rx_done = 1;
}

int main(void)
{
    usart1_rx_tx_init(115200);
    dma2_usart1_init();

    usart1_dma_rx_start(rx_data, 5);
    usart1_dma_tx_start(tx_data, sizeof(tx_data) - 1);

    while (1)
    {
    }
}