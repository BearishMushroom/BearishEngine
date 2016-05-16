#include "Keyboard.h"
#include "Window.h"

using namespace Bearish;
using namespace Core;

ButtonState Keyboard::_keys[(u32)Key::Max];
Window* Keyboard::_window;

void Keyboard::SetWindow(Window* window) {
	_window = window;
}

void Keyboard::Update() {
	for (i32 i = 0; i < (i32)Key::Max; i++) {
		ButtonState result = _window->GetKeyboardKey(i);

		if (result == ButtonState::Down) {
			if (_keys[i] == ButtonState::Up || _keys[i] == ButtonState::Released) {
				_keys[i] = ButtonState::Pressed;
			} else {
				_keys[i] = ButtonState::Down;
			}
		}

		if (result == ButtonState::Up) {
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