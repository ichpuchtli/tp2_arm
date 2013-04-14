#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include "stdtypes.h"

#define LEDMATRIX_SIZEX         4
#define LEDMATRIX_SIZEY         4
#define LEDMATRIX_AREA          (LEDMATRIX_SIZEX * LEDMATRIX_SIZEY)

struct rgb24_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void vLEDMatrixInit(void);
void vLEDMatrixIntHanlder(void);

/* Left to Right From Bottom
 *
 *  12 X X X X 15
 *  8  X X X X 11
 *  4  X X X X 7
 *  0  X X X X 3
 *
 */
void inline vLEDMatrixSetPixel(uint8_t x, uint8_t y, uint8_t* rgb);
void inline vLEDMatrixSetPixelN(uint8_t num, uint8_t* rgb);

void inline vLEDMatrixClearPixel(uint8_t x, uint8_t y);
void inline vLEDMatrixClearPixelN(uint8_t num);

void inline vLEDMatrixClearAll(void);

#endif
