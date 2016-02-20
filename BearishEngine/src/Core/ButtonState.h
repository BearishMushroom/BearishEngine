#ifndef _BEARISH_CORE_BUTTONSTATE_H_
#define _BEARISH_CORE_BUTTONSTATE_H_

#include "../Types.h"

namespace Bearish { namespace Core {
	enum class ButtonState : u8 {
		Up, Down, Pressed, Released
	};
} }

#endif