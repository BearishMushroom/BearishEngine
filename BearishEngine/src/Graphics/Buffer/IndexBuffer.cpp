#include "IndexBuffer.h"

using namespace Bearish;
using namespace Graphics;

IndexBuffer::IndexBuffer() {
	_size = 0;
	glGenBuffers(1, &_id);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_id);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(u32* indices, const u32 numIndices) {
	_size = numIndices;
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(u32), indices, GL_STATIC_DRAW);
}

const i32 IndexBuffer::GetSize() const {
	return _size;
}