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

#define DISPLAY_X 8
#define DISPLAY_Y 16

int linePosDisplays = 0;
int dontDisplay = 0;

extern SPI_HandleTypeDef hspi1;

uint8_t displayBuf[DISPLAY_Y];
int dataSize = sizeof(uint8_t);


void initDisplay(){
	memset(displayBuf, 0, DISPLAY_Y * dataSize);
	linePosDisplays = 0;
}

void setDisplay(uint8_t *toDisplay){
	memcpy(displayBuf, toDisplay, DISPLAY_Y * dataSize);
	linePosDisplays = 0;
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

	dontDisplay = 1;

	for(int i = 0; i < DISPLAY_Y; i++){
		displayBuf[i] = buffToLine(&buf[LINE_LENGTH * i]);
	}

	dontDisplay = 0;
}

bool drawLine(int display, uint8_t line, uint8_t lineNum){
	if(lineNum > 7){
		return false;
	}

	uint8_t lineSelect = 1 << lineNum;
	uint8_t lineContent = ~line;

	if(display == 0){
		HAL_GPIO_WritePin(SPI1_CS1_GPIO_Port, SPI1_CS1_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit(&hspi1, &lineSelect, 1, 100);
		HAL_SPI_Transmit(&hspi1, &lineContent, 1, 100);

		HAL_GPIO_WritePin(SPI1_CS1_GPIO_Port, SPI1_CS1_Pin, GPIO_PIN_SET);
	}else if(display == 1){
		HAL_GPIO_WritePin(SPI1_CS2_GPIO_Port, SPI1_CS2_Pin, GPIO_PIN_RESET);

		HAL_SPI_Transmit(&hspi1, &lineSelect, 1, 100);
		HAL_SPI_Transmit(&hspi1, &lineContent, 1, 100);


		HAL_GPIO_WritePin(SPI1_CS2_GPIO_Port, SPI1_CS2_Pin, GPIO_PIN_SET);
	}
	return true;
}


void displayInterruptHandler(){

	if(dontDisplay){
		linePosDisplays = 0;
		return;
	}

	if(linePosDisplays >= LINE_LENGTH){
		linePosDisplays = 0;
	}

	drawLine(0, displayBuf[linePosDisplays], linePosDisplays);
	drawLine(1, displayBuf[linePosDisplays + 8], linePosDisplays);
	linePosDisplays++;
}

