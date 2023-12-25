#ifndef TIMERS_H
#define TIMERS_H

#include "stm32f0xx_hal.h"

void setTimerPeriod(TIM_HandleTypeDef *timerToSet, double periodMs);
void stopTimer(TIM_HandleTypeDef *timer);
void startTimer(TIM_HandleTypeDef *timer);
void resetTimer(TIM_HandleTypeDef *timer);

#endif
