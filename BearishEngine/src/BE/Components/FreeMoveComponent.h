#ifndef _BEARISH_COMPONENTS_FREEMOVECOMPONENT_H_
#define _BEARISH_COMPONENTS_FREEMOVECOMPONENT_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Core/Keyboard.h>
#include <BE/Core/Actor.h>
#include <BE/Types.h>

namespace Bearish { namespace Components {
	class BEARISH_API FreeMoveComponent : public Core::IActorComponent, public Core::IAllocatable<FreeMoveComponent> {
	public:
		FreeMoveComponent(f32 speed, Core::Key forward = Core::Key::W, Core::Key backward = Core::Key::S,
			Core::Key left = Core::Key::A, Core::Key right = Core::Key::D,
			Core::Key up = Core::Key::E, Core::Key down = Core::Key::Q) : _speed(speed), _forward(forward), _backward(backward),
			_left(left), _right(right), _up(up), _down(down) { _id = "FreeMove"; }

		~FreeMoveComponent() {}

		virtual void FixedUpdate() override {
			if (Core::Keyboard::IsKeyDown(_forward))  _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Forward() * _speed);
			if (Core::Keyboard::IsKeyDown(_backward)) _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Back() * _speed);
			
			if (Core::Keyboard::IsKeyDown(_left))  _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Left() * _speed);
			if (Core::Keyboard::IsKeyDown(_right)) _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Right() * _speed);

			if (Core::Keyboard::IsKeyDown(_up))   _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Up() * _speed);
			if (Core::Keyboard::IsKeyDown(_down)) _actor->GetTransform().GetTranslation() += Math::vec3(_actor->GetTransform().GetRotation().Down() * _speed);
		}
	private:
		Core::Key _forward, _backward, _left, _right, _up, _down;
		f32 _speed;
	};
} }
#endif //_BEARISH_COMPONENTS_FREEMOVECOMPONENT_H_
