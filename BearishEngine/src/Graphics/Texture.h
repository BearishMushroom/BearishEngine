#ifndef _BEARISH_GRAPHICS_TEXTURE_H_
#define _BEARISH_GRAPHICS_TEXTURE_H_

#include "../Core/IAllocatable.h"
#include "../Math/vec2.h"
#include "../Math/vec4.h"
#include "../Types.h"
#include "OpenGL.h"
#include <SOIL.h>

namespace Bearish { namespace Graphics {
	static std::vector<i32> CubeMapFaces {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	enum class TextureType : u32 {
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		CubeMap   = GL_TEXTURE_CUBE_MAP,
	};

	enum class TextureAttachment : u32 {
		None    = GL_NONE,
		Color0  = GL_COLOR_ATTACHMENT0,
		Color1  = GL_COLOR_ATTACHMENT1,
		Color2  = GL_COLOR_ATTACHMENT2,
		Color3  = GL_COLOR_ATTACHMENT3,
		Color4  = GL_COLOR_ATTACHMENT4,
		Depth   = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
	};
	
	enum class TextureFormat : i32 {
		RG     = GL_RG,
		RGB    = GL_RGB,
		RGBA   = GL_RGBA,
		R32    = GL_R32F,
		RG32   = GL_RG32F,
		RGB32  = GL_RGB32F,
		RGBA32 = GL_RGBA32F,
		BGR    = GL_BGR,
		ABGR   = GL_ABGR_EXT,
		Dpeth  = GL_DEPTH_COMPONENT,
	};

	enum class TextureFilter : i32 {
		Nearest = GL_NEAREST,
		Linear  = GL_LINEAR,
	};

	class Texture : public Core::IAllocatable<Texture> {
	public:
		Texture(const string filename, const TextureType type = TextureType::Texture2D, 
				const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Nearest);

		Texture(const Math::vec2& size = Math::vec2(256, 256), const TextureType type = TextureType::Texture2D, const TextureFilter filter = TextureFilter::Nearest,
			const TextureAttachment attachment = TextureAttachment::Color0, const TextureFormat format = TextureFormat::RGBA, u8* data = 0);

		Texture(const Math::vec2& size, const TextureType type, std::vector<TextureAttachment> attachments, std::vector<TextureFormat> formats, u32 num, const TextureFilter filter = TextureFilter::Nearest);

		Texture(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ, 
			const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Nearest);

		Texture(const Math::vec4& color);

		~Texture();

		void Load(const string filename, const TextureType type, TextureFormat format);
		void Load(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ);
		void Unload();

		void InitCubeMapFace(const i32 face, const u8* data);
		void InitTextures(const TextureType type, std::vector<TextureFormat> format, u8** data, u32 num = 1);
		void InitRenderTargets(std::vector<TextureAttachment> attachments);

		void Bind(const u32 slot, const u32 id = 0) const;
		void Unbind(const u32 slot) const;

		void BindAsRenderTarget() const;
	
		void Clear() const;

		Math::vec2 GetSize() const;

		TextureType GetTextureType() const;
		TextureFormat GetTextureFormat() const;
		TextureAttachment GetTextureAttachment() const;

		void BindForWriting() const;
		void BindForReading() const;
		void SetReadBuffer(u32 buffer) const;

		string GetName() { return _filename; }
	private:
		string _filename;
		Math::vec2 _size;
		u32* _ids;
		u32 _framebuffer, _renderbuffer, _numTextures;
		
		TextureFilter _filter;
		TextureType _type;
		std::vector<TextureFormat> _formats;
		std::vector<TextureAttachment> _attachments;
	};
} }

#endif
