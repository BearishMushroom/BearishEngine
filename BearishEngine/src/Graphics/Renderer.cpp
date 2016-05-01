#include "Renderer.h"
#include "Vertex.h"

using namespace Bearish;
using namespace Graphics;

const BlendState BlendState::AlphaBlend = { BlendMode::SourceAlpha, BlendMode::OneMinusSourceAlpha };
const BlendState BlendState::Additive = { BlendMode::SourceAlpha, BlendMode::One };

BlendState Renderer::_blendState = BlendState::AlphaBlend;
FillMode Renderer::_renderMode = FillMode::Normal;
PrimitiveMode Renderer::_primitiveMode = PrimitiveMode::Triangles;

Renderer::Renderer() {
	Init();
}

Renderer::~Renderer() {
}

void Renderer::Init() const {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	glPointSize(1.0f);
	glLineWidth(1.0f);
}

void Renderer::EnableAttribArray(const u32 num) {
	glEnableVertexAttribArray(num);
}

void Renderer::DisableAttribArray(const u32 num) {
	glDisableVertexAttribArray(num);
}

void Renderer::SetAttribPointer(const u32 num, const u32 numElements, const u32 vertexSize, const u32 offset, const AttributeType type) {
	if (type == AttributeType::Float32) {
		glVertexAttribPointer(num, numElements, (GLenum)type, GL_FALSE, vertexSize, (void*)offset);
		return;
	}

	glVertexAttribIPointer(num, numElements, (GLenum)type, vertexSize, (void*)offset);
}

void Renderer::SetAttribDivisor(const u32 num, const u32 divisor) {
	glVertexAttribDivisor(num, divisor);
}


void Renderer::Clear() const {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::SetBlendMode(const BlendMode src, const BlendMode dst) {
	SetBlendState({ src, dst });
}

void Renderer::SetBlendState(const BlendState state) {
	if (state.source != _blendState.source || state.destination != _blendState.destination) {
		glBlendFunc((GLenum)state.source, (GLenum)state.destination);
		_blendState = state;
	}
}

void Renderer::SetFillMode(const FillMode mode) {
	if (_renderMode != mode) {
		if (mode != FillMode::Normal) {
			glDisable(GL_TEXTURE_2D);
		}
		else {
			glEnable(GL_TEXTURE_2D);
		}

		glPolygonMode(GL_FRONT_AND_BACK, (GLenum)mode);
		_renderMode = mode;
	}
}

void Renderer::SetPrimitiveMode(const PrimitiveMode priMode) {
	_primitiveMode = priMode;
}
