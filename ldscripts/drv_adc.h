#pragma once

#include "board.h"
#include "stm32f10x_dma.h"
#define ADC_BATTERY     0
#define ADC_CURRENT     1

typedef struct drv_adc_config_t
{
    uint8_t powerAdcChannel;     // which channel used for current monitor, allowed PA1, PB1 (ADC_Channel_1, ADC_Channel_9)
} drv_adc_config_t;


void adcInit(drv_adc_config_t *init);
uint16_t adcGetChannel(uint8_t channel);
