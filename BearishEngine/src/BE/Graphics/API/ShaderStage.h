#ifndef _BEARISH_GRAPHICS_API_SHADER_STAGE_H_
#define _BEARISH_GRAPHICS_API_SHADER_STAGE_H_

#include <BE/Types.h>
#include <BE/Graphics/API/ShaderModule.h>

namespace Bearish { namespace Graphics { namespace API {
	class ShaderStage {
	public:
		enum class Type {
			Vertex      = VK_SHADER_STAGE_VERTEX_BIT,
			Fragment    = VK_SHADER_STAGE_FRAGMENT_BIT,
			Geometry    = VK_SHADER_STAGE_GEOMETRY_BIT,
			TessControl = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
			TessEval    = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
			Compute     = VK_SHADER_STAGE_COMPUTE_BIT,
		};

		ShaderStage(ShaderModule* mod, Type type, string entrypoint = "main") {
			_module = mod;
			_type = type;

			_entrypoint = new c8[entrypoint.length() + 1];
			for (i32 i = 0; i < entrypoint.length(); i++) {
				_entrypoint[i] = entrypoint[i];
			}
			_entrypoint[entrypoint.length()] = '\0';

			_info = {};
			_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			_info.module = *_module;
			_info.stage = (VkShaderStageFlagBits)_type;
			_info.pName = _entrypoint;
		}

		~ShaderStage() {
			delete[] _entrypoint;
		}

		operator VkPipelineShaderStageCreateInfo() const { return _info; }
	private:
		c8*								_entrypoint;
		Type							_type;
		ShaderModule*					_module;
		VkPipelineShaderStageCreateInfo _info;
	};
} } }

#endif // _BEARISH_GRAPHICS_API_SHADER_STAGE_H_
