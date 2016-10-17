#ifndef _BEARISH_GUI_WIN32WINDOWBUTTON_H_
#define _BEARISH_GUI_WIN32WINDOWBUTTON_H_

#include "Win32WindowComponent.h"
#include <BE/Core/Callback.h>

namespace Bearish {namespace GUI {
	class BEARISH_API Win32WindowButton : public Win32WindowComponent {
	public:
		Win32WindowButton(string text, i32 x, i32 y, i32 width, i32 height, Core::Action<> action) :
			_text(text), _x(x), _y(y), _width(width), _height(height), _action(action) {}
	protected:
		 virtual void Register(void* parentHandle, i32 id) override;
		 virtual void Action() override;
	private:
		string _text;
		i32 _x, _y;
		i32 _width, _height;
		Core::Action<> _action;
	};
} }

#endif // _BEARISH_GUI_WIN32WINDOWBUTTON_H_
