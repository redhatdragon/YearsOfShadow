#pragma once
#include <vector>
#include <gl/glew.h>
#include <iostream>

struct VertexBufferElement {
	uint32_t type;
	uint32_t count;
	uint8_t normalized;
	bool isForInstance;

	inline uint32_t getSize() const {
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		std::cout << "VertexBufferElement::getSize(): VertexBufferElement's type isn't recognized" << std::endl;
		return 0;
	}
};

class VertexBufferLayout {
	std::vector<VertexBufferElement> elements;
	uint32_t stride;
public:
	VertexBufferLayout() { stride = 0; };
	~VertexBufferLayout() {};

	template<typename T>
	void push(uint32_t count, bool isForInstance = false) {
		static_assert(false);
	}
	template<>
	void push<float>(uint32_t count, bool isForInstance) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE, isForInstance });
		stride += sizeof(GLfloat) * count;
	}
	template<>
	void push<uint32_t>(uint32_t count, bool isForInstance) {
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, isForInstance });
		stride += sizeof(GLuint) * count;
	}
	template<>
	void push<uint8_t>(uint32_t count, bool isForInstance) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, isForInstance });
		stride += sizeof(GLchar) * count;
	}
	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline uint32_t getStride() const { return stride; }
};