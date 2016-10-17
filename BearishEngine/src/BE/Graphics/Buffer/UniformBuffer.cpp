#include "UniformBuffer.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;

UniformBuffer::UniformBuffer() {
	glGenBuffers(1, &_id);
	_base = 0;
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &_id);
}

void UniformBuffer::Bind(i32 base) {
	if (base == -1) base = _base;
	glBindBufferBase(GL_UNIFORM_BUFFER, base, _id);
}

void UniformBuffer::Unbind(i32 base) {
	if (base == -1) base = _base;
	glBindBufferBase(GL_UNIFORM_BUFFER, base, 0);
}

void UniformBuffer::SetData(void* data, u32 size) {
	Bind();
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STREAM_DRAW);
	//Unbind();
}

void UniformBuffer::UpdateData(void* data, u32 size) {
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	//Unbind();
}