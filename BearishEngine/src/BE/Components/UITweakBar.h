#ifndef _BEARISH_COMPONENTS_UITWEAKBAR_H_
#define _BEARISH_COMPONENTS_UITWEAKBAR_H_

#include <BE\Types.h>
#include <BE\Core\IActorComponent.h>
#include <BE\Core\Actor.h>
#include <BE\Core\Mouse.h>
#include <BE\Math\vec4.h>
#include <BE\Utils.h>

namespace Bearish { namespace Components {
	class UITweakBar : public Core::IActorComponent {
	public:
		UITweakBar(f32 min, f32 max, Graphics::Font* font, Math::vec4 colBack = Math::vec4(0.3, 0.3, 0.3, 1), Math::vec4 colBar = Math::vec4(0.5, 0.5, 0.5, 1)) 
			: _min(min), _max(max), _pct(0), _quad(Graphics::Mesh::CreateQuad()), _font(font) {
			_backT = new Graphics::Texture(colBack);
			_barT = new Graphics::Texture(colBar);
		}

		~UITweakBar() {
			delete _quad;
			delete _backT;
			delete _barT;
		}

		virtual void Init() override {
			UpdateRect();
		}

		virtual void FixedUpdate() override {
			Core::Transform& model = _actor->GetTransform();
			Math::vec2 pos = model.GetTranslation().xy;
			Math::vec2 scale = model.GetScale().xy;

			_minX = pos.x - scale.x / 2 + _side + 4;
			_maxX = pos.x + scale.x / 2 - _side - 4;
			UpdateRect(); 

			if (Core::Mouse::IsButtonPressed(Core::MouseButton::Left) && !_dragging) {
				if (PointInVec4(_rect, Core::Mouse::GetMousePosition())) {
					_dragging = true;
					_dragPos = Math::vec2(_rect.x + _side, _rect.y + _side) - Core::Mouse::GetMousePosition();
				}
			}

			if (Core::Mouse::IsButtonReleased(Core::MouseButton::Left)) {
				_dragging = false;
			}

			if (Core::Mouse::IsButtonDown(Core::MouseButton::Left) && _dragging) {
				Math::vec2 newpos = Core::Mouse::GetMousePosition() + _dragPos;
				f32 xpos = Util::Clamp(newpos.x, _minX, _maxX);
				_pct = (xpos - _minX) / (_maxX - _minX);
			}
		}

		virtual void Draw2D(Graphics::RenderingEngine* engine, Graphics::Shader* shader, Graphics::Camera* camera) override {
			Core::Transform model = _actor->GetTransform();

			_quad->Submit(0, model.GetTransformation(), camera);
			_backT->Bind(0);
			_quad->Flush(shader);

			Core::Transform bt(vec3(_minX + (_maxX - _minX) * _pct, model.GetTranslation().y, 10), vec3(_side*2, _side*2, 1));
			
			_quad->Submit(0, bt.GetTransformation(), camera);
			_barT->Bind(0);
			_quad->Flush(shader);

			f32 value = (_min + (_max - _min) * _pct);
			string whole = std::to_string((i32)value);
			string num = std::to_string(value);
			num = num.substr(0, whole.length() + (num.length() - whole.length() >= 3 ? 3 : num.length() - whole.length()));

			Graphics::Mesh* text = _font->GenerateMesh(num, _side);
			Core::Transform tt = bt;
			tt.GetScale() = vec3(1);
			tt.GetTranslation().z = 12;
			tt.GetTranslation().x -= num.length() * _side / 4;
			text->Submit(0, tt.GetTransformation(), camera);
			_font->GetTexture()->Bind(0);
			text->Flush(shader);
		}

		void SetPct(f32 pct) { _pct = pct; UpdateRect(); }
		
		f32 GetValue() {
			return _min + (_max - _min) * _pct;
		}
	private:
		void UpdateRect() {
			Core::Transform model = _actor->GetTransform();
			Math::vec2 size = model.GetScale().xy;
			Math::vec2 pos = model.GetTranslation().xy;
		
			_side = (size.y - 4) / 2;
			Math::vec2 tPos = Math::vec2(_minX + (_maxX - _minX) * _pct, pos.y);
			_rect = Math::vec4(tPos.x - _side, tPos.y - _side, tPos.x + _side, tPos.y + _side);
		}

		bool PointInVec4(Math::vec4 rect, Math::vec2 point) {
			return point.x >= rect.x &&
				point.x <= rect.z &&
				point.y <= rect.w &&
				point.y >= rect.y;
		}


		Math::vec4 _rect;
		f32 _min, _max, _pct;
		f32 _minX, _maxX, _side;
		bool _dragging;
		Math::vec2 _dragPos;
		Graphics::Mesh* _quad;
		Graphics::Font* _font;
		Graphics::Texture* _backT,* _barT;
	};
} }

#endif // _BEARISH_COMPONENTS_UITWEAKBAR_H_