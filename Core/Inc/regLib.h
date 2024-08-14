#ifndef REGLIB_H
#define REGLIB_H

#include <stm32f030x6.h>

#define ENABLE_GPIO_RCC_CLOCK (RCC->AHBENR |= 0x5E0000)
#define ENABLE_SPI1_RCC_CLOCK (RCC->APB2ENR |= 0x1000)
#define ENABLE_DMA1_RCC_CLOCK (RCC->AHBENR |= 0x1)

#define SET_GPIO_PIN(port, pin) ( (port)->BSRR = (1 << (pin)))
#define RESET_GPIO_PIN(port, pin) ( (port)->BSRR = (1 << (pin + 16)))

struct dmaConfig{
	int channel;
	uint16_t *CMAR;
	uint16_t CNDTR;
	uint16_t *CPAR;

	uint8_t MEM2MEM;
	uint8_t PL;
	uint8_t MSIZE;
	uint8_t PSIZE;
	uint8_t MINC;
	uint8_t PINC;
	uint8_t CIRC;
	uint8_t DIR;
	uint8_t TEIE;
	uint8_t HTIE;
	uint8_t TCIE;
};


void configureGPIOOutput(GPIO_TypeDef *port, uint32_t pin);

void configureSPISimple();
void configureSPIDMA();

void enableSPI();
void disableSPI();
void disableSPITXDMA();

void configureDMA(struct dmaConfig config);
void enableDMAChannel(int channel);
void disableDMAChannel(int channel);

void transmitSPI(uint16_t val, GPIO_TypeDef *port, uint16_t pin);
#endif
