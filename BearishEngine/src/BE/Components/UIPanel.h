#ifndef _BEARISH_COMPONENTS_UIPANEL_H_
#define _BEARISH_COMPONENTS_UIPANEL_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Graphics/Texture/Texture.h>
#include <BE/Graphics/Mesh/Mesh.h>
#include <BE/Core/Actor.h>
#include <BE/Types.h>

namespace Bearish { namespace Components {
	class BEARISH_API UIPanel : public Core::IActorComponent, public Core::IAllocatable<UIPanel> {
	public:
		UIPanel(Graphics::Texture* texture) : _texture(texture), _mesh(Graphics::Mesh::CreateQuad()) {}
		~UIPanel() {
			delete _mesh;
		}

		virtual void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override {
			Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
			Core::Transform model = _actor->GetTransform();
			model.GetTranslation() += offset - Math::vec3(0, 0, 1);
			_mesh->Submit(0, model.GetTransformation(), camera);

			_texture->Bind(0);
			_mesh->Flush(shader);
			_texture->Unbind(0);
		}
	private:
		Graphics::Mesh* _mesh;
		Graphics::Texture* _texture;
	};
} }

#endif // _BEARISH_COMPONENTS_UIPANEL_H_