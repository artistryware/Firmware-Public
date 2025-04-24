#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <sys/time.h>

#include <driver/gpio.h>

#include "qs_gpio.h"
#include "qs_i2c.h"
#include "qs_logic.h"

void app_main(void)
{
    // initialize io
    io_init();

    // initialize i2c
    i2c_init();

    // initialize bt
    bt_init();

    // create task when data receive from bluetooth
    xTaskCreate(&logic_task, "logic_task", 2048, NULL, 2, &btHandler);

    // create task for top and bottom shelf
    xTaskCreate(&scan_task, "scan_task", 2048, NULL, 3, &scanHandle);

    // small delay
    vTaskDelay(pdMS_TO_TICKS(500));

    // initialize the board
    logic_init();

    // this will automatic delete app_main task when return on this (app_main) function
}
