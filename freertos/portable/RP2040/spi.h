/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
 */
#ifndef _SPI_H_
#define _SPI_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
//
#include <stdbool.h>
// Pico includes
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "pico/types.h"

#define SPI_FILL_CHAR (0xFF)

// "Class" representing SPIs
typedef struct {
    // SPI HW
    spi_inst_t *hw_inst;
    const uint miso_gpio;  // SPI MISO pin number for GPIO
    const uint mosi_gpio;
    const uint sck_gpio;
    const uint baud_rate;
    // State variables:
    uint tx_dma;
    uint rx_dma;
    dma_channel_config tx_dma_cfg;
    dma_channel_config rx_dma_cfg;
    irq_handler_t dma_isr;
	uint irq_num;
    bool initialized;         // Assigned dynamically
    TaskHandle_t owner;       // Assigned dynamically
    SemaphoreHandle_t mutex;  // Assigned dynamically
} spi_t;

// SPI DMA interrupts
void spi_irq_handler(spi_t *pSPI);

bool spi_transfer(spi_t *pSPI, const uint8_t *tx, uint8_t *rx, size_t length);
bool my_spi_init(spi_t *pSPI);
void spi_lock(spi_t* pSPI);
void spi_unlock(spi_t* pSPI);

#define USE_LED 1
#if USE_LED
#   define LED_PIN 25
#   include "hardware/gpio.h"
#   define LED_INIT()                       \
    {                                    \
        gpio_init(LED_PIN);              \
        gpio_set_dir(LED_PIN, GPIO_OUT); \
    }
#   define LED_ON() gpio_put(LED_PIN, 1)
#   define LED_OFF() gpio_put(LED_PIN, 0)
#else
#   define LED_ON()
#   define LED_OFF()
#   define LED_INIT()
#endif

#endif
/* [] END OF FILE */
