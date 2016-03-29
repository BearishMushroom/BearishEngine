#ifndef _BEARISH_GRAPHICS_TEXTURECUBEMAP_H_
#define _BEARISH_GRAPHICS_TEXTURECUBEMAP_H_

#include "../../Types.h"
#include "Texture.h"

namespace Bearish { namespace Graphics {
	class TextureCubeMap : public Texture {
	public:
		TextureCubeMap(string right, string left, string top, string bottom, string front, string back, TextureFilter filter = TextureFilter::Linear)
		: Texture(right, left, top, bottom, front, back, TextureFormat::RGBA, filter) {
		}

		TextureCubeMap(std::vector<string> names, TextureFilter filter = TextureFilter::Linear) 
		: Texture(names[0], names[0], names[0], names[0], names[0], names[0], TextureFormat::RGBA, filter) {
			if (names.size() < 6) {
				Core::Logger::Error("Supples too few filenames to cubemap!");
			}
		}
	};
} }

#endif // _BEARISH_GRAPHICS_TEXTURECUBEMAP_H_
