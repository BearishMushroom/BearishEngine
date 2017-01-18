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

	struct GPUQueueIndex {
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
		GPUQueueIndex GetGraphicsQueue() const;

		const VkPhysicalDeviceFeatures& GetFeatures() const { return _features; }
		const VkPhysicalDeviceProperties& GetProperties() const { return _properties; }

		const Instance* GetInstance() const { return _instance; }

		operator VkPhysicalDevice() const { return _device; }
		const VkPhysicalDevice& GetPhyiscalDevice() const { return _device; }

		static std::vector<GPU> GetAll(Instance* instance);
		static GPU GetMostSuitable(std::vector<GPU> gpus);

	private:
		Instance* _instance;
		VkPhysicalDeviceFeatures _features;
		VkPhysicalDeviceProperties _properties;
		VkPhysicalDevice _device;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_ADAPTER_H_