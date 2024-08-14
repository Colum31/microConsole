/*
 * displayDriver.c
 *
 *  Created on: Sep 11, 2023
 *      Author: daniel
 */

#include "stm32f0xx_hal.h"
#include "string.h"

#include "consoleSettings.h"
#include "displayDriver.h"
#include "regLib.h"
#include "timers.h"
#include "main.h"

int linePosDisplays = 0;

uint16_t (*frontBuf)[DISPLAY_Y];

uint16_t transmitBuf[DISPLAY_Y];
uint16_t transmitBufB[DISPLAY_Y];

void initDisplayTimer(TIM_HandleTypeDef *timer){
	int totalRefreshRate = DISPLAY_REFRESH_RATE_HZ * LINES_PER_DISPLAY;
	double refreshPeriodMs = 1000.0/totalRefreshRate;

	setTimerPeriod(timer, refreshPeriodMs);
}

void initDisplay(){

	configureSPISimple();
	enableSPI();

	memset(transmitBuf, 0, DISPLAY_Y * sizeof(uint16_t));
	frontBuf = &transmitBuf;

	linePosDisplays = 0;
}


uint8_t buffToLine(uint8_t *buf){
	uint8_t result = 0;

	for(int i = 0; i < LINE_LENGTH; i++){
		if(buf[i]){
			result = result + (1 << i);
		}
	}

	return result;
}

uint16_t calculateBytesToTransmit(uint8_t line, uint8_t lineNum){
	uint8_t lineSelect = 1 << lineNum;
	uint8_t lineContent = ~line;

	uint16_t combined = (uint16_t) lineSelect << 8 | (uint16_t) lineContent;

	return combined;
}

uint16_t (*getBackBuf())[DISPLAY_Y]{
	if(frontBuf == &transmitBuf){
		return &transmitBufB;
	}

	return &transmitBuf;
}

void setDisplayFromBuf(uint8_t *buf){

	uint16_t (*backBuf)[DISPLAY_Y] = getBackBuf();

	for(int i = 0; i < DISPLAY_Y; i++){
		uint8_t line = buffToLine(&buf[LINE_LENGTH * i]);
		(*backBuf)[i] = calculateBytesToTransmit(line, i % 8);
	}

	frontBuf = backBuf;
}

bool drawLine(uint8_t lineNum){
	uint16_t toSend = (*frontBuf)[lineNum];
	transmitSPI(toSend, SPI1_CS1_GPIO_Port, SPI1_CS1_Pin);

	toSend = (*frontBuf)[lineNum + 8];
	transmitSPI(toSend, SPI1_CS2_GPIO_Port, SPI1_CS2_Pin);
	return true;
}


void displayInterruptHandler(){

	if(linePosDisplays >= LINE_LENGTH){
		linePosDisplays = 0;
	}

	drawLine(linePosDisplays);
	linePosDisplays++;
}
