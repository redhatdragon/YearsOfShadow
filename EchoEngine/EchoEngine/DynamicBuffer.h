#include <stdint.h>
#include <memory.h>

template<typename T, uint32_t capIncriment>
class DynamicBuffer {
	T* data = (T*)malloc(sizeof(T)*capIncriment);
	uint32_t count = 0;
	uint32_t capacity = capIncriment;
public:
	inline T& operator[](uint32_t index) {
		return data[index];
	}
	inline void push_back(T& element) {
		expandIfFull();
		data[count] = element;
		count++;
	}
private:
	inline void expandIfFull() {
		uint32_t newCapacity = capacity + capIncriment;
		T* otherBuffer = (T*)malloc(sizeof(T) * newCapacity);
		memcpy(otherBuffer, data, capacity);
		capacity = newCapacity;
		free(data);
		data = otherBuffer;
	}
};

#ifdef TESTING
#include <vector>
	void PT_DynamicArray(){

	}
#endif

void testDynamicArray() {
#ifdef TESTING
	void PT_DynamicArray() {

	}
#endif
}