#include "VBO.h"
#include "../Renderer.h"

using namespace Bearish;
using namespace Graphics;

VBO::VBO() {
	glGenBuffers(1, &_id);
}

VBO::~VBO(){
	glDeleteBuffers(1, &_id);
}

void VBO::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::BindForFeedback() const {
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _id);
}


void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::SetData(void* data, const u64 dataSize, const DrawStyle style) const {
	Bind();
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(dataSize), data, (GLenum)style);
}

void VBO::SetData(Vertex* vertices, const u64 numVertices, const DrawStyle style) const {
	SetData((f32*)vertices, numVertices * sizeof(Vertex), style);
}

void VBO::SubData(void* data, const u64 start, const u64 dataSize) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, start, dataSize, data);
}