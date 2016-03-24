#ifndef _BEARISH_GRAPHICS_MATERIAL_H_
#define _BEARISH_GRAPHICS_MATERIAL_H_

#include "../Core/IAllocatable.h"
#include "RenderingEngine.h"
#include "../Math/vec2.h"
#include "../Math/vec3.h"
#include "../Math/vec4.h"
#include "../Types.h"
#include "Texture/Texture.h"
#include "Shader.h"

namespace Bearish {namespace Graphics {
	class Material : public Core::IAllocatable<Material> {
	public:
		Material(string name, Shader* shader) : _shader(shader), _name(name) {
			Set("specularIntensity", 0.35f);
			Set("specularPower", 1.0f);
			Set("screen", Math::vec2(1280, 720));
		}
		~Material() {}

		void Set(string name, f32 value) { _floatMap[name] = value; }
		void Set(string name, i32 value) { _intMap[name] = value; }
		void Set(string name, Math::vec2 value) { _vec2Map[name] = value; }
		void Set(string name, Math::vec3 value) { _vec3Map[name] = value; }
		void Set(string name, Math::vec4 value) { _vec4Map[name] = value; }
		void Set(string name, Texture* value, i32 slot) { _textureMap[name] = std::make_pair(value, slot); }

		void Bind(RenderingEngine* engine) {
			for (auto& i : _floatMap) {
				_shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _intMap) {
				_shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec2Map) {
				_shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec3Map) {
				_shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec4Map) {
				_shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _textureMap) {
				_shader->SetUniform(i.first, i.second.second);
				i.second.first->Bind(i.second.second);
			}
		}

		Shader* GetShader() {
			return _shader;
		}

		void SetShader(Shader* shader) {
			_shader = shader;
		}

		string GetName() {
			return _name;
		}
	private:
		std::map<string, f32> _floatMap;
		std::map<string, i32> _intMap;
		std::map<string, Math::vec2> _vec2Map;
		std::map<string, Math::vec3> _vec3Map;
		std::map<string, Math::vec4> _vec4Map;
		std::map<string, std::pair<Texture*, i32>> _textureMap;
		string _name;
		Shader* _shader;
	};
} }

#endif // _BEARISH_GRAPHICS_MATERIAL_H_