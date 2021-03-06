#include <BE\Graphics\Texture\Texture.h>
#include <BE/Core/Timer.h>
#include <BETFile.h>

using namespace Bearish;
using namespace Graphics;
using namespace Math;
using namespace Core;

Texture::Texture(BET::BETFile* file, const TextureType type, const TextureFormat format, const TextureFilter filter) {
	_filter = filter;
	_wrap = TextureWrapMode::Repeat;
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	Load(file, type, format);
}

Texture::Texture(const string filename, const TextureType type, const TextureFormat format, const TextureFilter filter) {
	_filter = filter;
	_wrap = TextureWrapMode::Repeat;
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	Load(filename, type, format);
}


Texture::Texture(const vec2i size, const TextureType type, const TextureFilter filter, const TextureAttachment attachment, const TextureFormat format, i32 multisamples) {
	_size = size;
	_type = type;
	_filter = filter;
	_attachments = std::vector<TextureAttachment>{ attachment };
	_formats = std::vector<TextureFormat>{ format };
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	_multisamples = multisamples;
	_wrap = TextureWrapMode::Repeat;
	InitTextures(type, _formats, 0);
	InitRenderTargets(_attachments);
}

Texture::Texture(const vec2i size, const TextureType type, const TextureFilter filter, const TextureAttachment attachment, const TextureFormat format,
	u8* data, const TextureDataFormat dataFormat, const TextureWrapMode wrap) {
	_size = size;
	_type = type;
	_filter = filter;
	_attachments = std::vector<TextureAttachment>{ attachment };
	_formats = std::vector<TextureFormat>{ format };
	_dataFormats = std::vector<TextureDataFormat>{ dataFormat };
	_multisamples = 0;
	_wrap = wrap;

	InitTextures(type, _formats, &data);
	InitRenderTargets(_attachments);
}

Texture::Texture(const vec2i size, const TextureType type, std::vector<TextureAttachment> attachments, std::vector<TextureFormat> formats, u32 num, 
				const TextureFilter filter, i32 multisamples) {
	_size = size;
	_type = type;
	_filter = filter;
	_attachments = attachments;
	_formats = formats;
	_multisamples = multisamples;
	_wrap = TextureWrapMode::Repeat;

	u8* datas = 0;
	InitTextures(type, formats, 0, num);
	InitRenderTargets(attachments);
}


Texture::Texture(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ, const TextureFormat format, const TextureFilter filter) {
	_formats = std::vector<TextureFormat>{ format };
	_type = TextureType::CubeMap;
	_filter = filter;
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	_multisamples = 0;
	_wrap = TextureWrapMode::Repeat;

	Load(posX, negX, posY, negY, posZ, negZ);
}

Texture::Texture(BET::BETFile* posX, BET::BETFile* negX, BET::BETFile* posY, BET::BETFile* negY, BET::BETFile* posZ, BET::BETFile* negZ, const TextureFormat format, const TextureFilter filter) {
	_formats = std::vector<TextureFormat>{ format };
	_type = TextureType::CubeMap;
	_filter = filter;
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	_multisamples = 0;
	_wrap = TextureWrapMode::Repeat;

	Load(posX, negX, posY, negY, posZ, negZ);
}

Texture::Texture(const vec4& color) {
	u8* data = new u8[4] {
		(u8)(color.r * 255.f), (u8)(color.g * 255.f), (u8)(color.b * 255.f), (u8)(color.a * 255.f)
	};

	_attachments = std::vector<TextureAttachment>{ TextureAttachment::None };
	_formats = std::vector<TextureFormat>{ TextureFormat::RGBA };

	_size = vec2i(1, 1);
	_type = TextureType::Texture2D;
	_dataFormats = std::vector<TextureDataFormat>{ TextureDataFormat::Byte };
	_multisamples = 0;
	_wrap = TextureWrapMode::Repeat;

	InitTextures(_type, _formats, &data, 1);
	InitRenderTargets(_attachments);
}

Texture::Texture(const Texture& parent, i32 subIdx) {
	if (subIdx >= parent._numTextures) {
		Logger::Error("Invalid subtexture index %d(%d) when creating subtexture from %s.", subIdx, parent._numTextures, parent.GetName().c_str());
		return;
	}

	_size = parent._size;
	_ids = new u32;
	_ids[0] = parent._ids[subIdx];
	_framebuffer = parent._framebuffer;
	_renderbuffer = parent._renderbuffer;
	_numTextures = 1;
	_multisamples = parent._multisamples;

	_filter = parent._filter;
	_type = parent._type;
	_wrap = parent._wrap;
	_dataFormats = std::vector<TextureDataFormat>{ parent._dataFormats[subIdx] };
	_formats = std::vector<TextureFormat>{ parent._formats[subIdx] };
	_attachments = std::vector<TextureAttachment>{ parent._attachments[subIdx] };
}


Texture::~Texture() {
	Unload();
}

void Texture::Load(const string filename, const TextureType type, TextureFormat format) {
	Timer time;
	time.Start();
	_type = type;
	_formats = std::vector<TextureFormat>{ format };
	_filename = filename;

	BET::BETFile file;
	file.ReadFromFile(filename);
	u8* image = file.ConvertToRaw();
	_size = vec2i(file.width, file.height);

	InitTextures(type, std::vector<TextureFormat> { format }, &image);

	free(file.data);
	Logger::Info("Texture %s loaded in %.3fms", filename.c_str(), time.DeltaMS());
}

void Texture::Load(BET::BETFile* file, const TextureType type, TextureFormat format) {
	_type = type;
	_formats = std::vector<TextureFormat>{ format };
	_filename = "";

	_size = vec2i(file->width, file->height);

	InitTextures(type, std::vector<TextureFormat> { format }, &file->data);
}

void Texture::Load(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ) {
	Timer time;
	time.Start();
	std::vector<string> names{
		posX, negX, posY, negY, posZ, negZ
	};

	_ids = new u32;
	glGenTextures(1, _ids);
	_numTextures = 1;
	//Bind(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _ids[0]);

	for (i32 i = 0; i < 6; i++) {
		BET::BETFile file;
		file.ReadFromFile(names[i]);
		u8* image = file.ConvertToRaw();
		_size = vec2i(file.width, file.height);

		InitCubeMapFace(CubeMapFaces[i], image);

		free(file.data);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri((GLenum)_type, GL_TEXTURE_MAG_FILTER, (GLenum)_filter);
	glTexParameteri((GLenum)_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Unbind(0);
	Logger::Info("Cubemap %s loaded in %.3fms", posX.c_str(), time.DeltaMS());
}

void Texture::Load(BET::BETFile* posX, BET::BETFile* negX, BET::BETFile* posY, BET::BETFile* negY, BET::BETFile* posZ, BET::BETFile* negZ) {
	Timer time;
	time.Start();
	std::vector<BET::BETFile*> images {
		posX, negX, posY, negY, posZ, negZ
	};

	_ids = new u32;
	glGenTextures(1, _ids);
	_numTextures = 1;
	//Bind(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _ids[0]);

	for (i32 i = 0; i < 6; i++) {
		_size = vec2i(images[i]->width, images[i]->height);

		InitCubeMapFace(CubeMapFaces[i], images[i]->data);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri((GLenum)_type, GL_TEXTURE_MAG_FILTER, (GLenum)_filter);
	glTexParameteri((GLenum)_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Unbind(0);
	Logger::Info("Cubemap loaded in %.3fms", time.DeltaMS());
}

void Texture::Unload() {
	glDeleteTextures(_numTextures, _ids);
	_type = (TextureType)0;
	_filename = "";
	_size = vec2i(0, 0);
}

void Texture::InitCubeMapFace(const i32 face, const u8* data) {
	glTexImage2D((GLenum)face, 0, GL_RGBA, (GLsizei)_size.x, (GLsizei)_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::InitTextures(const TextureType type, std::vector<TextureFormat> formats, u8** datas, u32 num) {
	_numTextures = num;
	_ids = new u32[num];
	glGenTextures(num, _ids);

	if(_multisamples > 0)
		_type = TextureType::Texture2DMS;

	for (i32 i = 0; i < (i32)num; i++) {
		Bind(0, i);

		u8* d = 0;
		if (datas) d = datas[i];

		if (_multisamples == 0) {
			i32 format = GetFormatForInternalFormat(formats[i]);

			glTexImage2D((GLenum)_type, 0, (GLint)formats[i], static_cast<i32>(_size.x), static_cast<i32>(_size.y), 0, (GLint)format,
				_dataFormats.size() > 0 ? (GLenum)_dataFormats[i] : (GLenum)TextureDataFormat::Byte, d);

			if (format == GL_DEPTH_COMPONENT) {
				glTexParameteri((GLenum)_type, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
				glTexParameteri((GLenum)_type, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			}
			else if (formats[i] != TextureFormat::RG32 && formats[i] != TextureFormat::R32) {
				glGenerateMipmap((GLenum)_type);
			}
			else {
				glTexParameteri((GLenum)_type, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri((GLenum)_type, GL_TEXTURE_MAX_LEVEL, 0);
			}

			glTexParameteri((GLenum)_type, GL_TEXTURE_MAG_FILTER, _filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
			glTexParameteri((GLenum)_type, GL_TEXTURE_MIN_FILTER, (GLenum)_filter);

			glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_S, (GLenum)_wrap);
			glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_T, (GLenum)_wrap);
		}
		else {
			glTexImage2DMultisample((GLenum)_type, _multisamples, (GLint)formats[i], static_cast<i32>(_size.x), static_cast<i32>(_size.y), true);
		}

		if (type == TextureType::CubeMap || type == TextureType::Texture3D) {
			glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
	}

	Unbind(0);
}

void Texture::InitRenderTargets(std::vector<TextureAttachment> attachments) {
	//Bind(0);

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	if (attachments.size() == 0 || attachments[0] == TextureAttachment::None)
		return;

	TextureAttachment drawBuffers[32];
	bool hasDepth = false;

	for (i32 i = 0; i < (i32)_numTextures; i++) {
		if (attachments[i] == TextureAttachment::Depth) {
			drawBuffers[i] = TextureAttachment::None;
			hasDepth = true;
		}
		else {
			drawBuffers[i] = attachments[i];
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachments[i], (GLenum)_type, _ids[i], 0);

		if (_framebuffer == 0)
			return;
	}

	if (!hasDepth) {
		glGenRenderbuffers(1, &_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
		if (_multisamples == 0) {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<i32>(_size.x), static_cast<i32>(_size.y));
		}
		else {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, _multisamples, GL_DEPTH_COMPONENT, static_cast<i32>(_size.x), static_cast<i32>(_size.y));
		}
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer);
	}

	glDrawBuffers(_numTextures, (const GLenum*)&attachments[0]);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::Bind(const u32 slot, const u32 id) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	if (_multisamples > 0) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _ids[id]);
	}
	else {
		glBindTexture((const GLenum)_type, _ids[id]);
	}
}

void Texture::Unbind(const u32 slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture((const GLenum)TextureType::Texture2D, 0);
}

vec2i Texture::GetSize() const {
	return _size;
}

void Texture::BindAsRenderTarget() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, static_cast<i32>(_size.x), static_cast<i32>(_size.y));
}

void Texture::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

TextureType Texture::GetTextureType() const {
	return _type;
}

TextureFormat Texture::GetTextureFormat() const {
	return _formats[0];
}

TextureAttachment Texture::GetTextureAttachment() const {
	return _attachments[0];
}


void Texture::BindForWriting() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
}

void Texture::BindForReading() const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
}

void Texture::SetReadBuffer(u32 buffer) const {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + buffer);
}

i32 Texture::GetFormatForInternalFormat(const TextureFormat in) {
	switch (in) {
	case TextureFormat::R:
	case TextureFormat::R16:
	case TextureFormat::R32:
		return GL_RED;
		break;
	case TextureFormat::RG:
	case TextureFormat::RG16:
	case TextureFormat::RG32:
		return GL_RG;
		break;
	case TextureFormat::RGB:
	case TextureFormat::RGB16:
	case TextureFormat::RGB32:
		return GL_RGB;
		break;
	case TextureFormat::RGBA:
	case TextureFormat::RGBA16:
	case TextureFormat::RGBA32:
		return GL_RGBA;
		break;
	case TextureFormat::BGR:
		return GL_BGR;
		break;	
	case TextureFormat::ABGR:
		return GL_BGRA;
		break;
	case TextureFormat::Depth16:
	case TextureFormat::Depth24:
	case TextureFormat::Depth32:
		return GL_DEPTH_COMPONENT;
		break;
	}

	return GL_RGBA;
}
