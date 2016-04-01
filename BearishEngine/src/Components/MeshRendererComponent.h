#ifndef _BEARISH_COMPONENTS_MESHRENDERER_H_
#define _BEARISH_COMPONENTS_MESHRENDERER_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Core/Asset.h"
#include "../Graphics/Texture/Texture.h"
#include "../Graphics/Mesh/Mesh.h"
#include "../Types.h"
#include "../Graphics/Material.h"

namespace Bearish { namespace Components {
	class MeshRendererComponent : public Core::IActorComponent, public Core::IAllocatable<MeshRendererComponent> {
	public:
		MeshRendererComponent(Graphics::Mesh* mesh, Graphics::Material* material);
		~MeshRendererComponent();

		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override;
	private:
		Graphics::Mesh* _mesh;		
		Graphics::Material* _material;
	};

	class AnimatedMeshRendererComponent : public Core::IActorComponent, public Core::IAllocatable<AnimatedMeshRendererComponent> {
	public:
		AnimatedMeshRendererComponent(string name, f32 speed, Graphics::Mesh* mesh, Graphics::Material* material);
		~AnimatedMeshRendererComponent();

		virtual void FixedUpdate() override;
		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override;
		void SetMaterial(Graphics::Material* material) {
			_material = material;
		}
	private:
		Graphics::MeshAnimation* _anim;
		Graphics::Mesh* _mesh;
		Graphics::Material* _material;
	};
} }

#endif