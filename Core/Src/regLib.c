#include "regLib.h"



void setMODERGPIO(GPIO_TypeDef *port, uint32_t pin, uint32_t MODER0, uint32_t MODER1){

	if(MODER0){
		port->MODER |= (1 << (pin * 2));
	}else{
		port->MODER &= (UINT32_MAX ^ (1 << (pin * 2)));
	}

	if(MODER1){
		port->MODER |= (1 << (pin * 2 + 1));
	}else{
		port->MODER &= (UINT32_MAX ^ (1 << (pin * 2 + 1)));
	}
}

void configureGPIOPin(GPIO_TypeDef *port, uint32_t pin, uint32_t MODER0, uint32_t MODER1){
	port->OSPEEDR |= (1 << (pin*2));
	port->OSPEEDR |= (1 << (pin*2 + 1));

	port->PUPDR &= (UINT32_MAX ^ (2 << (pin * 2)));
	port->BSRR |= (1 << (pin + 16));

	setMODERGPIO(port, pin, MODER0, MODER1);
}

void setGPIOOutput(GPIO_TypeDef *port, uint32_t pin){
	configureGPIOPin(port, pin, 1, 0);
}


void configureSPIGPIO(){
	GPIOA->AFR[0] &= 0xF0F0FFFF;
	configureGPIOPin(GPIOA, 5, 0, 1); // PA5 auf SPI1 SCK konfiguriert
	configureGPIOPin(GPIOA, 7, 0, 1); // PA5 auf SPI1 MOSI konfiguriert
}

void configureSPI(){

	SPI1->CR1 = 0x304; // configure CR1
	SPI1->CR2 = 0xF00; // configure CR2

	SPI1->CR1 |= 0x40;
}


void transmitSPI(uint16_t val, GPIO_TypeDef *port, uint16_t pin){
	RESET_GPIO_PIN(port, pin - 1);
	SPI1->DR = val;

	while(1){
		if(!(SPI1->SR & SPI_SR_BSY_Msk)){
			SET_GPIO_PIN(port, pin - 1);
			return;
		}
	}
}
