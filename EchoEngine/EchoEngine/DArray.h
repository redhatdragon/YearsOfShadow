#pragma once
#include <stdint.h>

/*
Meant to only use trivial POD types.
Shouldn't be used to store a buffer of pointers to objects
*/
template<typename T>
class DArray {
public:
#pragma warning(push)
#pragma warning (disable : 4200)

	struct DataHeader {
		size_t count;
		T data[1];
	};
#pragma warning(pop)

	DataHeader* dataHeader = nullptr;

	DArray() = default;
	void init(size_t size) {
		dataHeader = (DataHeader*)malloc(sizeof(DataHeader) + sizeof(T) * size);
		if (dataHeader == nullptr) {
            throw std::logic_error("Error: DArray::init(size) threw, unable to allocate " 
				+ std::to_string(size) + " bytes via malloc. \n");
		}
		memset(dataHeader, 0, (uint64_t)sizeof(DataHeader) + sizeof(T) * size);
		dataHeader->count = size;
	}
	void initCopy(const DArray<T>& data) {
		initCopy(data->dataHeader->data, data->size());
	}
	void initCopy(const void* data, size_t size) {
        dataHeader = std::construct_at<DataHeader>(
			reinterpret_cast<DataHeader*>(malloc(sizeof(DataHeader) + sizeof(T) * size)), size);
		//if (dataHeader == nullptr)
        //    throw std::logic_error("Error: DArray::initCopy()'s failed to allocate enough memory!");
		memcpy(dataHeader->data, data, sizeof(T) * size);
		dataHeader->count = size;
	}
	//Allows for downsizing for free.  Upsizing will cost
	void replaceWith(const void* data, size_t size) {
		if (size > dataHeader->count) {
			free();
			initCopy(data, size);
		}
		dataHeader->count = size;
		memcpy(dataHeader->data, data, sizeof(T) * size);
	}
	void free() {
		//TODO: add better logging...
		if (dataHeader == nullptr) {
			std::cout << "Warning: DArray::free() attempted to deallocate while already empty" << std::endl;
			return;
		}
		::free(dataHeader);
		dataHeader = nullptr;
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