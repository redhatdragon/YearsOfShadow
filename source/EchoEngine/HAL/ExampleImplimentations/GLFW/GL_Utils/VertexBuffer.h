#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

class VertexBuffer {
	uint32_t id;
public:
	void init(const void* data, uint32_t size);
	void destruct();
	void buffer(const void* data, uint32_t size);
	void bind() const;
	void unbind() const;
};



void VertexBuffer::init(const void* data, uint32_t size) {
	glGenBuffers(1, &id);
	//glBindBuffer(GL_ARRAY_BUFFER, id);
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	buffer(data, size);
}
void VertexBuffer::destruct() {
	glDeleteBuffers(1, &id);
}
void VertexBuffer::buffer(const void* data, uint32_t size) {
	bind();
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}
void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}