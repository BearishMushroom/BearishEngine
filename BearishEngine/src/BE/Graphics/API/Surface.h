#ifndef _BEARISH_GRAPHICS_API_SURFACE_H_
#define _BEARISH_GRAPHICS_API_SURFACE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Device.h>
#include <BE/Graphics/API/Swapchain.h>
#include <BE/GUI/Win32Window.h>

namespace Bearish { namespace Graphics { namespace API {
	// @PLATFORM: This is Win32 only.
	
	class Surface {
	public:
		Surface(const GPU* gpu, const Instance* instance, GUI::Win32Window* window);
		~Surface();

		operator VkSurfaceKHR() const { return _surface; }

		SwapchainSupportDetails GetSwapchainSupportDetails() const;
		const GUI::Win32Window* GetWindow() const { return _window; }

		const Instance* GetInstance() const { return _instance; }
		const GPU* GetGPU() const { return _gpu; }
	private:
		const GUI::Win32Window* _window;
		const GPU* _gpu;
		const Instance* _instance;
		Swapchain _swapchain;
		VkSurfaceKHR _surface;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SURFACE_H_
