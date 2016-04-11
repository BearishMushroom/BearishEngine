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

	struct PBRData {
		vec4 AlbedoColor;
		vec3 SpecularColor;
		f32 GlossColor;
		f32 UsingAlbedoMap;
		f32 UsingSpecularMap;
		f32 UsingGlossMap;

		PBRData() : UsingAlbedoMap(0), UsingSpecularMap(0), UsingGlossMap(0) {}
	};

	class Material : public Core::IAllocatable<Material> {
	public:
		Material(string name, Shader* shader) : _shader(shader), _name(name) {
			_copies = 0;
			_ubo = new UBO();
			_ubo->SetData(0, sizeof(PBRData));
		}

		~Material() {}

		Material(Material& other) {
			_name = other._name + std::to_string(other._copies++);
			_shader = other._shader;
			_floatMap = other._floatMap;
			_intMap = other._intMap;
			_vec2Map = other._vec2Map;
			_vec3Map = other._vec3Map;
			_vec4Map = other._vec4Map;
			_textureMap = other._textureMap;
			_ubo = other._ubo;
			_pbrData = other._pbrData;
		}

		void Set(string name, f32 value) { _floatMap[name] = value; }
		void Set(string name, i32 value) { _intMap[name] = value; }
		void Set(string name, Math::vec2 value) { _vec2Map[name] = value; }
		void Set(string name, Math::vec3 value) { _vec3Map[name] = value; }
		void Set(string name, Math::vec4 value) { _vec4Map[name] = value; }
		void Set(string name, Texture* value, i32 slot) { _textureMap[name] = std::make_pair(value, slot); }

		void Bind(Shader* shader) {
			for (auto& i : _floatMap) {
				shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _intMap) {
				shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec2Map) {
				shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec3Map) {
				shader->SetUniform(i.first, i.second);
			}
			for (auto& i : _vec4Map) {
				shader->SetUniform(i.first, i.second);
			}

			for (auto& i : _textureMap) {
				shader->SetUniform(i.first, i.second.first);
				i.second.first->Bind(i.second.second);
			}

			_ubo->UpdateData(_pbrData);
			shader->SetUniformBlock("pbr_data", _ubo);
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

		bool operator==(const Material& other) {
			return _name == other._name;
		}

		PBRData& Data() { return _pbrData; }
	private:
		std::map<string, f32>                      _floatMap;
		std::map<string, i32>                      _intMap;
		std::map<string, Math::vec2>			   _vec2Map;
		std::map<string, Math::vec3>			   _vec3Map;
		std::map<string, Math::vec4>			   _vec4Map;
		std::map<string, std::pair<Texture*, i32>> _textureMap;
		string _name;
		Shader* _shader;
		i32 _copies;

		UBO* _ubo;
		PBRData _pbrData;
	};

} }

#endif // _BEARISH_GRAPHICS_MATERIAL_H_