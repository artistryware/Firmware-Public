
#include <driver/gpio.h>
#include "qs_gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

// IO Configuration
void io_init()
{
    // RESET_1508_PIN as Output - SX1508 Reset Pin
    gpio_reset_pin(RESET_1508_PIN);
    gpio_set_direction(RESET_1508_PIN, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(RESET_1508_PIN, GPIO_PULLUP_ONLY);
    //gpio_set_pull_mode(RESET_1508_PIN, GPIO_PULLUP_ENABLE);
    //gpio_set_pull_mode(RESET_1508_PIN, GPIO_PULLDOWN_DISABLE);
    gpio_set_level(RESET_1508_PIN, 1);

    // MCW_PIN as Output - AC Motor CW control
    gpio_reset_pin(AC1_PIN);
    gpio_set_direction(AC1_PIN, GPIO_MODE_OUTPUT);
    //gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_DISABLE);
    gpio_set_level(AC1_PIN, 0);

    //printf("Minimum free heap size: %ld bytes\n", esp_get_minimum_free_heap_size());
    // END ADDED

}