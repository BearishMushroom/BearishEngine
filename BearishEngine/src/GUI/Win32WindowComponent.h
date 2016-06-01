#ifndef _BEARISH_GUI_WIN32WINDOWCOMPONENT_H_
#define _BEARISH_GUI_WIN32WINDOWCOMPONENT_H_

#include "../Types.h"

namespace Bearish { namespace GUI {
	class Win32Window;

	class Win32WindowComponent {
	public:
		void Create(void* parentHandle, i32 id);
		void Trigger(i32 id);

		void AddChild(Win32WindowComponent* child);

		void SetWindow(Win32Window* window);
		void SetParent(Win32WindowComponent* parent);
	protected:
		virtual void Register(void* parentHandle, i32 id) {}
		virtual void PostRegister(void* parentHandle, i32 id) {}
		virtual void Action() {}

		i32 _id;
		void* _handle;
		std::vector<Win32WindowComponent*> _children;
		Win32Window* _window;
		Win32WindowComponent* _parent;
	private:
	};
} }

#endif // _BEARISH_GUI_WIN32WINDOWCOMPONENT_H_