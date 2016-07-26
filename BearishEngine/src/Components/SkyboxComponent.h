#ifndef _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_
#define _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Graphics/Texture/Texture.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Mesh/Mesh.h"
#include "../Types.h"

namespace Bearish { namespace Components {
	class SkyboxComponent : public Core::IActorComponent, public Core::IAllocatable<SkyboxComponent> {
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

			GLint oldCullFaceMode = GL_BACK;
//			glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
			GLint oldDepthFuncMode = GL_LESS;
//			glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);

			Core::Transform t(camera->GetTransform().GetTranslation(), Math::vec3(40, 40, 40));

			_texture->Bind(0);
			_mesh->Submit(0, t.GetTransformation(), camera);
			_mesh->Flush(_shader);

			glCullFace(oldCullFaceMode);
			glDepthFunc(oldDepthFuncMode);
		}
	private:
		Graphics::Mesh* _mesh;
		Graphics::Texture* _texture;
		Graphics::Shader* _shader;
	};
} }

#endif // _BEARISH_COMPONENTS_SKYBOXCOMPONENT_H_