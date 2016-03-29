#ifndef _BEARISH_GRAPHICS_TEXTURE1D_H_
#define _BEARISH_GRAPHICS_TEXTURE1D_H_

#include "../../Types.h"
#include "Texture.h"

namespace Bearish { namespace Graphics {
	class Texture1D : public Texture {
	public:
		Texture1D(string filename, TextureFilter filter = TextureFilter::Nearest) :
			Texture(filename, TextureType::Texture1D, TextureFormat::RGBA, filter) {
		}

		Texture1D(std::vector<Math::vec4> pixels, TextureFilter filter = TextureFilter::Nearest) {
			u8* data = new u8[4 * pixels.size()];

			i32 index = 0;
			for (auto& pixel : pixels) {
				data[index] = pixel.r;
				data[index + 1] = pixel.g;
				data[index + 2] = pixel.b;
				data[index + 3] = pixel.a;
				index += 4;
			}

			_attachments = std::vector<TextureAttachment>{ TextureAttachment::None };
			_formats = std::vector<TextureFormat>{ TextureFormat::RGBA };

			_size = Math::vec2(pixels.size(), 1);
			_type = TextureType::Texture1D;
			_filter = TextureFilter::Nearest;

			InitTextures(_type, _formats, &data, 1);
			InitRenderTargets(_attachments);

			delete[] data;
		}
	};
} }

#endif // _BEARISH_GRAPHICS_TEXTURE1D_H_