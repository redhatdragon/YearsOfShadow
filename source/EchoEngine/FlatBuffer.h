#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <bitset>
#include "BitBool/BitBool.h"

/*template <typename T, uint32_t maxLength>
class FlatFlaggedBuffer {
	T data[maxLength];
	uint8_t isValid[maxLength / 8 + 8];
	uint32_t count = 0;
public:
	FlatFlaggedBuffer() {
		for (uint32_t i = 0; i < maxLength / 8 + 8; i++) {
			isValid[i] = 0;
		}
	}
	__forceinline uint32_t toggleFirstInvalid() {
		uint32_t c = maxLength / 4 + 1;
		uint32_t* isValidPtr = (uint32_t*)isValid;
		for (uint32_t i = 0; i < c; i++) {
			if (isValidPtr[i] != -1) {
				for (unsigned int j = 0; j < 32; j++) {
					if (isValidPtr[i] | 1 << j == 0) {
						isValidPtr[i] |= 1 << j;
						//return i * j;
						return i * 32 + j;
					}
				}
			}
		}
		return -1;
	}
	__forceinline void setInvalid(uint32_t index) {
		uint32_t i = index / 8;
		uint32_t remainder = index % 8;
		isValid[i] &= ~(1 << remainder);
	}
	__forceinline void setValid(uint32_t index) {
		uint32_t i = index / 8;
		uint32_t remainder = index % 8;
		isValid[i] |= 1 << remainder;
	}
	__forceinline uint32_t insert(const T& component) {
		uint32_t index = toggleFirstInvalid();
		count++;
		data[index] = component;
		setValid(index);
		return index;
	}
	__forceinline bool getIsValid(uint32_t index) {
		uint32_t i = index / 8;
		uint32_t remainder = index % 8;
		return isValid[i] | 1 << remainder;
	}
	__forceinline uint32_t getCount() {
		return count;
	}
	__forceinline T operator[](uint32_t index) {
		return data[index];
	}
};*/

#define FB32_BIT_SET(number, n) number |= 1U << n;
#define FB32_BIT_CLEAR(number, n) number &= ~(1U << n);
#define FB32_BIT_TOGGLE(number, n) number ^= 1U << n;
#define FB32_BIT_TEST(number, n) (number >> n) & 1U;

/*template <typename T, uint32_t maxLength>
class FlatFlaggedBuffer {
	T data[maxLength];
	uint8_t isValid[maxLength];
	uint32_t count = 0;
	uint32_t maxIndex = 0;
public:
	FlatFlaggedBuffer() {
		clear();
	}
	__forceinline void clear() {
		for (uint32_t i = 0; i < maxLength; i++) {
			isValid[i] = 0;
		}
	}
	// Consider using in conjunction with incrementCount.
	__forceinline uint32_t toggleFirstInvalid() {
		uint32_t max = maxLength / 4;
		uint32_t* bufferAs32 = (uint32_t*)&isValid[0];
		for (unsigned int i = 0; i < max; i++) {
			if (bufferAs32[i] == -1) continue;
			for (unsigned int j = 0; j < 4; j++) {
				if (isValid[i * 4 + j] == 0) {
					setValid(i * 4 + j);
					return i * 4 + j;
				}
			}
		}
		return -1;
	}
	__forceinline void setInvalid(uint32_t index) {
		isValid[index] = 0;
	}
	__forceinline void setValid(uint32_t index) {
		isValid[index] = 255;
	}
	__forceinline uint32_t insert(const T& component) {
		uint32_t index = toggleFirstInvalid();
		count++;
		data[index] = component;
		return index;
	}
	__forceinline bool getIsValid(uint32_t index) {
		return (bool)isValid[index];
	}
	__forceinline uint32_t getCount() {
		return count;
	}
	// To be used with toggleFirstInvalid.
	__forceinline void incrementCount() {
		count++;
	}
	__forceinline void decrementCount() {
		count--;
	}
	__forceinline T& operator[](uint32_t index) {
		return data[index];
	}
};*/
/*template <typename T, uint32_t maxLength>
class FlatFlaggedBuffer {
	T data[maxLength];
	static constexpr uint32_t isValidSize = maxLength / 8 + 4;
	uint8_t isValid[isValidSize];
	uint32_t count = 0;
	uint32_t maxIndex = 0;
public:
	FlatFlaggedBuffer() {
		clear();
	}
	__forceinline void clear() {
		memset(&isValid[0], 0, isValidSize);
	}
	// Consider using in conjunction with incrementCount.
	__forceinline uint32_t toggleFirstInvalid() {
		uint32_t max = maxLength / (4*8);
		uint32_t* bufferAs32 = (uint32_t*)&isValid[0];
		for (unsigned int i = 0; i < max; i++) {
			if (bufferAs32[i] == -1) continue;
			
			for (uint32_t j = 0; j < 4; j++) {
				if (isValid[i * 4 + j] != 255) {
					for (uint32_t k = 0; k < 8; j++) {
						uint32_t index = (i * 4 + j) * 8;
						bool isSet = FB32_BIT_TEST(isValid[index], k);
						if (isSet == false)
							return index;
					}
				}
			}
		}
		return -1;
	}
	__forceinline void setInvalid(uint32_t index) {
		FB32_BIT_CLEAR(isValid[index / 8], index % 8);
	}
	__forceinline void setValid(uint32_t index) {
		FB32_BIT_SET(isValid[index / 8], index % 8);
	}
	__forceinline uint32_t insert(const T& component) {
		uint32_t index = toggleFirstInvalid();
		count++;
		data[index] = component;
		return index;
	}
	__forceinline bool getIsValid(uint32_t index) {
		auto retValue = FB32_BIT_TEST(isValid[index / 8], index % 8);
		return retValue;
	}
	__forceinline uint32_t getCount() {
		return count;
	}
	// To be used with toggleFirstInvalid.
	__forceinline void incrementCount() {
		count++;
	}
	__forceinline void decrementCount() {
		count--;
	}
	__forceinline T& operator[](uint32_t index) {
		return data[index];
	}
};*/



//#define _FB_USE_RAWBYTE
//#define _FB_USE_BITSET
//#define _FB_USE_BITBOOL //TODO: is having problems with large buffers for some reason?
#define _FB_USE_RAWBITS



template <typename T, uint32_t maxLength>
class FlatFlaggedBuffer {
	T data[maxLength];
	#ifdef _FB_USE_RAWBYTE
	uint8_t isValid[maxLength + 8];
	#elif defined _FB_USE_BITSET
	std::bitset<maxLength> isValid;
	#elif defined _FB_USE_BITBOOL
	BitBool<maxLength + 8> isValid;
	#elif defined _FB_USE_RAWBITS
	uint8_t isValid[maxLength / 8 + 1];
	#endif
	uint32_t count = 0;
	//uint32_t maxIndex = 0;
public:
	FlatFlaggedBuffer() {
		clear();
	}
	__forceinline void clear() {
		//isValid = std::bitset<maxLength>();
		//for (uint32_t i = 0; i < maxLength; i++) {
		//	isValid[i] = 0;  //TODO: Required for BitBool, should have something more optimized.
		//}
		memset(isValid, 0, sizeof(isValid));
	}
	// Consider using in conjunction with incrementCount.
	__forceinline uint32_t toggleFirstInvalid() {
		#ifdef _FB_USE_RAWBYTE
		//NOTE: Used with normal byte buffer, byte per element not bit per element.
		uint32_t max = maxLength / 4;
		uint32_t* bufferAs32 = (uint32_t*)&isValid[0];
		for (unsigned int i = 0; i < max; i++) {
			if (bufferAs32[i] == -1) continue;
			for (unsigned int j = 0; j < 4; j++) {
				if (isValid[i * 4 + j] == 0) {
					setValid(i * 4 + j);
					return i * 4 + j;
				}
			}
		}

		#elif defined _FB_USE_BITSET
		//NOTE: Used with bitset
		for (unsigned int i = 0; i < maxLength; i++) {
			if (isValid[i] == 0) {
				setValid(i);
				return i;
			}
		}

		#elif defined _FB_USE_BITBOOL
		uint32_t max = (maxLength / (4 * 8))+(4*8);
		uint32_t* bufferAs32 = (uint32_t*)&isValid.data[0];
		for (unsigned int i = 0; i < max; i++) {
			if (bufferAs32[i] == -1) continue;
			for (unsigned int j = i*(4*8); j < i*(4*8)+4*8; j++) {
				if (isValid[j] == 0) {
					setValid(j);
					return j;
				}
			}
		}

		#elif defined _FB_USE_RAWBITS
		constexpr uint32_t unrollCount = 1, bytesPerChunk = 8, bitsPerChunk = bytesPerChunk * 8;
		uint32_t max = maxLength / (bytesPerChunk * unrollCount);
		uint64_t* bufferAs64 = (uint64_t*)&isValid[0];
		for (uint32_t i = 0; i < max; i++) {
			uint64_t chunk = bufferAs64[i];
			if (chunk == -1) continue;
			for (uint32_t j = 0; j < 8 * bytesPerChunk; j++) {
				uint32_t index = j + i * bitsPerChunk * unrollCount;
				//if (isBitSet(chunk, j) == false) {
				if(getIsValid(index) == false) {
					//setBit(chunk, j);
					//setValid(j + i * bytesPerChunk);
					//return j + i * bytesPerChunk;
					setValid(index);
					return index;
				}
			}
		}
		#endif

		return -1;
	}
	__forceinline void setInvalid(uint32_t index) {
		#ifdef _FB_USE_RAWBITS
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		clearBit(isValid[chunkIndex], bitIndex);
		#elif
		isValid[index] = 0;
		#endif
	}
	__forceinline void setValid(uint32_t index) {
		#ifdef _FB_USE_RAWBITS
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		setBit(isValid[chunkIndex], bitIndex);
		#elif
		isValid[index] = 1;
		#endif
	}
	__forceinline uint32_t insert(const T& component) {
		uint32_t index = toggleFirstInvalid();
		count++;
		data[index] = component;
		return index;
	}
	__forceinline bool getIsValid(uint32_t index) {
		#ifdef _FB_USE_RAWBITS
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		return isBitSet(isValid[chunkIndex], bitIndex);
		#elif
		return (bool)isValid[index];
		#endif
	}
	__forceinline uint32_t getCount() {
		return count;
	}
	// To be used with toggleFirstInvalid.
	__forceinline void incrementCount() {
		count++;
	}
	__forceinline void decrementCount() {
		count--;
	}
	__forceinline T& operator[](uint32_t index) {
		return data[index];
	}

private:
	inline bool isBitSet(uint64_t n, uint8_t k) {
		if (n & (1 << k))
			return true;
		return false;
	}

	inline void setBit(uint8_t& n, uint8_t k) {
		n = (n | (1 << (k)));
	}
	inline void setBit(uint64_t& n, uint8_t k) {
		n = (n | (1 << (k)));
	}

	inline void clearBit(uint8_t& n, uint8_t k) {
		n = (n & (~(1 << (k))));
	}
	// Function to clear the kth bit of n
	inline void clearBit(uint64_t& n, uint8_t k) {
		n = (n & (~(1 << (k))));
	}

	// Function to toggle the kth bit of n
	inline void toggleBit(uint64_t& n, uint8_t k) {
		n = (n ^ (1 << (k)));
	}
};



template <uint32_t maxLength>
class FlatFlagBuffer {
	uint8_t isValid[maxLength / 8 + 1];
	uint32_t count = 0;
	//uint32_t maxIndex = 0;
public:
	FlatFlagBuffer() {
		clear();
	}
	__forceinline void clear() {
		memset(isValid, 0, sizeof(isValid));
	}
	// Consider using in conjunction with incrementCount.
	__forceinline uint32_t toggleFirstInvalid() {
		constexpr uint32_t unrollCount = 1, bytesPerChunk = 8, bitsPerChunk = bytesPerChunk * 8;
		uint32_t max = maxLength / (bitsPerChunk * unrollCount);
		uint64_t* bufferAs64 = (uint64_t*)&isValid[0];
		for (uint32_t i = 0; i < max; i++) {
			uint64_t chunk = bufferAs64[i];
			if (chunk == -1) continue;
			for (uint32_t j = 0; j < bitsPerChunk; j++) {
				uint32_t index = j + i * bitsPerChunk * unrollCount;
				//if (isBitSet(chunk*8, j) == false) {
				if(getIsValid(index) == false) {
					//setBit(chunk, j);
					setValid(index);
					return index;
				}
			}
		}

		throw;
		return -1;
	}
	uint32_t toggleFirstInvalid(uint32_t startingIndex) {
		uint32_t remainder = startingIndex % (8 * 8);
		for (uint32_t i = 0; i < remainder; i++)
			if (getIsValid(startingIndex + i) == false) {
				setValid(startingIndex + i);
				return startingIndex + i;
			}
		startingIndex = startingIndex + remainder;
		constexpr uint32_t unrollCount = 4, bytesPerChunk = 8, bitsPerChunk = bytesPerChunk * 8;
		uint32_t max = maxLength / (bitsPerChunk * unrollCount);
		uint64_t* bufferAs64 = (uint64_t*)&isValid[0];
		for (uint32_t i = startingIndex  / (bitsPerChunk * unrollCount); i < max; i+=unrollCount) {
			register uint64_t chunk = bufferAs64[i];
			register uint64_t chunk2 = bufferAs64[i+1];
			register uint64_t chunk3 = bufferAs64[i+2];
			register uint64_t chunk4 = bufferAs64[i+3];
			if (chunk == -1 && chunk2 == -1 && chunk3 == -1 && chunk4 == -1) continue;
			for (uint32_t j = 0; j < bitsPerChunk * unrollCount; j++) {
				uint32_t index = startingIndex + (j + i * bitsPerChunk);// * unrollCount;  //This right?
				//if (isBitSet(chunk*8, j) == false) {
				if(getIsValid(index) == false) {
					//setBit(chunk, j);
					setValid(index);
					return index;
				}
			}
		}

		throw;
		return -1;
	}
	__forceinline void setInvalid(uint32_t index) {
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		clearBit(isValid[chunkIndex], bitIndex);
	}
	__forceinline void setValid(uint32_t index) {
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		setBit(isValid[chunkIndex], bitIndex);
	}
	__forceinline bool getIsValid(uint32_t index) {
		uint32_t chunkIndex = index / 8;
		uint32_t bitIndex = index % 8;
		return isBitSet(isValid[chunkIndex], bitIndex);
	}
	__forceinline uint32_t getCount() {
		return count;
	}
	__forceinline void incrementCount() {
		count++;
	}
	__forceinline void decrementCount() {
		count--;
	}

private:
	inline bool isBitSet(uint64_t n, uint8_t k) {
		if (n & (1 << k))
			return true;
		return false;
	}

	inline void setBit(uint8_t& n, uint8_t k) {
		n = (n | (1 << (k)));
	}
	inline void setBit(uint64_t& n, uint8_t k) {
		n = (n | (1 << (k)));
	}

	inline void clearBit(uint8_t& n, uint8_t k) {
		n = (n & (~(1 << (k))));
	}
	// Function to clear the kth bit of n
	inline void clearBit(uint64_t& n, uint8_t k) {
		n = (n & (~(1 << (k))));
	}

	// Function to toggle the kth bit of n
	inline void toggleBit(uint64_t& n, uint8_t k) {
		n = (n ^ (1 << (k)));
	}
};



//template <typename T>
//class DynamicFlaggedBuffer {
//	T* data;
//	uint8_t isValid[maxLength / 8 + 1];
//	uint32_t count = 0;
//	uint32_t maxIndex = 0;
//public:
//	FlatFlaggedBuffer() {
//		clear();
//	}
//	__forceinline void clear() {
//		//isValid = std::bitset<maxLength>();
//		//for (uint32_t i = 0; i < maxLength; i++) {
//		//	isValid[i] = 0;  //TODO: Required for BitBool, should have something more optimized.
//		//}
//		memset(isValid, 0, sizeof(isValid));
//	}
//	// Consider using in conjunction with incrementCount.
//	__forceinline uint32_t toggleFirstInvalid() {
//#ifdef _FB_USE_RAWBYTE
//		//NOTE: Used with normal byte buffer, byte per element not bit per element.
//		uint32_t max = maxLength / 4;
//		uint32_t* bufferAs32 = (uint32_t*)&isValid[0];
//		for (unsigned int i = 0; i < max; i++) {
//			if (bufferAs32[i] == -1) continue;
//			for (unsigned int j = 0; j < 4; j++) {
//				if (isValid[i * 4 + j] == 0) {
//					setValid(i * 4 + j);
//					return i * 4 + j;
//				}
//			}
//		}
//
//#elif defined _FB_USE_BITSET
//		//NOTE: Used with bitset
//		for (unsigned int i = 0; i < maxLength; i++) {
//			if (isValid[i] == 0) {
//				setValid(i);
//				return i;
//			}
//		}
//
//#elif defined _FB_USE_BITBOOL
//		uint32_t max = (maxLength / (4 * 8)) + (4 * 8);
//		uint32_t* bufferAs32 = (uint32_t*)&isValid.data[0];
//		for (unsigned int i = 0; i < max; i++) {
//			if (bufferAs32[i] == -1) continue;
//			for (unsigned int j = i * (4 * 8); j < i * (4 * 8) + 4 * 8; j++) {
//				if (isValid[j] == 0) {
//					setValid(j);
//					return j;
//				}
//			}
//		}
//
//#elif defined _FB_USE_RAWBITS
//		constexpr uint32_t unrollCount = 1, bytesPerChunk = 8;
//		uint32_t max = maxLength / (bytesPerChunk * unrollCount);
//		uint64_t* bufferAs64 = (uint64_t*)&isValid[0];
//		for (uint32_t i = 0; i < max; i++) {
//			uint64_t chunk = bufferAs64[i];
//			if (chunk == -1) continue;
//			for (uint32_t j = 0; j < 8 * bytesPerChunk; j++) {
//				if (isBitSet(chunk, j) == false) {
//					setBit(chunk, j);
//					return j + i * bytesPerChunk;
//				}
//			}
//		}
//#endif
//
//		return -1;
//	}
//	__forceinline void setInvalid(uint32_t index) {
//#ifdef _FB_USE_RAWBITS
//		uint32_t chunkIndex = index / 8;
//		uint32_t bitIndex = index % 8;
//		clearBit(isValid[chunkIndex], bitIndex);
//#elif
//		isValid[index] = 0;
//#endif
//	}
//	__forceinline void setValid(uint32_t index) {
//#ifdef _FB_USE_RAWBITS
//		uint32_t chunkIndex = index / 8;
//		uint32_t bitIndex = index % 8;
//		setBit(isValid[chunkIndex], bitIndex);
//#elif
//		isValid[index] = 1;
//#endif
//	}
//	__forceinline uint32_t insert(const T& component) {
//		uint32_t index = toggleFirstInvalid();
//		count++;
//		data[index] = component;
//		return index;
//	}
//	__forceinline bool getIsValid(uint32_t index) {
//#ifdef _FB_USE_RAWBITS
//		uint32_t chunkIndex = index / 8;
//		uint32_t bitIndex = index % 8;
//		return isBitSet(isValid[chunkIndex], bitIndex);
//#elif
//		return (bool)isValid[index];
//#endif
//	}
//	__forceinline uint32_t getCount() {
//		return count;
//	}
//	// To be used with toggleFirstInvalid.
//	__forceinline void incrementCount() {
//		count++;
//	}
//	__forceinline void decrementCount() {
//		count--;
//	}
//	__forceinline T& operator[](uint32_t index) {
//		return data[index];
//	}
//
//private:
//	inline bool isBitSet(uint64_t n, uint8_t k) {
//		if (n & (1 << k))
//			return true;
//		return false;
//	}
//	inline void setBit(uint64_t n, uint8_t k) {
//		n = (n | (1 << (k - 1)));
//	}
//
//	// Function to clear the kth bit of n
//	inline void clearBit(uint64_t& n, uint8_t k) {
//		n = (n & (~(1 << (k - 1))));
//	}
//
//	// Function to toggle the kth bit of n
//	inline void toggleBit(uint64_t& n, uint8_t k) {
//		n = (n ^ (1 << (k - 1)));
//	}
//};



template <typename T, uint32_t maxLength>
class FlatBuffer {
	T data[maxLength];
public:
	uint32_t count = 0;
	__forceinline T& operator[](uint32_t index) {
		return data[index];
	}
	__forceinline void push(T _data) {
		data[count] = _data;
		count++;
	}
	__forceinline void pop() {
		count--;
	}
	__forceinline void clear() {
		count = 0;
	}
	__forceinline uint32_t findFirst(T _data) {
		for (uint32_t i = 0; i < count; i++)
			if (data[i] == _data) return i;
		return -1;
	}
	__forceinline bool has(T _data) {
		for (uint32_t i = 0; i < count; i++)
			if (data[i] == _data) return true;
		return false;
	}
};







#include <iostream>

#ifdef TESTING
std::string T_FlatFlaggedBufferInitialize() {
	FlatFlaggedBuffer<uint32_t, 10> ffb;
	for (uint32_t i = 10; i > 0; i--) {
		ffb.insert(i);
	}
	if (ffb.getCount() != 10) {
		std::string retValue = "Count is incorrect, expected 10, instead got ";
		retValue += std::to_string(ffb.getCount());
		return retValue;
	}
	uint32_t expectedBuffer[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	bool mismatch = false;
	for (uint32_t i = 0; i < ffb.getCount(); i++) {
		if (ffb[i] != expectedBuffer[i])
			mismatch = true;
	}
	if (mismatch) {
		std::string retValue = "Mismatched buffer, got...\n";
		for (uint32_t i = 0; i < ffb.getCount(); i++)
			retValue += std::to_string(ffb[i]) + std::string(" ");
		retValue += " \nexpected...\n";
		for (uint32_t i = 0; i < ffb.getCount(); i++)
			retValue += std::to_string(expectedBuffer[i]) + std::string(" ");;
		return retValue;
	}
	return "";
}
std::string T_FlatFlaggedBufferBigInitialize() {
	constexpr uint32_t max = 100000;
	static FlatFlaggedBuffer<uint32_t, max> ffb;
	for (uint32_t i = 0; i < max; i++) {
		ffb.insert(i);
	}
	for (uint32_t i = 0; i < max; i++) {
		if (ffb[i] != i) {
			std::string retValue = "";
			retValue += "Failed to do a big initialization, error at index ";
			retValue += std::to_string(i);
			retValue += " expected index value but got ";
			retValue += std::to_string(ffb[i]);
			return retValue;
		}
	}
	return "";
}
std::string T_FlatFlaggedBufferGetFirstInvalid() {
	FlatFlaggedBuffer<uint32_t, 100> ffb;
	for (uint32_t i = 0; i < 100; i++) {
		uint32_t index = ffb.toggleFirstInvalid();
		ffb[index] = i;
	}
	for (uint32_t i = 0; i < 100; i++) {
		if (ffb.getIsValid(i) == false) {
			std::string retValue = "";
			retValue += "Failed set element as valid at index ";
			retValue += std::to_string(i);
			return retValue;
		}
		if (ffb[i] != i) {
			std::string retValue = "";
			retValue += "Failed to match proper value ";
			retValue += std::to_string(ffb[i]);
			retValue += " but got ";
			retValue += std::to_string(i);
			return retValue;
		}
	}
	return "";
}
std::string T_FlatFlaggedBufferScramble() {
	FlatFlaggedBuffer<uint32_t, 100> ffb;
	for (uint32_t i = 0; i < 100; i++) {
		uint32_t index = ffb.toggleFirstInvalid();
		ffb[index] = i;
	}
	for (uint32_t i = 0; i < 100; i++) {
		if(i % 2)
			ffb.setInvalid(i);
	}
	for (uint32_t i = 0; i < 100; i++) {
		if (i % 2) {
			if (ffb.getIsValid(i) == true) {
				std::string retValue = "";
				retValue += "Failed set element as invalid at index ";
				retValue += std::to_string(i);
				return retValue;
			}
			continue;
		}
		if (ffb.getIsValid(i) == false) {
			std::string retValue = "";
			retValue += "Failed set element as valid at index ";
			retValue += std::to_string(i);
			return retValue;
		}
		if (ffb[i] != i) {
			std::string retValue = "";
			retValue += "Failed to match proper value ";
			retValue += std::to_string(ffb[i]);
			retValue += " but got ";
			retValue += std::to_string(i);
			return retValue;
		}
	}
	return "";
}
#endif
inline void testFlatFlaggedBuffer() {
#ifdef TESTING
	std::cout << "Testing flat flagged buffer..." << std::endl;
	std::string dbgStr = "";
	dbgStr = T_FlatFlaggedBufferInitialize();
	if (dbgStr != "") {
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_FlatFlaggedBufferBigInitialize();
	if (dbgStr != "") {
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_FlatFlaggedBufferGetFirstInvalid();
	if (dbgStr != "") {
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
	dbgStr = T_FlatFlaggedBufferScramble();
	if (dbgStr != "") {
		std::cout << dbgStr << std::endl;
		dbgStr = "";
	}
#endif
}