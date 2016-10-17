#ifndef _BEARISH_SCRIPTING_GRAPHICS_H_
#define _BEARISH_SCRIPTING_GRAPHICS_H_

#include "ScriptingEngine.h"
#include <BE/Graphics/RenderingEngine.h>
#include <BE/Graphics/Shader.h>
#include <BE/Graphics/Camera.h>
#include <BE/Types.h>

namespace Bearish { namespace Scripting {
	struct BEARISH_API texture_type_wrapper {};
	struct BEARISH_API texture_attachment_wrapper {};
	struct BEARISH_API texture_format_wrapper {};
	struct BEARISH_API texture_filter_wrapper {};

	BEARISH_API void RegisterGraphics();
} }

#endif // _BEARISH_SCRIPTING_GRAPHICS_H_