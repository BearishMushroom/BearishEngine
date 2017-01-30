#include <BE/Graphics/API/Swapchain.h>
#include <BE/Graphics/API/Surface.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Surface::Surface(const Device& device, GUI::Win32Window* window) : _device(device) {
	_gpu = _device.GetGPU();
	_window = window;

	VkWin32SurfaceCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = window->GetHandle();
	createInfo.hinstance = GetModuleHandle(nullptr);
	
	auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(*_device.GetInstance(), "vkCreateWin32SurfaceKHR");

	if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR
		(*_device.GetInstance(), &createInfo, nullptr, &_surface) != VK_SUCCESS) {
		Core::Logger::Fatal("Failed to create Win32 surface!");
	}

	SwapchainSupportDetails support = GetSwapchainSupportDetails();
	if (support.formats.empty() || support.presentModes.empty()) {
		Core::Logger::Fatal("No suitable swapchain for surface!");
	}

	_swapchain = Swapchain(support, _window);
}

Surface::~Surface() {
	vkDestroySurfaceKHR(*_device.GetInstance(), _surface, nullptr);
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
