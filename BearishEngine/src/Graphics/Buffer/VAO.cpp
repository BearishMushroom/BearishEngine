#include "VAO.h"

using namespace Bearish;
using namespace Graphics;

VAO::VAO() {
	glGenVertexArrays(1, &_id);
}

VAO::~VAO() {
	//glDeleteVertexArrays(1, &_id);
}

void VAO::Bind() const {
	glBindVertexArray(_id);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}