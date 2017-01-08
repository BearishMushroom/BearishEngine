#include <BE/Graphics/API/Instance.h>
#include <BE/Graphics/API/Util.h>
#include <BE/Core/Logger.h>

using namespace Bearish;
using namespace Graphics;
using namespace API;

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
													uint64_t obj, size_t location, int32_t code,
													const char* layerPrefix, const char* msg, void* userData) {
	string severity = "";
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		if (severity.length() > 0)
			severity += "|";
		severity += "INFO";
	}

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		if (severity.length() > 0)
			severity += "|";
		severity += "WARN";
	}

	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		if (severity.length() > 0)
			severity += "|";
		severity += "PERF";
	}

	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		if (severity.length() > 0)
			severity += "|";
		severity += "ERROR";
	}

	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		if (severity.length() > 0)
			severity += "|";
		severity += "DBG";
	}
	
	Core::Logger::Info("[VULKAN][%s(%s)](%lld) %s", layerPrefix, severity, obj, msg);

	return VK_FALSE;
}

Instance::Instance() {
	Init("BearishEngine");
}

Instance::Instance(string name) {
	Init(name);
}

Instance::~Instance() {
	DestroyDebugCallback();

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

	Core::Logger::Info("Vulkan extensions found: %d", _numExtensions);

	// How many layers are there?
	vkEnumerateInstanceLayerProperties(&_numLayers, NULL);
	// Allocate space for all layers.
	_layers = new VkLayerProperties[_numLayers];

	vkEnumerateInstanceLayerProperties(&_numLayers, _layers);

	Core::Logger::Info("Vulkan layers found: %d", _numLayers);

	_applicationInfo = {};
	_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	_applicationInfo.pApplicationName = name.c_str();
	_applicationInfo.pEngineName = "BearishEngine";
	_applicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	_applicationInfo.apiVersion = VK_API_VERSION_1_0;

	Core::Logger::Info("Vulkan instance API version: %s", VkVersionToString(VK_API_VERSION_1_0).c_str());

	VkInstanceCreateInfo incr = {};
	incr.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

#ifdef BEARISH_DEBUG
	incr.enabledLayerCount = 1;
	incr.ppEnabledLayerNames = new char*{ "VK_LAYER_LUNARG_standard_validation" };
	Core::Logger::Info("Requesting Vulkan validation layers...");
#endif

	incr.enabledExtensionCount = _numExtensions;

	char** names = new char*[_numExtensions];
	for (i32 i = 0; i < _numExtensions; i++) {
		names[i] = _extensions[i].extensionName;
	}
	incr.ppEnabledExtensionNames = names;

	incr.pApplicationInfo = &_applicationInfo;

	VkResult result = vkCreateInstance(&incr, NULL, &_instance);
	if (!result == VK_SUCCESS) {
		Core::Logger::Fatal("Failed to create Vulkan instance, error %d!", result);
	}

	CreateDebugCallback();
}

void Instance::CreateDebugCallback() {
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;// |
		//VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	createInfo.pfnCallback = DebugCallback;

	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugReportCallbackEXT");
	
	if (func != nullptr) {
		func(_instance, &createInfo, NULL, &_callback);
	}
	else {
		Core::Logger::Error("Failed to load debug callback extension.");
	}
}

void Instance::DestroyDebugCallback() {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(_instance, _callback, NULL);
	}
}
