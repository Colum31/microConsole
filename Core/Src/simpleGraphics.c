#include "simpleGraphics.h"

#include <math.h>

uint32_t colorTable[NUM_COLORS] = {colorNone, colorRed, colorGreen, colorBlue, colorCyan, colorMagenta, colorYellow, colorWhite};

enum baseColor getColorFromIndex(int i){
	return colorTable[i];
}

uint32_t hexColorMixer(uint8_t red, uint8_t green, uint8_t blue){
	return (red << 16) | (green << 8) | blue;
}

uint8_t getRedHexColor(uint32_t color){
	return (color >> 16) & 0xFF;
}

uint8_t getGreenHexColor(uint32_t color){
	return (color >> 8) & 0xFF;
}

uint8_t getBlueHexColor(uint32_t color){
	return color & 0xFF;
}


double getBrightnessHexColor(uint32_t color){
	uint8_t red, green, blue;

	red = getRedHexColor(color);
	green = getGreenHexColor(color);
	blue = getBlueHexColor(color);

	return sqrt((red*red) + (green*green) + (blue*blue));
}

uint8_t changeBrightnessChannel(uint8_t channelColor, int brightnessDelta){
	if(channelColor == 0) return 0;

	int ret = channelColor + brightnessDelta;

	if(ret < 0) return 0;
	if(ret > UINT8_MAX) return UINT8_MAX;

	return (uint8_t) ret;
}

uint32_t changeBrightnessSimple(uint32_t color, int brightnessDelta){

	uint8_t red, green, blue;

	red = changeBrightnessChannel(getRedHexColor(color), brightnessDelta);
	green = changeBrightnessChannel(getGreenHexColor(color), brightnessDelta);
	blue = changeBrightnessChannel(getBlueHexColor(color), brightnessDelta);

	return hexColorMixer(red, green, blue);
}
