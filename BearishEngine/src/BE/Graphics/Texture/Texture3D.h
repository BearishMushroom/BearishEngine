#ifndef _BEARISH_GRAPHICS_TEXTURE3D_H_
#define _BEARISH_GRAPHICS_TEXTURE3D_H_

#include "../../Types.h"
#include "Texture.h"

namespace Bearish { namespace Graphics {
	class BEARISH_API Texture3D : public Texture {
		Texture3D(const string& filename, const TextureFilter& filter = TextureFilter::Linear) :
			Texture(filename, TextureType::Texture3D, TextureFormat::RGBA, filter) {
		}
	};
} }

#endif // _BEARISH_GRAPHICS_TEXTURE3D_H_
