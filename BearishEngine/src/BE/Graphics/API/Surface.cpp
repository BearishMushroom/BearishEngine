#include <BE/Graphics/API/Surface.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Surface::Surface(const Device& device, HWND window) : _device(device) {
	VkWin32SurfaceCreateInfoKHR createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = window;
	createInfo.hinstance = GetModuleHandle(nullptr);
	
	auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(*_device.GetInstance(), "vkCreateWin32SurfaceKHR");

	if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR
		(*_device.GetInstance(), &createInfo, nullptr, &_surface) != VK_SUCCESS) {
		Core::Logger::Fatal("Failed to create Win32 surface!");
	}
}

Surface::~Surface() {
	vkDestroySurfaceKHR(*_device.GetInstance(), _surface, nullptr);
}