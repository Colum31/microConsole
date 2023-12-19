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

void initDisplay();
void setDisplay(uint8_t *toDisplay);
void setDisplayFromBuf(uint8_t *buf);

void displayInterruptHandler();


#endif /* INC_DISPLAYDRIVER_H_ */
