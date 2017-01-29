#ifndef _BEARISH_GRAPHICS_API_DEVICE_H_
#define _BEARISH_GRAPHICS_API_DEVICE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/GPU.h>
#include <BE/Graphics/API/Queue.h>

namespace Bearish { namespace Graphics { namespace API {
	class Surface;

	class Device {
	public:
		Device();
		Device(const GPU* gpu);

		~Device();

		void Init(Surface* surface);

		operator VkDevice() const { return _device; }
		operator VkDevice&() { return _device; }
		operator VkDevice*() { return &_device; }

		const GPU* GetGPU() const { return _gpu; }
		const Instance* GetInstance() const { return _instance; }
	private:
		void GetQueues();

		const GPU* _gpu;
		const Instance* _instance;

		Queue _graphicsQueue;
		Queue _presentQueue;
		Surface* _surface;

		VkDevice _device;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_DEVICE_H_
