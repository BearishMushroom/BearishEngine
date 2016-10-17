#ifndef _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_
#define _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Graphics/Texture/Texture.h>
#include <BE/Graphics/Renderer.h>
#include <BE/Graphics/Shader.h>
#include <BE/Graphics/Mesh/Mesh.h>
#include <BE/Types.h>

namespace Bearish { namespace Components {
	class BEARISH_API SkyboxComponent : public Core::IActorComponent, public Core::IAllocatable<SkyboxComponent> {
	public:
		SkyboxComponent(Graphics::Texture* texture) {
			_texture = texture;
			_shader = new Graphics::Shader("res/skybox.vert", "res/skybox.frag");
			_mesh = new Graphics::Mesh(Core::Model("res/models/sphere.bem").ToMesh());
			_id = "Skybox";
		}

		~SkyboxComponent() {
			delete _texture;
			delete _shader;
			delete _mesh;
		}

		virtual void PostDraw(Graphics::RenderingEngine* renderer, Graphics::Camera* camera) override {
			_shader->Bind();
			_shader->SetUniform("diffuse", 0);

			Graphics::Renderer::SetCullMode(Graphics::CullMode::Front);
			Graphics::Renderer::SetDepthMode(Graphics::DepthMode::LessOrEqual);

			Core::Transform t(camera->GetTransform().GetTranslation(), Math::vec3(40, 40, 40));

			_texture->Bind(0);
			_mesh->Submit(0, t.GetTransformation(), camera);
			_mesh->Flush(_shader);

			Graphics::Renderer::SetCullMode(Graphics::CullMode::Back);
			Graphics::Renderer::SetDepthMode(Graphics::DepthMode::Less);
		}
	private:
		Graphics::Mesh* _mesh;
		Graphics::Texture* _texture;
		Graphics::Shader* _shader;
	};
} }

#endif // _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_