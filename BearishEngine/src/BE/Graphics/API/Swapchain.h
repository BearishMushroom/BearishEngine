#ifndef _BEARISH_GRAPHICS_API_SWAPCHAIN_H_
#define _BEARISH_GRAPHICS_API_SWAPCHAIN_H_

#include <BE/Types.h>
#include <BE\Graphics\API\Util.h>
#include <BE\Graphics\API\ImageView.h>
#include <BE\GUI\Win32Window.h>

namespace Bearish { namespace Graphics { namespace API {
	class Surface;
	class Device;

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain {
	public:
		Swapchain();
		Swapchain(const Device* device);
		~Swapchain();

	private:
		VkSurfaceFormatKHR FindFormat();
		VkPresentModeKHR   FindPresentMode();
		VkExtent2D         FindExtent();

		const GUI::Win32Window* _window;
		const Surface* _surface;
		const Device* _device;

		VkSwapchainKHR _swapchain;
		VkSurfaceFormatKHR _format;
		VkPresentModeKHR _mode;
		VkExtent2D _extent;
		SwapchainSupportDetails _support;

		VkFormat _imageFormat;
		VkImage* _images;
		ImageView* _views;
		u32 _imageCount;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SWAPCHAIN_H_
