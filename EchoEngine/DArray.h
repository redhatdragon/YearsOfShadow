#pragma once
#include <stdint.h>

template<typename T>
class DArray {
public:
	struct DataHeader {
		size_t count;
		T data[];
	};
	DataHeader* dataHeader = nullptr;

	DArray() = default;
	void init(size_t size) {
		dataHeader = (DataHeader*)malloc(sizeof(DataHeader) + sizeof(T) * size);
		if (dataHeader == nullptr) {
			std::cout << "Error: DArray::init(size) threw, unable to allocate "
				<< size << " bytes via malloc." << std::endl;
			throw;
		}
		memset(dataHeader, 0, (uint64_t)sizeof(DataHeader) + sizeof(T) * size);
		dataHeader->count = size;
	}
	void initCopy(const void* data, size_t size) {
		//::free(dataHeader);
		dataHeader = (DataHeader*)malloc(sizeof(DataHeader) + sizeof(T) * size);
		if (dataHeader == nullptr) throw;
		memcpy(dataHeader, data, sizeof(DataHeader) + sizeof(T) * size);
		dataHeader->count = size;
	}
	void free() {
		::free(dataHeader);
	}
	T& operator[](size_t index) {
		return dataHeader->data[index];
	}
	const T& operator[](size_t index) const {
		return dataHeader->data[index];
	}
	T* data() {
		return dataHeader->data;
	}
	//void push_back(T data) {
	//
	//}
	size_t size() const {
		return dataHeader->count;
	}
};