#include <BE\Scripting\Core.h>
#include <BE\Scripting\Graphics.h>
#include <BE\Scripting\Math.h>
#include <BE\Scripting\ScriptingEngine.h>
#include <BE\Core\Keyboard.h>

using namespace Bearish;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Scripting;

BEARISH_API void Scripting::InitLua() {
	L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);
}

BEARISH_API void Scripting::KillLua() {
	lua_close(L);
}

BEARISH_API void Scripting::RunFile(string file) {
	struct stat st;
	stat(file.c_str(), &st);
	_loadedLuas[file] = (u32)st.st_mtime;
	if (luaL_dofile(L, file.c_str())) {
		Core::Logger::Error("Error running script: %s\nLua: %s\n", file.c_str(), lua_tostring(L, -1));
	}
}

BEARISH_API void Scripting::RunString(string cmd) {
	if (luaL_dostring(L, cmd.c_str())) {
		Core::Logger::Error("Error running string: %s\nLua: %s\n", cmd.c_str(), lua_tostring(L, -1));
	}
}

BEARISH_API void Scripting::InitMoonScript() {
	try {
		luabind::object moon = luabind::call_function<luabind::object>(Scripting::L, "require", "moonscript");
		moonToLua = luabind::rawget(moon, "to_lua");
		moonDoFile = luabind::rawget(moon, "dofile");
	}
	catch (luabind::error e) {
		printf("%s\n", e.what());
		printf("");
	}

}

BEARISH_API void Scripting::RunMoon(string moon) {
	string str = CompileMoon(moon);
	Core::Logger::Info("%s", str.c_str());
	RunString(str);
}

BEARISH_API string Scripting::CompileMoon(string moon) {
	return luabind::call_function<string>(moonToLua, moon);
}

BEARISH_API LuaObject Scripting::DoMoonFile(string filename) {
	try {
		struct stat st;
		stat(filename.c_str(), &st);
		_loadedMoons[filename] = (u32)st.st_mtime;
		auto obj = luabind::call_function<luabind::object>(moonDoFile, filename);
		return LuaObject(obj);
	}
	catch (luabind::error e) {
		Core::Logger::Error("Lua: %s\n", lua_tostring(Scripting::L, -1));
		Core::Logger::Error(e.what());
		return LuaObject();
	}
}

BEARISH_API LuaObject Scripting::CreateInstance(string func) {
	try {
		luaL_loadstring(L, ("return " + func).c_str());
		luabind::object obj(luabind::from_stack(L, -1));
		return LuaObject(obj());
	}
	catch (luabind::error e) {
		Core::Logger::Error("Lua: %s\n", lua_tostring(Scripting::L, -1));
		Core::Logger::Error(e.what());
		return LuaObject();
	}
}

BEARISH_API void Scripting::UpdateScripts() {
	for (auto&& it : _loadedLuas) {
		struct stat st;
		stat(it.first.c_str(), &st);
		if (st.st_mtime > it.second) {
			RunFile(it.first);
		}
	}

	for (auto&& it : _loadedMoons) {
		struct stat st;
		stat(it.first.c_str(), &st);
		if (st.st_mtime > it.second) {
			DoMoonFile(it.first);
		}
	}
}

BEARISH_API lua_State* Scripting::GetVM() {
	return Scripting::L;
}

BEARISH_API void Scripting::RegisterMath() {
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

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

		class_<vec2i>("vec2i")
			.def(constructor<i32, i32>())

			.def_readwrite("x", &vec2i::x)
			.def_readwrite("r", &vec2i::r)
			.def_readwrite("s", &vec2i::s)

			.def_readwrite("y", &vec2i::y)
			.def_readwrite("g", &vec2i::g)
			.def_readwrite("t", &vec2i::t)

			.def("__add", &vec2i::Add)
			.def("__sub", &vec2i::Subtract)
			.def("__mul", &vec2i::Multiply)
			.def("__div", &vec2i::Divide)
			.def("__tostring", &vec2i::ToString)

			.def("Cross", &vec2i::Cross)
			.def("Dot", &vec2i::Dot)
			.def("Length", &vec2i::Length)
			.def("Lerp", &vec2i::Lerp)
			.def("Max", &vec2i::Max)
			.def("Min", &vec2i::Min)
			.def("Normalize", &vec2i::Normalize)
			.def("Rotate", &vec2i::Rotate),

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

			//.def("__add", &vec4::Add)
			//.def("__sub", &vec4::Subtract)
			//.def("__mul", &vec4::Multiply)
			//.def("__div", &vec4::Divide)
			.def("__tostring", &vec4::ToString)

			.def("Dot", &vec4::Dot)
			.def("Length", &vec4::Length)
			.def("Lerp", &vec4::Lerp)
			.def("Max", &vec4::Max)
			.def("Min", &vec4::Min)
			.def("Normalize", &vec4::Normalize),

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

		class_<quat>("quat")
			.def(constructor<const mat4&>())
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
			.def(constructor<>())
			.def("CreateIdentity", &mat4::CreateIdentity)
			.def("CreateTranslation", &mat4::CreateTranslation)
			.def("CreateScale", &mat4::CreateScale)
			.def("CreateRotationEuler", (mat4(mat4::*)(f32, f32, f32))&mat4::CreateRotation)
			.def("CreateRotationFU", (mat4(mat4::*)(const vec3&, const vec3&))&mat4::CreateRotation)
			.def("CreateProjectionFUR", (mat4(mat4::*)(const vec3&, const vec3&, const vec3&))&mat4::CreateRotation)
			.def("CreateOrthographic", &mat4::CreateOrthographic)
			.def("CreateProjection", &mat4::CreateProjection)
			//.def("Transform", (vec4(mat4::*)(const vec4&))&mat4::Transform)
			.def("Transpose", &mat4::Transpose)
			//.def("Get", &mat4::Get)
			//.def("Set", &mat4::Set)
			.def("__mul", &mat4::operator*)
			.def("__tostring", &mat4::ToString)
	];
}

BEARISH_API void Scripting::RegisterGraphics() {
	using namespace luabind;

	module(L)[
		class_<RenderingEngine>("RenderingEngine")
			.def("SetCamera", &RenderingEngine::SetCamera),

			class_<Camera>("Camera")
			.def(constructor<const f32&, const f32&, const f32&, const f32&, const Transform&>())
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
			.def("SetUniformMat4", static_cast<void (Graphics::Shader::*)(const string&, const Math::mat4&)>(&Graphics::Shader::SetUniform)),

			class_<Mesh>("Mesh")
			.scope[
				def("CreateQuad", &Mesh::CreateQuad)
			]
			.def(constructor<std::vector<Vertex>, std::vector<u32>, bool>())
					.def(constructor<Model*>())
					.def(constructor<u32, vec3*, vec2*, vec3*, vec3*, vec4i*, vec4*, u32, u32*>())
					.def("Submit", &Mesh::Submit)
					.def("Flush", &Mesh::Flush)
					.def("Animate", &Mesh::Animate)
					.def("DecoupleSkeleton", &Mesh::DecoupleSkeleton),

					class_<Texture>("Texture")
					.def(constructor<string, TextureType, TextureFormat, TextureFilter>())
					.def(constructor<vec2i, TextureType, TextureFilter, TextureAttachment, TextureFormat, u8*>())
					.def(constructor<string, string, string, string, string, string, TextureFormat, TextureFilter>())
					.def(constructor<vec4>())
					.def("Unload", &Texture::Unload)
					.def("Bind", &Texture::Bind)
					.def("Unbind", &Texture::Unbind)
					.def("BindAsRenderTarget", &Texture::BindAsRenderTarget)
					.def("Clear", &Texture::Clear)
					.def("GetSize", &Texture::GetSize)
					//.def("GetType", &Texture::GetTextureType)
					//.def("GetFormat", &Texture::GetTextureFormat)
					//.def("GetAttachment", &Texture::GetTextureAttachment)
					.def("GetName", &Texture::GetName)
					.def("BindForWriting", &Texture::BindForWriting)
					.def("BindForReading", &Texture::BindForWriting)
					.def("SetReadBuffer", &Texture::SetReadBuffer),

					class_<texture_type_wrapper>("TextureType")
					.enum_("Constants")[
						value("Texture1D", (i32)TextureType::Texture1D),
							value("Texture2D", (i32)TextureType::Texture2D),
							value("Texture3D", (i32)TextureType::Texture3D),
							value("Cubemap", (i32)TextureType::CubeMap)
					],

					class_<texture_format_wrapper>("TextureFormat")
							.enum_("Constants")[
								value("RG", (i32)TextureFormat::RG),
									value("RGB", (i32)TextureFormat::RGB),
									value("RGBA", (i32)TextureFormat::RGBA),
									value("R32", (i32)TextureFormat::R32),
									value("RG32", (i32)TextureFormat::RG32),
									value("RGB32", (i32)TextureFormat::RGB32),
									value("RGBA32", (i32)TextureFormat::RGBA32),
									value("BGR", (i32)TextureFormat::BGR),
									value("ABGR", (i32)TextureFormat::ABGR),
									value("Depth16", (i32)TextureFormat::Depth16),
									value("Depth24", (i32)TextureFormat::Depth24),
									value("Depth32", (i32)TextureFormat::Depth32)
							],

							class_<texture_filter_wrapper>("TextureFilter")
									.enum_("Constants")[
										value("Nearest", (i32)TextureFilter::Nearest),
											value("Linear", (i32)TextureFilter::Linear)
									],

									class_<texture_attachment_wrapper>("TextureAttachment")
											.enum_("Constants")[
												value("None", (i32)TextureAttachment::None),
													value("Color0", (i32)TextureAttachment::Color0),
													value("Color1", (i32)TextureAttachment::Color1),
													value("Color2", (i32)TextureAttachment::Color2),
													value("Color3", (i32)TextureAttachment::Color3),
													value("Color4", (i32)TextureAttachment::Color4),
													value("Depth", (i32)TextureAttachment::Depth),
													value("Stencil", (i32)TextureAttachment::Stencil)
											],

											class_<Font>("Font")
													.def(constructor<string, i32>())
													.def("GenerateMesh", &Font::GenerateMesh)
													.def("GetTexture", &Font::GetTexture)
	];
}

BEARISH_API void Scripting::RegisterCore() {
	using namespace luabind;

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
			.def("GetComponentsByID", &Actor::GetComponents)
			.def("GetTransform", &Actor::GetTransform)
			.def("GetTransformation", &Actor::GetTransformation)
			.def("GetTranslation", &Actor::GetTranslation)
			.def("GetParent", &Actor::GetParent)
			.def("HasParent", &Actor::HasParent)
			.def("SetParent", &Actor::SetParent)
			.def("Kill", &Actor::Kill),

			class_<Mouse>("Mouse")
			.scope[
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
					.scope[
						def("IsKeyDown", &Keyboard::IsKeyDown),
							def("IsKeyUp", &Keyboard::IsKeyUp),
							def("IsKeyPressed", &Keyboard::IsKeyPressed),
							def("IsKeyReleased", &Keyboard::IsKeyReleased)
					],

					class_<key_wrappper>("Key")
							.enum_("constants")[
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
									.enum_("Constants")[
										value("Left", (i32)MouseButton::Left),
											value("Right", (i32)MouseButton::Right),
											value("Middle", (i32)MouseButton::Middle)
									]
	];
}