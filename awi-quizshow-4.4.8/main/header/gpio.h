#pragma once

#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

// NEW BOARDS
/*
#define RESET_1508_PIN              4
#define INTR_1508_PIN               36
#define EXP_INTR_1                  25
#define EXP_INTR_2                  27
#define EXP_INTR_3                  26
#define EXP_INTR_4                  14
*/
// OLD BOARDS
#define RESET_1508_PIN              2
#define INTR_1508_PIN               4
#define EXP_INTR_1                  36
#define EXP_INTR_2                  39
#define EXP_INTR_3                  34
#define EXP_INTR_4                  35

#define AC1_PIN                     13
#define AC2_PIN                     12

void io_config(void);

#ifdef __cplusplus
}
#endif

#endif