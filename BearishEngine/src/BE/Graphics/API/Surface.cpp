#include <BE/Graphics/API/Swapchain.h>
#include <BE/Graphics/API/Surface.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Surface::Surface(const GPU* gpu, const Instance* instance, GUI::Win32Window* window) {
	_instance = instance;
	_gpu = gpu;
	_window = window;

	VkWin32SurfaceCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = window->GetHandle();
	createInfo.hinstance = GetModuleHandle(nullptr);
	
	auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(*instance, "vkCreateWin32SurfaceKHR");

	if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR
		(*instance, &createInfo, nullptr, &_surface) != VK_SUCCESS) {
		Core::Logger::Fatal("Failed to create Win32 surface!");
	}
}

Surface::~Surface() {
	vkDestroySurfaceKHR(*_instance, _surface, nullptr);
}

SwapchainSupportDetails Surface::GetSwapchainSupportDetails() const {
	SwapchainSupportDetails support;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*_gpu, _surface, &support.capabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*_gpu, _surface, &formatCount, nullptr);

	if (formatCount != 0) {
		support.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*_gpu, _surface, &formatCount, support.formats.data());
	}

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*_gpu, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		support.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*_gpu, _surface, &presentModeCount, support.presentModes.data());
	}

	return support;
}
