#include "ImageBuffer.h"

using namespace Bearish;
using namespace Graphics;
using namespace Math;
using namespace Core;

void ImageBuffer::SetPixel(i32 x, i32 y, const vec4& color) {
	u8 r = (u8)(color.r * 255.f);
	u8 g = (u8)(color.g * 255.f);
	u8 b = (u8)(color.b * 255.f);
	u8 a = (u8)(color.a * 255.f);
	_data[x * 4     + y * _width] = r;
	_data[x * 4 + 1 + y * _width] = g;
	_data[x * 4 + 2 + y * _width] = b;
	_data[x * 4 + 3 + y * _width] = a;
}

void ImageBuffer::SetPixel(i32 x, i32 y, u32 color) {
	u8 r = (u8)((color & 0xFF000000) >> 24);
	u8 g = (u8)((color & 0x00FF0000) >> 16);
	u8 b = (u8)((color & 0x0000FF00) >> 8);
	u8 a = (u8)(color & 0x000000FF);
	_data[x * 4 + y * _width] = r;
	_data[x * 4 + 1 + y * _width] = g;
	_data[x * 4 + 2 + y * _width] = b;
	_data[x * 4 + 3 + y * _width] = a;
}

vec4 ImageBuffer::GetPixel(i32 x, i32 y) const {
	return vec4(_data[x * 4 + y * _width],
					_data[x * 4 + 1 + y * _width],
					_data[x * 4 + 2 + y * _width],
					_data[x * 4 + 3 + y * _width]);
}

u32 ImageBuffer::GetPixelU32(i32 x, i32 y) const {
	return (u32)_data[x * 4 +     y * _width] << 24 |
		    (u32)_data[x * 4 + 1 + y * _width] << 16 |
		    (u32)_data[x * 4 + 2 + y * _width] << 8  |
		    (u32)_data[x * 4 + 3 + y * _width];
}