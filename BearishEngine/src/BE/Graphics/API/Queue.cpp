#include <BE/Graphics/API/Device.h>
#include <BE/Graphics/API/Queue.h>
#include <BE/Graphics/API/GPU.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Queue::Queue() {}


Queue::Queue(const Device& device, const GPUQueueIndex& index) {
	if (!index.IsValid()) {
		Core::Logger::Fatal("Tried to create Queue with invalid index!");
	}

	// Get the 0th queue from the queue family specified by the index.
	vkGetDeviceQueue(device, index, 0, &_queue);
}

Queue::~Queue() {}

VkDeviceQueueCreateInfo Queue::CreateDeviceQueue(const GPUQueueIndex& index, f32 priority) {
	VkDeviceQueueCreateInfo info {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = index.index;
	info.queueCount = 1;
	info.pQueuePriorities = new f32{ priority };
	return info;
}
