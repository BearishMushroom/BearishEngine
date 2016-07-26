#ifndef _BEARISH_COMPONENTS_UIBUTTON_H_
#define _BEARISH_COMPONENTS_UIBUTTON_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Core/Actor.h"
#include "../Core/Mouse.h"
#include "../Types.h"
namespace Bearish { namespace Components {
		class UIButton : public Core::IActorComponent, public Core::IAllocatable<UIButton> {
		public:
			UIButton(Graphics::Texture* texture, std::function<void(UIButton*)> action) : _mesh(Graphics::Mesh::CreateQuad()), _texture(texture), _action(action) {}
			~UIButton() {
				delete _mesh;
				delete _texture;
			}

			void FixedUpdate() override {
				Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
				Core::Transform model = _actor->GetTransform();
				model.GetTranslation() += offset - Math::vec3(0, 0, 1);

				Math::vec2 pos = model.GetTranslation().xy;
				Math::vec2 size = model.GetScale().xy;

				Math::vec4 rect = Math::vec4(pos.x - size.x / 2, pos.y - size.y / 2, pos.x + size.x / 2, pos.y + size.y / 2);

				if (Core::Mouse::IsButtonReleased(Core::MouseButton::Left) && PointInVec4(rect, Core::Mouse::GetMousePosition())) {
					_action(this);
				}
			}

			void Draw2D(Graphics::RenderingEngine* renderer, Graphics::Shader* shader, Graphics::Camera* camera) override {
				Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
				Core::Transform model = _actor->GetTransform();
				model.GetTranslation() += offset - Math::vec3(0, 0, 1);
				_mesh->Submit(0, model.GetTransformation(), camera);

				_texture->Bind(0);
				_mesh->Flush(shader);
				_texture->Unbind(0);
			}
		private:
			bool PointInVec4(Math::vec4 rect, Math::vec2 point) {
				return point.x >= rect.x &&
					point.x <= rect.z &&
					point.y <= rect.w &&
					point.y >= rect.y;
			}

			Graphics::Mesh* _mesh;
			Graphics::Texture* _texture;
			std::function<void(UIButton*)> _action;
		};
} }
#endif // _BEARISH_COMPONENTS_UIBUTTON_H_
