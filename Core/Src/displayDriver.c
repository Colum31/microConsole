/*
 * displayDriver.c
 *
 *  Created on: Sep 11, 2023
 *      Author: daniel
 */

#include "stm32f0xx_hal.h"
#include "string.h"

#include "displayDriver.h"
#include "main.h"


#define LINE_LENGTH 8
#define LINES_PER_DISPLAY 8

#define DISPLAY_X 8
#define DISPLAY_Y 16

int linePosDisplays = 0;

extern SPI_HandleTypeDef hspi1;

uint8_t displayBuf[DISPLAY_Y];
int dataSize = sizeof(uint8_t);


void initDisplay(){
	memset(displayBuf, 0, DISPLAY_Y * dataSize);
	linePosDisplays = 0;
}

void setDisplay(uint8_t *toDisplay){
	memcpy(displayBuf, toDisplay, DISPLAY_Y * dataSize);
}

uint8_t buffToLine(int *buf){
	uint8_t result = 0;

	for(int i = 0; i < LINE_LENGTH; i++){
		if(buf[i]){
			result = result + (1 << i);
		}
	}

	return result;
}

void setDisplayFromBuf(int *buf){
	for(int i = 0; i < DISPLAY_Y; i++){
		displayBuf[i] = buffToLine(&buf[LINE_LENGTH * i]);
	}
}

bool drawLine(int display, uint8_t line, uint8_t lineNum){
	if(lineNum > (LINES_PER_DISPLAY - 1)){
		return false;
	}

	uint8_t lineSelect = 1 << lineNum;
	uint8_t lineContent = ~line;

	GPIO_TypeDef *GPIO_Ports[NUM_DISPLAYS] = {SPI1_CS1_GPIO_Port, SPI1_CS2_GPIO_Port};
	uint16_t GPIO_Pins[NUM_DISPLAYS] = {SPI1_CS1_Pin, SPI1_CS2_Pin};

	HAL_GPIO_WritePin(GPIO_Ports[display], GPIO_Pins[display], GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &lineSelect, 1, 100);
	HAL_SPI_Transmit(&hspi1, &lineContent, 1, 100);
	HAL_GPIO_WritePin(GPIO_Ports[display], GPIO_Pins[display], GPIO_PIN_SET);

	return true;
}


void displayInterruptHandler(){

	if(linePosDisplays >= LINE_LENGTH){
		linePosDisplays = 0;
	}

	drawLine(0, displayBuf[linePosDisplays], linePosDisplays);
	drawLine(1, displayBuf[linePosDisplays + 8], linePosDisplays);
	linePosDisplays++;
}
