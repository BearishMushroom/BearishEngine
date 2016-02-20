#ifndef _BEARISH_GRAPHICS_IMAGE_BUFFER_H_
#define _BEARISH_GRAPHICS_IMAGE_BUFFER_H_

#include "../Types.h"
#include "../Core/IAllocatable.h"
#include "../Math/vec4.h"

namespace Bearish { namespace Graphics {
	class ImageBuffer : public Core::IAllocatable<ImageBuffer> {
	public:
		static const i32 IMAGE_CHANNELS = 4;
		
		ImageBuffer(i32 width, i32 height, u8* data = 0) : _width(width), _height(height), _data(data ? data : new u8[width * height * IMAGE_CHANNELS]) {
			memset(_data, 0, width * height * IMAGE_CHANNELS);
		}

		~ImageBuffer() { delete[] _data; }

		void SetPixel(i32 x, i32 y, const Math::vec4& color);
		void SetPixel(i32 x, i32 y, u32 color);
		
		Math::vec4 GetPixel(i32 x, i32 y) const;
		u32 GetPixelU32(i32 x, i32 y) const;

		u8* GetData() { return _data; }
		i32 GetWidth() { return _width; }
		i32 GetHeight() { return _height; }
		Math::vec2 GetDimensions() { return Math::vec2((f32)_width, (f32)_height); }
	private:
		u8* _data;
		i32 _width, _height;
	};
} }

#endif // _BEARISH_GRAPHICS_IMAGE_BUFFER_H_ 


/*

test.roar

roarc test.roar -o test.exe

func i32 [x: i32, y: i32] add >>
	return<x + y>
<<

class vec2 >>
	private f32 x, y;

	public func f32 [] length >>
		return sqrt<x * x + y * y>
	<<
<<

func i32 [args: string{}] main >>
	print<"Wowee">
	return<0>
<<

*/