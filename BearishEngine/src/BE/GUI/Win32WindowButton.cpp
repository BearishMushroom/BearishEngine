#include "Win32WindowButton.h"
#include <Windows.h>

using namespace Bearish;
using namespace Core;
using namespace GUI;

void Win32WindowButton::Register(void* parentHandle, i32 id) {
	_handle = CreateWindowEx(NULL, "BUTTON", _text.c_str(), WS_VISIBLE | WS_CHILD | BS_USERBUTTON,
							_x, _y, _width, _height, (HWND)parentHandle, (HMENU)id, GetModuleHandle(NULL), NULL);
}

void Win32WindowButton::Action() {
	_action();
}

