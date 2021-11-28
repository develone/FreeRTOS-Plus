#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOS_time.h"
#include "task.h"

#include "pico/multicore.h" // get_core_num()
#include "pico/stdlib.h"

#include "crash.h"
#include "stdio_cli.h"

static void prvLaunchRTOS() {}

int main() {

    crash_handler_init();
    stdio_init_all();
    FreeRTOS_time_init();
    CLI_Start();

    printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    /* should never reach here */
    panic_unsupported();

    return 0;
}
