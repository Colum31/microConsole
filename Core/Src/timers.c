#include "timers.h"

#include "stm32f0xx_hal.h"

void setTimerPeriod(TIM_HandleTypeDef *timerToSet, double periodMs){

	long baseClockFreqMs = HAL_RCC_GetHCLKFreq() / 1000;
	long clockDiv = (periodMs * baseClockFreqMs);

	long counter = 0;
	long prescaler = 0;

	for(counter = 3; counter < UINT16_MAX; counter++){
		prescaler = clockDiv / counter;

		if(clockDiv % counter == 0 && prescaler <= UINT16_MAX){
			break;
		}
	}
	__HAL_TIM_SET_PRESCALER(timerToSet, prescaler - 1);
	__HAL_TIM_SET_AUTORELOAD(timerToSet, counter - 1);
}

void stopTimer(TIM_HandleTypeDef *timer){
	HAL_TIM_Base_Stop_IT(timer);
}

void startTimer(TIM_HandleTypeDef *timer){
	HAL_TIM_Base_Start_IT(timer);
}

void resetTimer(TIM_HandleTypeDef *timer){
	timer->Instance->CNT = 0;
}
