
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <esp_log.h>
//#include <freertos/FreeRTOS.h>    // First called in qs_i2c.h
//#include <freertos/task.h>        // First called in qs_i2c.h

#include "qs_blue.h"

#ifndef __LOGIC_H__
#define __LOGIC_H__


#ifdef __cplusplus
extern "C" {
#endif

// Definitions for player bit from I2C read
#define PLYR1(r)    ((*r & (1<<0)) >= 1)    // Player 1 Pushbutton pressed
#define PLYR2(r)    ((*r & (1<<1)) >= 1)    // Player 2 Pushbutton pressed
#define PLYR3(r)    ((*r & (1<<2)) >= 1)    // Player 3 Pushbutton pressed
#define PLYR4(r)    ((*r & (1<<3)) >= 1)    // Player 4 Pushbutton pressed
#define PLYR5(r)    ((*r & (1<<4)) >= 1)    // Player 5 Pushbutton pressed
#define PLYR6(r)    ((*r & (1<<5)) >= 1)    // Player 6 Pushbutton pressed

// Definitions for player lamp/indicator I2C values
// Data order - Register address, Port 0 value, Port 1 value
#define CLEARLAMPS  { REG_9555_OUTPUT_0, 0b00111111, 0b00000000 }
#define PLYR1FIRST  { REG_9555_OUTPUT_0, 0b00000001, 0b00000001 }
#define PLYR2FIRST  { REG_9555_OUTPUT_0, 0b00000010, 0b00000010 }
#define PLYR3FIRST  { REG_9555_OUTPUT_0, 0b00000100, 0b00000100 }
#define PLYR4FIRST  { REG_9555_OUTPUT_0, 0b00001000, 0b00001000 }
#define PLYR5FIRST  { REG_9555_OUTPUT_0, 0b00010000, 0b00010000 }
#define PLYR6FIRST  { REG_9555_OUTPUT_0, 0b00100000, 0b00100000 }

// Definitions for turning AC outputs ON/OFF
#define BUZZER_ON   gpio_set_level(13, 1)
#define BUZZER_OFF  gpio_set_level(13, 0)
// buzzer on time
#define BUZZTIME    1500

// Definition for BT notification value
#define RST_VAL     *((uint32_t*) "RST")

// Handles for external access to logic tasks
extern TaskHandle_t scanHandle;

// Declarations of logic methods
void logic_init();
void logic_task();
void scan_task();
void send_player();

#ifdef __cplusplus
}
#endif

#endif