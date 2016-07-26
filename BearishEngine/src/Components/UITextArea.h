#ifndef _BEARISH_COMPONENT_UITEXTAREA_H_
#define _BEARISH_COMPONENT_UITEXTAREA_H_

#include "../Types.h"
#include "../Core/IActorComponent.h"
#include "../Core/Actor.h"
#include "../Core/IAllocatable.h"
#include "../Core/Callback.h"
#include "../Core/Mouse.h"
#include "../Utils.h"

namespace Bearish { namespace Components {
	class UITextArea : public Core::IActorComponent, public Core::IAllocatable<UITextArea> {
	public:
		UITextArea(Math::vec2 bounds, Graphics::Font* font, f32 scale) : _bounds(bounds), _font(font), _scale(scale) {
			_text = "";
			_focus = false;
		}

		~UITextArea() {
			if (_mesh) delete _mesh;
			if (_focus) Core::Keyboard::Deafen([&](Core::Key k, Core::ButtonState b) -> bool { return AddChar(k, b); });
		}

		virtual void FixedUpdate() override {
			if (Core::Mouse::IsButtonPressed(Core::MouseButton::Left)) {
				Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
				Core::Transform model = _actor->GetTransform();
				model.GetTranslation() += offset;

				Math::vec2 position = model.GetTranslation().xy;

				Math::vec2 min = position - _bounds / 2;
				Math::vec2 max = position + _bounds / 2;

				Math::vec2 cursor = Core::Mouse::GetMousePosition();

				if (cursor.x < max.x && cursor.x > min.x &&
					cursor.y < max.y && cursor.y > min.y) {
					if (_focus == false) {
						_focus = true;
						Core::Keyboard::Listen([&](Core::Key k, Core::ButtonState b) -> bool { return AddChar(k, b); });
					}
				}
				else {
					_focus = false;
					Core::Keyboard::Deafen([&](Core::Key k, Core::ButtonState b) -> bool { return AddChar(k, b); });
				}
			}
		}

		virtual void Draw2D(Graphics::RenderingEngine* engine, Graphics::Shader* shader, Graphics::Camera* camera) override {
			if (_mesh) {
				Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
				Core::Transform model = _actor->GetTransform();
				model.GetTranslation() += offset;
				model.GetTranslation() -= vec3((_bounds / 2).x, -(_bounds / 2).y, 0);

				_mesh->Submit(0, model.GetTransformation(), camera);

				_font->GetTexture()->Bind(0);
				_mesh->Flush(shader);
				_font->GetTexture()->Unbind(0);
			}
		}

		void SetText(string text) {
			_text = text;
			NewMesh();
		}

		void Listen(Core::Action<string>::Function fn) {
			_send += fn;
		}
		
		void Deafen(Core::Action<string>::Function fn) {
			_send -= fn;
		}
	private:
		void NewMesh() {
			if (_mesh) {
				delete _mesh;
				_mesh = 0;
			}

			_mesh = _font->GenerateMesh(_text, _scale);
		}

		bool AddChar(Core::Key key, Core::ButtonState state) {
			if (state == Core::ButtonState::Pressed) {
				if (key == Core::Key::Enter) {
					_send(_text);
					return true;
				}

				if (key == Core::Key::Backspace) {
					_text = _text.substr(0, _text.size() - 1);
					NewMesh();
					return true;
				}

				_text += Util::KeyToString(key);
				NewMesh();
				return true;
			}

			return false;
		}

		bool _focus;
		Math::vec2 _bounds;
		string _text;
		f32 _scale;
		Graphics::Mesh* _mesh;
		Graphics::Font* _font;
		Core::Action<string> _send;
	}; 
} }

#endif // _BEARISH_COMPONENT_UITEXTAREA_H_
