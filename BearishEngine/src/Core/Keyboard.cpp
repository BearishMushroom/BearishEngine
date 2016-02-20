#include "Keyboard.h"
#include "Window.h"

using namespace Bearish;
using namespace Core;

ButtonState Keyboard::_keys[(u32)Key::Num];
GLFWwindow* Keyboard::_window;

void Keyboard::SetWindow(GLFWwindow* window) {
	_window = window;
}

void Keyboard::Update() {
	for (i32 i = 0; i < (i32)Key::Num; i++) {
		i32 result = glfwGetKey(_window, i);

		if (result == GLFW_PRESS) {
			if (_keys[i] == ButtonState::Up || _keys[i] == ButtonState::Released) {
				_keys[i] = ButtonState::Pressed;
			} else {
				_keys[i] = ButtonState::Down;
			}
		}

		if (result == GLFW_RELEASE) {
			if (_keys[i] == ButtonState::Down || _keys[i] == ButtonState::Pressed) {
				_keys[i] = ButtonState::Released;
			} else {
				_keys[i] = ButtonState::Up;
			}
		}
	}
}

const ButtonState Keyboard::GetKeyState(const Key key) {
	return _keys[(i32)key];
}

bool Keyboard::IsKeyDown(const Key key) {
	return _keys[(i32)key] == ButtonState::Down || _keys[(i32)key] == ButtonState::Pressed;
}

bool Keyboard::IsKeyUp(const Key key) {
	return _keys[(i32)key] == ButtonState::Up || _keys[(i32)key] == ButtonState::Released;
}

bool Keyboard::IsKeyPressed(const Key key) {
	return _keys[(i32)key] == ButtonState::Pressed;
}

bool Keyboard::IsKeyReleased(const Key key) {
	return _keys[(i32)key] == ButtonState::Released;
}