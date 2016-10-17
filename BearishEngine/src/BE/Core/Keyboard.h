#ifndef _BEARISH_CORE_KEYBOARD_H_
#define _BEARISH_CORE_KEYBOARD_H_

#include <BE/Core/IAllocatable.h>
#include <BE/Core/ButtonState.h>
#include <BE/Core/Callback.h>
#include <BE/Types.h>

namespace Bearish { namespace Core {
	enum class Key : u16 {
#ifdef _WIN32
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		Zero  = 48,
		One   = 49,
		Two   = 50,
		Three = 51,
		Four  = 52,
		Five  = 53,
		Six   = 54,
		Seven = 55,
		Eight = 56,
		Nine  = 57,

		NumpadZero  = VK_NUMPAD0,
		NumpadOne   = VK_NUMPAD1,
		NumpadTwo   = VK_NUMPAD2,
		NumpadThree = VK_NUMPAD3,
		NumpadFour  = VK_NUMPAD4,
		NumpadFive  = VK_NUMPAD5,
		NumpadSix   = VK_NUMPAD6,
		NumpadSeven = VK_NUMPAD7,
		NumpadEight = VK_NUMPAD8,
		NumpadNine  = VK_NUMPAD9,

		Period     = VK_OEM_PERIOD,
		Comma      = VK_OEM_COMMA,
		LeftShift  = VK_LSHIFT,
		RightShift = VK_RSHIFT,
		LeftAlt    = VK_LMENU,
		RightAlt   = VK_RMENU,
		LeftCtrl   = VK_LCONTROL,
		RightCtrl  = VK_RCONTROL,
		Home       = VK_HOME,

		Up    = VK_UP,
		Down  = VK_DOWN,
		Left  = VK_LEFT,
		Right = VK_RIGHT,

		F1  = VK_F1,
		F2  = VK_F2,
		F3  = VK_F3,
		F4  = VK_F4,
		F5  = VK_F5,
		F6  = VK_F6,
		F7  = VK_F7,
		F8  = VK_F8,
		F9  = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,

		Backspace = VK_BACK,
		Tab       = VK_TAB,
		Enter     = VK_RETURN,
		Escape    = VK_ESCAPE,
		Spacebar  = VK_SPACE,

		Minus = VK_OEM_MINUS,
		Plus = VK_OEM_PLUS,
#endif // _WIN32
		Max = 512,
	};

	class BEARISH_API Keyboard {
	public:
		static void Update();

		static const ButtonState GetKeyState(const Key key);
		static bool IsKeyDown(const Key key);
		static bool IsKeyUp(const Key key);
		static bool IsKeyPressed(const Key key);
		static bool IsKeyReleased(const Key key);

		static void SetKeyState(const Key key, const ButtonState state);

		static void Listen(Callback<bool, Key, ButtonState>::Function function) {
			_callbacks += function;
		}

		static void Deafen(Callback<bool, Key, ButtonState>::Function function) {
			_callbacks -= function;
		}
	private:
		static Callback<bool, Key, ButtonState> _callbacks;
		static ButtonState _keys[(u32)Key::Max];
	};
} }

#endif