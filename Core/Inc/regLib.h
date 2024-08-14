#ifndef REGLIB_H
#define REGLIB_H

#include <stm32f030x6.h>

#define ENABLE_GPIO_RCC_CLOCK (RCC->AHBENR |= 0x5E0000)
#define ENABLE_SPI1_RCC_CLOCK (RCC->APB2ENR |= 0x1000)

#define SET_GPIO_PIN(port, pin) ( (port)->BSRR = (1 << (pin)))
#define RESET_GPIO_PIN(port, pin) ( (port)->BSRR = (1 << (pin + 16)))

void setGPIOOutput(GPIO_TypeDef *port, uint32_t pin);


void configureSPIGPIO();
void configureSPI();
void transmitSPI(uint16_t val, GPIO_TypeDef *port, uint16_t pin);
#endif
