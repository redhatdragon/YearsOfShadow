#pragma once

#include "miniz/miniz.h"
#include "Crunch.h"
#include <iostream>

bool Crunch::comp(void* inData, uint32_t inCount, void* outData, uint32_t* outCount) {
    int code = mz_compress((uint8_t*)outData, (unsigned long*)outCount, (uint8_t*)inData, inCount);
    if (code != MZ_OK)
        std::cout << "Error: Crunch::comp(), compression failed; " << mz_error(code);
	return false;
}
bool Crunch::decomp(void *inData, uint32_t inCount, void *outData, uint32_t *outCount) {
    int code = mz_uncompress((uint8_t *)outData, (unsigned long *)outCount, (uint8_t *)inData, inCount);
    if (code != MZ_OK)
        std::cout << "Error: Crunch::decomp(), compression failed; " << mz_error(code);
	return false;
}

