#ifndef _BEARISH_GRAPHICS_API_INSTANCE_H_
#define _BEARISH_GRAPHICS_API_INSTANCE_H_

#include <BE/Types.h>

#include <vulkan\vulkan.h>

namespace Bearish { namespace Graphics { namespace API {
	class Instance {
	public:
		Instance();
		Instance(string name);
		~Instance();


		void Init(string name);

		// Implicit conversion for calling vkXXX
		operator VkInstance() { return _instance; } 
	private:
		void CreateDebugCallback();
		void DestroyDebugCallback();

		VkApplicationInfo _applicationInfo;
		VkInstance _instance;
		VkDebugReportCallbackEXT _callback;

		u32 _numExtensions;
		VkExtensionProperties* _extensions;

		u32 _numLayers;
		VkLayerProperties* _layers;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_INSTANCE_H_
