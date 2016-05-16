#ifndef _BEARISH_CORE_MOUSE_H_
#define _BEARISH_CORE_MOUSE_H_

#include "../Math/vec2.h"
#include "ButtonState.h"
#include "../Types.h"
#include "Window.h"

namespace Bearish { namespace Core {
	enum class MouseButton : u8 {
		Left   = 0,
		Right  = 1,
		Middle = 2,
		Four   = 3,
		Five   = 4,
		Six    = 5,
		Seven  = 6,
		Eight  = 7,
		Num    = 9
	};

	class Mouse {
	public:
		static void Update();
		static const ButtonState GetButtonState(const MouseButton key);
		static bool IsButtonDown(const MouseButton key);
		static bool IsButtonUp(const MouseButton key);
		static bool IsButtonPressed(const MouseButton key);
		static bool IsButtonReleased(const MouseButton key);

		static const Math::vec2 GetMousePosition();
		static const Math::vec2 GetMouseDelta();

		static void LockToCentre();
		static void FreeFromCentre();

		static void SetWindow(Window* window);
	private:
		static Math::vec2 _mousePosition, _mouseDelta;
		static ButtonState _buttons[(u32)MouseButton::Num];
		static Window* _window;
	};
} }

#endif