#ifndef _BEARISH_GRAPHICS_API_INSTANCE_H_
#define _BEARISH_GRAPHICS_API_INSTANCE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Util.h>

namespace Bearish { namespace Graphics { namespace API {
	class Instance {
	public:
		Instance();
		Instance(string name);
		~Instance();


		void Init(string name);

		// Implicit conversion for calling vkXXX
		operator VkInstance() const { return _instance; }
		operator VkInstance&() { return _instance; }
		operator VkInstance*() { return &_instance; }

		u32 GetExtensionCount() const { return _numExtensions; }
		u32 GetValidationLayerCount() const { return _numLayers; }

		VkExtensionProperties* GetExtensions() const { return _extensions; }
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
