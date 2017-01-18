#include <BE/Graphics/API/Device.h>
#include <BE/Graphics/API/Queue.h>
#include <BE/Graphics/API/Util.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Bearish::Graphics::API::Device::Device()
{
}

Device::Device(const GPU* gpu) {
	_gpu = gpu;

	VkDeviceQueueCreateInfo gqinf = Queue::CreateDeviceQueue(gpu->GetGraphicsQueue(), 1.0f);
	VkPhysicalDeviceFeatures features = gpu->GetFeatures();

	VkDeviceCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &gqinf;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &features;

	//createInfo.enabledExtensionCount = gpu->GetInstance()->GetNumExtensions();
	//
	//VkExtensionProperties* exts = gpu->GetInstance()->GetExtensions();
	//char** names = new char*[createInfo.enabledExtensionCount];
	//
	//for (i32 i = 0; i < createInfo.enabledExtensionCount; i++) {
	//	names[i] = exts[i].extensionName;
	//}
	//
	//createInfo.ppEnabledExtensionNames = names;

#ifdef BEARISH_DEBUG
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = new char*{ "VK_LAYER_LUNARG_standard_validation" };
	Core::Logger::Info("Requesting Vulkan device valdation layers...");
#endif

	if (VkError(vkCreateDevice(gpu->GetPhyiscalDevice(), &createInfo, nullptr, &_device))) {
		Core::Logger::Fatal("Failed to create Vulkan logical device.");
	}
}

Device::~Device() {
	if (_device) {
		vkDestroyDevice(_device, nullptr);
		_device = nullptr;
	}
}
