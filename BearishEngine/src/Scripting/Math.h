#ifndef _BEARISH_SCRIPTING_MATH_H_
#define _BEARISH_SCRIPTING_MATH_H_

#include "ScriptingEngine.h"
#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"

namespace Bearish { namespace Scripting {
	static void RegisterMath() {
		// Vec2
		Sqrat::Class<Math::vec2> v2(Squirrel, "vec2", true);
		v2.Ctor<f32, f32>();
		v2.Var("x", &Math::vec2::x);
		v2.Var("y", &Math::vec2::y);
		v2.Func("_add", &Math::vec2::Add);
		v2.Func("_sub", &Math::vec2::Subtract);
		v2.Func("_mul", &Math::vec2::Multiply);
		v2.Func("_div", &Math::vec2::Divide);
		v2.Func("_tostring", &Math::vec2::ToString);
		v2.Func("Equals", &Math::vec2::operator==);
		v2.Func("Min", &Math::vec2::Min);
		v2.Func("Max", &Math::vec2::Max);
		v2.Func("Dot", &Math::vec2::Dot);
		v2.Func("Cross", &Math::vec2::Cross);
		v2.Func("Length", &Math::vec2::Length);
		v2.Func("Normalize", &Math::vec2::Normalize);
		v2.Func("Rotate", &Math::vec2::Rotate);
		v2.Func("Lerp", &Math::vec2::Lerp);

		Sqrat::RootTable(Squirrel).Bind("vec2", v2);

		// Vec3
		Sqrat::Class<Math::vec3> v3(Squirrel, "vec3", true);
		v3.Ctor<f32, f32, f32>();
		v3.Var("x", &Math::vec3::x);
		v3.Var("y", &Math::vec3::y);
		v3.Var("z", &Math::vec3::z);
		v3.Var("xy", &Math::vec3::xy);
		v3.Var("yz", &Math::vec3::yz);
		v3.Func("_add", &Math::vec3::Add);
		v3.Func("_sub", &Math::vec3::Subtract);
		v3.Func("_mul", &Math::vec3::Multiply);
		v3.Func("_div", &Math::vec3::Divide);
		v3.Func("_tostring", &Math::vec3::ToString);
		v3.Func("Equals", &Math::vec3::operator==);
		v3.Func("Min", &Math::vec3::Min);
		v3.Func("Max", &Math::vec3::Max);
		v3.Func("Dot", &Math::vec3::Dot);
		v3.Func("Cross", &Math::vec3::Cross);
		v3.Func("Length", &Math::vec3::Length);
		v3.Func("Normalize", &Math::vec3::Normalize);
		v3.Func("Rotate", static_cast<Math::vec3(Math::vec3::*)(f32, const Math::vec3&) const>(&Math::vec3::Rotate));
		v3.Func("Lerp", &Math::vec3::Lerp);

		Sqrat::RootTable(Squirrel).Bind("vec3", v3);

		// Vec4
		Sqrat::Class<Math::vec4> v4(Squirrel, "vec4", true);
		v4.Ctor<f32, f32, f32, f32>();
		v4.Var("x", &Math::vec4::x);
		v4.Var("y", &Math::vec4::y);
		v4.Var("z", &Math::vec4::z);
		v4.Var("w", &Math::vec4::w);
		v4.Var("xy", &Math::vec4::xy);
		v4.Var("yz", &Math::vec4::yz);
		v4.Var("zw", &Math::vec4::zw);
		v4.Var("xyz", &Math::vec4::xyz);
		v4.Var("yzw", &Math::vec4::yzw);
		v4.Func("_add", &Math::vec4::Add);
		v4.Func("_sub", &Math::vec4::Subtract);
		v4.Func("_mul", &Math::vec4::Multiply);
		v4.Func("_div", &Math::vec4::Divide);
		v4.Func("_tostring", &Math::vec4::ToString);
		v4.Func("Equals", &Math::vec4::operator==);
		v4.Func("Min", &Math::vec4::Min);
		v4.Func("Max", &Math::vec4::Max);
		v4.Func("Dot", &Math::vec4::Dot);
		v4.Func("Length", &Math::vec4::Length);
		v4.Func("Normalize", &Math::vec4::Normalize);
		v4.Func("Lerp", &Math::vec4::Lerp);

		Sqrat::RootTable(Squirrel).Bind("vec4", v4);

		// quat
		Sqrat::Class<Math::quat> quat(Squirrel, "quat", true);
		quat.Ctor<f32, f32, f32, f32>();
		quat.Var("x", &Math::quat::x);
		quat.Var("y", &Math::quat::y);
		quat.Var("z", &Math::quat::z);
		quat.Var("w", &Math::quat::w);
		quat.Func("_add", &Math::quat::operator+);
		quat.Func("_sub", &Math::quat::operator-);
		quat.Func("_mul", static_cast<Math::quat(Math::quat::*)(const Math::quat&) const>(&Math::quat::operator*));
		quat.Func("_tostring", &Math::quat::ToString);
		quat.Func("Equals", &Math::quat::operator==);
		quat.Func("Dot", &Math::quat::Dot);
		quat.Func("Length", &Math::quat::Length);
		quat.Func("Normalize", &Math::quat::Normalize);

		Sqrat::RootTable(Squirrel).Bind("quat", quat);

		// mat4
		Sqrat::Class<Math::mat4> mat4(Squirrel, "mat4", true);
		mat4.Func("_mul", &Math::mat4::operator*);
		mat4.Func("_tostring", &Math::mat4::ToString);
		mat4.Func("CreateIdentity", &Math::mat4::CreateIdentity);
		mat4.Func("CreateTranslation", &Math::mat4::CreateTranslation);
		mat4.Func("CreateRotation", static_cast<Math::mat4(Math::mat4::*)(const Math::vec3&, const Math::vec3&, const Math::vec3&)>(&Math::mat4::CreateRotation));
		mat4.Func("CreateProjection", &Math::mat4::CreateProjection);
		mat4.Func("CreateScale", &Math::mat4::CreateScale);
		mat4.Func("CreateOrthographic", &Math::mat4::CreateOrthographic);
		mat4.Func("Transform", &Math::mat4::Transform);
		mat4.Func("Get", &Math::mat4::Get);

		Sqrat::RootTable(Squirrel).Bind("mat4", mat4);
	}
} }

#endif // _BEARISH_SCRIPTING_MATH_H_