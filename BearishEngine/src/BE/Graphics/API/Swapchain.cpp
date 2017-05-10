#include "pch.h"
#include <BE\Graphics\API\Swapchain.h>
#include <BE\Graphics\API\Surface.h>
#include <BE\Core\Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Swapchain::Swapchain() {
	_swapchain = 0;
}

Swapchain::Swapchain(const Device* device) {
	_framebuffers = 0;
	_device = device;
	_surface = _device->GetSurface();
	_window = _surface->GetWindow();
	_support = _surface->GetSwapchainSupportDetails();

	if (_support.formats.empty() || _support.presentModes.empty()) {
		Core::Logger::Fatal("No suitable swapchain for surface!");
	}

	_format = FindFormat();
	Core::Logger::Info("Swapchain format: %d, %d", _format.format, _format.colorSpace);

	_mode = FindPresentMode();
	Core::Logger::Info("Swapchain present mode: %d", _mode);

	_extent = FindExtent();
	Core::Logger::Info("Swapchain extent: %dx%d", _extent.width, _extent.height);

	// We want min image count + 1 to properly implement tripple buffering.
	u32 imageCount = _support.capabilities.minImageCount + 1;

	// If there is a limit to how many images we can have in the swapchain, make sure we don't exceed it.
	if (_support.capabilities.maxImageCount > 0 && imageCount > _support.capabilities.maxImageCount) {
		imageCount = _support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR info {};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = *_surface;
	info.minImageCount = imageCount;
	info.imageFormat = _format.format;
	info.imageColorSpace = _format.colorSpace;
	info.imageExtent = _extent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	u32 indices[] = { _surface->GetGPU()->GetGraphicsQueue(), _surface->GetGPU()->GetPresentQueue(_surface) };

	// @PERFORMANCE: Concurrent mode can be slow!
	if (indices[0] != indices[1]) {
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = new u32[2] { indices[0], indices[1] };
	} else {
		info.queueFamilyIndexCount = 1;
		info.pQueueFamilyIndices = new u32 { indices[0] };
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	info.preTransform = _support.capabilities.currentTransform;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	info.presentMode = _mode;
	info.clipped = 1;

	if (VkError(vkCreateSwapchainKHR(*_device, &info, nullptr, &_swapchain))) {
		Core::Logger::Fatal("Failed to create Vulkan swapchain!");
	}

	_imageFormat = _format.format;
	vkGetSwapchainImagesKHR(*_device, _swapchain, &_imageCount, nullptr);
	_images = new VkImage[_imageCount];
	vkGetSwapchainImagesKHR(*_device, _swapchain, &_imageCount, _images);

	_views = new ImageView*[_imageCount];
	for (u32 i = 0; i < _imageCount; i++) {
		_views[i] = new ImageView(_device, _images[i], _imageFormat, ImageView::Swizzle::RGBA);
	}


}

Swapchain::~Swapchain() {
	
	if (_views) {
		for (i32 i = 0; i < _imageCount; i++) {
			delete _views[i];
		}

		delete[] _views;
	}

	if (_framebuffers) {
		for (i32 i = 0; i < _imageCount; i++) {
			vkDestroyFramebuffer(*_device, _framebuffers[i], nullptr);
		}

		delete[] _framebuffers;
	}

	delete[] _images;

	if (_swapchain) {
		vkDestroySwapchainKHR(*_device, _swapchain, nullptr);
	}
}

void Swapchain::CreateFramebuffers(VkRenderPass* renderpass) {
	_framebuffers = new VkFramebuffer[_imageCount];
	for (u32 i = 0; i < _imageCount; i++) {
		VkImageView views[] = {
			*_views[i]
		};

		VkFramebufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = *renderpass;
		info.attachmentCount = 1;
		info.pAttachments = views;
		info.width = _extent.width;
		info.height = _extent.height;
		info.layers = 1;

		if (VkError(vkCreateFramebuffer(*_device, &info, nullptr, &_framebuffers[i]))) {
			Core::Logger::Fatal("Failed to create swapchain framebuffer!");
		}
	}
}

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