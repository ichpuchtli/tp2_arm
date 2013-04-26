#ifndef _SD_CARD_H_
#define _SD_CARDe_H_

#include "fatfs/src/diskio.h"
#include "fatfs/src/ff.h"
#include "stdtypes.h"


void vSDCardInit(void); 

void vOpenWavFile(uint8_t num);
size_t vReadWavFile(uint8_t num, uint16_t* buffer, size_t count);
void vCloseWavFile(uint8_t num);

#endif
