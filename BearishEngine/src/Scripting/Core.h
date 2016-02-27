#ifndef _BEARISH_SCRIPTING_CORE_H_
#define _BEARISH_SCRIPTING_CORE_H_

#include "../Core/IActorComponent.h"
#include "ScriptingEngine.h"
#include "../Core/Actor.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	static void RegisterCore() {
		using namespace luabind;
		using namespace Core;

		module(L)[
			class_<Transform>("Transform")
				.def(constructor<vec3, vec3, quat>())
				.def("GetTranslation", &Transform::GetTranslation)
				.def("GetScale", &Transform::GetScale)
				.def("GetRotation", &Transform::GetRotation)
				.def("SetTranslation", &Transform::SetTranslation)
				.def("SetScale", &Transform::SetScale)
				.def("SetRotation", &Transform::SetRotation)
				.def("GetTransformation", &Transform::GetTransformation)
				.def("GetCameraTransformation", &Transform::GetCameraTransformation)
				.def("Rotate", static_cast<void(Transform::*)(const vec3&, const f32)>(&Transform::Rotate)),

			class_<Actor>("Actor")
				.def(constructor<Transform>())
				.def("GetComponentsByID", &Actor::GetComponentsByID)
				.def("GetTransform", &Actor::GetTransform)
				.def("GetTransformation", &Actor::GetTransformation)
				.def("GetParent", &Actor::GetParent)
				.def("SetParent", &Actor::SetParent)
		];
	}
} }

#endif // _BEARISH_SCRIPTING_CORE_H_