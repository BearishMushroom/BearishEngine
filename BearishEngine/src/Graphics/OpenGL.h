#ifndef _BEARISH_GRAPHICS_OPENGL_H_
#define _BEARISH_GRAPHICS_OPENGL_H_

#include "../Types.h"
#include "../Core/Logger.h"

namespace Bearish { namespace Graphics {
	class OpenGL {
	private:
		static bool _isOGLLoaded;
		static std::vector<string> _extensions;
	public:
		static void Init();

		static void GetAllExtensions();
		static bool IsExtensionSupported(string extension);
		static void PrintSupportedExtensions();

		static void* GetFn(const char* name);

		static void SetDebugCallback();
		static bool HandleErrors();
		static string GetErrorString(i32 error);
	};
} }

#endif