#pragma once

#include <stdint.h>

typedef volatile struct EXTI_s {
    uint32_t IMR;       // 0x00 - Interrupt Mask
    uint32_t EMR;       // 0x04 - Event Mask
    uint32_t RTSR;      // 0x08 - Rising Trigger
    uint32_t FTSR;      // 0x0C - Falling Trigger
    uint32_t SWIER;     // 0x10 - Software Interrupt
    uint32_t PR;        // 0x14 - Pending Register
} exti_t;

#define EXTI_IMR_IM0    (0x00000001)
#define EXTI_EMR_EM0    (0x00000001)
#define EXTI_RTSR_RT0   (0x00000001)
#define EXTI_FTSR_FT0   (0x00000001)
#define EXTI_PR_PR0     (0x00000001)