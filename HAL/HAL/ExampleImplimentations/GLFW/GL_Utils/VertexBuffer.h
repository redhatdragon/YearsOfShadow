#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>

class VertexBuffer {
	uint32_t id;
public:
	VertexBuffer() = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) : id(std::exchange(other.id, 0)) {

	}
	VertexBuffer& operator=(VertexBuffer&& other) {
		if (id != 0)
			GL_CALL(glDeleteBuffers(1, &id));
		id = std::exchange(other.id, 0);
		return *this;
	}
	VertexBuffer(const void* data, uint32_t size) {
		GL_CALL(glGenBuffers(1, &id));
		buffer(data, size);
	}
	~VertexBuffer() {
		if(id != 0)
			GL_CALL(glDeleteBuffers(1, &id));
	}
	void buffer(const void* data, uint32_t size);
	void bind() const;
	void unbind() const;
};



inline void VertexBuffer::buffer(const void* data, uint32_t size) {
	bind();
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
inline void VertexBuffer::bind() const {
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}
inline void VertexBuffer::unbind() const {
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}