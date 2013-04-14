#ifndef SD_H_
#define SD_H_

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "fatfs/src/diskio.h"

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
void sd_write(char *fileName, char *data, WORD size);
char *sd_read(char *fileName, WORD size);
void sd_create(void);
char *sd_ReadWav(char* fileName, WORD size, DWORD location);

#endif /* SD_H_ */
