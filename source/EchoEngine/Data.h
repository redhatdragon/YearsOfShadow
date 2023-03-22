#pragma once
#include <stdlib.h>
#include <memory.h>
#include <iostream>

/*class Data {
	void *data;
	size_t size;
public:
	void create(void* from, size_t _size) {
		data = malloc(_size);
		memcpy(data, from, _size);
		size = _size;
	}
	void destruct() {
		free(data);
		size = 0;
	}
	void* get() {
		return data;
	}
	template<typename T>
	T* get() {
		return (T*)data;
	}
};*/

class Data {
	size_t size;
	uint8_t data[1];
public:
	static Data* create(void* from, size_t _size) {
		Data* retValue;
		retValue = (Data*)malloc(_size + sizeof(size_t));
		if (retValue == NULL) throw;
		retValue->size = _size;
		memcpy(&retValue->data[0], from, _size);
		return retValue;
	}
	void set(void* dest, void* src, size_t _size) {

	}
	void* get() {
		return &data[0];
	}
};

/*void foo() {
	int x = 5;
	Data* data = Data::create(&x, sizeof(int));
	char str[] = "Hi";
	Data* data2 = Data::create(&str[0], sizeof(char) * 3);
	std::cout << data2.get<char> << std::endl;
	data.destruct();
	data2.destruct();
}*/