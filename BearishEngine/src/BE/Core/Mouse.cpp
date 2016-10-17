#include <BE/Graphics/Renderer.h>
#include "Mouse.h"
#include <BE/Math/vec2.h>
#include <BE/Core/Settings.h>

using namespace Bearish;
using namespace Core;
using namespace Math;

ButtonState Mouse::_buttons[(u32)MouseButton::Num];
Math::vec2 Mouse::_mousePosition;
Math::vec2 Mouse::_mouseDelta;

void Mouse::Update() {
	for (auto i = 0; i < (i32)MouseButton::Num; i++) {
		if (_buttons[i] == ButtonState::Pressed) {
			_buttons[i] = ButtonState::Down;
		}

		if (_buttons[i] == ButtonState::Released) {
			_buttons[i] = ButtonState::Up;
		}
	}

	_mouseDelta = vec2(0, 0);
}

const ButtonState Mouse::GetButtonState(const MouseButton key) {
	return _buttons[(u32)key];
}

bool Mouse::IsButtonDown(const MouseButton key) {
	return _buttons[(u32)key] == ButtonState::Down || _buttons[(u32)key] == ButtonState::Pressed;
}


bool Mouse::IsButtonUp(const MouseButton key) {
	return _buttons[(u32)key] == ButtonState::Up || _buttons[(u32)key] == ButtonState::Released;
}

bool Mouse::IsButtonPressed(const MouseButton key) {
	return _buttons[(u32)key] == ButtonState::Pressed;
}

bool Mouse::IsButtonReleased(const MouseButton key) {
	return _buttons[(u32)key] == ButtonState::Released;
}

const Math::vec2 Mouse::GetMousePosition() {
	return _mousePosition;
}

const Math::vec2 Mouse::GetMouseDelta() {
	return _mouseDelta;
}

void Mouse::LockToCentre() {
	//_window->LockCursor();
}

void Mouse::FreeFromCentre() {
	//_window->FreeCursor();
}

void Mouse::SetButtonState(const MouseButton button, const ButtonState state) {
	if (state == ButtonState::Pressed) {
		if (_buttons[(i32)button] == ButtonState::Up || _buttons[(i32)button] == ButtonState::Released) {
			_buttons[(i32)button] = ButtonState::Pressed;
		}
		else {
			_buttons[(i32)button] = ButtonState::Down;
		}
	}

	if (state == ButtonState::Released) {
		if (_buttons[(i32)button] == ButtonState::Down || _buttons[(i32)button] == ButtonState::Pressed) {
			_buttons[(i32)button] = ButtonState::Released;
		}
		else {
			_buttons[(i32)button] = ButtonState::Up;
		}
	}
}

void Mouse::SetCursorPosition(const i32 x, const i32 y) {
	Math::vec2 currentPos((f32)x / Settings::Get<f32>("resolution_x") * 1920.f, (f32)y / Settings::Get<f32>("resolution_y") * 1080.f);

	_mouseDelta = currentPos - _mousePosition;
	_mousePosition = currentPos;
}
