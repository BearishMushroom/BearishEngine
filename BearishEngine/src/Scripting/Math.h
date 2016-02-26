#ifndef _BEARISH_SCRIPTING_MATH_H_
#define _BEARISH_SCRIPTING_MATH_H_

#include "ScriptingEngine.h"
#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"

namespace Bearish { namespace Scripting {
	static void RegisterMath() {
		using namespace Math;
		using namespace luabind;

		module(L)[
			class_<vec2>("vec2")
				.def(constructor<f32, f32>())

				.def_readwrite("x", &vec2::x)
				.def_readwrite("r", &vec2::r)
				.def_readwrite("s", &vec2::s)

				.def_readwrite("y", &vec2::y)
				.def_readwrite("g", &vec2::g)
				.def_readwrite("t", &vec2::t)

				.def("__add", &vec2::Add)
				.def("__sub", &vec2::Subtract)
				.def("__mul", &vec2::Multiply)
				.def("__div", &vec2::Divide)
				.def("__tostring", &vec2::ToString)

				.def("Cross", &vec2::Cross)
				.def("Dot", &vec2::Dot)
				.def("Length", &vec2::Length)
				.def("Lerp", &vec2::Lerp)
				.def("Max", &vec2::Max)
				.def("Min", &vec2::Min)
				.def("Normalize", &vec2::Normalize)
				.def("Rotate", &vec2::Rotate),

				class_<vec3>("vec3")
				.def(constructor<f32>())
				.def(constructor<f32, f32, f32>())

				.def_readwrite("x", &vec3::x)
				.def_readwrite("r", &vec3::r)
				.def_readwrite("s", &vec3::s)

				.def_readwrite("y", &vec3::y)
				.def_readwrite("g", &vec3::g)
				.def_readwrite("t", &vec3::t)

				.def_readwrite("z", &vec3::z)
				.def_readwrite("b", &vec3::b)

				.def_readwrite("xy", &vec3::xy)
				.def_readwrite("yz", &vec3::yz)

				.def("__add", &vec3::Add)
				.def("__sub", &vec3::Subtract)
				.def("__mul", &vec3::Multiply)
				.def("__div", &vec3::Divide)
				.def("__tostring", &vec3::ToString)

				.def("Cross", &vec3::Cross)
				.def("Dot", &vec3::Dot)
				.def("Length", &vec3::Length)
				.def("LengthSquared", &vec3::LengthSq)
				.def("Lerp", &vec3::Lerp)
				.def("Max", &vec3::Max)
				.def("Min", &vec3::Min)
				.def("Normalize", &vec3::Normalize)
				.def("Rotate", (vec3(vec3::*)(f32, const vec3&) const)&vec3::Rotate),

				class_<vec4>("vec4")
				.def(constructor<f32, f32, f32, f32>())

				.def_readwrite("x", &vec4::x)
				.def_readwrite("r", &vec4::r)
				.def_readwrite("s", &vec4::s)

				.def_readwrite("y", &vec4::y)
				.def_readwrite("g", &vec4::g)
				.def_readwrite("t", &vec4::t)

				.def_readwrite("z", &vec4::z)
				.def_readwrite("b", &vec4::b)

				.def_readwrite("w", &vec4::w)
				.def_readwrite("a", &vec4::a)

				.def_readwrite("xy", &vec4::xy)
				.def_readwrite("yz", &vec4::yz)
				.def_readwrite("zw", &vec4::zw)

				.def_readwrite("xyz", &vec4::xyz)
				.def_readwrite("yzw", &vec4::yzw)

				.def("__add", &vec4::Add)
				.def("__sub", &vec4::Subtract)
				.def("__mul", &vec4::Multiply)
				.def("__div", &vec4::Divide)
				.def("__tostring", &vec4::ToString)

				.def("Dot", &vec4::Dot)
				.def("Length", &vec4::Length)
				.def("Lerp", &vec4::Lerp)
				.def("Max", &vec4::Max)
				.def("Min", &vec4::Min)
				.def("Normalize", &vec4::Normalize),

				class_<quat>("quat")
				.def(constructor<mat4>())
				.def(constructor<f32, f32, f32, f32>())

				.def_readwrite("x", &quat::x)
				.def_readwrite("y", &quat::y)
				.def_readwrite("z", &quat::z)
				.def_readwrite("w", &quat::w)

				.def("__add", &quat::operator+)
				.def("__sub", &quat::operator-)
				.def("__mul", (quat(quat::*)(const quat&) const)&quat::operator*)
				.def("__tostring", &quat::ToString)

				.def("Dot", &quat::Dot)
				.def("Length", &quat::Length)
				.def("Conjugate", &quat::Conjugate)
				.def("NLerp", &quat::NLerp)
				.def("SLerp", &quat::SLerp)
				.def("GetLookAtDirection", &quat::GetLookAtDirection)
				.def("Normalize", &quat::Normalize)

				.def("Forward", &quat::Forward)
				.def("Back", &quat::Back)
				.def("Up", &quat::Up)
				.def("Down", &quat::Down)
				.def("Right", &quat::Right)
				.def("Left", &quat::Left),

			class_<mat4>("mat4")
				.def("CreateIdentity", &mat4::CreateIdentity)
				.def("CreateTranslation", &mat4::CreateTranslation)
				.def("CreateScale", &mat4::CreateScale)
				.def("CreateRotationEuler", (mat4 (mat4::*)(f32, f32, f32))&mat4::CreateRotation)
				.def("CreateRotationFU", (mat4 (mat4::*)(const vec3&, const vec3&))&mat4::CreateRotation)
				.def("CreateProjectionFUR", (mat4(mat4::*)(const vec3&, const vec3&, const vec3&))&mat4::CreateRotation)
				.def("CreateOrthographic", &mat4::CreateOrthographic)
				.def("CreateProjection", &mat4::CreateProjection)
				.def("Transform", &mat4::Transform)
				.def("Transpose", &mat4::Transpose)
				.def("Get", &mat4::Get)
				.def("Set", &mat4::Set)
				.def("__mul", &mat4::operator*)
				.def("__tostring", &mat4::ToString)
		];
	}
} }

#endif // _BEARISH_SCRIPTING_MATH_H_