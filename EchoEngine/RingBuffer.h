#pragma once
#include <stdint.h>

template<typename T, uint32_t size>
class RingBuffer {
	T data[size];
	uint32_t index;
public:
	void init() {
		memset(this, 0, sizeof(*this));
	}
	void advance() {
		if (index == size-1)
			index = 0;
		else
			index++;
	}
	void rewind() {
		if (index == 0)
			index = size - 1;
		else
			index--;
	}
	void advanceFor(uint32_t num) {
		for (uint32_t i = 0; i < num; i++)
			advance();
	}
	void rewindFor(uint32_t num) {
		for (uint32_t i = 0; i < num; i++)
			rewind();
	}
	T& get() {
		return data[index];
	}
	void set(T& _data) {
		data[index] = _data;
	}
};