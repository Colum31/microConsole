/*
 * displayDriver.h
 *
 *  Created on: Sep 11, 2023
 *      Author: daniel
 */

#ifndef INC_DISPLAYDRIVER_H_
#define INC_DISPLAYDRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define NUM_DISPLAYS 2

#define LINE_LENGTH 8
#define LINES_PER_DISPLAY 8

#define DISPLAY_X 8
#define DISPLAY_Y 16

void initDisplay();
void initDisplayTimer(TIM_HandleTypeDef *timer);
void setDisplay(uint8_t *toDisplay);
void setDisplayFromBuf(uint8_t *buf);

void writeToDisplayComplete();

void displayInterruptHandler();


#endif /* INC_DISPLAYDRIVER_H_ */
