#include "Win32WindowComponent.h"
#include "Win32Window.h"

using namespace Bearish;
using namespace GUI;

void Win32WindowComponent::Create(void* parentHandle, i32 id) {
	_id = id;

	Register(parentHandle, id);

	for (usize i = 0; i < _children.size(); i++) {
		_children.at(i)->Create(_handle == 0 ? parentHandle : _handle, id * 100 + i);
	}

	PostRegister(parentHandle, id);
}

void Win32WindowComponent::Trigger(i32 id) {
	if (_id == id) {
		Action();
	}

	for (auto& c : _children) {
		c->Trigger(id);
	}
}

void Win32WindowComponent::AddChild(Win32WindowComponent* child) {
	child->SetWindow(_window);
	child->SetParent(this);
	_children.push_back(child);
}

void Win32WindowComponent::SetWindow(Win32Window* window) {
	_window = window;
}

void Win32WindowComponent::SetParent(Win32WindowComponent* parent) {
	_parent = parent;
}