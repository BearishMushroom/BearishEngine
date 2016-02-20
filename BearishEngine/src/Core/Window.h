#ifndef _BEARISH_CORE_WINDOW_H_
#define _BEARISH_CORE_WINDOW_H_

#include <glfw3.h>
#include "../Types.h"
#include "IAllocatable.h"

namespace Bearish { namespace Core {
	class Window : public IAllocatable<Window> {
	private:
		GLFWwindow* _window;
		u32 _width, _height;
		string _title;

		static bool _GLFWInit;
	public:
		Window(string title, u32 width, u32 height, bool fs = false);
		~Window();

		void Update();
		bool ShouldClose();
		void SetVsync(i32 frames);
		void SetFullscreen(bool yes);
		void BindAsRenderTarget();

		GLFWwindow* GetWindow() const { return _window; }
		u32 GetWidth() const { return _width; }
		u32 GetHeight() const { return _height; }
	};
} }

#endif