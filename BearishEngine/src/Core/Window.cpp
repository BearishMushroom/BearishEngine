#include "Keyboard.h"
#include "Window.h"
#include "Logger.h"
#include "Mouse.h"
#include "../Math/vec2.h"
#include <glfw3.h>

using namespace Bearish;
using namespace Core;
using namespace Math;

bool Window::_GLFWInit = false;

Window::Window(string title, u32 width, u32 height, bool fs) {
	_width = width;
	_height = height;
	_title = title;

	if (!_GLFWInit) {
		_GLFWInit = glfwInit() == GL_TRUE;
		
		if (!_GLFWInit) {
			Logger::Fatal("GLFW failed to init!");
		}
		else {
			Logger::Info("GLFW init!");
		}
	}

	_window = glfwCreateWindow(width, height, title.c_str(), fs ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (!_window) {
		Logger::Error("Failed to create window!");
	}
	else {
		Logger::Info("Window \"%s\" created of size %d x %d", _title.c_str(), _width, _height);
	}

	glfwMakeContextCurrent(_window);
	Keyboard::SetWindow(this);
	Mouse::SetWindow(this);
	glfwSwapInterval(0);
}

Window::~Window() {
	Logger::Info("Window %s destroyed!", _title.c_str());
	glfwMakeContextCurrent(0);
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::Update() {
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(_window) != 0;
}

void Window::SetVsync(i32 frames) {
	glfwSwapInterval(frames);
}

void Window::BindAsRenderTarget() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _width, _height);
}

void Window::LockCursor() {
//	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::FreeCursor() {
//	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

ButtonState Window::GetMouseButton(i32 index) {
	return (ButtonState)false;// glfwGetMouseButton(_window, index);
}

vec2 Window::GetMousePosition() {
	f64 x = 0, y = 0;
	//glfwGetCursorPos(_window, &x, &y);
	return vec2((f32)x, (f32)y);
}

ButtonState Window::GetKeyboardKey(i32 index) {
	return (ButtonState)false;//glfwGetKey(_window, index);
}

void Window::SetFullscreen(bool yes) {
	//glfwDestroyWindow(_window);
	_window = glfwCreateWindow(_width, _height, _title.c_str(), yes ? glfwGetPrimaryMonitor() : NULL, NULL);
	//glfwMakeContextCurrent(_window);
	Keyboard::SetWindow(this);
	Mouse::SetWindow(this);
}
