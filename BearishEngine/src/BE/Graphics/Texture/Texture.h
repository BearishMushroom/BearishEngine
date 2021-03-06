#ifndef _BEARISH_GRAPHICS_TEXTURE_H_
#define _BEARISH_GRAPHICS_TEXTURE_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Math/vec2.h>
#include <BE/Math/vec4.h>
#include <BE/Types.h>
#include <BE/Graphics/OpenGL.h>
#include <BETFile.h>

namespace Bearish { namespace Graphics {
	enum class TextureType : u32 {
		Texture1D   = GL_TEXTURE_1D,
		Texture2D   = GL_TEXTURE_2D,
		Texture3D   = GL_TEXTURE_3D,
		CubeMap     = GL_TEXTURE_CUBE_MAP,
		Texture2DMS = GL_TEXTURE_2D_MULTISAMPLE,
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
		R       = GL_RED,
		RG      = GL_RG,
		RGB     = GL_RGB,
		RGBA    = GL_RGBA,
		R16     = GL_R16F,
		RG16    = GL_RG16F,
		RGB16   = GL_RGB16F,
		RGBA16  = GL_RGBA16F,
		R32     = GL_R32F,
		RG32    = GL_RG32F,
		RGB32   = GL_RGB32F,
		RGBA32  = GL_RGBA32F,
		BGR     = GL_BGR,
		ABGR    = GL_ABGR_EXT,
		Depth16 = GL_DEPTH_COMPONENT16,
		Depth24 = GL_DEPTH_COMPONENT24,
		Depth32 = GL_DEPTH_COMPONENT32,
	};

	enum class TextureFilter : i32 {
		Nearest = GL_NEAREST_MIPMAP_NEAREST,
		Linear  = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class TextureDataFormat : i32 {
		Byte  = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT
	};

	enum class TextureWrapMode : i32 {
		Clamp  = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT,
	};

	class BEARISH_API Texture : public Core::IAllocatable<Texture> {
	public:
		Texture(Core::BET::BETFile* file, const TextureType type = TextureType::Texture2D,
				const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Linear);

		Texture(const string filename, const TextureType type = TextureType::Texture2D, 
				const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Linear);

		Texture(const Math::vec2i size = Math::vec2i(256, 256), const TextureType type = TextureType::Texture2D, const TextureFilter filter = TextureFilter::Nearest,
				const TextureAttachment attachment = TextureAttachment::Color0, const TextureFormat format = TextureFormat::RGBA, u8* data = 0,
				const TextureDataFormat dataFormat = TextureDataFormat::Byte, const TextureWrapMode wrap = TextureWrapMode::Clamp);

		Texture(const Math::vec2i size, const TextureType type = TextureType::Texture2D, const TextureFilter filter = TextureFilter::Nearest,
				const TextureAttachment attachment = TextureAttachment::Color0, const TextureFormat format = TextureFormat::RGBA, i32 multisamples = 0);

		Texture(const Math::vec2i size, const TextureType type, std::vector<TextureAttachment> attachments, std::vector<TextureFormat> formats, u32 num, 
				const TextureFilter filter = TextureFilter::Nearest, i32 multisamples = 0);

		Texture(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ,
			const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Nearest);

		Texture(Core::BET::BETFile* posX, Core::BET::BETFile* negX, Core::BET::BETFile* posY, Core::BET::BETFile* negY, Core::BET::BETFile* posZ, Core::BET::BETFile* negZ,
			const TextureFormat format = TextureFormat::RGBA, const TextureFilter filter = TextureFilter::Nearest);

		Texture(const Math::vec4& color);

		Texture(const Texture& parent, i32 subIdx);

		~Texture();

		void Load(const string filename, const TextureType type, TextureFormat format);
		void Load(Core::BET::BETFile* file, const TextureType type, TextureFormat format);

		void Load(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ);
		void Load(Core::BET::BETFile* posX, Core::BET::BETFile* negX, Core::BET::BETFile* posY, Core::BET::BETFile* negY, Core::BET::BETFile* posZ, Core::BET::BETFile* negZ);

		void Unload();

		void InitCubeMapFace(const i32 face, const u8* data);
		void InitTextures(const TextureType type, std::vector<TextureFormat> format, u8** data, u32 num = 1);
		void InitRenderTargets(std::vector<TextureAttachment> attachments);

		void Bind(const u32 slot, const u32 id = 0) const;
		void Unbind(const u32 slot) const;

		void BindAsRenderTarget() const;
	
		void Clear() const;

		Math::vec2i GetSize() const;

		TextureType GetTextureType() const;
		TextureFormat GetTextureFormat() const;
		TextureAttachment GetTextureAttachment() const;

		void BindForWriting() const;
		void BindForReading() const;
		void SetReadBuffer(u32 buffer) const;

		const string& GetName()      const { return _filename; }
		const string& GetAssetName() const { return _assetname; }

		void SetAssetName(string name) { _assetname = name; }

		i32 GetFormatForInternalFormat(const TextureFormat in);
	protected:
		const std::vector<i32> CubeMapFaces {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		};

		string _filename, _assetname;
		Math::vec2i _size;
		u32* _ids;
		u32 _framebuffer, _renderbuffer, _numTextures;
		i32 _multisamples;
		
		TextureFilter _filter;
		TextureType _type;
		TextureWrapMode _wrap;
		std::vector<TextureDataFormat> _dataFormats;
		std::vector<TextureFormat> _formats;
		std::vector<TextureAttachment> _attachments;
	};
} }

#endif
