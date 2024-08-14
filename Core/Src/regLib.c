#include "regLib.h"


uint32_t getMaskSimple(uint8_t width, uint8_t pos){
	uint32_t ret = 2;

	for(int i = 0; i < width - 1; i++){
		ret *= 2;
	}

	ret--;

	ret = ret << pos;
	return ret;
}

void setRegisterBits(volatile uint32_t *registerPtr, uint32_t value, uint8_t valueWidth, uint8_t pos){
	uint32_t mask = getMaskSimple(valueWidth, pos);

	uint32_t newValue = *registerPtr & ~mask;
	newValue |= (value << pos);

	*registerPtr = newValue;
}

void setGPIOMode(GPIO_TypeDef *port, uint32_t pin, uint32_t mode){
	 setRegisterBits(&port->MODER, mode, 2, pin * 2);
}

void setGPIOSpeed(GPIO_TypeDef *port, uint32_t pin, uint32_t speed){
	setRegisterBits(&port->OSPEEDR, speed, 2, pin * 2);
}

void setGPIOPullUpPullDown(GPIO_TypeDef *port, uint32_t pin, uint32_t pupd){
	setRegisterBits(&port->PUPDR, pupd, 2, pin * 2);
}

void setGPIOAlternateFunction(GPIO_TypeDef *port, uint32_t pin, uint32_t af){

	volatile uint32_t *GPIOAFPtr;

	GPIOAFPtr = (pin > 7) ? &port->AFR[1] : &port->AFR[0];
	pin = (pin > 7) ? pin - 8 : pin;

	setRegisterBits(GPIOAFPtr, af, 4, pin * 4);
}

void configureGPIOPin(GPIO_TypeDef *port, uint32_t pin, uint32_t gpioSpeed, uint32_t gpioMode, uint32_t pupd, uint32_t af){
	setGPIOSpeed(port, pin, gpioSpeed);
	setGPIOPullUpPullDown(port, pin, pupd);
	setGPIOAlternateFunction(port, pin, af);
	RESET_GPIO_PIN(port, pin);
	setGPIOMode(port, pin, gpioMode);
}

void configureGPIOOutput(GPIO_TypeDef *port, uint32_t pin){
	configureGPIOPin(port, pin, 0b11, 0b01, 0b00, 0b00);
}


void configureGPIOSPI(){
	configureGPIOPin(GPIOA, 5, 0b11, 0b10, 0b00, 0b00); // PA5 auf SPI1 SCK konfiguriert
	configureGPIOPin(GPIOA, 7, 0b11, 0b10, 0b00, 0b00); // PA7 auf SPI1 MOSI konfiguriert
}

void enableSPI(){
	SPI1->CR1 |= 0x40;
}

DMA_Channel_TypeDef *getDMAChannelHandler(int channel){
	switch(channel){
		case 1:
				return DMA1_Channel1;
		case 2:
				return DMA1_Channel2;
		case 3:
				return DMA1_Channel3;
		case 4:
				return DMA1_Channel4;
		case 5:
				return DMA1_Channel5;
		default:
				return (DMA_Channel_TypeDef *) 0x0;
	}
}

void enableDMAChannel(int channel){
	DMA_Channel_TypeDef *dmaChannel = getDMAChannelHandler(channel);

	if(!dmaChannel){
		return;
	}

	dmaChannel->CCR |= 0x1;
}

void disableDMAChannel(int channel){
	DMA_Channel_TypeDef *dmaChannel = getDMAChannelHandler(channel);

	if(!dmaChannel){
		return;
	}

	dmaChannel->CCR &= 0xFFFFFFFE;
}

void configureSPIDMA(){
	ENABLE_GPIO_RCC_CLOCK;
	ENABLE_SPI1_RCC_CLOCK;
	configureGPIOSPI();

	SPI1->CR1 = 0x314; // configure CR1
	SPI1->CR2 = 0xF02; // configure CR2, DMATX
}

void configureDMA(struct dmaConfig config){
	ENABLE_DMA1_RCC_CLOCK;
	DMA_Channel_TypeDef *dmaChannel =  getDMAChannelHandler(config.channel);

	if(!dmaChannel){
		return;
	}

	dmaChannel->CPAR = (uint32_t) config.CPAR;
	dmaChannel->CMAR = (uint32_t) config.CMAR;
	dmaChannel->CNDTR = config.CNDTR;

	uint32_t dmaCCR = (config.MEM2MEM << DMA_CCR_MEM2MEM_Pos) | (config.PL << DMA_CCR_PL_Pos) | (config.MSIZE << DMA_CCR_MSIZE_Pos)
						| (config.PSIZE << DMA_CCR_PSIZE_Pos) | (config.MINC << DMA_CCR_MINC_Pos) | (config.PINC << DMA_CCR_PINC_Pos)
						| (config.CIRC << DMA_CCR_CIRC_Pos) | (config.DIR << DMA_CCR_DIR_Pos) | (config.TEIE << DMA_CCR_TEIE_Pos)
						| (config.HTIE << DMA_CCR_HTIE_Pos) | (config.TCIE << DMA_CCR_TCIE_Pos);

	dmaChannel->CCR = dmaCCR;

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

void disableSPI(){
	while((SPI1->SR & SPI_SR_FTLVL_Msk) || SPI1->SR & SPI_SR_BSY_Msk){
	}

	SPI1->CR1 &= ~SPI_CR1_SPE_Msk;

	while(SPI1->SR & SPI_SR_FTLVL_Msk){
		volatile uint32_t dummy = SPI1->DR;
	}
}

void disableSPITXDMA(){
	SPI1->CR2 &= ~SPI_CR2_LDMATX_Msk;
}



void configureSPISimple(){
	ENABLE_GPIO_RCC_CLOCK;
	ENABLE_SPI1_RCC_CLOCK;
	configureGPIOSPI();

	SPI1->CR1 = 0x304; // configure CR1
	SPI1->CR2 = 0xF00; // configure CR2
}

