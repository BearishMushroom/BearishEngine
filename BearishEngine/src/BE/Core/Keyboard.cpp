#include "Keyboard.h"

using namespace Bearish;
using namespace Core;

ButtonState Keyboard::_keys[(u32)Key::Max];
Callback<bool, Key, ButtonState> Keyboard::_callbacks;

void Keyboard::Update() {
	for (auto i = 0; i < (i32)Key::Max; i++) {
		if (_keys[i] == ButtonState::Pressed) {
			_keys[i] = ButtonState::Down;
		}

		if (_keys[i] == ButtonState::Released) {
			_keys[i] = ButtonState::Up;
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

void Keyboard::SetKeyState(const Key key, const ButtonState state) {
	std::vector<bool> res = _callbacks(key, state);
	if (std::find(res.begin(), res.end(), true) != res.end()) {
		return;
	}

	if (state == ButtonState::Pressed) {
		if (_keys[(i32)key] == ButtonState::Up || _keys[(i32)key] == ButtonState::Released) {
			_keys[(i32)key] = ButtonState::Pressed;
		}
		else {
			_keys[(i32)key] = ButtonState::Down;
		}
	}
	
	if (state == ButtonState::Released) {
		if (_keys[(i32)key] == ButtonState::Down || _keys[(i32)key] == ButtonState::Pressed) {
			_keys[(i32)key] = ButtonState::Released;
		}
		else {
			_keys[(i32)key] = ButtonState::Up;
		}
	}
}
