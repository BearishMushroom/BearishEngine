#ifndef _BEARISH_GRAPHICS_API_DEVICE_H_
#define _BEARISH_GRAPHICS_API_DEVICE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/GPU.h>

namespace Bearish { namespace Graphics { namespace API {
	class Device {
	public:
		Device();
		Device(const GPU* gpu);

		~Device();

		operator VkDevice() { return _device; }

		const GPU* GetGPU() const { return _gpu; }
	private:
		const GPU* _gpu;
		VkDevice _device;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_DEVICE_H_
