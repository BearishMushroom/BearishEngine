#ifndef _BEARISH_COMPONENTS_FREELOOKCOMPONENT_H_
#define _BEARISH_COMPONENTS_FREELOOKCOMPONENT_H_

#include "../Core/IActorComponent.h"
#include "../Core/IAllocatable.h"
#include "../Core/Mouse.h"
#include "../Core/Actor.h"
#include "../Types.h"

namespace Bearish { namespace Components {

	class FreeLookComponent : public Core::IActorComponent, public Core::IAllocatable<FreeLookComponent> {
	public:
		FreeLookComponent(f32 sensitivity = 0.2f) : _sensitivity(sensitivity) { _id = "FreeLook"; }
		~FreeLookComponent() {}

		virtual void Init() override {
			Core::Mouse::LockToCentre();
		}

		virtual void FixedUpdate() override {
			Math::vec2 d = Core::Mouse::GetMouseDelta();
			_actor->GetTransform().Rotate(Math::vec3(0, 1, 0), -d.x * (_sensitivity / SensitivityDivisor));
			_actor->GetTransform().Rotate(_actor->GetTransform().GetRotation().Right(), -d.y * (_sensitivity / SensitivityDivisor));
		}
	private:
		const f32 SensitivityDivisor = 100.f;
		f32 _sensitivity;
	};
} }

#endif