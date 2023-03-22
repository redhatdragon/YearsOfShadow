#pragma once
#include <stdint.h>

uint8_t getPNGData(const char *fileName, uint32_t **outBuffer,
	unsigned int *outWidth, unsigned int *outHeight);