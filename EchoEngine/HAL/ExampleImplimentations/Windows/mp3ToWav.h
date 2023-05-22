#pragma once
#include <stdint.h>

#define MP3_HEADER_SIZE 4
struct MP3Header {
	uint32_t sync : 12;
	uint8_t id : 1;
	uint8_t layer : 2;
	uint8_t protBit : 1;
	uint8_t bitRate : 4;
	uint8_t frequency : 2;
	uint8_t padBit : 1;
	uint8_t privBit : 1;
	uint8_t mode : 2;
	uint8_t modeExtension : 2;
	uint8_t copy : 1;
	uint8_t home : 1;
	uint8_t emphasis : 2;
	uint8_t data[0];
};