#include <BE/Graphics/API/Queue.h>
#include <BE/Graphics/API/GPU.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

VkDeviceQueueCreateInfo Queue::CreateDeviceQueue(const GPUQueueIndex& index, f32 priority) {
	VkDeviceQueueCreateInfo info {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = index.index;
	info.queueCount = 1;
	info.pQueuePriorities = new f32{ priority };
	return info;
}
