#ifndef _BEARISH_SCRIPTING_GRAPHICS_H_
#define _BEARISH_SCRIPTING_GRAPHICS_H_

#include "ScriptingEngine.h"
#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	static void RegisterGraphics() {
		using namespace Core;
		using namespace Math;
		using namespace Graphics;
		using namespace luabind;

		module(L)[
			class_<RenderingEngine>("RenderingEngine")
				.def("SetCamera", &RenderingEngine::SetCamera),

			class_<Camera>("Camera")
				.def(constructor<const mat4&, const Transform&>())
				.def("GetTransform", &Camera::GetTransform)
				.def("GetProjection", &Camera::GetProjection)
				.def("SetTransform", &Camera::SetTransform)
				.def("SetProjection", &Camera::SetProjection)
				.def("GetViewMatrix", &Camera::GetViewMatrix),

			class_<Shader>("Shader")
				.def(constructor<const string&, const string&>())
				.def("Bind", &Shader::Bind)
				.def("Unbind", &Shader::Unbind)
				.def("SetUniformi", static_cast<void (Graphics::Shader::*)(const string&, i32)>(&Graphics::Shader::SetUniform))
				.def("SetUniformf", static_cast<void (Graphics::Shader::*)(const string&, f32)>(&Graphics::Shader::SetUniform))
				.def("SetUniformVec2", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec2&)>(&Graphics::Shader::SetUniform))
				.def("SetUniformVec3", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec3&)>(&Graphics::Shader::SetUniform))
				.def("SetUniformVec4", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec4&)>(&Graphics::Shader::SetUniform))
				.def("SetUniformMat4", static_cast<void (Graphics::Shader::*)(const string&, Math::mat4)>(&Graphics::Shader::SetUniform))
		];
	}
} }

#endif // _BEARISH_SCRIPTING_GRAPHICS_H_