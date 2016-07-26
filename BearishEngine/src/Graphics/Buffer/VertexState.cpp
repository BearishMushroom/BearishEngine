#include "VertexState.h"

using namespace Bearish;
using namespace Graphics;

VertexState::VertexState() {
	glGenVertexArrays(1, &_id);
}

VertexState::~VertexState() {
	glDeleteVertexArrays(1, &_id);
}

void VertexState::Bind() const {
	glBindVertexArray(_id);
}

void VertexState::Unbind() {
	glBindVertexArray(0);
}