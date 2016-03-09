#ifndef _BEARISH_SCRIPTING_CORE_H_
#define _BEARISH_SCRIPTING_CORE_H_

#include "../Core/IActorComponent.h"
#include "../Core/Mouse.h"
#include "../Core/Actor.h"

#include "ScriptingEngine.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	struct key_wrappper {};
	struct button_wrapper {};

	static void RegisterCore() {
		using namespace luabind;
		using namespace Core;

		module(L)[
			class_<Transform>("Transform")
				.def(constructor<Transform>())
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
				.def("GetTranslation", &Actor::GetTranslation)
				.def("GetParent", &Actor::GetParent)
				.def("HasParent", &Actor::HasParent)
				.def("SetParent", &Actor::SetParent)
				.def("Kill", &Actor::Kill),

			class_<Mouse>("Mouse")
				.scope [
					def("GetPosition", &Mouse::GetMousePosition),
					def("GetDelta", &Mouse::GetMouseDelta),
					def("LockToCentre", &Mouse::LockToCentre),
					def("FreeFromCentre", &Mouse::FreeFromCentre),
					def("IsButtonPressed", &Mouse::IsButtonPressed),
					def("IsButtonReleased", &Mouse::IsButtonReleased),
					def("IsButtonDown", &Mouse::IsButtonDown),
					def("IsButtonUp", &Mouse::IsButtonUp)
				],

			class_<Keyboard>("Keyboard")
				.scope [
					def("IsKeyDown", &Keyboard::IsKeyDown),
					def("IsKeyUp", &Keyboard::IsKeyUp),
					def("IsKeyPressed", &Keyboard::IsKeyPressed),
					def("IsKeyReleased", &Keyboard::IsKeyReleased)
				],

			class_<key_wrappper>("Key")
				.enum_("constants") [
					value("A", (i32)Key::A), value("B", (i32)Key::B), value("C", (i32)Key::C),
					value("D", (i32)Key::D), value("E", (i32)Key::E), value("F", (i32)Key::F),
					value("G", (i32)Key::G), value("H", (i32)Key::H), value("I", (i32)Key::I),
					value("J", (i32)Key::J), value("K", (i32)Key::K), value("L", (i32)Key::L),
					value("M", (i32)Key::M), value("N", (i32)Key::N), value("O", (i32)Key::O),
					value("P", (i32)Key::P), value("Q", (i32)Key::Q), value("R", (i32)Key::R),
					value("S", (i32)Key::S), value("T", (i32)Key::T), value("U", (i32)Key::U),
					value("V", (i32)Key::V), value("W", (i32)Key::W), value("X", (i32)Key::X),
					value("Y", (i32)Key::Y), value("Z", (i32)Key::Z),

					value("One", (i32)Key::One), value("Two", (i32)Key::Two), value("Three", (i32)Key::Three),
					value("Four", (i32)Key::Four), value("Five", (i32)Key::Five), value("Six", (i32)Key::Six),
					value("Seven", (i32)Key::Seven), value("Eight", (i32)Key::Eight), value("Nine", (i32)Key::Nine),
					value("Zero", (i32)Key::Zero),

					value("Left", (i32)Key::Left),
					value("Right", (i32)Key::Right),
					value("Up", (i32)Key::Up),
					value("Down", (i32)Key::Down),

					value("LeftAlt", (i32)Key::LeftAlt)
				],

			class_<button_wrapper>("MouseButton")
				.enum_("Constants") [
					value("Left", (i32)MouseButton::Left),
					value("Right", (i32)MouseButton::Right),
					value("Middle", (i32)MouseButton::Middle)
				]
		];
	}
} }

#endif // _BEARISH_SCRIPTING_CORE_H_