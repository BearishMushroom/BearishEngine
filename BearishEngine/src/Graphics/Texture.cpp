#include "Texture.h"
#include "OpenGL.h"
#include "../Core/Timer.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;
using namespace Core;

Texture::Texture(const string filename, const TextureType type, const TextureFormat format, const TextureFilter filter) {
	_filter = filter;
	Load(filename, type, format);
}

Texture::Texture(const vec2& size, const TextureType type, const TextureFilter filter, const TextureAttachment attachment, const TextureFormat format, u8* data) {
	_size = size;
	_type = type;
	_filter = filter;
	_attachments = std::vector<TextureAttachment>{ attachment };
	_formats = std::vector<TextureFormat>{ format };
	InitTextures(type, _formats, &data);
	InitRenderTargets(_attachments);
}

Texture::Texture(const vec2& size, const TextureType type, std::vector<TextureAttachment> attachments, std::vector<TextureFormat> formats, u32 num, const TextureFilter filter) {
	_size = size;
	_type = type;
	_filter = filter;
	_attachments = attachments;
	_formats = formats;
	u8* datas = 0;
	InitTextures(type, formats, 0, num);
	InitRenderTargets(attachments);
}


Texture::Texture(const string posX, const string negX, const string posY, const string negY, const string posZ, const string negZ, const TextureFormat format, const TextureFilter filter) {
	_formats = std::vector<TextureFormat>{ format };
	_type = TextureType::CubeMap;
	_filter = filter;
	Load(posX, negX, posY, negY, posZ, negZ);
}

Texture::Texture(const vec4& color) {
	u8* data = new u8[4] {
		(u8)(color.r * 255.f), (u8)(color.g * 255.f), (u8)(color.b * 255.f), (u8)(color.a * 255.f)
	};

	_attachments = std::vector<TextureAttachment>{ TextureAttachment::None };
	_formats = std::vector<TextureFormat>{ TextureFormat::RGBA };

	_size = vec2(1, 1);
	_type = TextureType::Texture2D;

	InitTextures(_type, _formats, &data, 1);
	InitRenderTargets(_attachments);
}


Texture::~Texture() {
	Unload();
}

void Texture::Load(const string filename, const TextureType type, TextureFormat format) {
	Timer time;
	time.Start();
	_type = type;
	_formats = std::vector<TextureFormat> { format };
	_filename = filename;

	i32 width, height;
	u8* image = SOIL_load_image(filename.c_str(), &width, &height, 0, 0);
	_size = vec2(static_cast<f32>(width), static_cast<f32>(height));

	InitTextures(type, std::vector<TextureFormat> { format }, &image);

	SOIL_free_image_data(image);
	Logger::Info("Texture %s loaded in %.3fms", filename.c_str(), time.DeltaMS());
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
		i32 width, height;
		u8* image = SOIL_load_image(names[i].c_str(), &width, &height, 0, 0);
		_size = vec2(width, height);

		InitCubeMapFace(CubeMapFaces[i], image);

		SOIL_free_image_data(image);
	}

	glTexParameteri((GLenum)_type, GL_TEXTURE_MAG_FILTER, (GLenum)_filter);
	glTexParameteri((GLenum)_type, GL_TEXTURE_MIN_FILTER, (GLenum)_filter);

	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri((GLenum)_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Unbind(0);
	Logger::Info("Cubemap %s loaded in %.3fms", posX.c_str(), time.DeltaMS());
}

void Texture::Unload() {
	glDeleteTextures(_numTextures, _ids);
	_type = (TextureType)0;
	_filename = "";
	_size = vec2(0, 0);
}

void Texture::InitCubeMapFace(const i32 face, const u8* data) {
	glTexImage2D((GLenum)face, 0, GL_RGBA, (GLsizei)_size.x, (GLsizei)_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::InitTextures(const TextureType type, std::vector<TextureFormat> formats, u8** datas, u32 num) {
	_numTextures = num;
	_ids = new u32[num];
	glGenTextures(num, _ids);

	for (i32 i = 0; i < num; i++) {
		Bind(0, i);

		u8* d = 0;
		if (datas) d = datas[i];

		glTexImage2D((GLenum)type, 0, (GLint)formats[i], static_cast<i32>(_size.x), static_cast<i32>(_size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, d);
		glTexParameteri((GLenum)type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri((GLenum)type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (type == TextureType::CubeMap || type == TextureType::Texture3D) {
			glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		glTexParameteri((GLenum)type, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri((GLenum)type, GL_TEXTURE_MAX_LEVEL, 0);
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

	for (i32 i = 0; i < _numTextures; i++) {
		if (attachments[i] == TextureAttachment::Depth) {
			drawBuffers[i] = TextureAttachment::None;
			hasDepth = true;
		}
		else {
			drawBuffers[i] = attachments[i];
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachments[i], GL_TEXTURE_2D, _ids[i], 0);

		if (_framebuffer == 0)
			return;
	}

	if (!hasDepth) {
		glGenRenderbuffers(1, &_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<i32>(_size.x), static_cast<i32>(_size.y));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer);
	}

	glDrawBuffers(_numTextures, (const GLenum*)&attachments[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::Bind(const u32 slot, const u32 id) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture((const GLenum)_type, _ids[id]);
}

void Texture::Unbind(const u32 slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture((const GLenum)TextureType::Texture2D, 0);
}

vec2 Texture::GetSize() const {
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