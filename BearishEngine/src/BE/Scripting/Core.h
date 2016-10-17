#ifndef _BEARISH_SCRIPTING_CORE_H_
#define _BEARISH_SCRIPTING_CORE_H_

#include <BE/Core/IActorComponent.h>
#include <BE/Core/Mouse.h>
#include <BE/Core/Actor.h>

#include "ScriptingEngine.h"
#include <BE/Types.h>

namespace Bearish { namespace Scripting {
	struct BEARISH_API key_wrappper {};
	struct BEARISH_API button_wrapper {};

	BEARISH_API void RegisterCore();
} }

#endif // _BEARISH_SCRIPTING_CORE_H_