#ifndef _BEARISH_CORE_MOUSE_H_
#define _BEARISH_CORE_MOUSE_H_

#include <BE/Math/vec2.h>
#include <BE/Core/ButtonState.h>
#include <BE/Types.h>

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
		static BEARISH_API void Update();
		static BEARISH_API const ButtonState GetButtonState(const MouseButton key);
		static BEARISH_API bool IsButtonDown(const MouseButton key);
		static BEARISH_API bool IsButtonUp(const MouseButton key);
		static BEARISH_API bool IsButtonPressed(const MouseButton key);
		static BEARISH_API bool IsButtonReleased(const MouseButton key);

		static BEARISH_API const Math::vec2 GetMousePosition();
		static BEARISH_API const Math::vec2 GetMouseDelta();

		static BEARISH_API void LockToCentre();
		static BEARISH_API void FreeFromCentre();

		static BEARISH_API void SetButtonState(const MouseButton button, const ButtonState state);
		static BEARISH_API void SetCursorPosition(const i32 x, const i32 y);
	private:
		static Math::vec2 _mousePosition, _mouseDelta;
		static ButtonState _buttons[(u32)MouseButton::Num];
	};
} }

#endif