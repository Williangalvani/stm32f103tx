//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"
#include "board.h"
#include "Timer.h"
#include "system_setup.h"
#include "usart.h"



// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// Definitions visible only within this translation unit.
namespace {
// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
constexpr Timer::ticks_t BLINK_ON_TICKS = Timer::FREQUENCY_HZ * 2 / 3;
constexpr Timer::ticks_t BLINK_OFF_TICKS = Timer::FREQUENCY_HZ - BLINK_ON_TICKS;
}

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef  ADC_InitStructure;

void stat_adc_stuff(){
	/* Configure PA.01 (ADC Channel1) as analog input -------------------------*/
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		  GPIO_Init(GPIOA, &GPIO_InitStructure);

		  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		  ADC_InitStructure.ADC_ScanConvMode = DISABLE; // Single Channel
		  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Scan on Demand
		  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		  ADC_InitStructure.ADC_NbrOfChannel = 1;

		  ADC_Init(ADC1, &ADC_InitStructure);

		  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

		  /* Enable ADC1 */
		  ADC_Cmd(ADC1, ENABLE);

		  /* Enable ADC1 reset calibaration register */
		  ADC_ResetCalibration(ADC1);


		  /* Check the end of ADC1 reset calibration register */
		  while(ADC_GetResetCalibrationStatus(ADC1));

		  /* Start ADC1 calibaration */
		  ADC_StartCalibration(ADC1);

		  /* Check the end of ADC1 calibration */
		  while(ADC_GetCalibrationStatus(ADC1));

		  /* Start ADC1 Software Conversion */
		  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

int read_pots(){
	int adc;
	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET) {


		adc = ADC_GetConversionValue(ADC1);


		/* Probably overkill */
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

		/* Start ADC1 Software Conversion */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
	return adc;
} // sourcer32@gmail.com



int main(int argc, char* argv[]) {
	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");


	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);

	Timer timer;
	timer.start();

	// Perform all necessary initialisations for the LED.
	//blinkLed.powerUp();
	systemInit();


    stat_adc_stuff();

	uint32_t seconds = 0;

	//Serial serial;
	//serial.init(115200,2);

	// Infinite loop
	while (1) {
		LED0_ON;
		timer.sleep(BLINK_ON_TICKS);
		LED0_OFF;
		timer.sleep(BLINK_OFF_TICKS);
		trace_printf("ADC %u\n",read_pots());
		++seconds;

		// Count seconds on the trace device.
		trace_printf("Second %u\n", seconds);
	}
	// Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
