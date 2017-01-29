#ifndef _BEARISH_GRAPHICS_API_QUEUE_H_
#define _BEARISH_GRAPHICS_API_QUEUE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/GPU.h>

namespace Bearish { namespace Graphics { namespace API {
	struct GPUQueueIndex;
	class Device;

	class Queue {
	public:
		Queue();
		Queue(const Device& device, const GPUQueueIndex& index);
		~Queue();

		operator VkQueue() { return _queue; }
		operator VkQueue&() { return _queue; }
		operator VkQueue*() { return &_queue; }

		static VkDeviceQueueCreateInfo CreateDeviceQueue(const GPUQueueIndex& index, f32 priority = 1.0f);
	private:
		VkQueue _queue;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_QUEUE_H_
