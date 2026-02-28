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

enum baseColor {colorRed = 0x00030000,
				colorGreen = 0x00000300,
				colorBlue = 0x00000003,
				colorYellow = 0x00030300,
				colorCyan	= 0x00000303,
				colorMagenta = 0x00030003,
				colorWhite = 0x00030303,
				colorNone	= 0x0,
			};



enum baseColor getColorFromIndex(int i);
uint32_t hexColorMixer(uint8_t red, uint8_t green, uint8_t blue);
uint32_t changeBrightnessSimple(uint32_t color, int brightnessDelta);

void renderColorTableLookup(uint8_t *img, uint16_t *buf, int len);

extern uint32_t colorTable[];

#endif /* INC_SIMPLEGRAPHICS_H_ */
