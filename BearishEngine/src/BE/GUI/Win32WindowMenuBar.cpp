#include "Win32WindowMenuBar.h"

using namespace Bearish;
using namespace GUI;

Win32WindowMenuBar::Win32WindowMenuBar() {
}

Win32WindowMenuBar::~Win32WindowMenuBar() {
}

void Win32WindowMenuBar::Register(void* parentHandle, i32 id) {
	_handle = (void*)CreateMenu();
}

void Win32WindowMenuBar::PostRegister(void* parentHandle, i32 id) {
	SetMenu((HWND)parentHandle, (HMENU)_handle);
}