#ifndef _BEARISH_CORE_BUTTONSTATE_H_
#define _BEARISH_CORE_BUTTONSTATE_H_

#include <BE/Types.h>

namespace Bearish { namespace Core {
	enum class BEARISH_API ButtonState : u8 {
		Up, Down, Pressed, Released
	};
} }

#endif