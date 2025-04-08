#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <driver/gpio.h>
#include <driver/i2c.h>

#pragma once

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_SDA_PIN                 21
#define I2C_SCL_PIN                 22
#define I2C_MASTER_NUM              0
#define I2C_MASTER_FREQ             100000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0 
#define I2C_MASTER_TIMEOUT_MS       100
#define I2C_MASTER_TIMEOUT          I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS


#define I2C_1508_ADD                0x20
#define I2C_UE101_ADD               0x24
#define I2C_UE102_ADD               0x25

// led output on sx1508
#define ORG     0
#define RED     1
#define GRN     2
#define BLU     3

void i2c_init(void);

void i2c_write(uint8_t, uint8_t[], uint8_t);

void i2c_write_read(uint8_t, uint8_t[], uint8_t, uint8_t[], uint8_t);

void i2c_write_bit(uint8_t, uint8_t, bool);

#ifdef __cplusplus
}
#endif

#endif