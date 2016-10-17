#include "ParticleSystem.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;

const i32 ParticleSystem::MAX_PARTICLES = 2000;

ParticleSystem::ParticleSystem(Texture* texture, std::function<Particle(const Math::vec3&)> emitFunc, std::function<void(Particle*, f32)> updateFunc) :
	_texture(texture), _emit(emitFunc), _update(updateFunc) {
	_particles.reserve(MAX_PARTICLES);
	Particle ex;
	ex.life = 0;
	for (i32 i = 0; i < MAX_PARTICLES; i++) {
		_particles.push_back(ex);
	}

	_state.Bind();
	_renderData.SetData((void*)0, MAX_PARTICLES * sizeof(Particle), DrawStyle::Stream);
	VertexLayout<Particle> layout;
	layout.PushComponent("POSITION", sizeof(vec3));
	layout.PushComponent("SCALE", sizeof(vec3));
	layout.PushComponent("ROTATION", sizeof(f32));
	layout.PushComponent("COLOR", sizeof(vec4));
	_renderData.SetLayout(layout);
	_state.Unbind();

	_shader = new Shader();
	_shader->AddShader(Shader::LoadShader("res/particle.vert"), ShaderType::Vertex);
	_shader->AddShader(Shader::LoadShader("res/particle.geom"), ShaderType::Geometry);
	_shader->AddShader(Shader::LoadShader("res/particle.frag"), ShaderType::Fragment);
	_shader->Compile();
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::Emit(i32 count, const Math::vec3& position) {
	i32 lastUnused = 0;
	while (count) {
		for (i32 i = lastUnused; i < MAX_PARTICLES; i++) {
			Particle& p = _particles[i];
			if (p.life <= 0) {
				_particles[i] = _emit(position);
				lastUnused = i;
				count--;
				break;
			}
		}
	}
}

void ParticleSystem::Update(Camera* camera, f32 dt) {
	_aliveParticles = 0;
	for (i32 i = 0; i < MAX_PARTICLES; i++) {
		Particle* p = &_particles[i];
		if (p->life > 0) {
			_update(p, dt);
			p->cameraDistance = (camera->GetTransform().GetTranslation() - p->position).LengthSq();
			_aliveParticles++;
		}
		else {
			p->cameraDistance = -1.0;
		}
	}

	//std::sort(_particles.begin(), _particles.end());
	_renderData.SubData((void*)&_particles[0], 0, _aliveParticles * sizeof(Particle));
}

void ParticleSystem::Draw(Camera* camera) {
	_shader->Bind();
	_shader->SetUniform("diffuse", 0);
	_shader->SetUniform("MVP", camera->GetViewMatrix());
	_shader->SetUniform("eyePos", camera->GetTransform().GetTranslation());
	_shader->SetUniform("up", camera->GetTransform().GetRotation().Up());
	_texture->Bind(0);
	_state.Bind();
	glDrawArrays(GL_POINTS, 0, _aliveParticles);
	_state.Unbind();
	_texture->Unbind(0);
	_shader->Unbind();
}