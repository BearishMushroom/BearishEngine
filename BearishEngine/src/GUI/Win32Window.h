#ifndef _BEARISH_GUI_WINDOW_H_
#define _BEARISH_GUI_WINDOW_H_

#include "../Types.h"
#include "../Core/IAllocatable.h"
#include "Win32WindowComponent.h"

namespace Bearish {	namespace GUI {
	class Win32Window : public Core::IAllocatable<Win32Window> {
	public:
		static Win32Window* ACTIVE_WINDOW;

		Win32Window(string title, i32 width, i32 height) : _title(title), _width(width), _height(height) {}
		~Win32Window();

		void AddComponent(Win32WindowComponent* component);
		void RegisterComponents(void* handle);
		void Command(i32 id);

		void Open();
		void Close();
		void Update();

		bool IsOpen() const { return _open; }
		void SetOpen(bool open) { _open = open; }

		template<typename T>
		T* GetComponentOfType() {
			for (auto& c : _components) {
				T* test = dynamic_cast<T*>(c);
				if (test)
					return test;
			}

			return 0;
		}

		const i32 GetWidth() const { return _width; }
		const i32 GetHeight() const { return _height; }
	private:
		string _title;
		i32 _width, _height;
		bool _open;
		void* _handle;

		std::vector<Win32WindowComponent*> _components;

		const c8* CLASS_NAME = "BearishEngineWindow";
	};
} }

#endif // _BEARISH_GUI_WINDOW_H_