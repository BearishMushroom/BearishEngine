#include "UBO.h"

using namespace Bearish;
using namespace Core;
using namespace Graphics;

UBO::UBO() {
	glGenBuffers(1, &_id);
	_base = 0;
}

UBO::~UBO() {
	glDeleteBuffers(1, &_id);
}

void UBO::Bind(i32 base) {
	if (base == -1) base = _base;
	glBindBufferBase(GL_UNIFORM_BUFFER, base, _id);
}

void UBO::Unbind(i32 base) {
	if (base == -1) base = _base;
	glBindBufferBase(GL_UNIFORM_BUFFER, base, 0);
}

void UBO::SetData(void* data, u32 size) {
	Bind();
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	Unbind();
}

void UBO::UpdateData(void* data, u32 size) {
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	Unbind();
}