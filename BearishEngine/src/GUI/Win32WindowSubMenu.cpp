#include "Win32WindowSubMenu.h"

using namespace Bearish;
using namespace Core;
using namespace GUI;

Win32WindowSubMenuItem::Win32WindowSubMenuItem(string name, Core::Action<> clicked) {
	_name = name;
	_clicked = clicked;
}

Win32WindowSubMenuItem::~Win32WindowSubMenuItem() {
}

void Win32WindowSubMenuItem::Register(void* parentHandle, i32 id) {
	AppendMenu((HMENU)parentHandle, MF_STRING, id, _name.c_str());
}

void Win32WindowSubMenuItem::Action() {
	_clicked();
}

Win32WindowSubMenu::Win32WindowSubMenu(string name) {
	_name = name;
}

Win32WindowSubMenu::~Win32WindowSubMenu() {

}

void Win32WindowSubMenu::Register(void* parentHandle, i32 id) {
	_handle = (void*)CreatePopupMenu();
}

void Win32WindowSubMenu::PostRegister(void* parentHandle, i32 id) {
	AppendMenu((HMENU)parentHandle, MF_STRING | MF_POPUP, (UINT)_handle, _name.c_str());
}