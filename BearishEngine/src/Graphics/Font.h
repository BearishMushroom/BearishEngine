#ifndef _BEARISH_GRAPHICS_FONT_H_
#define _BEARISH_GRAPHICS_FONT_H_

#include "../Core/IAllocatable.h"
#include "../Math/vec2.h"
#include "Mesh\Mesh.h"
#include "../Types.h"
#include "Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Bearish { namespace Graphics {
	class Glyph : public Core::IAllocatable<Glyph> {
	public:
		Glyph(f32 x, f32 y, Math::vec2 m, Math::vec2 ma, f32 sx, f32 sy, f32 ox, f32 oy) : sizeX(x), sizeY(y), texMin(m), texMax(ma), spaceX(sx), spaceY(sy), offsetX(ox), offsetY(oy) {}
		f32 sizeX, sizeY;
		Math::vec2 texMin, texMax;
		f32 spaceX, spaceY;
		f32 offsetX, offsetY;
	};

	class Font : public Core::IAllocatable<Font> {
	public:
		Font(const string& filepath, i32 padding = 1, f32 spread = 1.5f);
		~Font();

		Mesh* GenerateMesh(const string& text, f32 glyphScale) const;
		Texture* GetTexture() const { return _texture; }
	private:
		static i32 FONT_CHAR_MIN;
		static i32 FONT_CHAR_MAX;
		static i32 FONT_TEXTURE_SIZE;

		static FT_Library _Freetype;
		static void InitFT();

		i32 SquareDist(i32 x1, i32 y1, i32 x2, i32 y2);
		u8* GenerateDistanceField(u8* inImage, i32 width, i32 height);
		bool IsInside(Math::vec4 rgb);
		Math::vec4 DistanceToRGB(f32 signedDistance);
		f32 FindSignedDistance(i32 centerX, i32 centerY, bool* bitmap, i32 width, i32 height);

	public:
		Texture* _texture;
		std::unordered_map<c8, Glyph> _glyphs;
		f32 _spread;
		f32 _newline;
	};
} }

#endif // _BEARISH_GRAPHICS_FONT_H_