// Preface

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "processWav.h"
#include "inc/hw_types.h"
#include "../SDcard/sd.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
uint32_t location;


//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t chunkId;
	uint32_t chunkSize;
	uint32_t format;

	uint32_t subChunk1Id;
	uint32_t subChunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;

	uint32_t subChunk2Id;
	uint32_t subChunk2Size;
} waveInfo;

waveInfo test;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// processWav_GetWavInfo
//
// Processes the first 44 bytes of a given wave file and places them into an
// instance of a struct. This struct then contains all the information for
// processing the wav file.
//------------------------------------------------------------------------------
void processWav_GetWavInfo(char *fileName)
{

	char *wavInfo;
	wavInfo = sd_ReadWav(fileName, 44, 0);

	test.chunkId = (uint32_t)((wavInfo[0] << 24) | (wavInfo[1] << 16) | (wavInfo[2] << 8) | (wavInfo[3]));
	test.chunkSize = (uint32_t)((wavInfo[4]) | (wavInfo[5] << 8) | (wavInfo[6] << 16) | (wavInfo[7] << 24));
	test.format =  (uint32_t)((wavInfo[8] << 24) | (wavInfo[9] << 16) | (wavInfo[10] << 8) | (wavInfo[11]));

	test.subChunk1Id = (uint32_t)((wavInfo[12] << 24) | (wavInfo[13] << 16) | (wavInfo[14] << 8) | (wavInfo[15]));
	test.subChunk1Size = (uint32_t)((wavInfo[16]) | (wavInfo[17] << 8) | (wavInfo[18] << 16) | (wavInfo[19] << 24));
	test.audioFormat = (uint16_t)((wavInfo[20]) | (wavInfo[21] << 8));
	test.numChannels = (uint16_t)((wavInfo[22]) | (wavInfo[23] << 8));
	test.sampleRate = (uint32_t)((wavInfo[24]) | (wavInfo[25] << 8) | (wavInfo[26] << 16) | (wavInfo[27] << 24));
	test.byteRate = (uint32_t)((wavInfo[28]) | (wavInfo[29] << 8) | (wavInfo[30] << 16) | (wavInfo[31] << 24));
	test.blockAlign = (uint16_t)((wavInfo[32]) | (wavInfo[33] << 8));
	test.bitsPerSample = (uint16_t)((wavInfo[34]) | (wavInfo[35] << 8));

	test.subChunk2Id = (uint32_t)((wavInfo[36] << 24) | (wavInfo[37] << 16) | (wavInfo[38] << 8) | (wavInfo[39]));
	test.subChunk2Size = (uint32_t)((wavInfo[40]) | (wavInfo[41] << 8) | (wavInfo[42] << 16) | (wavInfo[43] << 24));

//	uint32_t buff[5];
//	buff[0] = (char)test.chunkId;
//	buff[1] = test.format;
//	buff[2] = test.numChannels;
//	buff[3] = test.sampleRate;
//	buff[4] = test.bitsPerSample;
//	sd_write("this.txt", buff, 5);
}

void processWav_GetWavData(char *fileName)
{

}
