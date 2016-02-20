#ifndef _BEARISH_SCRIPTING_GRAPHICS_H_
#define _BEARISH_SCRIPTING_GRAPHICS_H_

#include "ScriptingEngine.h"
#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	static void RegisterGraphics() {
		Sqrat::Class<Graphics::RenderingEngine> re(Squirrel, "RenderingEngine", true);
		re.Func("SetActorReference", &Graphics::RenderingEngine::SetActorReference);
		re.Func("SetCamera", &Graphics::RenderingEngine::SetCamera);
		Sqrat::RootTable(Squirrel).Bind("RenderingEngine", re);

		Sqrat::Class<Graphics::Camera> camera(Squirrel, "Camera", true);
		camera.Ctor<const Math::mat4&, const Core::Transform&>();
		camera.Func("GetTransform", &Graphics::Camera::GetTransform);
		camera.Func("GetProjection", &Graphics::Camera::GetProjection);
		camera.Func("SetTransform", &Graphics::Camera::SetTransform);
		camera.Func("SetProjection", &Graphics::Camera::SetProjection);
		camera.Func("GetViewMatrix", &Graphics::Camera::GetViewMatrix);
		Sqrat::RootTable(Squirrel).Bind("Camera", camera);

		Sqrat::Class<Graphics::Shader> shader(Squirrel, "Shader", true);
		shader.Ctor<const string&, const string&>();
		//shader.Func("LoadShader", &Graphics::Shader::LoadShader);
		shader.Func("AddShader", &Graphics::Shader::AddShader);
		shader.Func("Compile", &Graphics::Shader::Compile);
		shader.Func("RegisterUniform", &Graphics::Shader::RegisterUniform);
		shader.Func("SetAttribLocation", &Graphics::Shader::SetAttribLocation);
		shader.Func("Bind", &Graphics::Shader::Bind);
		shader.Func("Unbind", &Graphics::Shader::Unbind);
		shader.Func("AddAllStructs", &Graphics::Shader::AddAllStructs);
		shader.Func("AddAllAttributes", &Graphics::Shader::AddAllAttributes);
		shader.Func("AddAllUniforms", &Graphics::Shader::AddAllUniforms);
		shader.Func("AddUniform", &Graphics::Shader::AddUniform);
		shader.Func("SetUniformi", static_cast<void (Graphics::Shader::*)(const string&, i32)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformf", static_cast<void (Graphics::Shader::*)(const string&, f32)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformVec2", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec2&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformVec3", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec3&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformVec4", static_cast<void (Graphics::Shader::*)(const string&, const Math::vec4&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformMat4", static_cast<void (Graphics::Shader::*)(const string&, Math::mat4)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformLight", static_cast<void (Graphics::Shader::*)(const string&, const Graphics::Light&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformDirectionalLight", static_cast<void (Graphics::Shader::*)(const string&, const Graphics::DirectionalLight&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformAttenuation", static_cast<void (Graphics::Shader::*)(const string&, const Graphics::Attenuation&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformPointLight", static_cast<void (Graphics::Shader::*)(const string&, const Graphics::PointLight&)>(&Graphics::Shader::SetUniform));
		shader.Func("SetUniformSpotLight", static_cast<void (Graphics::Shader::*)(const string&, const Graphics::SpotLight&)>(&Graphics::Shader::SetUniform));
		Sqrat::RootTable(Squirrel).Bind("Shader", shader);
	}
} }

#endif // _BEARISH_SCRIPTING_GRAPHICS_H_