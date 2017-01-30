#include "pch.h"
#include <BE\Graphics\API\Swapchain.h>
#include <BE\Core\Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Swapchain::Swapchain() {}

Swapchain::Swapchain(SwapchainSupportDetails support, const GUI::Win32Window* window) {
	_window = window;
	_support = support;
	_format = FindFormat();
	Core::Logger::Info("Swapchain format: %d, %d", _format.format, _format.colorSpace);
	_mode = FindPresentMode();
	Core::Logger::Info("Swapchain present mode: %d", _mode);
	_extent = FindExtent();
	Core::Logger::Info("Swapchain extent: %dx%d", _extent.width, _extent.height);
}

Swapchain::~Swapchain() {}

VkSurfaceFormatKHR Swapchain::FindFormat() {
	auto& formats = _support.formats;

	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
		// If the swapchain doesn't care about the format, use our preferred one!
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& format : formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			// If one of the preferred formats matches our own, use that!
			return format;
		}
	}

	// If it doesn't support what we want, use the GPU's most preferred one.
	return formats[0];
}

VkPresentModeKHR Swapchain::FindPresentMode() {
	for (const auto& mode : _support.presentModes) {
		// Use this one if we can.
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return mode;
		}
	}

	// This is guarenteed to always be there.
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::FindExtent() {
	if (_support.capabilities.currentExtent.width != (u32)-1) {
		return _support.capabilities.currentExtent;
	} else {
		VkExtent2D extent = { _window->GetWidth(), _window->GetHeight() };

		extent.width = max(_support.capabilities.minImageExtent.width, min(_support.capabilities.maxImageExtent.width, extent.width));
		extent.height = max(_support.capabilities.minImageExtent.height, min(_support.capabilities.maxImageExtent.height, extent.height));

		return extent;
	}
}