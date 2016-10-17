#ifndef _BEARISH_COMPONENTS_UIDRAGGABLE_H_
#define _BEARISH_COMPONENTS_UIDRAGGABLE_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Core/Actor.h>
#include <BE/Core/Mouse.h>
#include <BE/Types.h>

namespace Bearish {	namespace Components {
	enum class UIDragPosition : u8 {
		Top,
		Bottom,
		Left,
		Right,
		Centre,
	};

	class BEARISH_API UIDraggable : public Core::IActorComponent, public Core::IAllocatable<UIDraggable> {
	public:
		UIDraggable(UIDragPosition pos = UIDragPosition::Top, f32 margin = 50) : _pos(pos), _margin(margin) {}
		~UIDraggable() {}

		void FixedUpdate() override {
			Math::vec3 offset = _actor->GetParent() ? _actor->GetParent()->GetTranslation() : Math::vec3(0);
			Core::Transform model = _actor->GetTransform();
			model.GetTranslation() += Math::vec3(offset.x, offset.y, 0);

			Math::vec2 pos = model.GetTranslation().xy;
			Math::vec2 size = model.GetScale().xy;

			switch (_pos) {
			case UIDragPosition::Top:
				_rect = Math::vec4(pos.x - size.x / 2, pos.y - size.y / 2, pos.x + size.x / 2, pos.y - size.y / 2 + _margin);
				break;

			case UIDragPosition::Bottom:
				_rect = Math::vec4(pos.x - size.x / 2, pos.y + size.y / 2 - _margin, pos.x + size.x / 2, pos.y + size.y / 2);
				break;

			case UIDragPosition::Left:
				_rect = Math::vec4(pos.x - size.x / 2, pos.y - size.y / 2, pos.x - size.x / 2 + _margin, pos.y + size.y / 2);
				break;

			case UIDragPosition::Right:
				_rect = Math::vec4(pos.x + size.x / 2 - _margin, pos.y - size.y / 2, pos.x + size.x / 2 - _margin, pos.y - size.y / 2);
				break;

			case UIDragPosition::Centre:
				_rect = Math::vec4(pos.x - size.x / 2 + _margin, pos.y - size.y / 2 + _margin, pos.x + size.x / 2 - _margin, pos.y + size.y / 2 - _margin);
				break;
			}

			if (Core::Mouse::IsButtonPressed(Core::MouseButton::Left)) {
				_isDragging = PointInVec4(_rect, Core::Mouse::GetMousePosition());
				_dragPos = pos - Core::Mouse::GetMousePosition();
			}

			if (Core::Mouse::IsButtonDown(Core::MouseButton::Left) && _isDragging) {
				_actor->GetTransform().GetTranslation() = Math::vec3((Core::Mouse::GetMousePosition() + _dragPos).x,
																	(Core::Mouse::GetMousePosition() + _dragPos).y) - offset;
			}
		}
	private:
		bool PointInVec4(Math::vec4 rect, Math::vec2 point) {
			return point.x >= rect.x &&
				point.x <= rect.z &&
				point.y <= rect.w &&
				point.y >= rect.y;
		}

		f32 _margin;
		UIDragPosition _pos;
		Math::vec4 _rect;
		Math::vec2 _dragPos;
		bool _isDragging;
	};
} }
#endif // _BEARISH_COMPONENTS_UIDRAGGABLE_H_
