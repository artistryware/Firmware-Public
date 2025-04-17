
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>
//#include <freertos/FreeRTOS.h>    // First called in qs_i2c.h
//#include <freertos/task.h>        // First called in qs_i2c.h

#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_bt_api.h>
#include <esp_bt_device.h>
#include <esp_spp_api.h>

#ifndef __BLUE_H__
#define __BLUE_H__

#ifdef __cplusplus
extern "C" {
#endif

// BL task notification handler
// note: declare this variable here as EXTERN
// note: if you did not decalre as extern this variable, 
// what will happen is everytime you include this header file on, 
// it will declare a different variable
extern TaskHandle_t btHandler;

// Declarations of bluetooth methods
void bt_init();
void blue_send(uint8_t*);

#ifdef __cplusplus
}
#endif

#endif