#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <HAL/ExampleImplimentations/GLFW/GL_Utils/GL_Call.h>

class IndexBuffer {
	uint32_t id;
	uint32_t count;
public:

	IndexBuffer() = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& other) noexcept
		: id(std::exchange(other.id, 0)), count(std::exchange(other.count, 0)) {}
	IndexBuffer& operator=(IndexBuffer&& other) noexcept {
		if (id != 0)
			GL_CALL(glDeleteBuffers(1, &id));
		id = std::exchange(other.id, 0);
		count = std::exchange(other.count, 0);
		return *this;
	}
	IndexBuffer(const uint32_t* data, uint32_t size) :
	IndexBuffer(data, count, 4) {

	}
	~IndexBuffer() {
		if (id != 0)
			GL_CALL(glDeleteBuffers(1, &id));
	}

	IndexBuffer(const void* data, uint32_t count, uint8_t bytesPerIndex) {
		GL_CALL(glGenBuffers(1, &id));
		buffer(data, count, bytesPerIndex);
	}

	void destruct() {
		GL_CALL(glDeleteBuffers(1, &id));
	}
	void buffer(const void* data, uint32_t count, uint8_t bytesPerIndex = 4) {
		this->count = count;
		bind();
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * bytesPerIndex, data, GL_DYNAMIC_DRAW));
	}
	void bind() const {
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	}
	void unbind() const {
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	inline uint32_t getCount() const {
		return count;
	}
};