#include <BE/Graphics/API/Instance.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

Instance::Instance() {
	Init("BearishEngine");
}

Instance::Instance(string name) {
	Init(name);
}

Instance::~Instance() {
	if (_extensions)
		delete[] _extensions;
	if (_layers)
		delete[] _layers;

	vkDestroyInstance(_instance, NULL);
}

void Instance::Init(string name) {
	// Get how many extensions we can load.
	vkEnumerateInstanceExtensionProperties(NULL, &_numExtensions, NULL);
	// Allocate space for all extensions.
	_extensions = new VkExtensionProperties[_numExtensions];
	vkEnumerateInstanceExtensionProperties(NULL, &_numExtensions, _extensions);

	Core::Logger::Info("Vulken extensions found: %d", _numExtensions);

	// How many layers are there?
	vkEnumerateInstanceLayerProperties(&_numLayers, NULL);
	// Allocate space for all layers.
	_layers = new VkLayerProperties[_numLayers];

	vkEnumerateInstanceLayerProperties(&_numLayers, _layers);

	Core::Logger::Info("Vulken layers found: %d", _numLayers);


	_applicationInfo = {};
	_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	_applicationInfo.pApplicationName = name.c_str();
	_applicationInfo.pEngineName = "BearishEngine";
	_applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	_applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo incr = {};
	incr.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	incr.pApplicationInfo = &_applicationInfo;

	VkResult result = vkCreateInstance(&incr, NULL, &_instance);
	if (!result == VK_SUCCESS) {
		Core::Logger::Fatal("Failed to create Vulkan instance!!!");
	}
}
