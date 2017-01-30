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
		Surface(const Device& device, GUI::Win32Window* window);
		~Surface();

		operator VkSurfaceKHR() const { return _surface; }

		SwapchainSupportDetails GetSwapchainSupportDetails() const;
	private:
		const GUI::Win32Window* _window;
		const GPU* _gpu;
		const Device& _device;
		Swapchain _swapchain;
		VkSurfaceKHR _surface;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SURFACE_H_
