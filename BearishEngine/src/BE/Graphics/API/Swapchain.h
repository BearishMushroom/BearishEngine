#ifndef _BEARISH_GRAPHICS_API_SWAPCHAIN_H_
#define _BEARISH_GRAPHICS_API_SWAPCHAIN_H_

#include <BE/Types.h>
#include <BE\Graphics\API\Util.h>
#include <BE\GUI\Win32Window.h>

namespace Bearish { namespace Graphics { namespace API {
	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain {
	public:
		Swapchain();
		Swapchain(SwapchainSupportDetails support, const GUI::Win32Window* window);
		~Swapchain();

	private:
		VkSurfaceFormatKHR FindFormat();
		VkPresentModeKHR   FindPresentMode();
		VkExtent2D         FindExtent();

		const GUI::Win32Window* _window;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR _mode;
		VkExtent2D _extent;
		SwapchainSupportDetails _support;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SWAPCHAIN_H_
