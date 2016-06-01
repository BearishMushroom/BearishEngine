#ifndef _BEARISH_CORE_KEYBOARD_H_
#define _BEARISH_CORE_KEYBOARD_H_

#include "IAllocatable.h"
#include "ButtonState.h"
#include "../Types.h"
#include "Window.h"

namespace Bearish { namespace Core {
	enum class Key : u16 {
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

		NumpadZero  = 320,
		NumpadOne   = 321,
		NumpadTwo   = 322,
		NumpadThree = 323,
		NumpadFour  = 324,
		NumpadFive  = 325,
		NumpadSix   = 326,
		NumpadSeven = 327,
		NumpadEight = 328,
		NumpadNine  = 329,

		Period     = 46,
		Comma      = 44,
		LeftShift  = 340,
		RightShift = 344,
		LeftAlt    = 342,
		RightAlt   = 346,
		LeftCtrl   = 341,
		RightCtrl  = 345,
		Home       = 268,

		Up    = 265,
		Down  = 264,
		Left  = 263,
		Right = 262,

		F1  = 290,
		F2  = 291,
		F3  = 292,
		F4  = 293,
		F5  = 294,
		F6  = 295,
		F7  = 296,
		F8  = 297,
		F9  = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,

		Max = 512,
	};

	class Keyboard {
	public:
		static void Update();

		static const ButtonState GetKeyState(const Key key);
		static bool IsKeyDown(const Key key);
		static bool IsKeyUp(const Key key);
		static bool IsKeyPressed(const Key key);
		static bool IsKeyReleased(const Key key);

		static void SetKeyState(const Key key, const ButtonState state);
	private:
		static ButtonState _keys[(u32)Key::Max];
	};
} }

#endif