#ifndef _BEARISH_GUI_WIN32WINDOWGLVIEWPORT_H_
#define _BEARISH_GUI_WIN32WINDOWGLVIEWPORT_H_

#include "../Types.h"
#include "Win32WindowComponent.h"


namespace Bearish {	namespace GUI {
	class Win32WindowGLViewport : public Win32WindowComponent {
	public:
		Win32WindowGLViewport(i32 x, i32 y, i32 width, i32 height) : _x(x), _y(y), _width(width), _height(height) {}
		~Win32WindowGLViewport() {}

		void Update();
	protected:
		virtual void Register(void* parentHandle, i32 id) override;
	private:
		i32 _x, _y, _width, _height;
		void *_hglrc, *_hdc, *_hwnd;

		typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
		typedef BOOL  (WINAPI * PFNWGLSWAPINTERVALEXTPROC)         (int interval);
		
		PFNWGLCREATECONTEXTATTRIBSARBPROC _createProc;
		PFNWGLSWAPINTERVALEXTPROC         _vSyncProc;
	};
} }

#endif // _BEARISH_GUI_WIN32WINDOWGLVIEWPORT_H_
