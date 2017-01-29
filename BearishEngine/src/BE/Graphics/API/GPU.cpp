#include "pch.h"
#include <BE/Graphics/API/Surface.h>
#include <BE/Graphics/API/Util.h>
#include <BE/Graphics/API/GPU.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

GPU::GPU() {
	_extensions = nullptr;
}

GPU::GPU(const GPU& other) {
	_instance = other._instance;
	_device = other._device;

	vkGetPhysicalDeviceProperties(_device, &_properties);
	vkGetPhysicalDeviceFeatures(_device, &_features);

	QueryExtensions();
}

GPU::GPU(GPU& other) {
	_instance = other._instance;
	_device = other._device;

	vkGetPhysicalDeviceProperties(_device, &_properties);
	vkGetPhysicalDeviceFeatures(_device, &_features);

	QueryExtensions();
}

GPU::GPU(Instance* instance, VkPhysicalDevice device) {
	_instance = instance;
	_device = device;

	vkGetPhysicalDeviceProperties(device, &_properties);
	vkGetPhysicalDeviceFeatures(device, &_features);

	QueryExtensions();
}

GPU::GPU(Instance* instance, VkPhysicalDeviceFeatures features, VkPhysicalDeviceProperties properties, VkPhysicalDevice device) {
	_instance = instance;
	_device = device;

	_properties = properties;
	_features = features;

	QueryExtensions();
}

GPU::~GPU() {
	// @MEMORY: This isn't working for some reason.

	//if (_extensions != nullptr) {
	//	delete[] _extensions;
	//}
}

std::vector<GPU> GPU::GetAll(Instance* instance) {
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

	std::vector<GPU> gpus;
	for (auto& device : devices) {
		gpus.push_back(GPU(instance, device));
	}

	return gpus;
}

GPU GPU::GetMostSuitable(std::vector<GPU> gpus) {
	if (gpus.size() < 1) {
		Core::Logger::Fatal("Failed to find suitable Vulkan GPU!");
	}
	
	GPU gpur;
	bool found = false;

	// look for dedicated:
	if (!found) {
		for (GPU& gpu : gpus) {
			if (gpu.GetAdapterType() == AdapterType::Discrete) {
				gpur = gpu;
				found = true;
			}
		}
	}

	// look for virtual
	if (!found) {
		for (GPU& gpu : gpus) {
			if (gpu.GetAdapterType() == AdapterType::Virtual) {
				gpur = gpu;
				found = true;
			}
		}
	}

	// look for integrated
	if (!found) {
		for (GPU& gpu : gpus) {
			if (gpu.GetAdapterType() == AdapterType::Integrated) {
				gpur = gpu;
				found = true;
			}
		}
	}

	// no idea
	if (!found) {
		gpur = gpus[0];
	}

	Core::Logger::Info("Found suitable GPU (%s).", gpur.GetName().c_str());
	Core::Logger::Info("Vulkan GPU driver version: %s", VkVersionToString(gpur.GetProperties().apiVersion).c_str());
	return gpur;
}

GPU GPU::GetMostSuitable(Instance* instance) {
	return GetMostSuitable(GetAll(instance));
}

void GPU::QueryExtensions() {
	vkEnumerateDeviceExtensionProperties(_device, nullptr, &_extensionCount, nullptr);
	_extensions = new VkExtensionProperties[_extensionCount];
	vkEnumerateDeviceExtensionProperties(_device, nullptr, &_extensionCount, _extensions);
}

std::vector<VkQueueFamilyProperties> GPU::GetAllQueues() const {
	u32 count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);

	std::vector<VkQueueFamilyProperties> queues(count);
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, queues.data());
	return queues;
}


GPUQueueIndex GPU::GetGraphicsQueue() const {
	GPUQueueIndex q;

	i32 i = 0;
	for (const auto& queue : GetAllQueues()) {
		if (queue.queueCount > 0 && queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			q.index = i;
			break;
		}

		i++;
	}

	return q;
}

GPUQueueIndex GPU::GetPresentQueue(Surface* surface) const {
	GPUQueueIndex q;

	i32 i = 0;
	for (const auto& queue : GetAllQueues()) {
		VkBool32 support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, *surface, &support);
		if (queue.queueCount > 0 && support) {
			q.index = i;
			break;
		}

		i++;
	}

	return q;
}
