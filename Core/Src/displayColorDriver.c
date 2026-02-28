/*
 * displayColorDriver.c
 *
 *  Created on: Aug 14, 2024
 *      Author: daniel
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "displayColorDriver.h"
#include "ws2812.h"
#include "settings.h"

#include "simpleGraphics.h"

#include "regLib.h"


uint16_t dmaBuf[BUFSIZE_DMA];
uint8_t mirroredBoard[BOARDSIZE];

struct dmaConfig spiDMAConf;

int bufferEventFlag = 0;
int halfBufferFlag = 0;
int fullBufferFlag = 0;

int termHalfFlag = 0;
int termFullFlag = 0;

int padNextBufferEvent = 0;

int dmaActive = 0;


uint8_t *boardCopyPtr;
size_t boardCopyRemaining;

void handleDMAInterrupt();

void DMA1_Channel2_3_IRQHandler_skibidi(){
	NVIC_ClearPendingIRQ(DMA1_Channel2_3_IRQn);

	// SET_GPIO_PIN(GPIOA, 2);
	// RESET_GPIO_PIN(GPIOA, 2);

	if (DMA1->ISR & DMA_ISR_GIF3){

		if(DMA1->ISR & DMA_ISR_HTIF3){
			DMA1->IFCR |= DMA_IFCR_CHTIF3;

			 if(termHalfFlag){
				 disableDMAChannel(3);
			 }

			halfBufferFlag = 1;
			bufferEventFlag = 1;
			// SET_GPIO_PIN(GPIOA, 0);
			// RESET_GPIO_PIN(GPIOA, 0);
		}

		if(DMA1->ISR & DMA_ISR_TCIF3){
			DMA1->IFCR |= DMA_IFCR_CTCIF3;

			 if(termFullFlag){
				 disableDMAChannel(3);
			 }

			fullBufferFlag = 1;
			bufferEventFlag = 1;
			// SET_GPIO_PIN(GPIOA, 1);
			// RESET_GPIO_PIN(GPIOA, 1);
		}

		handleDMAInterrupt();
	}
}


struct dmaConfig getDMAConfig(uint16_t *dmaBuf){
	  struct dmaConfig config = 		{	.channel = 3,
			  	  	  	  	  	  	  	  	.CMAR = dmaBuf,
											.CNDTR = BUFSIZE_DMA,
											.CPAR = (uint16_t*) &(SPI1->DR),
											.MEM2MEM = 0,
											.PL = 1,
											.MSIZE = 1,
											.PSIZE = 1,
											.MINC = 1,
											.PINC = 0,
											.CIRC = 1,
											.DIR = 1,
											.TEIE = 1,
											.HTIE = 1,
											.TCIE = 1};

	  return config;
}


void setTermFlag(){
	if(halfBufferFlag){
		termHalfFlag = 1;
	}

	if(fullBufferFlag){
		termFullFlag = 1;
	}
}

void resetTermFlags(){
	termFullFlag = 0;
	termHalfFlag = 0;
}

void resetBufferFlags(){
	halfBufferFlag = 0;
	fullBufferFlag = 0;
	bufferEventFlag = 0;
}

void startContinousSPIDMA(){
	  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	  enableDMAChannel(3);
	  configureSPIDMA();
	  enableSPI();
}


void stopContnousSPIDMA(){
	 disableSPI();
	 disableSPITXDMA();
	 resetBufferFlags();
	 resetTermFlags();
}

uint16_t *getDMABufPtr(struct dmaConfig config){
	uint16_t *dmaBufPtr = halfBufferFlag ? config.CMAR : config.CMAR + (config.CNDTR / 2);
	return dmaBufPtr;
}

size_t initContinousSPIDMA(uint8_t *data, size_t dataLen, struct dmaConfig config){

	configureDMA(config);

	size_t copyLen = config.CNDTR / WS_COLOR_SIZE_UINT_16 ;

	if(dataLen <= config.CNDTR / WS_COLOR_SIZE_UINT_16){
		copyLen = dataLen;
		termFullFlag = 1;
	}

	memset(config.CMAR, 0, config.CNDTR * sizeof(uint16_t));
	// memcpy(config.CMAR, data, copyLen * sizeof(uint16_t));

	renderColorTableLookup(data, config.CMAR, copyLen);

	return copyLen;
}

size_t writeToContinousSPIDMA(uint8_t *data, size_t dataLen, struct dmaConfig config){
	uint16_t *dmaBufPtr = getDMABufPtr(config);

	size_t dmaBufLen = (config.CNDTR / 2) / WS_COLOR_SIZE_UINT_16 ;

	size_t copyLen = dataLen < dmaBufLen ? dataLen : dmaBufLen;
	size_t padLen = dataLen <= dmaBufLen ? dmaBufLen - dataLen : 0;


	renderColorTableLookup(data, dmaBufPtr, copyLen);
	// memcpy(dmaBufPtr, data, copyLen * sizeof(uint16_t));
	memset(dmaBufPtr + copyLen, 0, padLen * sizeof(uint16_t));

	return copyLen;
}

void handleDMAInterrupt(){
	 if((termHalfFlag && halfBufferFlag) || (termFullFlag && fullBufferFlag)){
		 stopContnousSPIDMA();
		 dmaActive = 0;
		 return;
	 }


	 if(bufferEventFlag && boardCopyRemaining != 0){

		 size_t written = writeToContinousSPIDMA(boardCopyPtr, boardCopyRemaining, spiDMAConf);
		 boardCopyPtr += written;
		 boardCopyRemaining -= written;

		 if(boardCopyRemaining == 0){
			 setTermFlag();
			 padNextBufferEvent = 1;
		 }

		 resetBufferFlags();
	 }

	 if(bufferEventFlag && padNextBufferEvent){

		 uint16_t *dmaBufPtr = getDMABufPtr(spiDMAConf);
		 memset(dmaBufPtr, 0, (spiDMAConf.CNDTR / 2) * sizeof(uint16_t));

		 resetBufferFlags();
		 padNextBufferEvent = 0;
	 }
}


void initColorDisplay(){
	spiDMAConf = getDMAConfig(dmaBuf);
	generateWSColorTable(colorTable, NUM_COLORS);
}

void mirrorBoard(uint8_t *origBoard, uint8_t *mirroredBoard){

	int mirrorOffset = (BOARDSIZE / 2) - 1;

	for(int i = 0; i < BOARDSIZE / 4; i++){
		uint8_t value = origBoard[i];
		mirroredBoard[i] = origBoard[mirrorOffset];
		mirroredBoard[mirrorOffset] = value;
		mirrorOffset--;
	}

}

void setDisplayFromBuf(uint8_t *buf){
	if(dmaActive){
		return;
	}

	dmaActive = 1;

	mirrorBoard(buf, mirroredBoard);
	mirrorBoard(buf + (BOARDSIZE / 2), mirroredBoard + (BOARDSIZE / 2));

	boardCopyPtr = mirroredBoard;
	boardCopyRemaining = BOARDSIZE;

	size_t written = initContinousSPIDMA(boardCopyPtr, boardCopyRemaining, spiDMAConf);
	boardCopyRemaining -= written;
	boardCopyPtr += written;

	startContinousSPIDMA();

}
