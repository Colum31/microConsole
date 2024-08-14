/*
 * simpleGraphics.h
 *
 *  Created on: Jul 20, 2024
 *      Author: daniel
 */

#ifndef INC_SIMPLEGRAPHICS_H_
#define INC_SIMPLEGRAPHICS_H_

#include <inttypes.h>

#define NUM_COLORS 8

enum baseColor {colorRed = 0x00010000,
				colorGreen = 0x00000100,
				colorBlue = 0x00000001,
				colorYellow = 0x00010100,
				colorCyan	= 0x00000101,
				colorMagenta = 0x00010001,
				colorWhite = 0x00010101,
				colorNone	= 0x0,
			};



enum baseColor getColorFromIndex(int i);
uint32_t hexColorMixer(uint8_t red, uint8_t green, uint8_t blue);
uint32_t changeBrightnessSimple(uint32_t color, int brightnessDelta);

void renderColorTableLookup(uint8_t *img, uint16_t *buf, int len);

extern uint32_t colorTable[];

#endif /* INC_SIMPLEGRAPHICS_H_ */
