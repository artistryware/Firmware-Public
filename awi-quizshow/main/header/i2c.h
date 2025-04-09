#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <driver/gpio.h>
#include <driver/i2c_master.h>

#pragma once

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

// Definitions for the I2C master bus
#define SDA_IO_PIN                  21
#define SCL_IO_PIN                  22
#define MASTER_FREQUENCY            CONFIG_I2C_MASTER_FREQUENCY
#define I2C_MASTER_TIMEOUT_MS       100
#define PORT_NUMBER                 -1

// Handles for external access to I2C devices
extern i2c_master_dev_handle_t     dev1508Handle;
extern i2c_master_dev_handle_t     devUE101Handle;
extern i2c_master_dev_handle_t     devUE102Handle;

// Definitions for the I2C devices
#define I2C_1508_ADDR               0x0020
#define I2C_UE101_ADDR              0x0024
#define I2C_UE102_ADDR              0x0025
#define I2C_DEVICE_SPEED            100000

// LED output bits on sx1508
#define ORG     0
#define RED     1
#define GRN     2
#define BLU     3

// Declarations of I2C methods
void i2c_init(void);
void i2c_write(i2c_master_dev_handle_t, uint8_t[], uint8_t);
void i2c_write_read(i2c_master_dev_handle_t, uint8_t[], uint8_t, uint8_t[], uint8_t);
void i2c_write_bit(i2c_master_dev_handle_t, uint8_t, uint8_t, bool);

// Definitions for the PCAL9555 registers
#define REG_9555_INPUT_0        0x00
#define REG_9555_INPUT_1        0x01
#define REG_9555_OUTPUT_0       0x02
#define REG_9555_OUTPUT_1       0x03
#define REG_9555_POLARITY_0     0x04
#define REG_9555_POLARITY_1     0x05
#define REG_9555_CONFIG_0       0x06
#define REG_9555_CONFIG_1       0x07
#define REG_9555_IMASK_0        0x4a
#define REG_9555_IMASK_1        0x4b
#define REG_9555_ISTATUS_0      0x4c
#define REG_9555_ISTATUS_1      0x4d

// Definitions for the SX1508B registers


#ifdef __cplusplus
}
#endif

#endif