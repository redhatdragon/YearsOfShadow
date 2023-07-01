#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>

class VertexBuffer {
	uint32_t id;
public:
	void init() {
		GL_CALL(glGenBuffers(1, &id));
	}
	void init(const void* data, uint32_t size);
	void destruct();
	void buffer(const void* data, uint32_t size);
	void bind() const;
	void unbind() const;
};



void VertexBuffer::init(const void* data, uint32_t size) {
	GL_CALL(glGenBuffers(1, &id));
	buffer(data, size);
}
void VertexBuffer::destruct() {
	GL_CALL(glDeleteBuffers(1, &id));
}
void VertexBuffer::buffer(const void* data, uint32_t size) {
	bind();
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
void VertexBuffer::bind() const {
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}
void VertexBuffer::unbind() const {
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}