#ifndef _BEARISH_GRAPHICS_API_SHADER_MODULE_H_
#define _BEARISH_GRAPHICS_API_SHADER_MODULE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/Device.h>
#include <BE/Core/Logger.h>

namespace Bearish { namespace Graphics { namespace API {
	class ShaderModule {
	public:
		ShaderModule() {
			_module = 0;
		}

		ShaderModule(const Device* device, c8* code, u32 length) {
			_device = device;

			VkShaderModuleCreateInfo info {};
			info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			info.codeSize = length;
			info.pCode = (u32*)code;

			if (VkError(vkCreateShaderModule(*_device, &info, nullptr, &_module))) {
				Core::Logger::Error("Failed to create Vulkan shader module!");
			}
		}

		~ShaderModule() {
			if (_module) {
				vkDestroyShaderModule(*_device, _module, nullptr);
			}
		}

		operator VkShaderModule() const { return _module; }
	private:
		const Device* _device;
		VkShaderModule _module;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SHADER_MODULE_H_
