#ifndef _BEARISH_SCRIPTING_GRAPHICS_H_
#define _BEARISH_SCRIPTING_GRAPHICS_H_

#include "ScriptingEngine.h"
#include "../Graphics/RenderingEngine.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Types.h"

namespace Bearish { namespace Scripting {
	struct texture_type_wrapper {};
	struct texture_attachment_wrapper {};
	struct texture_format_wrapper {};
	struct texture_filter_wrapper {};

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
				.def("SetUniformMat4", static_cast<void (Graphics::Shader::*)(const string&, Math::mat4)>(&Graphics::Shader::SetUniform)),

			class_<Mesh>("Mesh")
				.scope [
					def("CreateQuad", &Mesh::CreateQuad)
				]
				.def(constructor<std::vector<Vertex>, std::vector<u32>, bool>())
				.def(constructor<Model>())
				.def(constructor<u32, vec3*, vec2*, vec3*, vec3*, vec4i*, vec4*, u32, u32*>())
				.def("Submit", &Mesh::Submit)
				.def("Flush", &Mesh::Flush)
				.def("Animate", &Mesh::Animate)
				.def("DecoupleSkeleton", &Mesh::DecoupleSkeleton),

			class_<Texture>("Texture")
				.def(constructor<string, TextureType, TextureFormat, TextureFilter>())
				.def(constructor<vec2, TextureType, TextureFilter, TextureAttachment, TextureFormat, u8*>())
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
				.enum_("Constants") [
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
					value("Depth", (i32)TextureFormat::Depth)
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
} }

#endif // _BEARISH_SCRIPTING_GRAPHICS_H_