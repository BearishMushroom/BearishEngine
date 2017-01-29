#ifndef _BEARISH_GRAPHICS_API_GPU_H_
#define _BEARISH_GRAPHICS_API_GPU_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Instance.h>

namespace Bearish { namespace Graphics { namespace API {
	class Surface;

	enum class AdapterType {
		Unknown,
		Integrated,
		Discrete,
		Virtual, 
		CPU,
	};

	struct GPUQueueIndex {
		i32 index = -1;

		operator i32() const { return index; }

		bool IsValid() const {
			return index >= 0;
		}
	};

	class GPU {
	public:
		GPU();
		GPU(const GPU& other);
		GPU(GPU& other);
		GPU(Instance* instance, VkPhysicalDevice device);
		GPU(Instance* instance, VkPhysicalDeviceFeatures features, VkPhysicalDeviceProperties properties, VkPhysicalDevice device);

		~GPU();

		AdapterType GetAdapterType() { return (AdapterType)_properties.deviceType; }
		std::string GetName() { return std::string(_properties.deviceName); }
		
		std::vector<VkQueueFamilyProperties> GetAllQueues() const;
		GPUQueueIndex GetGraphicsQueue() const;
		GPUQueueIndex GetPresentQueue(Surface* surface) const;

		const VkPhysicalDeviceFeatures& GetFeatures() const { return _features; }
		const VkPhysicalDeviceProperties& GetProperties() const { return _properties; }

		VkExtensionProperties* GetExtensions() const { return _extensions; }
		const u32 GetExtensionCount() const { return _extensionCount; }

		const Instance* GetInstance() const { return _instance; }

		operator VkPhysicalDevice() const { return _device; }
		operator VkPhysicalDevice&() { return _device; }
		const VkPhysicalDevice& GetPhyiscalDevice() const { return _device; }

		static std::vector<GPU> GetAll(Instance* instance);
		static GPU GetMostSuitable(std::vector<GPU> gpus);
		static GPU GetMostSuitable(Instance* instance);

	private:
		void QueryExtensions();

		Instance* _instance;

		VkPhysicalDeviceFeatures _features;
		VkPhysicalDeviceProperties _properties;
		VkPhysicalDevice _device;

		VkExtensionProperties* _extensions;
		u32 _extensionCount;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_GPU_H_