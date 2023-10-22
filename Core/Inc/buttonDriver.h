/*
 * buttonDriver.h
 *
 *  Created on: Oct 6, 2023
 *      Author: daniel
 */

#ifndef INC_BUTTONDRIVER_H_
#define INC_BUTTONDRIVER_H_

#define NUM_BUTTONS 7
#define REPEAT_BUTTON_DELAY_INITIAL 10
#define REPEAT_BUTTON_DELAY_HELD ( REPEAT_BUTTON_DELAY_INITIAL / 4)

enum buttons{buttonUp, buttonLeft, buttonMid, buttonRight, buttonDown, buttonF1, buttonF2};

void initDrivenButtons();
void readButtonAllStates(int *buttonStates);

#endif /* INC_BUTTONDRIVER_H_ */
