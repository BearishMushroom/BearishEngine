#include "pch.h"
#include <BE/Graphics/API/GPU.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

GPU::GPU() {
}

GPU::GPU(Instance instance, VkPhysicalDevice device) {
	_instance = instance;
	_device = device;

	vkGetPhysicalDeviceProperties(device, &_properties);
	vkGetPhysicalDeviceFeatures(device, &_features);
}

GPU::GPU(Instance instance, VkPhysicalDeviceFeatures features, VkPhysicalDeviceProperties properties, VkPhysicalDevice device) {
	_instance = instance;
	_device = device;

	_properties = properties;
	_features = features;
}

GPU::~GPU() {
}

std::vector<GPU> GPU::GetAll(Instance instance) {
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::vector<GPU> gpus(deviceCount);
	for (auto& device : devices) {
		gpus.push_back(GPU(instance, device));
	}

	return gpus;
}

GPU GPU::GetMostSuitable(std::vector<GPU> gpus) {
	if (gpus.size() < 1) {
		Core::Logger::Fatal("Failed to find suitable Vulkan GPU!");
	}
	
	// look for dedicated:
	for (GPU& gpu : gpus) {
		if (gpu.GetAdapterType() == AdapterType::Discrete) {
			return gpu;
		}
	}

	// look for virtual
	for (GPU& gpu : gpus) {
		if (gpu.GetAdapterType() == AdapterType::Virtual) {
			return gpu;
		}
	}

	// look for integrated
	for (GPU& gpu : gpus) {
		if (gpu.GetAdapterType() == AdapterType::Integrated) {
			return gpu;
		}
	}

	// no idea
	return gpus[0];
}

GPUQueue GPU::GetGraphicsQueue() {
	GPUQueue q;

	u32 count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);

	std::vector<VkQueueFamilyProperties> queues(count);
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, queues.data());

	i32 i = 0;
	for (const auto& queue : queues) {
		if (queue.queueCount > 0 && queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			q.index = i;
			break;
		}

		i++;
	}

	return q;
}
