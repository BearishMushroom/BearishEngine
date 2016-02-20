#include "../Graphics/Renderer.h"
#include "Mouse.h"

using namespace Bearish;
using namespace Core;

Window* Mouse::_window;
ButtonState Mouse::_buttons[(u32)MouseButton::Num];
Math::vec2 Mouse::_mousePosition;
Math::vec2 Mouse::_mouseDelta;

void Mouse::Update() {
	f64 x, y;
	glfwGetCursorPos(_window->GetWindow(), &x, &y);
	i32 width, height;
	glfwGetWindowSize(_window->GetWindow(), &width, &height);

	Math::vec2 currentPos(x * ((f32)Graphics::Renderer::UI_RESOLUTION_X / (f32)width), y * ((f32)Graphics::Renderer::UI_RESOLUTION_Y / (f32)height));

	_mouseDelta = currentPos - _mousePosition;
	_mousePosition = currentPos;

	for (i32 i = 0; i < (i32)MouseButton::Num; i++) {
		i32 result = glfwGetMouseButton(_window->GetWindow(), i);

		if (result == GLFW_PRESS) {
			if (_buttons[i] == ButtonState::Up || _buttons[i] == ButtonState::Released) {
				_buttons[i] = ButtonState::Pressed;
			}
			else {
				_buttons[i] = ButtonState::Down;
			}
		}

		if (result == GLFW_RELEASE) {
			if (_buttons[i] == ButtonState::Down || _buttons[i] == ButtonState::Pressed) {
				_buttons[i] = ButtonState::Released;
			}
			else {
				_buttons[i] = ButtonState::Up;
			}
		}
	}
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
	glfwSetInputMode(_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Mouse::FreeFromCentre() {
	glfwSetInputMode(_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
