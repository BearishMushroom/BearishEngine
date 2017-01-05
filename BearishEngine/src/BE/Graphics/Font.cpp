#include "Font.h"
#include <BE/Core/Logger.h>

#include <ft2build.h>
#include FT_FREETYPE_H


using namespace Bearish;
using namespace Core;
using namespace Math;
using namespace Graphics;

FT_LibraryRec_* Font::_Freetype;

i32 Font::FONT_TEXTURE_SIZE = 1024;
i32 Font::FONT_CHAR_MIN = 32;
i32 Font::FONT_CHAR_MAX = 32 + 420; // Deprecated

void Font::InitFT() {
	if (!_Freetype) {
		i32 error = FT_Init_FreeType(&_Freetype);
		if (error) {
			Logger::Error("Font: Failed to initialize Freetype.");
		}
	}
}

Font::Font(const string& filepath, i32 padding) {
	InitFT();

	FT_Face face;
	i32 error = FT_New_Face(_Freetype, filepath.c_str(), 0, &face);
	if (error) {
		Logger::Error("Font: Failed to load face from %s.", filepath.c_str());
	}

	FT_Set_Pixel_Sizes(face, 0, 64);

	i32 channels = 4;
	u8* alphaBuffer = new u8[FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE];
	memset(alphaBuffer, 0, FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE);
	
	// Start one padding in the texture.
	i32 penPositionX = padding;
	i32 penPositionY = padding;

	// 2D Array get: x + y * width

	for (i32 i = FONT_CHAR_MIN; penPositionY < FONT_TEXTURE_SIZE - padding - 64; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			Logger::Error("Font: Can't get char %d from %s.", i, filepath.c_str());
			continue;
		}


		for (i32 x = 0; x < (i32)face->glyph->bitmap.width; x++) {
			for (i32 y = 0; y < (i32)face->glyph->bitmap.rows; y++) {
				alphaBuffer[(FONT_TEXTURE_SIZE - (penPositionX + x)) + ((penPositionY + y) * FONT_TEXTURE_SIZE)] =
					face->glyph->bitmap.buffer[x + y * face->glyph->bitmap.width];	//A
			}
		}

		f32 glyphTextureStartX = (penPositionX - padding) / (f32)FONT_TEXTURE_SIZE;
		f32 glyphTextureStartY = ((f32)FONT_TEXTURE_SIZE - (penPositionY - padding)) / (f32)FONT_TEXTURE_SIZE;
		f32 glyphTextureEndX = (penPositionX + face->glyph->bitmap.width + padding) / (f32)FONT_TEXTURE_SIZE;
		f32 glyphTextureEndY = ((f32)FONT_TEXTURE_SIZE - (penPositionY + face->glyph->bitmap.rows + padding)) / (f32)FONT_TEXTURE_SIZE;

		_glyphs.emplace(i, Glyph{ (f32)face->glyph->bitmap.width / 64.f, (f32)face->glyph->bitmap.rows / 64.f,
								vec2(glyphTextureStartX, glyphTextureStartY), vec2(glyphTextureEndX, glyphTextureEndY),
								(f32)face->glyph->advance.x / 64.f / 64.f, (f32)face->glyph->advance.y / 64.f / 64.f,
								(f32)face->glyph->bitmap_left / 64.f, -(f32)face->glyph->bitmap_top / 64.f });
	
		penPositionX += face->glyph->bitmap.width + padding * 2;
		if (penPositionX > FONT_TEXTURE_SIZE - 24) {
			penPositionY += ((face->ascender - face->descender + face->max_advance_height) / 64) + padding*2;
			penPositionX = padding;
		}
	}

	u32 alphaBufferSize = FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE;
	u8* textureData = new u8[FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE * channels];
	for (i32 pos = 0; pos < FONT_TEXTURE_SIZE * FONT_TEXTURE_SIZE * channels; pos += 4) {
		textureData[pos] = 255;
		textureData[pos + 1] = 255;
		textureData[pos + 2] = 255;
		textureData[pos + 3] = alphaBuffer[alphaBufferSize - pos / 4];
	}

	//s_spread = spread;
	_newline = (f32)(face->ascender - face->descender) / 64.f / 64.f + 0.15f;
	//u8* sdf = GenerateDistanceField(textureData, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);

	// TODO:
	// GET SDF WORKING

	_texture = new Texture(vec2i(FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE), TextureType::Texture2D, TextureFilter::Linear, TextureAttachment::None, TextureFormat::RGBA, textureData);
	
	delete[] alphaBuffer;
	delete[] textureData;
	//delete[] sdf;
}

Mesh* Font::GenerateMesh(const string& text, f32 glyphScale) const {
	const c8* cstr = text.c_str();
	Mesh* res;
	std::vector<Vertex> vertices;
	std::vector<u32> indices;
	vertices.reserve(text.length() * 4);
	indices.reserve(text.length() * 6);

	f32 xpos = 0;
	f32 ypos = 0;
	u32 currentIndex = 0;

	for (const c8* ch = cstr; *ch; ch++) {
		if (*ch == '\n') {
			ypos += _newline * glyphScale;
			xpos = 0;
			continue;
		}

		Glyph current = _glyphs.at(*ch);
		f32 thisx = xpos + current.offsetX * glyphScale;
		f32 thisy = ypos + current.offsetY * glyphScale;
		
		try {
			vertices.push_back(Vertex(vec3(thisx, thisy, 0), current.texMin));
			vertices.push_back(Vertex(vec3(thisx + current.sizeX * glyphScale, thisy, 0), vec2(current.texMax.x, current.texMin.y)));
			vertices.push_back(Vertex(vec3(thisx + current.sizeX * glyphScale, thisy + current.sizeY * glyphScale, 0), current.texMax));
			vertices.push_back(Vertex(vec3(thisx, thisy + current.sizeY * glyphScale, 0), vec2(current.texMin.x, current.texMax.y)));
		}
		catch (std::exception e) {
		//	//printf("Oops.\n");
		//	throw;
		}

		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 1);
		indices.push_back(currentIndex + 2);
		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 2);
		indices.push_back(currentIndex + 3);
		
		currentIndex += 4;
		
		xpos += current.spaceX * glyphScale;
		ypos += current.spaceY * glyphScale;
	}

	res = new Mesh(vertices, indices);
	return res;
}

i32 Font::SquareDist(i32 x1, i32 y1, i32 x2, i32 y2) {
	i32 dx = x1 - x2;
	i32 dy = y1 - y2;
	return dx*dx + dy*dy;
}

u8* Font::GenerateDistanceField(u8* inImage, i32 width, i32 height) {
	i32 inWidth = width;
	i32 inHeight = height;

	u8* outImage = new u8[width * height * 4];

	// Note: coordinates reversed to mimic storage of alphaBufferedImage, for memory locality
	bool* bitmap = new bool[width * height];
	for (i32 y = 0; y < inHeight; ++y) {
		for (i32 x = 0; x < inWidth; ++x) {
			bitmap[x + y * width] = IsInside(vec4(inImage[x * 4 + y * 4 * width], inImage[x * 4 + 1 + y * 4 * width],
				inImage[x * 4 + 2 + y * 4 * width], inImage[x * 4 + 3 + y * 4 * width]));
		}
	}

	for (i32 y = 0; y < inHeight; ++y) {
		for (i32 x = 0; x < inWidth; ++x) {
			f32 signedDistance = FindSignedDistance(x, y, bitmap, width, height);
			vec4 col = DistanceToRGB(signedDistance);
			outImage[x * 4 + y * 4 * width] = (u8)col.x;
			outImage[x * 4 + 1 + y * 4 * width] = (u8)col.y;
			outImage[x * 4 + 2 + y * 4 * width] = (u8)col.z;
			outImage[x * 4 + 3 + y * 4 * width] = (u8)col.w;
		}
	}

	return outImage;
}

bool Font::IsInside(vec4 rgb) {
	u32 rgbi = ((u32)rgb.z) | ((u32)rgb.y << 8) | ((u32)rgb.x << 16) | ((u32)rgb.w << 24);
	return (rgbi & 0x808080) != 0 && (rgbi & 0x80000000) != 0;
}

vec4 Font::DistanceToRGB(f32 signedDistance) {
	f32 alpha = 0.5f + 0.5f * (signedDistance / _spread);
	alpha = min(1, max(0, alpha)); // compensate for rounding errors
	u8 alphaByte = (u8)(alpha * 0xFF); // no unsigned byte in Java :(
	return vec4(0xFF, 0xFF, 0xFF, alphaByte);
}

f32 Font::FindSignedDistance(i32 centerX, i32 centerY, bool* bitmap, i32 width, i32 height) {
	bool base = bitmap[centerY * width + centerX];

	i32 delta = (i32)ceil(_spread);
	i32 startX = max(0, centerX - delta);
	i32 endX = min(width - 1, centerX + delta);
	i32 startY = max(0, centerY - delta);
	i32 endY = min(height - 1, centerY + delta);

	f32 closestSquareDist = (f32)(delta * delta);

	for (i32 y = startY; y <= endY; ++y) {
		for (i32 x = startX; x <= endX; ++x) {
			if (base != bitmap[y * width + x]) {
				i32 squareDist = SquareDist(centerX, centerY, x, y);
				if (squareDist < closestSquareDist) {
					closestSquareDist = (f32)squareDist;
				}
			}
		}
	}

	f32 closestDist = (f32)sqrtf(closestSquareDist);
	return (base ? 1 : -1) * min(closestDist, _spread);
}
