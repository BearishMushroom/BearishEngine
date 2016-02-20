#include "../Core/Actor.h"
#include "MeshRendererComponent.h"
#include "../Graphics/Material.h"

using namespace Bearish;
using namespace Components;
using namespace Graphics;

MeshRendererComponent::MeshRendererComponent(Graphics::Mesh* mesh, Graphics::Texture* texture, Graphics::Texture* normalMap) {
	_mesh = mesh;
	_texture = texture;
	_normalMap = normalMap;
	_id = "MeshRenderer";
	_material = new Material(_texture->GetName(), 0);
	_material->Set("diffuse", _texture, 5);
	_material->Set("normalMap", _normalMap, 6);
}

MeshRendererComponent::~MeshRendererComponent() { }

void MeshRendererComponent::Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
	_mesh->Submit(_actor->GetTransform().GetTransformation(), camera->GetViewMatrix() * _actor->GetTransform().GetTransformation());
	if (!_material->GetShader()) {
		_material->SetShader(renderer->_phongShader);
	}
	renderer->Submit(_mesh, _material);
}

AnimatedMeshRendererComponent::AnimatedMeshRendererComponent(string name, f32 speed, Graphics::Mesh* mesh, Graphics::Texture* texture, Graphics::Texture* normalMap) {
	_mesh = mesh;
	_texture = texture;
	_normalMap = normalMap;
	_id = "MeshRenderer";
	_anim = mesh->GetAnimation(name);
	_anim->SetTimeScale(speed);
	_anim->Start();
	_mesh->DecoupleSkeleton();
	_material = new Material(_texture->GetName(), 0);
	_material->Set("diffuse", _texture, 5);
	_material->Set("normalMap", _normalMap, 6);
}

AnimatedMeshRendererComponent::~AnimatedMeshRendererComponent() { }

void AnimatedMeshRendererComponent::FixedUpdate() {
	_mesh->Animate(1.f / 60.f);
}

void AnimatedMeshRendererComponent::Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
	_mesh->Submit(_actor->GetTransform().GetTransformation(), camera->GetViewMatrix() * _actor->GetTransform().GetTransformation());

	if (!_material->GetShader()) {
		_material->SetShader(renderer->_phongShader);
	}

	i32 matID = renderer->PushMaterial(_material);

	shader->SetUniform("bones", _mesh->GetBones());

	if (shader->GetName() == "geom") {
		shader->SetUniform("matID", matID);
	}

	_mesh->Flush();
}