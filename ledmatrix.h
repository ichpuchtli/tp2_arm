#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include "stdint.h"

#define LEDMATRIX_SIZEX         4
#define LEDMATRIX_SIZEY         4
#define LEDMATRIX_AREA          (LEDMATRIX_SIZEX * LEDMATRIX_SIZEY)

struct rgb24_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void vLEDMatrixInit(void);
void vLEDMatrixMultiplexer_Hook(void);

/* Left to Right From Bottom
 *
 *  12 X X X X 15
 *  8  X X X X 11
 *  4  X X X X 7
 *  0  X X X X 3
 *
 */
void vLEDMatrixSetPixel(uint8_t x, uint8_t y, uint8_t* rgb);
void vLEDMatrixSetPixelN(uint8_t num, uint8_t* rgb);

void vLEDMatrixClearPixel(uint8_t x, uint8_t y);
void vLEDMatrixClearPixelN(uint8_t num);

void vLEDMatrixClearAll(void);

#endif
