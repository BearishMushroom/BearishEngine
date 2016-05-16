#ifndef _BEARISH_GUI_WIN32WINDOWSUBMENU_H_
#define _BEARISH_GUI_WIN32WINDOWSUBMENU_H_

#include "../Types.h"
#include "Win32WindowComponent.h"
#include "../Core/Callback.h"

namespace Bearish { namespace GUI {
	class Win32WindowSubMenuItem : public Win32WindowComponent {
	public:
		Win32WindowSubMenuItem(string name, Core::Action<> clicked);
		~Win32WindowSubMenuItem();
	protected:
		virtual void Register(void* parentHandle, i32 id) override;
		virtual void Action() override;
	private:
		string _name;
		Core::Action<> _clicked;
	};

	class Win32WindowSubMenu : public Win32WindowComponent {
	public:
		Win32WindowSubMenu(string name);
		~Win32WindowSubMenu();
	protected:
		virtual void Register(void* parentHandle, i32 id) override;
		virtual void PostRegister(void* parentHandle, i32 id) override;
	private:
		string _name;
	};
} }

#endif // _BEARISH_GUI_WIN32WINDOWSUBMENU_H_