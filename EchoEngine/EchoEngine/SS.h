#pragma once
#include <stdint.h>

class SoftwareStack {
	uint32_t *data;
	//uint32_t size;
	uint32_t capacity;
	uint32_t *sp;
public:
	SoftwareStack() {
		constexpr uint32_t startingCapacity = 1024*4*4;
		//size = 0;
		capacity = startingCapacity;
		data = (uint32_t*)malloc(startingCapacity);
		sp = &data[0];
	}
	SoftwareStack(uint32_t startingCapacity) {
		//size = 0;
		capacity = startingCapacity;
		data = (uint32_t*)malloc(startingCapacity);
		sp = &data[0];
	}
	template<typename T>
	void pushUnsafe(T data) {
		memcpy(sp, data, sizeof(T));
		sp += typeSizeAfterPadding<T>();
	}
	template<typename T>
	void push(T data) {
		uint32_t* newStackPointerPosition = sp+typeSizeAfterPadding<T>();
		if (newStackPointerPosition >= data + capacity) {
			capacity *= 2;
			realloc(data, capacity);
		}
		sp = newStackPointerPosition;
	}
	template<typename T>
	T pop() {

	}
private:
	template<typename T>
	__forceinline constexpr uint32_t typeSizeAfterPadding() {
		if constexpr (sizeof(T) % 4)
			return sizeof(T) / 4 + 1;
		else
			return sizeof(T)/4;
	}
};