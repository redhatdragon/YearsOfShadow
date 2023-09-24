#pragma once
#include <stdint.h>

namespace Crunch {
    bool comp(void *inData, uint32_t inCount, void *outData, uint32_t *outCount);
    bool decomp(void *inData, uint32_t inCount, void *outData, uint32_t *outCount);
}