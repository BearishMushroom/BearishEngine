#include "IBO.h"

using namespace Bearish;
using namespace Graphics;

IBO::IBO() {
	_size = 0;
	glGenBuffers(1, &_id);
}

IBO::~IBO() {
	glDeleteBuffers(1, &_id);
}

void IBO::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IBO::SetData(u32* indices, const u32 numIndices) {
	_size = numIndices;
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(u32), indices, GL_STATIC_DRAW);
}

const i32 IBO::GetSize() const {
	return _size;
}