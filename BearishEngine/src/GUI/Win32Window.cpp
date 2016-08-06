#include "Win32Window.h"
#include "../Core/Keyboard.h"
#include "../Core/Mouse.h"
#include "Win32WindowGLViewport.h"

#include <Windows.h>
#include <windowsx.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace Bearish;
using namespace GUI;

Win32Window* Win32Window::ACTIVE_WINDOW = 0;

LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE:
		Win32Window::ACTIVE_WINDOW->RegisterComponents(hwnd);
		break;
	case WM_DESTROY:
		Win32Window::ACTIVE_WINDOW->SetOpen(false);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		Win32Window::ACTIVE_WINDOW->Command(LOWORD(wparam));
		break;
	case WM_KEYDOWN:
		Core::Keyboard::SetKeyState((Core::Key)wparam, Core::ButtonState::Pressed);
		break;
	case WM_KEYUP:
		Core::Keyboard::SetKeyState((Core::Key)wparam, Core::ButtonState::Released);
		break;
	case WM_LBUTTONDOWN:
		Core::Mouse::SetButtonState(Core::MouseButton::Left, Core::ButtonState::Pressed);
		break;
	case WM_LBUTTONUP:
		Core::Mouse::SetButtonState(Core::MouseButton::Left, Core::ButtonState::Released);
		break;
	case WM_RBUTTONDOWN:
		Core::Mouse::SetButtonState(Core::MouseButton::Right, Core::ButtonState::Pressed);
		break;
	case WM_RBUTTONUP:
		Core::Mouse::SetButtonState(Core::MouseButton::Right, Core::ButtonState::Released);
		break;
	case WM_MBUTTONDOWN:
		Core::Mouse::SetButtonState(Core::MouseButton::Middle, Core::ButtonState::Pressed);
		break;
	case WM_MBUTTONUP:
		Core::Mouse::SetButtonState(Core::MouseButton::Middle, Core::ButtonState::Released);
		break;
	case WM_MOUSEMOVE:
		//TRACKMOUSEEVENT track;
		//track.cbSize = sizeof(TRACKMOUSEEVENT);
		//track.dwFlags = TME_LEAVE;
		//track.hwndTrack = hwnd;
		//
		//TrackMouseEvent(&track);

		Core::Mouse::SetCursorPosition((i32)GET_X_LPARAM(lparam), (i32)GET_Y_LPARAM(lparam));
		break;
	case WM_MOUSELEAVE:
		Core::Mouse::SetButtonState(Core::MouseButton::Left, Core::ButtonState::Released);
		Core::Mouse::SetButtonState(Core::MouseButton::Right, Core::ButtonState::Released);
		Core::Mouse::SetButtonState(Core::MouseButton::Middle, Core::ButtonState::Released);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	}

	return 0;
}

Win32Window::~Win32Window() {

}

void Win32Window::AddComponent(Win32WindowComponent* component) {
	component->SetWindow(this);
	_components.push_back(component);
}

void Win32Window::RegisterComponents(void* handle) {
	for (auto i = 0; i < (i32)_components.size(); i++) {
		_components.at(i)->Create(handle, i);
	}
}

void Win32Window::Command(i32 id) {
	for (auto& c : _components) {
		c->Trigger(id);
	}
}

void Win32Window::Open() {
	HINSTANCE hinstance = GetModuleHandle(NULL);
	
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndproc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(3);
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = LoadIcon(hinstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, "FAILED TO REGISTER WINDOW CLASS!", "BearishEngine", NULL);
		return;
	}

	ACTIVE_WINDOW = this;
	HWND hwnd = CreateWindow(CLASS_NAME, _title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								_width, _height, NULL, NULL, hinstance, NULL);

	if (!hwnd) {
		MessageBox(NULL, "FAILED TO OPEN WINDOW!", "BearishEngine", NULL);
		return;
	}

	_handle = (void*)hwnd;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	_open = true;
}

void Win32Window::Close() {
	PostMessage((HWND)_handle, WM_CLOSE, 0, 0);
}

void Win32Window::Update() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	auto gl = GetComponentOfType<Win32WindowGLViewport>();
	if (gl)
		gl->Update();
}