#ifndef DISP_COLOR_DRIVER
#define DISP_COLOR_DRIVER

#include "ws2812.h"

#define BUFSIZE_DMA ((FULL_SPI_BUF_LEN_UINT16 / NUM_LINES) * 2)

void initColorDisplay();

#endif
