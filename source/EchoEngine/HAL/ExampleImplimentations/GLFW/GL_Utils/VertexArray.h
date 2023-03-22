#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	uint32_t id;
public:
	void init();
	void destruct();
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind() const;
	void unbind() const;
};



void VertexArray::init() {
	glGenVertexArrays(1, &id);
}
void VertexArray::destruct() {
	glDeleteVertexArrays(1, &id);
}
void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	uint32_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * element.getSize();
		if (element.isForInstance)
			glVertexAttribDivisor(i, 1);  //ever 'instance' (not vertex) gets this attribute
	}
}
void VertexArray::bind() const {
	glBindVertexArray(id);
}
void VertexArray::unbind() const {
	glBindVertexArray(0);
}