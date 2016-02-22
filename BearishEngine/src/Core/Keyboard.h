#ifndef _BEARISH_CORE_KEYBOARD_H_
#define _BEARISH_CORE_KEYBOARD_H_

#include "IAllocatable.h"
#include "ButtonState.h"
#include "../Types.h"
#include "Window.h"

namespace Bearish { namespace Core {
	enum class Key : u16 {
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,

		Zero = GLFW_KEY_0,
		One = GLFW_KEY_1,
		Two = GLFW_KEY_2,
		Three = GLFW_KEY_3,
		Four = GLFW_KEY_4,
		Five = GLFW_KEY_5,
		Six = GLFW_KEY_6,
		Seven = GLFW_KEY_7,
		Eight = GLFW_KEY_8,
		Nine = GLFW_KEY_9,

		NumpadZero = GLFW_KEY_KP_0,
		NumpadOne = GLFW_KEY_KP_1,
		NumpadTwo = GLFW_KEY_KP_2,
		NumpadThree = GLFW_KEY_KP_3,
		NumpadFour = GLFW_KEY_KP_4,
		NumpadFive = GLFW_KEY_KP_5,
		NumpadSix = GLFW_KEY_KP_6,
		NumpadSeven = GLFW_KEY_KP_7,
		NumpadEight = GLFW_KEY_KP_8,
		NumpadNine = GLFW_KEY_KP_9,

		Period = GLFW_KEY_PERIOD,
		Comma = GLFW_KEY_COMMA,
		LeftShift = GLFW_KEY_LEFT_SHIFT,
		RightShift = GLFW_KEY_RIGHT_SHIFT,
		LeftAlt = GLFW_KEY_LEFT_ALT,
		RightAlt = GLFW_KEY_RIGHT_ALT,
		LeftCtrl = GLFW_KEY_LEFT_CONTROL,
		RightCtrl = GLFW_KEY_RIGHT_CONTROL,
		Home = GLFW_KEY_HOME,

		Up = GLFW_KEY_UP,
		Down = GLFW_KEY_DOWN,
		Left = GLFW_KEY_LEFT,
		Right = GLFW_KEY_RIGHT,

		Num = GLFW_KEY_LAST,

		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
	};

	class Keyboard {
	public:
		static void Update();
		static const ButtonState GetKeyState(const Key key);
		static bool IsKeyDown(const Key key);
		static bool IsKeyUp(const Key key);
		static bool IsKeyPressed(const Key key);
		static bool IsKeyReleased(const Key key);
		static void SetWindow(GLFWwindow* window);
	private:
		static ButtonState _keys[(u32)Key::Num];
		static GLFWwindow* _window;
	};
} }

#endif