#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

class IndexBuffer {
	uint32_t id;
	uint32_t count;
public:
	void init(const void* data, uint32_t count, uint8_t bytesPerIndex = 4);
	void destruct();
	void buffer(const void* data, uint32_t count, uint8_t bytesPerIndex = 4);
	void bind() const;
	void unbind() const;
	inline uint32_t getCount() const;
};

void IndexBuffer::init(const void* data, uint32_t count, uint8_t bytesPerIndex) {
	glGenBuffers(1, &id);
	buffer(data, count, bytesPerIndex);
}
void IndexBuffer::destruct() {
	glDeleteBuffers(1, &id);
}
void IndexBuffer::buffer(const void* data, uint32_t count, uint8_t bytesPerIndex) {
	this->count = count;
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * bytesPerIndex, data, GL_DYNAMIC_DRAW);
}
void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
inline uint32_t IndexBuffer::getCount() const {
	return count;
}