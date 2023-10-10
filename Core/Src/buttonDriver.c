/*
 * buttonDriver.c
 *
 *  Created on: Oct 6, 2023
 *      Author: daniel
 */

#include "stm32g4xx_hal.h"
#include "main.h"
#include "buttonDriver.h"

struct buttonState{
	int buttonPin;
	GPIO_TypeDef * buttonGPIOPort;
	int buttonPrevState;
	int buttonRepeatPressThreshold;
	int buttonRepeatPresses;
};

struct buttonState drivenButtons[NUM_BUTTONS];

void initDrivenButtons(){
	int buttonPins[NUM_BUTTONS] = {ButtonUp_Pin, ButtonLeft_Pin, ButtonMid_Pin, ButtonRight_Pin, ButtonDown_Pin};
	GPIO_TypeDef * buttonGPIOPorts[NUM_BUTTONS] = {ButtonUp_GPIO_Port, ButtonLeft_GPIO_Port, ButtonMid_GPIO_Port, ButtonRight_GPIO_Port, ButtonDown_GPIO_Port};


	for(int i = 0; i < NUM_BUTTONS; i++){
		struct buttonState *curButton = &(drivenButtons[i]);

		curButton->buttonPin = buttonPins[i];
		curButton->buttonGPIOPort = buttonGPIOPorts[i];
		curButton->buttonPrevState = GPIO_PIN_SET;
		curButton->buttonRepeatPresses = 0;
		curButton->buttonRepeatPressThreshold = REPEAT_BUTTON_DELAY_INITIAL;
	}
}


int readButtonState(int buttonNum){

	struct buttonState *curButtonState = &(drivenButtons[buttonNum]);
	GPIO_PinState buttonReadout = HAL_GPIO_ReadPin(curButtonState->buttonGPIOPort, curButtonState->buttonPin);

	if(buttonReadout == GPIO_PIN_SET){
		// Button not pressed
		curButtonState->buttonRepeatPresses = 0;
		curButtonState->buttonRepeatPressThreshold = REPEAT_BUTTON_DELAY_INITIAL;
		curButtonState->buttonPrevState = GPIO_PIN_SET;
		return 0;
	}

	if(buttonReadout == GPIO_PIN_RESET && curButtonState->buttonPrevState == GPIO_PIN_SET){
		// Button pressed, detected first time
		curButtonState->buttonPrevState = GPIO_PIN_RESET;
		return 1;
	}

	// Button being held down
	if(curButtonState->buttonRepeatPresses == curButtonState->buttonRepeatPressThreshold){
			curButtonState->buttonRepeatPresses = 0;
			curButtonState->buttonRepeatPressThreshold = REPEAT_BUTTON_DELAY_HELD;
			return 1;
	}else{
			curButtonState->buttonRepeatPresses += 1;
	}

	return 0;
}


void readButtonAllStates(int *buttonStates){
	for(int i = 0; i < NUM_BUTTONS; i++){
		buttonStates[i] = readButtonState(i);
	}
}
