#ifndef _BEARISH_GRAPHICS_API_UTIL_H_
#define _BEARISH_GRAPHICS_API_UTIL_H_

#include <BE/Types.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan\vulkan.h>

namespace Bearish { namespace Graphics { namespace API {
	static string VkVersionToString(u32 version) {
		u32 maj = VK_VERSION_MAJOR(version);
		u32 min = VK_VERSION_MINOR(version);
		u32 rev = VK_VERSION_PATCH(version);

		return std::to_string(maj) + "." + std::to_string(min) + "." + std::to_string(rev);
	}

	static bool VkError(VkResult res) {
		return res != VK_SUCCESS;
	}
} } }

#endif //_BEARISH_GRAPHICS_API_UTIL_H_
