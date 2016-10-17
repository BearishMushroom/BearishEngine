#ifndef _BEARISH_COMPONENTS_MESHRENDERER_H_
#define _BEARISH_COMPONENTS_MESHRENDERER_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Core/Asset.h>
#include <BE/Graphics/Texture/Texture.h>
#include <BE/Graphics/Mesh/Mesh.h>
#include <BE/Types.h>
#include <BE/Graphics/Material.h>

namespace Bearish { namespace Components {
	class BEARISH_API MeshRendererComponent : public Core::IActorComponent, public Core::IAllocatable<MeshRendererComponent> {
	public:
		MeshRendererComponent(Graphics::Mesh* mesh, Graphics::Material* material);
		~MeshRendererComponent();

		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override;

		virtual Math::vec3 GetBounds() override {
			return _mesh->GetExtremes();
		}
	private:
		Graphics::Mesh* _mesh;		
		Graphics::Material* _material;
	};

	class BEARISH_API AnimatedMeshRendererComponent : public Core::IActorComponent, public Core::IAllocatable<AnimatedMeshRendererComponent> {
	public:
		AnimatedMeshRendererComponent(string name, f32 speed, Graphics::Mesh* mesh, Graphics::Material* material);
		~AnimatedMeshRendererComponent();

		virtual void FixedUpdate() override;
		virtual void Draw(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override;
		void SetMaterial(Graphics::Material* material) {
			_material = material;
		}

		virtual Math::vec3 GetBounds() override {
			return _mesh->GetExtremes();
		}
	private:
		Graphics::MeshAnimation* _anim;
		Graphics::Mesh* _mesh;
		Graphics::Material* _material;
	};
} }

#endif