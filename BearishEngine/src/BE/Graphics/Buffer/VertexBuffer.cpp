#include <BE/Graphics/Buffer/VertexBuffer.h>
#include <BE/Graphics/Renderer.h>

using namespace Bearish;
using namespace Graphics;

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &_id);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::BindForFeedback() const {
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _id);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(void* data, const u64 dataSize, const DrawStyle style) const {
	Bind();
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(dataSize), data, (GLenum)style);
}

void VertexBuffer::SetData(Vertex* vertices, const u64 numVertices, const DrawStyle style) const {
	SetData((f32*)vertices, numVertices * sizeof(Vertex), style);
}

void VertexBuffer::SubData(void* data, const u64 start, const u64 dataSize) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)start, (GLsizeiptr)dataSize, data);
}