#ifndef _BEARISH_COMPONENTS_UIPANEL_H_
#define _BEARISH_COMPONENTS_UIPANEL_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Mesh/Mesh.h"
#include "../Core/Actor.h"
#include "../Types.h"

namespace Bearish { namespace Components {
	class UIPanel : public Core::IActorComponent, public Core::IAllocatable<UIPanel> {
	public:
		UIPanel(Graphics::Texture* texture) : _texture(texture), _mesh(Graphics::Mesh::CreateQuad()) {}
		~UIPanel() {}

		virtual void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override {
			Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
			Core::Transform model = _actor->GetTransform();
			model.GetTranslation() += offset - Math::vec3(0, 0, 1);
			_mesh->Submit(model.GetTransformation(), camera->GetViewMatrix() * model.GetTransformation());

			_texture->Bind(0);
			_mesh->Flush();
			_texture->Unbind(0);
		}
	private:
		Graphics::Mesh* _mesh;
		Graphics::Texture* _texture;
	};
} }

#endif // _BEARISH_COMPONENTS_UIPANEL_H_