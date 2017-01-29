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
	_instance = gpu->GetInstance();
}

void Device::Init(Surface* surface) {
	_surface = surface;

	std::vector<VkDeviceQueueCreateInfo> gqinfs = {
		Queue::CreateDeviceQueue(_gpu->GetGraphicsQueue(), 1.0f),
		Queue::CreateDeviceQueue(_gpu->GetPresentQueue(surface), 1.0f)
	};

	// @HACK: Make this good.
	if (gqinfs[0].queueFamilyIndex == gqinfs[1].queueFamilyIndex) {
		gqinfs = std::vector<VkDeviceQueueCreateInfo>{ gqinfs[0] };
	}

	VkPhysicalDeviceFeatures features = _gpu->GetFeatures();

	VkDeviceCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &gqinfs[0];
	createInfo.queueCreateInfoCount = gqinfs.size();
	createInfo.pEnabledFeatures = &features;

	u32 count = _gpu->GetExtensionCount();
	VkExtensionProperties* exts = _gpu->GetExtensions();
	std::vector<VkExtensionProperties> extensions;

	for (i32 i = 0; i < count; i++) {
		VkExtensionProperties& ext = exts[i];
		
		// @TODO: Make this not hardcoded.
		if (strcmp(ext.extensionName, "VK_NVX_device_generated_commands") == 0) {
			continue;
		}

		extensions.push_back(ext);
	}

	createInfo.enabledExtensionCount = extensions.size();
	char** names = new char*[createInfo.enabledExtensionCount];
	
	i32 i = 0;
	for (auto& ext : extensions) {
		names[i++] = ext.extensionName;
	}
	
	createInfo.ppEnabledExtensionNames = names;

#ifdef BEARISH_DEBUG
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = new char*{ "VK_LAYER_LUNARG_standard_validation" };
	Core::Logger::Info("Requesting Vulkan device valdation layers...");
#endif

	if (VkError(vkCreateDevice(_gpu->GetPhyiscalDevice(), &createInfo, nullptr, &_device))) {
		Core::Logger::Fatal("Failed to create Vulkan logical device.");
	}

	GetQueues();
}

Device::~Device() {
	if (_device) {
		vkDestroyDevice(_device, nullptr);
		_device = nullptr;
	}

	if (_surface) {
		delete _surface;
		_surface = nullptr;
	}
}

void Device::GetQueues() {
	_graphicsQueue = Queue(*this, _gpu->GetGraphicsQueue());
	_presentQueue = Queue(*this, _gpu->GetPresentQueue(_surface));
}