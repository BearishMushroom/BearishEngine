#include "../Graphics/Renderer.h"
#include "Mouse.h"
#include "../Math/vec2.h"

using namespace Bearish;
using namespace Core;
using namespace Math;

Window* Mouse::_window;
ButtonState Mouse::_buttons[(u32)MouseButton::Num];
Math::vec2 Mouse::_mousePosition;
Math::vec2 Mouse::_mouseDelta;

void Mouse::Update() {
	/*vec2 pos = _window->GetMousePosition();
	
	i32 width = _window->GetWidth(), height = _window->GetHeight();

	Math::vec2 currentPos(pos.x * ((f32)Graphics::Renderer::UI_RESOLUTION_X / (f32)width), pos.y * ((f32)Graphics::Renderer::UI_RESOLUTION_Y / (f32)height));

	_mouseDelta = currentPos - _mousePosition;
	_mousePosition = currentPos;

	for (i32 i = 0; i < (i32)MouseButton::Num; i++) {
		ButtonState result = _window->GetMouseButton(i);

		if (result == ButtonState::Down) {
			if (_buttons[i] == ButtonState::Up || _buttons[i] == ButtonState::Released) {
				_buttons[i] = ButtonState::Pressed;
			}
			else {
				_buttons[i] = ButtonState::Down;
			}
		}

		if (result == ButtonState::Up) {
			if (_buttons[i] == ButtonState::Down || _buttons[i] == ButtonState::Pressed) {
				_buttons[i] = ButtonState::Released;
			}
			else {
				_buttons[i] = ButtonState::Up;
			}
		}
	}*/
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

void Mouse::SetWindow(Window* window) {
	_window = window;
}

void Mouse::LockToCentre() {
	_window->LockCursor();
}

void Mouse::FreeFromCentre() {
	_window->FreeCursor();
}
