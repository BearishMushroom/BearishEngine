#ifndef _BEARISH_GRAPHICS_TEXTURE2D_H_
#define _BEARISH_GRAPHICS_TEXTURE2D_H_

#include "Texture.h"
#include "../../Core/IAllocatable.h"
#include "../../Types.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API Texture2D : public Texture {
	public:
		Texture2D(const string& filename, const TextureFilter& filter = TextureFilter::Linear) : 
			Texture(filename, TextureType::Texture2D, TextureFormat::RGBA, filter) {
		}

		Texture2D(const Math::vec4& color) {
			u8* data = new u8[4] {
				(u8)(color.r * 255.f), (u8)(color.g * 255.f), (u8)(color.b * 255.f), (u8)(color.a * 255.f)
			};

			_attachments = std::vector<TextureAttachment>{ TextureAttachment::None };
			_formats = std::vector<TextureFormat>{ TextureFormat::RGBA };

			_size = Math::vec2i(1, 1);
			_type = TextureType::Texture2D;
			_filter = TextureFilter::Nearest;

			InitTextures(_type, _formats, &data, 1);
			InitRenderTargets(_attachments);

			delete[] data;
		}
	};
} }

#endif // _BEARISH_GRAPHICS_TEXTURE2D_H_
