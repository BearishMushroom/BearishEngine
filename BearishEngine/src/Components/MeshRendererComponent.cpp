#include "../Core/Actor.h"
#include "MeshRendererComponent.h"
#include "../Graphics/Material.h"

using namespace Bearish;
using namespace Components;
using namespace Graphics;

MeshRendererComponent::MeshRendererComponent(Graphics::Mesh* mesh, Graphics::Material* material) {
	_mesh = mesh;
	_id = "MeshRenderer";
	_material = material;
}

MeshRendererComponent::~MeshRendererComponent() { }

void MeshRendererComponent::Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
	auto trans = _actor->GetTransform().GetTransformation();
	_mesh->Submit(&_actor->GetTransform(), trans, camera);
	renderer->Submit(_mesh, _material);
}

AnimatedMeshRendererComponent::AnimatedMeshRendererComponent(string name, f32 speed, Graphics::Mesh* mesh, Graphics::Material* material) {
	_mesh = mesh;
	_id = "MeshRenderer";
	_anim = mesh->GetAnimation(name);
	_anim->SetTimeScale(speed);
	_anim->Start();
	_mesh->DecoupleSkeleton();
	_material = material;
}

AnimatedMeshRendererComponent::~AnimatedMeshRendererComponent() { }

void AnimatedMeshRendererComponent::FixedUpdate() {
	_mesh->Animate(1.f / 60.f);
}

void AnimatedMeshRendererComponent::Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) {
	auto trans = _actor->GetTransform().GetTransformation();
	_mesh->Submit(&_actor->GetTransform(), trans, camera);

	shader->SetUniform("bones", _mesh->GetBones());

	_mesh->Flush(shader);
}