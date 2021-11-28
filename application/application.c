#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOS_time.h"
#include "task.h"

#include "pico/multicore.h" // get_core_num()
#include "pico/stdlib.h"
#include "tusb.h"

#include "crash.h"
#include "stdio_cli.h"

static void prvLaunchRTOS() {}

void stdio_init(void) {
    gpio_init(PICO_DEFAULT_UART_RX_PIN);
    gpio_set_pulls(PICO_DEFAULT_UART_RX_PIN, 1, 0);
    sleep_ms(1);
    bool v1 = gpio_get(PICO_DEFAULT_UART_RX_PIN);
    gpio_set_pulls(PICO_DEFAULT_UART_RX_PIN, 0, 1);
    sleep_ms(1);
    bool v2 = gpio_get(PICO_DEFAULT_UART_RX_PIN);
    gpio_set_pulls(PICO_DEFAULT_UART_RX_PIN, 0, 0);
    if (v1 != v2) {
        stdio_usb_init();
        while (!tud_cdc_connected())
            sleep_ms(1000);
    } else {
        stdio_uart_init();
        getchar_timeout_us(1000);
    }
}

int main() {

    crash_handler_init();
    stdio_init();
    FreeRTOS_time_init();
    CLI_Start();

    printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    /* should never reach here */
    panic_unsupported();

    return 0;
}
