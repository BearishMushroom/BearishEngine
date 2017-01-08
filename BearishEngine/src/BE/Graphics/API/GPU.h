#ifndef _BEARISH_GRAPHICS_API_ADAPTER_H_
#define _BEARISH_GRAPHICS_API_ADAPTER_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Instance.h>

namespace Bearish { namespace Graphics { namespace API {
	enum class AdapterType {
		Unknown,
		Integrated,
		Discrete,
		Virtual, 
		CPU,
	};

	struct GPUQueue {
		i32 index = -1;

		operator i32() { return index; }

		bool IsValid() {
			return index >= 0;
		}
	};

	class GPU {
	public:
		GPU();
		GPU(Instance* instance, VkPhysicalDevice device);
		GPU(Instance* instance, VkPhysicalDeviceFeatures features, VkPhysicalDeviceProperties properties, VkPhysicalDevice device);

		~GPU();

		AdapterType GetAdapterType() { return (AdapterType)_properties.deviceType; }
		std::string GetName() { return std::string(_properties.deviceName); }
		GPUQueue GetGraphicsQueue();

		const VkPhysicalDeviceFeatures& GetFeatures() { return _features; }
		const VkPhysicalDeviceProperties& GetProperties() { return _properties; }

		static std::vector<GPU> GetAll(Instance* instance);
		static GPU GetMostSuitable(std::vector<GPU> gpus);
	private:

		Instance* _instance;
		std::vector<GPUQueue> _queues;
		VkPhysicalDeviceFeatures _features;
		VkPhysicalDeviceProperties _properties;
		VkPhysicalDevice _device;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_ADAPTER_H_