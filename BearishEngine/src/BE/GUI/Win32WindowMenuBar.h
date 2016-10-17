#ifndef _BEARISH_GUI_WIN32WINDOWMENUBAR_H_
#define _BEARISH_GUI_WIN32WINDOWMENUBAR_H_

#include <BE/Types.h>
#include "Win32WindowComponent.h"
#include "Win32WindowSubMenu.h"

namespace Bearish {namespace GUI {
	class BEARISH_API Win32WindowMenuBar : public Win32WindowComponent {
	public:
		Win32WindowMenuBar();
		~Win32WindowMenuBar();
	protected:
		virtual void Register(void* parentHandle, i32 id) override;
		virtual void PostRegister(void* parentHandle, i32 id) override;
	};
} }

#endif // _BEARISH_GUI_WIN32WINDOWMENUBAR_H_
