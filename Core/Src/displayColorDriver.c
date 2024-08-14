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

#include "simpleGraphics.h"

#include "regLib.h"


uint16_t dmaBuf[BUFSIZE_DMA];
struct dmaConfig spiDMAConf;

int bufferEventFlag = 0;
int halfBufferFlag = 0;
int fullBufferFlag = 0;

int termHalfFlag = 0;
int termFullFlag = 0;

int padNextBufferEvent = 0;

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

uint16_t *getDMABufPtr(struct dmaConfig config){
	uint16_t *dmaBufPtr = halfBufferFlag ? config.CMAR : config.CMAR + (config.CNDTR / 2);
	return dmaBufPtr;
}

size_t initContinousSPIDMA(uint16_t *data, size_t dataLen, struct dmaConfig config){

	configureDMA(config);

	size_t copyLen = config.CNDTR;

	if(dataLen <= config.CNDTR){
		copyLen = dataLen;
		termFullFlag = 1;
	}

	memset(config.CMAR, 0, config.CNDTR * sizeof(uint16_t));
	memcpy(config.CMAR, data, copyLen * sizeof(uint16_t));

	return copyLen;
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

void initColorDisplay(){
	spiDMAConf = getDMAConfig(dmaBuf);
	generateWSColorTable(colorTable, NUM_COLORS);
}
