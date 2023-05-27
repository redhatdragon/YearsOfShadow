#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	uint32_t id;
	uint32_t layoutCount;
	uint32_t lastOffset;
public:
	void init();
	void destruct();
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind() const;
	void unbind() const;
};



void VertexArray::init() {
	glGenVertexArrays(1, &id);
	layoutCount = 0;
	lastOffset = 0;
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
		glEnableVertexAttribArray(i+layoutCount);
		glVertexAttribPointer(i+layoutCount, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * element.getSize();
		if (element.isForInstance)
			glVertexAttribDivisor(i+layoutCount, 1);  //ever 'instance' (not vertex) gets this attribute
	}
	layoutCount += elements.size();
}
void VertexArray::bind() const {
	glBindVertexArray(id);
}
void VertexArray::unbind() const {
	glBindVertexArray(0);
}