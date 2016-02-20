#ifndef _BEARISH_CORE_MOUSE_H_
#define _BEARISH_CORE_MOUSE_H_

#include "../Math/vec2.h"
#include "ButtonState.h"
#include "../Types.h"
#include "Window.h"

namespace Bearish { namespace Core {
	enum class MouseButton : u8 {
		Left   = GLFW_MOUSE_BUTTON_LEFT,
		Right  = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Four   = GLFW_MOUSE_BUTTON_4,
		Five   = GLFW_MOUSE_BUTTON_5,
		Six    = GLFW_MOUSE_BUTTON_6,
		Seven  = GLFW_MOUSE_BUTTON_7,
		Eight  = GLFW_MOUSE_BUTTON_8,
		Num    = GLFW_MOUSE_BUTTON_LAST
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