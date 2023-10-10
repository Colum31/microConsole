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

void initDisplay();
void setDisplay(uint8_t *toDisplay);
void setDisplayFromBuf(int *buf);

void displayInterruptHandler();


#endif /* INC_DISPLAYDRIVER_H_ */
